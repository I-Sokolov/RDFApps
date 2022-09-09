// InstanceInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "LeftPane.h"
#include "stringCreation.h"
#include "AttributesView.h"


// CInstanceInfo

IMPLEMENT_DYNCREATE(CAttributesView, CEditView)

CAttributesView::CAttributesView()
{

}

CAttributesView::~CAttributesView()
{
}

BEGIN_MESSAGE_MAP(CAttributesView, CEditView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CInstanceInfo diagnostics

#ifdef _DEBUG
void CAttributesView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CAttributesView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInstanceInfo message handlers

int CAttributesView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetEditCtrl().SetReadOnly();
	//GetEditCtrl().ModifyStyle(0, ES_MULTILINE | ES_READONLY);

	return 0;
}


void CAttributesView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();
}


void CAttributesView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	SetWindowText(L"");

	auto pTree  = GetViewerDoc()->GetModelTreeView();
	if (!pTree) {
		ASSERT(false);
		return;
	}

	auto pInstance = pTree->GetSelectedInstance();
	if (!pInstance) {
		return;
	}

	auto text = CreateToolTipText(pInstance->ifcInstance);
	text.Replace (L"\n", L"\r\n");
	SetWindowText(text);
}




