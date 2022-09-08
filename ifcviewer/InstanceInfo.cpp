// InstanceInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "LeftPane.h"
#include "InstanceInfo.h"


// CInstanceInfo

IMPLEMENT_DYNCREATE(CInstanceInfo, CEditView)

CInstanceInfo::CInstanceInfo()
{

}

CInstanceInfo::~CInstanceInfo()
{
}

BEGIN_MESSAGE_MAP(CInstanceInfo, CEditView)
END_MESSAGE_MAP()


// CInstanceInfo diagnostics

#ifdef _DEBUG
void CInstanceInfo::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CInstanceInfo::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInstanceInfo message handlers

void CInstanceInfo::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	GetEditCtrl().SetReadOnly();
}


void CInstanceInfo::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	SetWindowText(L"");

	auto pTree  = GetViewerDoc()->GetModelTreeView();
	if (!pTree) {
		ASSERT(false);
		return;
	}

	auto pItem = pTree->GetSelectedInstance();
	if (!pItem) {
		return;
	}

	SetWindowText(pItem->nameBuffer);
}


