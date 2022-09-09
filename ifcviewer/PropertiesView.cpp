// PropertiesView.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "PropertiesView.h"


// CPropertiesView

IMPLEMENT_DYNCREATE(CPropertiesView, CFormView)

CPropertiesView::CPropertiesView()
	: CFormView(IDD_PropertiesView)
{

}

CPropertiesView::~CPropertiesView()
{
}

void CPropertiesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPropertiesView, CFormView)
END_MESSAGE_MAP()


// CPropertiesView diagnostics

#ifdef _DEBUG
void CPropertiesView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPropertiesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPropertiesView message handlers
