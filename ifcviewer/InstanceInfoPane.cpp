// InstanceInfoPane.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "InstanceInfoPane.h"
#include "AttributesView.h"


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
	AddView(RUNTIME_CLASS(CEditView), L"IFC Properties");
	AddView(RUNTIME_CLASS(CEditView), L"IFC Relationships");
	AddView(RUNTIME_CLASS(CEditView), L"Issues");

	return 0;
}

void CInstanceInfoPane::OnInitialUpdate()
{
	CTabView::OnInitialUpdate();
}


