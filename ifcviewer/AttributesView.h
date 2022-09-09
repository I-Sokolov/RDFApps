#pragma once

#include "ifcviewerDoc.h"

// CInstanceInfo view

class CAttributesView : public CEditView
{
	DECLARE_DYNCREATE(CAttributesView)

protected:
	CAttributesView();           // protected constructor used by dynamic creation
	virtual ~CAttributesView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	CifcviewerDoc* GetViewerDoc() { return DYNAMIC_DOWNCAST(CifcviewerDoc, GetDocument()); }

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


