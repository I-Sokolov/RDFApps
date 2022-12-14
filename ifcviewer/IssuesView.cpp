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


void CIssuesView::GetIssues(ValidationResults* results)
{
	auto issue = validateGetFirstIssue(results);
	while (issue) {

		m_lstIssues.push_back(CIssuesView::Issue());
		auto& i = m_lstIssues.back();
		
		i.issueId = GetIssueId(issue);
		i.stepId = GetStepId(issue);
		i.entityName = GetEntityName(issue);
		i.attrName = GetAttrName(issue);
		i.attrIndex = GetAttrIndex(issue);
		
		auto aggrLevel = validateGetAggrLevel(issue);
		auto aggrIndArray = validateGetAggrIndArray(issue);
		for (int_t j = 0; j < aggrLevel; j++) {
			i.arrgegation.push_back(aggrIndArray[j]);
		}

		i.level = validateGetIssueLevel(issue);
		i.text = validateGetDescription(issue);

		issue = validateGetNextIssue(issue);
	}
}

void CIssuesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	auto& wndList = GetListCtrl();
	wndList.DeleteAllItems();

	m_lstIssues.clear();

	if (globalIfcModel) {
		validateSetOptions(1, -1, false, 0, 0);
		auto checkRes = validateModel(globalIfcModel);
		GetIssues(checkRes);
		validateFreeResults(checkRes);
	}
}


void CIssuesView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	auto& wndList = GetListCtrl();

	int_t instance = 0;
	if (lHint == (LPARAM) CifcviewerDoc::UpdateHint::SetActiveInstance) {
		auto pInstance = dynamic_cast<CifcviewerDoc::ActiveInstanceHint*> (pHint);

		wndList.DeleteAllItems();

		if (pInstance) {
			instance = pInstance->GetIntstance();
		}
	}

	if (!instance) {
		Invalidate();
		return;
	}

	//auto stepid = internalGetP21Line (pInstance->ifcInstance);

	int rWidth[4] = {0,0,0,0};
	for (int i = 0; i < 4; i++) {
		rWidth[i] = GetListCtrl().GetColumnWidth(i);
	}

	for (auto& issue : m_lstIssues) {

		if (issue.RelatedInstances().find(instance) != issue.RelatedInstances().end()) {

			CString id;
			id.Format(L"%lld", issue.stepId);
			auto item = GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), id);
			rWidth[0] = max(rWidth[0], GetListCtrl().GetStringWidth(id));

			CString entity(issue.entityName);
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

			CString attr(issue.attrName);
			if (!aggrIndex.IsEmpty())
				attr += aggrIndex;

			GetListCtrl().SetItemText (item, 2, attr);
			rWidth[2] = max(rWidth[2], GetListCtrl().GetStringWidth(attr));

			GetListCtrl().SetItemText(item, 3, issue.text);
			rWidth[3] = max(rWidth[3], GetListCtrl().GetStringWidth(issue.text));
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
/// </summary>
const std::set<int_t>& CIssuesView::Issue::RelatedInstances()
{
	if (!m_relatedInsancesCollected) {
		m_relatedInsancesCollected = true;

		auto instance = internalGetInstanceFromP21Line(globalIfcModel, stepId);
		ASSERT(instance);
		if (instance) {

			int_t searchEntities[3] = {
			sdaiGetEntity(globalIfcModel, (char*) L"IfcProduct"),
			sdaiGetEntity(globalIfcModel, (char*) L"IfcProject"),
			0};

			CollectReferencingInstancesRecursive(m_relatedInstances, instance, searchEntities);
		}
	}

	return m_relatedInstances;
}