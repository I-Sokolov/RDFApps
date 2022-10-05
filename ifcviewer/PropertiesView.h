#pragma once

#include "ifcviewerDoc.h"

struct STRUCT__PROPERTY__SET;

// CPropertiesView form view

class CPropertiesView : public CFormView
{
	DECLARE_DYNCREATE(CPropertiesView)

protected:
	CPropertiesView();           // protected constructor used by dynamic creation
	virtual ~CPropertiesView();

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

public:
	void AddTypeObject(SdaiInstance typeObject);
	void AddClassification(SdaiInstance classificationSelect);

private:
	void AddRelationshipsOf(SdaiInstance instance);

	void AddPropertySetsOf(SdaiInstance instance);
	void AddPropertySet(STRUCT__PROPERTY__SET* propertySets, CMFCPropertyGridProperty* pGroup = NULL);

private:
	CMFCPropertyGridCtrl m_wndProps;
};


