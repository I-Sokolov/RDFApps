// InstanceInfoPane.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "InstanceInfoPane.h"
#include "InstanceInfo.h"


// CInstanceInfoPane

IMPLEMENT_DYNCREATE(CInstanceInfoPane, CTabView)

CInstanceInfoPane::CInstanceInfoPane()
{

}

CInstanceInfoPane::~CInstanceInfoPane()
{
}

BEGIN_MESSAGE_MAP(CInstanceInfoPane, CTabView)
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


void CInstanceInfoPane::OnInitialUpdate()
{
	CTabView::OnInitialUpdate();

	AddView(RUNTIME_CLASS(CInstanceInfo), L"#");
	AddView(RUNTIME_CLASS(CEditView), L"IFC Properties");
	AddView(RUNTIME_CLASS(CEditView), L"IFC Relationships");
	AddView(RUNTIME_CLASS(CEditView), L"Issues");
}
