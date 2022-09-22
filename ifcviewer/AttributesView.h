#pragma once

#include "ifcviewerDoc.h"

// CInstanceInfo view

class CAttributesView : public CFormView
{
	DECLARE_DYNCREATE(CAttributesView)

protected:
	CAttributesView();           // protected constructor used by dynamic creation
	virtual ~CAttributesView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PropertiesView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	CifcviewerDoc* GetViewerDoc() { return DYNAMIC_DOWNCAST(CifcviewerDoc, GetDocument()); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

private:
	void AddStepId(int_t instance);
	void AddEntityName(int_t entity);
	void AddAttributes(int_t entity, int_t instance, std::set<int_t>& visitedEntities);

private:
	CMFCPropertyGridCtrl m_wndProps;
};


