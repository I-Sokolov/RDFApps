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
	m_wndProps.RemoveAll();
}


void CPropertiesView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	if (lHint == (LPARAM) CifcviewerDoc::UpdateHint::SetActiveInstance) {
		auto pInstance = dynamic_cast<CifcviewerDoc::ActiveInstanceHint*> (pHint);

		m_wndProps.RemoveAll();

		if (pInstance) {
			auto instance = pInstance->GetIntstance();

			if (instance) {
				AddPropertySetsOf(instance);
				AddTypeObjectsOf(instance);
			}
		}
	}

	Invalidate();
}

struct TypeObjectCollector : public RDF::CModelChecker::InstanceVisitor
{
	TypeObjectCollector (CPropertiesView& me, SdaiInstance instance) : m_me(me), m_inst (instance)
	{
		m_entityRelDefinesByType = sdaiGetEntity(globalIfcModel, (char*) L"IFCRELDEFINESBYTYPE");
		m_attrRelatedObjects = sdaiGetAttrDefinition(m_entityRelDefinesByType, (char*) L"RelatedObjects");
		m_attrRelatingType = sdaiGetAttrDefinition(m_entityRelDefinesByType, (char*) L"RelatingType");
	}

	virtual bool OnVisitInstance(SdaiInstance visiting) override
	{
		if (sdaiIsKindOf(visiting, m_entityRelDefinesByType)) {
			SdaiAggr relatedObjects = NULL;
			sdaiGetAttr(visiting, m_attrRelatedObjects, sdaiAGGR, &relatedObjects);
			if (AggregationContainsInstance(relatedObjects, m_inst)) {
				SdaiInstance typeObject = NULL;
				sdaiGetAttr(visiting, m_attrRelatingType, sdaiINSTANCE, &typeObject);
				if (typeObject) {
					m_me.AddTypeObject(typeObject);
				}
			}
		}
		return true;
	}

private:
	CPropertiesView& m_me;
	SdaiInstance	 m_inst;
	SdaiEntity		 m_entityRelDefinesByType;
	void*			 m_attrRelatedObjects;
	void*			 m_attrRelatingType;
};

void CPropertiesView::AddTypeObjectsOf(SdaiInstance instance)
{
	TypeObjectCollector toc(*this, instance);
	RDF::CModelChecker::VisitAllInstances(globalIfcModel, toc);
}

static CString GetInstanceDescription(SdaiInstance instance)
{
	CString description;

	description.Format(L"#%lld", internalGetP21Line(instance));

	const char* typeName = nullptr;
	engiGetEntityName(sdaiGetInstanceType(instance), sdaiSTRING, &typeName);
	if (typeName && *typeName) {
		description += "=";
		description += typeName;
	}

	const wchar_t* descr = nullptr;
	sdaiGetAttrBN(instance, (char*) L"Description", sdaiUNICODE, &descr);

	if (descr && *descr) {
		description += ", ";
		description += descr;
	}

	return description;
}

void CPropertiesView::AddTypeObject(SdaiInstance typeObject)
{
	const wchar_t* typeName = nullptr;
	sdaiGetAttrBN(typeObject, (char*) L"Name", sdaiUNICODE, &typeName);

	CString descr = GetInstanceDescription(typeObject);

	auto pPropTO = new CMFCPropertyGridProperty(typeName);
	pPropTO->SetDescription(descr);

	STRUCT__PROPERTY__SET* propertySets = nullptr;
	CreateTypeObjectProperties(globalIfcModel, &propertySets, typeObject, units);

	if (propertySets) {
		AddPropertySet(propertySets, pPropTO);

		DeletePropertySets(propertySets);
	}

	m_wndProps.AddProperty(pPropTO);
}


void CPropertiesView::AddPropertySetsOf(SdaiInstance instance)
{
	STRUCT__PROPERTY__SET* propertySets = nullptr;
	CreateIfcInstanceProperties(globalIfcModel, &propertySets, instance, units, false);

	if (propertySets) {
		AddPropertySet(propertySets);

		DeletePropertySets(propertySets);
	}
}

void CPropertiesView::AddPropertySet(STRUCT__PROPERTY__SET* propertySet, CMFCPropertyGridProperty* pGroup)
{
	auto pSet = new CMFCPropertyGridProperty(propertySet->name);

	CString description = GetInstanceDescription (propertySet->ifcInstance);
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

		description = GetInstanceDescription (prop->ifcInstance);

		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(prop->name, value, description);
		pProp->AllowEdit(false);

		pSet->AddSubItem(pProp);
	}

	if (pGroup) {
		pGroup->AddSubItem(pSet);
	}
	else {
		m_wndProps.AddProperty(pSet);
	}

	if (propertySet->next) {
		AddPropertySet(propertySet->next);
	}
}
