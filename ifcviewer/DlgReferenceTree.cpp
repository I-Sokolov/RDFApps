// DlgReferenceTree.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "stringCreation.h"
#include "IFCEngineInteract.h"
#include "ifcviewerDoc.h"
#include "DlgReferenceTree.h"

#define CHILD_LIMIT1	5
#define CHILD_LIMIT2    25

//

struct TreeItemData
{
	enum class Type	{Regular, AddMore};

	Type type = Type::Regular;

	int_t instance = 0;		//this item reflects instance
	int_t* aggregation = 0; //this item reflects aggregation

	HTREEITEM cyclicParent = NULL; 
	
	int childLimit = 0;  //item was expanded
};

// CDlgReferenceTree dialog

IMPLEMENT_DYNAMIC(CDlgReferenceTree, CDialogEx)

CDlgReferenceTree::CDlgReferenceTree(int_t instance, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REFERENCEVIEW, pParent)
	, m_rootInstance (instance)
{
}

CDlgReferenceTree::~CDlgReferenceTree()
{
}

void CDlgReferenceTree::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REFERENCE_TREE, m_wndTree);
}


BEGIN_MESSAGE_MAP(CDlgReferenceTree, CDialogEx)
	ON_NOTIFY(TVN_DELETEITEM, IDC_REFERENCE_TREE, &CDlgReferenceTree::OnDeleteitemReferenceTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_REFERENCE_TREE, &CDlgReferenceTree::OnItemexpandingReferenceTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_REFERENCE_TREE, &CDlgReferenceTree::OnSelchangedReferenceTree)
	ON_NOTIFY(NM_DBLCLK, IDC_REFERENCE_TREE, &CDlgReferenceTree::OnDblclkReferenceTree)
	ON_NOTIFY(TVN_GETINFOTIP, IDC_REFERENCE_TREE, &CDlgReferenceTree::OnGetinfotipReferenceTree)
END_MESSAGE_MAP()


// CDlgReferenceTree message handlers


BOOL CDlgReferenceTree::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	auto hIcon = ::LoadIcon(NULL, IDI_QUESTION);
	SetIcon(hIcon, true);
	SetIcon(hIcon, false);

	m_refDir.Create(IDB_REFERENCE_DIRECTION, 16, 1, RGB(255, 255, 255));
	m_wndTree.SetImageList(&m_refDir, TVSIL_NORMAL);

	InsertRegularItem (m_rootInstance, nullptr, 0, false, TVI_ROOT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgReferenceTree::InsertRegularItem(int_t instance, int_t* aggregation, void* attr, bool inverseReference, HTREEITEM hParent)
{	
	HTREEITEM hCyclicParent = FindParentWithInstance(hParent, instance);
	if (hCyclicParent && inverseReference) {
		return; //do not include cyclic parent for inverse reference
	}

	CString itemText;

	if (instance) {
		auto stepId = internalGetP21Line(instance);
		itemText.Format(L"#%lld= ", stepId);

		auto txt = GetTreeItemName_ifcInstance(instance);
		itemText += CString (txt);
		delete txt;
	}

	if (aggregation) {
		auto N = sdaiGetMemberCount(aggregation);
		itemText.Format (L"[aggregation size %lld]", N);
	}

	CString attrName;
	if (attr) {
		const char* name = nullptr;
		engiGetAttributeTraits(attr, &name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
		attrName = name;
	}

	CString text;
	if (inverseReference) {
		text.Format(L"%s references parent by %s", itemText.GetString(), attrName.GetString());
	}
	else {
		text.Format(L"%s: %s", attrName.GetString(), itemText.GetString());
	}

	auto hItem = m_wndTree.InsertItem(text, hParent);

	auto pData = new TreeItemData;
	pData->instance = instance;
	pData->aggregation = aggregation;
	
	if (instance) {
		pData->cyclicParent = FindParentWithInstance(hParent, instance);
	}

	m_wndTree.SetItemData(hItem, (DWORD_PTR) pData);

	int image = 0;
	if (hParent != TVI_ROOT) {
		if (pData->cyclicParent) {
			image = 3;
		}
		else {
			image = inverseReference ? 2 : 1;
		}
	}
	m_wndTree.SetItemImage(hItem, image, image);

	if (!pData->cyclicParent) {
		InsertAddMoreItem(hItem);
	}
}

void CDlgReferenceTree::InsertAddMoreItem(HTREEITEM hParent)
{
	auto hAdd = m_wndTree.InsertItem(L"Double-click to see more...", hParent);
	m_wndTree.SetItemImage(hAdd, 4, 4);

	auto pData = new TreeItemData;
	pData->type = TreeItemData::Type::AddMore;
	m_wndTree.SetItemData(hAdd, (DWORD_PTR) pData);
}


HTREEITEM CDlgReferenceTree::FindParentWithInstance(HTREEITEM hParent, int_t instance)
{
	while (hParent && hParent != TVI_ROOT) {

		auto data = m_wndTree.GetItemData(hParent);
		if (data) {
			auto pData = (TreeItemData*) data;
			if (pData->instance == instance) {
				return hParent;
			}
		}

		hParent = m_wndTree.GetNextItem(hParent, TVGN_PARENT);
	}

	return nullptr;
}


void CDlgReferenceTree::OnDeleteitemReferenceTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	if (pNMTreeView->itemOld.mask & TVIF_HANDLE) {
		auto data = m_wndTree.GetItemData(pNMTreeView->itemOld.hItem);
		if (data) {
			auto pData = (TreeItemData*) data;
			delete pData;
		}
	}
	else {
		ASSERT(0);
	}

	*pResult = 0;
}

int CDlgReferenceTree::AddChildItemsWithNewLimit(HTREEITEM hItem, TreeItemData& data)
{
	int oldChilds = 0;

	if (data.type == TreeItemData::Type::Regular) {

		while (auto hStubChild = m_wndTree.GetChildItem(hItem)) {
			oldChilds++;
			m_wndTree.DeleteItem(hStubChild);
		}

		if (data.instance) {
			InsertReferencedInstances(hItem, data);
			InsertReferencingInstances(hItem, data);
		}

		if (data.aggregation) {
			InsertAggregationElements(hItem, data);
		}
	}
	else ASSERT(false);

	return oldChilds;
}

void CDlgReferenceTree::IncreaseChildLimit(HTREEITEM hAddItem)
{
	auto hItem = m_wndTree.GetParentItem(hAddItem);
	ASSERT(hItem); if (!hItem) return;

	auto dwdata = m_wndTree.GetItemData(hItem);
	ASSERT(dwdata); if (!dwdata) return;

	TreeItemData& data = *((TreeItemData*) dwdata);
	ASSERT(data.type == TreeItemData::Type::Regular); if (data.type != TreeItemData::Type::Regular) return;

	m_wndTree.SelectItem(hItem);

	if (data.childLimit < CHILD_LIMIT2)
		data.childLimit = CHILD_LIMIT2;
	else
		data.childLimit *= 10;

	auto childNum = AddChildItemsWithNewLimit(hItem, data);

	//set selection
	auto hSelect = m_wndTree.GetChildItem(hItem);
	
	while (hSelect && childNum > 1) {
		hSelect = m_wndTree.GetNextSiblingItem(hSelect);
		childNum--;
	}

	if (hSelect) {
		m_wndTree.SelectItem(hSelect);
	}
}


void CDlgReferenceTree::OnItemexpandingReferenceTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	if (pNMTreeView->itemNew.mask & TVIF_HANDLE) {
		auto hItem = pNMTreeView->itemNew.hItem;
		if (m_wndTree.GetItem(&(pNMTreeView->itemNew))) {
			auto data = m_wndTree.GetItemData(hItem);
			if (data) {
				auto pData = (TreeItemData*) data;
				if (pData && pData->type == TreeItemData::Type::Regular && !pData->childLimit) {
					//first expansion
					pData->childLimit = CHILD_LIMIT1;
					AddChildItemsWithNewLimit(hItem, *pData);
				}
			}
		}
	}

	*pResult = 0;
}

