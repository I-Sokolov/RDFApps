
// ifcviewerView.cpp : implementation of the CifcviewerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ifcviewer.h"
#endif

#include "ifcviewerDoc.h"
#include "ifcviewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CifcviewerView

IMPLEMENT_DYNCREATE(CifcviewerView, CView)

BEGIN_MESSAGE_MAP(CifcviewerView, CView)
END_MESSAGE_MAP()

// CifcviewerView construction/destruction

CifcviewerView::CifcviewerView()
{
	// TODO: add construction code here

}

CifcviewerView::~CifcviewerView()
{
}

BOOL CifcviewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CifcviewerView drawing

void CifcviewerView::OnDraw(CDC* /*pDC*/)
{
	CifcviewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CifcviewerView diagnostics

#ifdef _DEBUG
void CifcviewerView::AssertValid() const
{
	CView::AssertValid();
}

void CifcviewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CifcviewerDoc* CifcviewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CifcviewerDoc)));
	return (CifcviewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CifcviewerView message handlers
