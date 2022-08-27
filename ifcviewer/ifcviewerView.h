
// ifcviewerView.h : interface of the CifcviewerView class
//

#pragma once


class CifcviewerView : public CView
{
protected: // create from serialization only
	CifcviewerView();
	DECLARE_DYNCREATE(CifcviewerView)

// Attributes
public:
	CifcviewerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CifcviewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ifcviewerView.cpp
inline CifcviewerDoc* CifcviewerView::GetDocument() const
   { return reinterpret_cast<CifcviewerDoc*>(m_pDocument); }
#endif