void CDlgReferenceTree::OnDblclkReferenceTree(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	auto hItem = m_wndTree.GetSelectedItem();
	if (hItem) {
		auto data = m_wndTree.GetItemData(hItem);
		if (data) {
			auto pData = (TreeItemData*) data;
			if (pData->type == TreeItemData::Type::AddMore) {
				IncreaseChildLimit(hItem);
			}
			else if (pData->cyclicParent) {
				m_wndTree.SelectItem(pData->cyclicParent);
			}
		}
	}
	*pResult = 0;
}


void CDlgReferenceTree::InsertReferencedInstances(HTREEITEM hItem, TreeItemData& data)
{
	auto entity = sdaiGetInstanceType(data.instance);

	auto NAttr = engiGetEntityNoAttributesEx(entity, true, false);
	for (int i = 0; i < NAttr; i++) {
		auto attr = engiGetEntityAttributeByIndex(entity, i, true, false);

		auto sdaiType = engiGetAttrType(attr);
		if (sdaiType & engiTypeFlagAggr) {
			sdaiType = sdaiAGGR;
		}
		else if (sdaiType & engiTypeFlagAggrOption) {
			sdaiType = sdaiADB;
		}

		switch (sdaiType) {
			case sdaiINSTANCE:
			{
				int_t inst = 0;
				sdaiGetAttr(data.instance, (void*) attr, sdaiINSTANCE, &inst);
				if (inst) {
					InsertRegularItem(inst, 0, attr, false, hItem);
				}
				break;
			}

			case sdaiAGGR:
			{
				int_t* aggr = nullptr;
				sdaiGetAttr(data.instance, (void*) attr, sdaiAGGR, &aggr);
				if (aggr && AggregationContainsInstance(aggr)) {
					InsertRegularItem(0, aggr, attr, false, hItem);
				}
				break;
			}
		}
	}
}

