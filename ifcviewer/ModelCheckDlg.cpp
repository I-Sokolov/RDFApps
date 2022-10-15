// ModelCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "MainFrm.h"
#include "ModelCheckDlg.h"

#include "IFCEngineInteract.h"

struct IssueData
{
	int_t stepId = -1;
	std::set<int_t> relatingInstances;
	bool relatingInstancesCollected = false;
};

// CModelCheckDlg dialog

IMPLEMENT_DYNAMIC(CModelCheckDlg, CDialog)

/// <summary>
/// 
/// </summary>
CModelCheckDlg::CModelCheckDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MODELCHECK, pParent)
	, m_nSortColumn(-1)
	, m_bSortAscending(false)
{

}

/// <summary>
/// 
/// </summary>
CModelCheckDlg::~CModelCheckDlg()
{
}

/// <summary>
/// 
/// </summary>
void CModelCheckDlg::OnNewModel()
{
	if (m_wndIssueList.GetSafeHwnd()) {
		FillIssueList();
	}
}


/// <summary>
/// 
/// </summary>
void CModelCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ISSUELIST, m_wndIssueList);
}


BEGIN_MESSAGE_MAP(CModelCheckDlg, CDialog)
	ON_NOTIFY(LVN_DELETEITEM, IDC_ISSUELIST, &CModelCheckDlg::OnDeleteitemIssuelist)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_ISSUELIST, &CModelCheckDlg::OnColumnclickIssuelist)
	ON_NOTIFY(NM_DBLCLK, IDC_ISSUELIST, &CModelCheckDlg::OnDblclkIssuelist)
	ON_NOTIFY(NM_CLICK, IDC_ISSUELIST, &CModelCheckDlg::OnClickIssuelist)
END_MESSAGE_MAP()


// CModelCheckDlg message handlers


/// <summary>
/// 
/// </summary>
BOOL CModelCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	FormatIssueList();
	FillIssueList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


/// <summary>
/// 
/// </summary>
void CModelCheckDlg::FormatIssueList()
{
	const wchar_t* rTitle[] = {L"Description", L"#", L"Entity", L"Attribute"};
	
	for (int i = 0; i < _countof(rTitle); i++) {
		m_wndIssueList.InsertColumn(i, rTitle[i]);
		
		auto w = m_wndIssueList.GetStringWidth(rTitle[i]);
		m_wndIssueList.SetColumnWidth(i, w + 10 * GetSystemMetrics(SM_CXBORDER));
	}
}

/// <summary>
/// 
/// </summary>
void CModelCheckDlg::FillIssueList()
{
	m_wndIssueList.DeleteAllItems();

	if (globalIfcModel) {

		auto checks = RDF::ModelChecker::CheckModel(globalIfcModel);

		int rWidth[4] = {0,0,0,0};
	
		for (auto issue = RDF::ModelChecker::FirstIssue(checks); issue; issue = RDF::ModelChecker::NextIssue (issue))
		{
			AddIssue(issue, rWidth);
		}

		RDF::ModelChecker::DisposeResults(checks);

		for (int i = 0; i < 4; i++) {
			m_wndIssueList.SetColumnWidth(i, rWidth[i] + 20 * GetSystemMetrics(SM_CXBORDER));
		}
	}
}

