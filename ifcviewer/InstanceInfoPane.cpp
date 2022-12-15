// InstanceInfoPane.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "InstanceInfoPane.h"
#include "AttributesView.h"
#include "IssuesView.h"
#include "PropertiesView.h"


// CInstanceInfoPane

IMPLEMENT_DYNCREATE(CInstanceInfoPane, CTabView)

CInstanceInfoPane::CInstanceInfoPane()
{

}

CInstanceInfoPane::~CInstanceInfoPane()
{
}

BEGIN_MESSAGE_MAP(CInstanceInfoPane, CTabView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CInstanceInfoPane diagnostics

#ifdef _DEBUG
void CInstanceInfoPane::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CInstanceInfoPane::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInstanceInfoPane message handlers

int CInstanceInfoPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddView(RUNTIME_CLASS(CAttributesView), L"Attributes");
	AddView(RUNTIME_CLASS(CPropertiesView), L"IFC Properties");
	
	m_nIssueView = -1;	
	
	// Nicely hack to access protected member
	class CMFCTabCtrlEx : public CMFCTabCtrl
	{
	public:
		void SetDisableScroll() { m_bScroll = FALSE; }
	};

	// One-Liner to Disable navigation control
	((CMFCTabCtrlEx*) &GetTabControl())->SetDisableScroll();

	return 0;
}

void CInstanceInfoPane::OnInitialUpdate()
{
	CTabView::OnInitialUpdate();

	if (m_nIssueView >= 0) {
		RemoveView(m_nIssueView);
		m_nIssueView = -1;
	}

	Invalidate();
}




void CInstanceInfoPane::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint == (LPARAM)CifcviewerDoc::UpdateHint::ValidationResults && pSender != this) {

		if (m_nIssueView >= 0) {
			ASSERT(false);
			RemoveView(m_nIssueView);
			m_nIssueView = -1;
		}

		auto pResults = dynamic_cast<CifcviewerDoc::ValidationResultsHint*> (pHint);

		if (pResults) {
			m_nIssueView = AddView(RUNTIME_CLASS(CIssuesView), L"Issues");
			GetDocument()->UpdateAllViews(this, lHint, pHint);
		}
		else {
			ASSERT(false);
		}

		Invalidate();
	}
}
