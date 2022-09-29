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
	
	const wchar_t* rTitle[] = {L"#", L"Entity", L"Attribute", L"Issue"};
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

		for (int_t i = 0; i < issue.aggrLevel; i++) {
			m_list.back().arrgegation.push_back(issue.aggrIndArray[i]);
		}
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


void CIssuesView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	int_t instance = 0;
	if (lHint == (LPARAM) CifcviewerDoc::UpdateHint::SetActiveInstance) {
		auto pInstance = dynamic_cast<CifcviewerDoc::ActiveInstanceHint*> (pHint);
		if (pInstance) {
			instance = pInstance->GetIntstance();
		}
	}
	if (!instance)
		return;

	auto& wndList = GetListCtrl();
	wndList.DeleteAllItems();

	//auto stepid = internalGetP21Line (pInstance->ifcInstance);

	int rWidth[4] = {0,0,0,0};
	for (int i = 0; i < 4; i++) {
		rWidth[i] = GetListCtrl().GetColumnWidth(i);
	}

	for (auto& issue : m_lstIssues) {

		if (issue.RelatedInstances().find(instance) != issue.RelatedInstances().end()) {

			CString id;
			id.Format(L"%lld", issue.info.stepId);
			auto item = GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), id);
			rWidth[0] = max(rWidth[0], GetListCtrl().GetStringWidth(id));

			CString entity(issue.info.entity);
			GetListCtrl().SetItemText(item, 1, entity);
			rWidth[1] = max(rWidth[1], GetListCtrl().GetStringWidth(entity));


			CString aggrIndex;
			for (auto i : issue.arrgegation) {
				if (aggrIndex.IsEmpty()) {
					aggrIndex.Append(L"[");
				}
				else {
					aggrIndex.Append(L",");
				}

				CString ind;
				ind.Format(L"%I64d", i);
				aggrIndex.Append(ind);
			}
			if (!aggrIndex.IsEmpty()) {
				aggrIndex.Append(L"]");
			}

			CString attr(issue.info.attrName);
			if (!aggrIndex.IsEmpty())
				attr += aggrIndex;

			GetListCtrl().SetItemText (item, 2, attr);
			rWidth[2] = max(rWidth[2], GetListCtrl().GetStringWidth(attr));

			GetListCtrl().SetItemText(item, 3, issue.descr);
			rWidth[3] = max(rWidth[3], GetListCtrl().GetStringWidth(issue.descr));
		}
	}

	for (int i = 0; i < 4; i++) {
		GetListCtrl().SetColumnWidth(i, rWidth[i] + 15 * GetSystemMetrics(SM_CXBORDER));
	}

	wndList.Invalidate();
	Invalidate();
}



/// <summary>
/// 


/// <summary>
/// 
/// </summary>
const std::set<int_t>& CIssuesView::Issue::RelatedInstances()
{
	if (!m_relatedInsancesCollected) {
		m_relatedInsancesCollected = true;

		auto instance = internalGetInstanceFromP21Line(globalIfcModel, info.stepId);
		ASSERT(instance);
		if (instance) {

			int_t searchEntities[3] = {
			sdaiGetEntity(globalIfcModel, (char*) L"IfcProduct"),
			sdaiGetEntity(globalIfcModel, (char*) L"IfcProject"),
			0};

			RDF::CModelChecker::CollectReferencingInstancesRecirsive(m_relatedInstances, instance, searchEntities);
		}
	}

	return m_relatedInstances;
}