void CModelCheckDlg::AddIssue(RDF::ModelChecker::IssueInfo* issue, int rWidth[4])
{
	CString text (RDF::ModelChecker::Description(issue));
	auto item = m_wndIssueList.InsertItem(0, text);
	rWidth[0] = max(rWidth[0], m_wndIssueList.GetStringWidth(text));

	CString id;
	id.Format(L"%I64d", RDF::ModelChecker::StepId (issue));
	m_wndIssueList.SetItemText(item, 1, id);
	rWidth[1] = max(rWidth[1], m_wndIssueList.GetStringWidth(id));

	CString entity(RDF::ModelChecker::EntityName (issue));
	m_wndIssueList.SetItemText(item, 2, entity);
	rWidth[2] = max(rWidth[2], m_wndIssueList.GetStringWidth(entity));

	CString aggrIndex;
	for (int i = 0; i < RDF::ModelChecker::AggrLevel (issue); i++) {
		if (aggrIndex.IsEmpty()) {
			aggrIndex.Append(L"[");
		}
		else {
			aggrIndex.Append(L",");
		}

		CString ind;
		ind.Format(L"%I64d", RDF::ModelChecker::AggrIndArray (issue)[i]);
		aggrIndex.Append(ind);
	}
	if (!aggrIndex.IsEmpty()) {
		aggrIndex.Append(L"]");
	}

	CString attr(RDF::ModelChecker::AttrName (issue));
	if (!aggrIndex.IsEmpty())
		attr += aggrIndex;
	m_wndIssueList.SetItemText(item, 3, attr);
	rWidth[3] = max(rWidth[3], m_wndIssueList.GetStringWidth(attr));

	auto data = new IssueData();
	data->stepId = RDF::ModelChecker::StepId (issue);
	m_wndIssueList.SetItemData(item, (DWORD_PTR)data);
}

void CModelCheckDlg::OnDeleteitemIssuelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	auto data = m_wndIssueList.GetItemData (pNMLV->iItem);
	if (data) {
		auto p = (IssueData*) data;
		delete p;
	}
	*pResult = 0;
}

int CALLBACK CModelCheckDlg::SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamMe)
{
	CModelCheckDlg& me = *(CModelCheckDlg*) lParamMe;

	CString s1 = me.m_wndIssueList.GetItemText((int)lParam1, me.m_nSortColumn);
	CString s2 = me.m_wndIssueList.GetItemText((int)lParam2, me.m_nSortColumn);

	if (me.m_nSortColumn == 1) {
		int a1 = _wtoi(s1);
		int a2 = _wtoi(s2);

		if (a1 > a2)
			return me.m_bSortAscending ? 1 : -1;
		else if (a2 > a1)
			return me.m_bSortAscending ? -1 : 1;
		else
			return 0;
	}
	else {
		if (me.m_bSortAscending)
			return s1.Compare(s2);
		else
			return s2.Compare(s1);
	}
}

void CModelCheckDlg::OnColumnclickIssuelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->iSubItem == m_nSortColumn) {
		m_bSortAscending = !m_bSortAscending;
	}
	else {
		m_nSortColumn = pNMLV->iSubItem;
		m_bSortAscending = true;
	}

	m_wndIssueList.SortItemsEx(SortFunc, (DWORD_PTR)this);
	*pResult = 0;
}


void CModelCheckDlg::OnActivateListItem(int iItem)
{
	auto pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	ASSERT(pFrame); if (!pFrame) return;

	if (iItem >= 0) {
		auto data = m_wndIssueList.GetItemData(iItem);
		if (data) {
			auto p = (IssueData*) data;

			if (!p->relatingInstancesCollected) {
				p->relatingInstancesCollected = true;
				auto instance = internalGetInstanceFromP21Line(globalIfcModel, p->stepId);
				ASSERT(instance);
				if (instance) {

					int_t searchEntities[3] = {
					sdaiGetEntity(globalIfcModel, (char*) L"IfcProduct"),
					sdaiGetEntity(globalIfcModel, (char*) L"IfcProject"),
					0};

					RDF::ModelChecker::CollectReferencingInstancesRecursive(p->relatingInstances, instance, searchEntities);
				}

			}

			for (auto inst : p->relatingInstances) {
				if (pFrame->SelectInstance(inst)) {
					return; //>>>>
				}
			}
		}
	}

	//nothing found
	pFrame->SelectInstance(NULL);
}


void CModelCheckDlg::OnDblclkIssuelist(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//OnActivateListItem(pNMItemActivate->iItem);
	*pResult = 0;
}


void CModelCheckDlg::OnClickIssuelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnActivateListItem(pNMItemActivate->iItem);
	*pResult = 0;
}
