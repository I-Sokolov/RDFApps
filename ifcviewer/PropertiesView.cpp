// PropertiesView.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "LeftPane.h"
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
	DDX_Control(pDX, IDC_PROPERTYGRID, m_wndProps);
}

BEGIN_MESSAGE_MAP(CPropertiesView, CFormView)
	ON_WM_CREATE()
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


int CPropertiesView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CPropertiesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}


void CPropertiesView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	m_wndProps.RemoveAll();

	auto pTree = GetViewerDoc()->GetModelTreeView();
	if (!pTree) {
		ASSERT(false);
		return;
	}

	auto pInstance = pTree->GetSelectedInstance();
	if (!pInstance) {
		return;
	}

	STRUCT__PROPERTY__SET* propertySets = nullptr;
	CreateIfcInstanceProperties(pInstance->ifcModel, &propertySets, pInstance->ifcInstance, units);
	if (propertySets) {
		AddPropertySet(propertySets);

		DeleteIfcInstanceProperties(propertySets);
	}

	Invalidate();
}

void CPropertiesView::AddPropertySet(STRUCT__PROPERTY__SET* propertySet)
{
	auto pSet = new CMFCPropertyGridProperty(propertySet->name);

	for (auto prop = propertySet->properties; prop; prop = prop->next) {
		CMFCPropertyGridProperty* pProp(new CMFCPropertyGridProperty(prop->name, L"TODO: add value", L"TODO: add details"));
		pSet->AddSubItem(pProp);
	}

	m_wndProps.AddProperty(pSet);

	if (propertySet->next) {
		AddPropertySet(propertySet->next);
	}
}
