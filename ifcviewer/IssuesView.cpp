// IssuesView.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "IFCEngineInteract.h"
#include "LeftPane.h"
#include "IssuesView.h"


// CIssuesView

IMPLEMENT_DYNCREATE(CIssuesView, CListView)

CIssuesView::CIssuesView()
{

}

CIssuesView::~CIssuesView()
{
}

BEGIN_MESSAGE_MAP(CIssuesView, CListView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CIssuesView diagnostics

#ifdef _DEBUG
void CIssuesView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CIssuesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CIssuesView message handlers


int CIssuesView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	auto& wndList = GetListCtrl();
	
	wndList.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	
	const wchar_t* rTitle[] = {L"Attribute", L"Issue"};
	for (int i = 0; i < _countof(rTitle); i++) {
		GetListCtrl().InsertColumn(i, rTitle[i]);

		auto w = GetListCtrl().GetStringWidth(rTitle[i]);
		GetListCtrl().SetColumnWidth(i, w + 10 * GetSystemMetrics(SM_CXBORDER));
	}

	return 0;
}

struct IssuesCollector : public RDF::CModelChecker::ModelCheckerLog
{
	IssuesCollector(CIssuesView::IssueList& list) : m_list(list) {}

	virtual void ReportIssue(RDF::CModelChecker::IssueInfo& issue) override
	{
		m_list.push_back(CIssuesView::Issue());
		m_list.back().info = issue;
		m_list.back().descr = issue.text;
	}

	CIssuesView::IssueList& m_list;
};


void CIssuesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	m_lstIssues.clear();

	if (globalIfcModel) {
		IssuesCollector log(m_lstIssues);
		RDF::CModelChecker::CheckModel(globalIfcModel, &log);
	}
}


void CIssuesView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	auto& wndList = GetListCtrl();
	wndList.Invalidate();

	wndList.DeleteAllItems();

	auto pTree = GetViewerDoc()->GetModelTreeView();
	if (!pTree) {
		ASSERT(false);
		return;
	}

	auto pInstance = pTree->GetSelectedInstance();
	if (!pInstance) {
		return;
	}

	auto stepid = internalGetP21Line (pInstance->ifcInstance);

	int rWidth[2] = {0,0};
	for (int i = 0; i < 2; i++) {
		rWidth[i] = GetListCtrl().GetColumnWidth(i);
	}

	for (auto& issue : m_lstIssues) {
		if (issue.info.stepId == stepid) {

			CString aggrIndex;
			for (int i = 0; i < issue.info.aggrLevel; i++) {
				if (aggrIndex.IsEmpty()) {
					aggrIndex.Append(L"[");
				}
				else {
					aggrIndex.Append(L",");
				}

				CString ind;
				ind.Format(L"%I64d", issue.info.aggrIndArray[i]);
				aggrIndex.Append(ind);
			}
			if (!aggrIndex.IsEmpty()) {
				aggrIndex.Append(L"]");
			}

			CString attr(issue.info.attrName);
			if (!aggrIndex.IsEmpty())
				attr += aggrIndex;

			auto item = GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), attr);
			rWidth[0] = max(rWidth[0], GetListCtrl().GetStringWidth(attr));

			GetListCtrl().SetItemText(item, 1, issue.descr);
			rWidth[1] = max(rWidth[1], GetListCtrl().GetStringWidth(issue.descr));
		}
	}

	for (int i = 0; i < 2; i++) {
		GetListCtrl().SetColumnWidth(i, rWidth[i] + 10 * GetSystemMetrics(SM_CXBORDER));
	}

	Invalidate();
}
