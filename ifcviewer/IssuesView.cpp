// IssuesView.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
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