void CDlgReferenceTree::InsertAggregationElements(HTREEITEM hItem, TreeItemData& data)
{
	int_t sdaiType = -1;
	engiGetAggrType(data.aggregation, &sdaiType);

	auto N = sdaiGetMemberCount(data.aggregation);
	for (int_t i = 0; i < N && i < data.childLimit; i++) {
		switch (sdaiType) {
			case sdaiINSTANCE:
			{
				int_t	inst = 0;
				sdaiGetAggrByIndex(data.aggregation, i, sdaiINSTANCE, &inst);
				if (inst) {
					InsertRegularItem(inst, nullptr, 0, false, hItem);
				}
				break;
			}

			case sdaiAGGR:
			{
				int_t	* aggr = nullptr;
				sdaiGetAggrByIndex(data.aggregation, i, sdaiAGGR, &aggr);
				if (aggr) {
					InsertRegularItem(0, aggr, 0, false, hItem);
				}
				break;
			}

			case sdaiADB:
				ASSERT(0); //TODO
				break;

			default:
				ASSERT(false);
		}
	}
	
	if (N > data.childLimit) {
		InsertAddMoreItem(hItem);
	}
}


void CDlgReferenceTree::CheckInsertReferencingInstance(int_t instance, int_t referencedInstance,HTREEITEM hParent, int& childCounter, int childLimit)
{
	//auto stepId = internalGetP21Line(instance);

	void* refAttr = 0;

	auto entity = sdaiGetInstanceType(instance);
	auto NAttr = engiGetEntityNoAttributesEx(entity, true, false);
	
	for (int i = 0; !refAttr && i < NAttr; i++) {
		auto attr = engiGetEntityAttributeByIndex(entity, i, true, false);

		//const char* attrName = NULL;
		//engiGetAttributeTraits(attr, &attrName, 0, 0, 0, 0, 0, 0, 0);

		auto sdaiType = engiGetAttrType(attr);
		if (sdaiType & engiTypeFlagAggr) {
			sdaiType = sdaiAGGR;
		}
		else if (sdaiType & engiTypeFlagAggrOption) {
			sdaiType = sdaiADB;
		}

		switch (sdaiType) {
			case sdaiINSTANCE:
			{
				int_t inst = 0;
				sdaiGetAttr(instance, (void*) attr, sdaiINSTANCE, &inst);
				if (inst == referencedInstance) {
					refAttr = attr;
				}
				break;
			}

			case sdaiAGGR:
			{
				int_t* aggr = nullptr;
				sdaiGetAttr(instance, (void*) attr, sdaiAGGR, &aggr);
				if (aggr && AggregationContainsInstance (aggr, referencedInstance)) {
					refAttr = attr;
				}
				break;
			}
		}
	}

	if (refAttr) {
		childCounter++;
		if (childCounter < childLimit) {
			InsertRegularItem(instance, 0, refAttr, true, hParent);
		}
		else {
			InsertAddMoreItem(hParent);
		}
	}
}



void CDlgReferenceTree::InsertReferencingInstances(HTREEITEM hItem, TreeItemData& data)
{
	auto all = xxxxGetAllInstances(globalIfcModel);
	auto N = sdaiGetMemberCount(all);
	int childCounter = 0;
	for (int_t i = 0; i < N; i++) {
		int_t instance = 0;
		sdaiGetAggrByIndex(all, i, sdaiINSTANCE, &instance);
		if (instance && instance != data.instance) {
			CheckInsertReferencingInstance(instance, data.instance, hItem, childCounter, data.childLimit);
			if (childCounter >= data.childLimit)
				break;
		}
	}
}


void CDlgReferenceTree::OnSelchangedReferenceTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	if (pNMTreeView->itemNew.mask & TVIF_HANDLE) {
		auto hItem = pNMTreeView->itemNew.hItem;
		auto data = m_wndTree.GetItemData(hItem);
		if (data) {
			auto pData = (TreeItemData*) data;
			if (pData && pData->type == TreeItemData::Type::Regular && pData->instance) {
				auto pMainWnd = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
				if (pMainWnd) {
					auto pDoc = pMainWnd->GetActiveDocument();
					if (pDoc) {
						CifcviewerDoc::ActiveInstanceHint hint(pData->instance);
						pDoc->UpdateAllViews(nullptr, (LPARAM) CifcviewerDoc::UpdateHint::SetActiveInstance, &hint);
					}
					else ASSERT(false);
				}
				else ASSERT(false);
			}
		}
	}

	*pResult = 0;
}

void CDlgReferenceTree::OnCancel()
{
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void CDlgReferenceTree::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;
}




void CDlgReferenceTree::OnGetinfotipReferenceTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMTVGETINFOTIP>(pNMHDR);
	
	auto pData = (TreeItemData*) pGetInfoTip->lParam;

	static CString tip;
	tip.Empty();

	if (pData) {
		if (pData->type == TreeItemData::Type::AddMore) {
			tip.LoadString(IDS_REFTREE_ADDMORE_TIP);
		}
		else if (pData->cyclicParent) {
			tip.LoadString(IDS_REFTREE_CYCLIC_TIP);
		}
	}

	wcscpy_s(pGetInfoTip->pszText, pGetInfoTip->cchTextMax - 1, tip);

	*pResult = 0;
}
