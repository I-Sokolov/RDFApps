// ModelCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "ModelCheckDlg.h"

#include "IFCEngineInteract.h"

struct ModelCheckerLog : public RDF::CModelChecker::ModelCheckerLog
{
	ModelCheckerLog(CListCtrl& wndList) : m_wndList(wndList) { for (auto& w : rWidth) { w = 0; } }

	virtual void ReportIssue(RDF::CModelChecker::IssueInfo& issue) override;
	
	CListCtrl& m_wndList;
	int        rWidth[4];
};

// CModelCheckDlg dialog

IMPLEMENT_DYNAMIC(CModelCheckDlg, CDialog)

/// <summary>
/// 
/// </summary>
CModelCheckDlg::CModelCheckDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MODELCHECK, pParent)
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
	const wchar_t* rTitle[] = {L"#", L"Entity", L"Attribute", L"Description"};
	
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

		ModelCheckerLog log(m_wndIssueList);
		RDF::CModelChecker::CheckModel(globalIfcModel, &log);

		for (int i = 0; i < 4; i++) {
			m_wndIssueList.SetColumnWidth(i, log.rWidth[i] + 10 * GetSystemMetrics(SM_CXBORDER));
		}
	}
}

void ModelCheckerLog::ReportIssue(RDF::CModelChecker::IssueInfo& issue)
{
	CString id;
	id.Format(L"%I64d", issue.stepId);
	auto item = m_wndList.InsertItem(0, id);
	rWidth[0] = max(rWidth[0], m_wndList.GetStringWidth(id));

	CString entity(issue.entity);
	m_wndList.SetItemText(item, 1, entity);
	rWidth[1] = max(rWidth[1], m_wndList.GetStringWidth(entity));

	CString aggrIndex;
	for (int i = 0; i < issue.aggrLevel; i++) {
		if (aggrIndex.IsEmpty()) {
			aggrIndex.Append(L"[");
		}
		else {
			aggrIndex.Append(L",");
		}

		CString ind;
		ind.Format(L"%I64d", issue.aggrIndArray[i]);
		aggrIndex.Append(ind);
	}
	if (!aggrIndex.IsEmpty()) {
		aggrIndex.Append(L"]");
	}

	CString attr(issue.attrName);
	if (!aggrIndex.IsEmpty())
		attr += aggrIndex;
	m_wndList.SetItemText(item, 2, attr);
	rWidth[2] = max(rWidth[2], m_wndList.GetStringWidth(attr));

	CString text(issue.text);
	m_wndList.SetItemText(item, 3, text);
	rWidth[3] = max(rWidth[3], m_wndList.GetStringWidth(text));
}