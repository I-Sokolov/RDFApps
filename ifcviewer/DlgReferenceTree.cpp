// DlgReferenceTree.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "stringCreation.h"
#include "IFCEngineInteract.h"
#include "DlgReferenceTree.h"

//

struct TreeItemData
{
	int_t instance = 0;		//this item reflects instance
	int_t* aggregation = 0; //this item reflects aggregation

	int_t attrToIgnore = 0; //ignore this attribute when expand this instance
	
	bool expanded = false;  //item was expanded
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
END_MESSAGE_MAP()


// CDlgReferenceTree message handlers


BOOL CDlgReferenceTree::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InsertTreeItem (m_rootInstance, nullptr, 0, false, TVI_ROOT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgReferenceTree::InsertTreeItem(int_t instance, int_t* aggregation, int_t attr, bool inverseReference, HTREEITEM hParent)
{
	//need to check cycles?

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

	if (attr) {
		const char* attrName = nullptr;
		engiGetAttributeTraits(attr, &attrName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
		CString strName(attrName);
		
		itemText = strName + (inverseReference ? "<-" : ": ") + itemText;
	}

	auto hItem = m_wndTree.InsertItem(itemText, hParent);

	auto pData = new TreeItemData;
	pData->instance = instance;
	pData->aggregation = aggregation;
	if (attr && inverseReference) {
		pData->attrToIgnore = attr;
	}
	m_wndTree.SetItemData(hItem, (DWORD_PTR) pData);

	m_wndTree.InsertItem(L"... to be expanded ....", hItem);
}

void CDlgReferenceTree::OnDeleteitemReferenceTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	auto data = m_wndTree.GetItemData(pNMTreeView->itemOld.hItem);

	if (data) {
		auto pData = (TreeItemData*) data;
		delete pData;
	}

	*pResult = 0;
}


void CDlgReferenceTree::OnItemexpandingReferenceTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	auto hItem = pNMTreeView->itemNew.hItem;
	auto data = m_wndTree.GetItemData(hItem);
	if (data) {
		auto pData = (TreeItemData*) data;
		if (pData && !pData->expanded) {
			pData->expanded = true;
			
			auto hStubChild = m_wndTree.GetChildItem(hItem);
			if (hStubChild) {
				m_wndTree.DeleteItem(hStubChild);
			}
			else {
				assert(false);
			}

			if (pData->instance) {
				InsertReferencedInstances(hItem, pData);
				InsertReferencingInstances(hItem, pData);
			}

			if (pData->aggregation) {
				InsertAggregationElements(hItem, pData);
			}
		}
	}

	*pResult = 0;
}

void CDlgReferenceTree::InsertReferencedInstances(HTREEITEM hItem, TreeItemData* pData)
{
	auto entity = sdaiGetInstanceType(pData->instance);

	auto NAttr = engiGetEntityNoAttributesEx(entity, true, false);
	for (int i = 0; i < NAttr; i++) {
		auto attr = engiGetEntityAttributeByIndex(entity, i, true, false);
		if (attr != pData->attrToIgnore) {
			auto sdaiType = engiGetAttributeType(attr);
			switch (sdaiType) {
				case sdaiINSTANCE:
				{
					int_t inst = 0;
					sdaiGetAttr(pData->instance, (void*) attr, sdaiINSTANCE, &inst);
					if (inst) {
						InsertTreeItem (inst, 0, attr, false, hItem);
					}
					break;
				}

				case sdaiAGGR:
				{
					int_t* aggr = nullptr;
					sdaiGetAttr(pData->instance, (void*) attr, sdaiAGGR, &aggr);
					if (aggr && AggregationContainsInstance(aggr)) {
						InsertTreeItem(0, aggr, attr, false, hItem);
					}
					break;
				}
			}
		}
	}
}

void CDlgReferenceTree::InsertAggregationElements(HTREEITEM hItem, TreeItemData* pData)
{
	int_t sdaiType = -1;
	engiGetAggrType(pData->aggregation, &sdaiType);

	auto N = sdaiGetMemberCount(pData->aggregation);
	for (int_t i = 0; i < N; i++) {
		switch (sdaiType) 			{
			case sdaiINSTANCE:
			{
				int_t inst = 0;
				engiGetAggrElement(pData->aggregation, i, sdaiINSTANCE, &inst);
				if (inst) {
					InsertTreeItem(inst, nullptr, 0, false, hItem);
				}
				break;
			}

			case sdaiAGGR:
			{
				int_t* aggr = nullptr;
				engiGetAggrElement(pData->aggregation, i, sdaiAGGR, &aggr);
				if (aggr) {
					InsertTreeItem(0, aggr, 0, false, hItem);
				}
				break;
			}

			case sdaiADB:
				assert(0); //TODO
				break;

			default:
				assert(false);
		}
	}
	
}

bool CDlgReferenceTree::AggregationContainsInstance(int_t* aggregation, int_t checkInstance)
{
	int_t sdaiType = -1;
	engiGetAggrType(aggregation, &sdaiType);

	auto N = sdaiGetMemberCount(aggregation);
	for (int_t i = 0; i < N; i++) {
		switch (sdaiType) {
			case sdaiINSTANCE:
			{
				int_t inst = 0;
				engiGetAggrElement(aggregation, i, sdaiINSTANCE, &inst);
				if ((checkInstance && inst == checkInstance) || (!checkInstance && inst != 0)) {
					return true; //>>>>>>>>>>>>>.
				}
				break;
			}

			case sdaiAGGR:
			{
				int_t* aggr = nullptr;
				engiGetAggrElement(aggregation, i, sdaiAGGR, &aggr);
				if (aggr) {
					if (AggregationContainsInstance(aggr, checkInstance)) {
						return true; //>>>>>>>>>>>>>.
					}
				}
				break;
			}

			case sdaiADB:
				//TODO
				break;
		}
	}

	return false;
}

void CDlgReferenceTree::CheckInsertReferencingInstance(int_t instance, int_t referencedInstance,HTREEITEM hParent)
{
	int_t refAttr = 0;

	auto entity = sdaiGetInstanceType(instance);
	auto NAttr = engiGetEntityNoAttributesEx(entity, true, false);
	
	for (int i = 0; !refAttr && i < NAttr; i++) {
		auto attr = engiGetEntityAttributeByIndex(entity, i, true, false);
		auto sdaiType = engiGetAttributeType(attr);
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
		InsertTreeItem(instance, 0, refAttr, true, hParent);
	}
}


struct ReferenceSearch : public RDF::CModelChecker::InstanceVisitor
{
	ReferenceSearch (CDlgReferenceTree& me, int_t refInstance, HTREEITEM hParent)
		: m_me (me)
		, m_refInstance (refInstance)
		, m_hParent (hParent)
	{}

	virtual void OnVisitInstance(int_t instance) override
	{
		if (instance != m_refInstance) {
			m_me.CheckInsertReferencingInstance(instance, m_refInstance, m_hParent);
		}
	}

	CDlgReferenceTree&	m_me;
	int_t				m_refInstance;
	HTREEITEM           m_hParent;
};

void CDlgReferenceTree::InsertReferencingInstances(HTREEITEM hItem, TreeItemData* pData)
{
	ReferenceSearch visitor (*this, pData->instance, hItem);
	RDF::CModelChecker::VisitAllInstances(globalIfcModel, visitor);
}


void CDlgReferenceTree::OnSelchangedReferenceTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
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
