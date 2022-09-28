// PropertiesView.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "LeftPane.h"
#include "IFCEngineInteract.h"
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


void CPropertiesView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	if (lHint == (LPARAM) CifcviewerDoc::UpdateHint::SetActiveInstance) {
		auto pInstance = DYNAMIC_DOWNCAST(CifcviewerDoc::ActiveInstanceHint, pHint);

		if (pInstance) {
			auto instance = pInstance->GetIntstance();

			if (instance) {
				m_wndProps.RemoveAll();

				STRUCT__PROPERTY__SET* propertySets = nullptr;
				CreateIfcInstanceProperties(globalIfcModel, &propertySets, instance, units);
				if (propertySets) {
					AddPropertySet(propertySets);

					DeleteIfcInstanceProperties(propertySets);
				}

				Invalidate();
			}
		}
	}
}

void CPropertiesView::AddPropertySet(STRUCT__PROPERTY__SET* propertySet)
{
	auto pSet = new CMFCPropertyGridProperty(propertySet->name);

	CString description;
	description.Format(L"#%lld", internalGetP21Line(propertySet->ifcInstance));

	const char* typeName = nullptr;
	engiGetEntityName(sdaiGetInstanceType(propertySet->ifcInstance), sdaiSTRING, &typeName);
	if (typeName && *typeName) {
		description += "=";
		description += typeName;
	}

	if (propertySet->description && *propertySet->description) {
		description += ", ";
		description += propertySet->description;
	}

	pSet->SetDescription(description);

	for (auto prop = propertySet->properties; prop; prop = prop->next) {

		CString value;
		if (prop->nominalValue)
			value += prop->nominalValue;
		if (prop->lengthValue)
			value += prop->lengthValue;
		if (prop->areaValue)
			value += prop->areaValue;
		if (prop->volumeValue)
			value += prop->volumeValue;
		if (prop->countValue)
			value += prop->countValue;
		if (prop->weigthValue)
			value += prop->weigthValue;
		if (prop->timeValue)
			value += prop->timeValue;

		if (prop->unitName && *prop->unitName) {
			value += " ";
			value += prop->unitName;
		}

		description.Format(L"#%lld", internalGetP21Line(prop->ifcInstance));

		typeName = nullptr;
		engiGetEntityName(sdaiGetInstanceType(prop->ifcInstance), sdaiSTRING, &typeName);
		if (typeName && *typeName) {
			description += "=";
			description += typeName;
		}

		if (prop->description && *prop->description) {
			description += ", ";
			description += prop->description;
		}

		CMFCPropertyGridProperty* pProp(new CMFCPropertyGridProperty(prop->name, value, description));

		pProp->AllowEdit(false);
		pSet->AddSubItem(pProp);
	}

	m_wndProps.AddProperty(pSet);

	if (propertySet->next) {
		AddPropertySet(propertySet->next);
	}
}
