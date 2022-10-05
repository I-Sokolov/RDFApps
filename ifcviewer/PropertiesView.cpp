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
				AddRelationshipsOf(instance);
			}
		}
	}

	Invalidate();
}

struct RelationshipsVisitor : public RDF::CModelChecker::InstanceVisitor
{
	RelationshipsVisitor(CPropertiesView& me, SdaiInstance instance) : m_me(me), m_inst(instance)
	{
		m_entityRelDefinesByType = sdaiGetEntity(globalIfcModel, (char*) L"IFCRELDEFINESBYTYPE");
		m_attrRelatedObjectsDBT = sdaiGetAttrDefinition(m_entityRelDefinesByType, (char*) L"RelatedObjects");
		m_attrRelatingType = sdaiGetAttrDefinition(m_entityRelDefinesByType, (char*) L"RelatingType");

		m_entityRelAssociatesClassification = sdaiGetEntity(globalIfcModel, (char*) L"IfcRelAssociatesClassification");
		m_attrRelatedObjectsAC = sdaiGetAttrDefinition(m_entityRelAssociatesClassification, (char*) L"RelatedObjects");
		m_attrRelatingClassification = sdaiGetAttrDefinition(m_entityRelAssociatesClassification, (char*) L"RelatingClassification");
	}

	SdaiInstance GetRelating(SdaiInstance visiting, SdaiEntity relEntyity, void* attrRelated, void* attrRelating)
	{
		SdaiInstance relating = NULL;

		if (sdaiIsKindOf(visiting, relEntyity)) {
			SdaiAggr relatedObjects = NULL;
			sdaiGetAttr(visiting, attrRelated, sdaiAGGR, &relatedObjects);
			if (AggregationContainsInstance(relatedObjects, m_inst)) {
				sdaiGetAttr(visiting, attrRelating, sdaiINSTANCE, &relating);
			}
		}
		
		return relating;
	}

	virtual bool OnVisitInstance(SdaiInstance visiting) override
	{
		auto relating = GetRelating(visiting, m_entityRelDefinesByType, m_attrRelatedObjectsDBT, m_attrRelatingType);
		if (relating) {
			m_me.AddTypeObject(relating);
			return true;
		}

		relating = GetRelating(visiting, m_entityRelAssociatesClassification, m_attrRelatedObjectsAC, m_attrRelatingClassification);
		if (relating) {
			m_me.AddClassification(relating);
			return true;
		}

		return true;
	}

private:
	CPropertiesView& m_me;
	SdaiInstance	 m_inst;

	SdaiEntity		 m_entityRelDefinesByType;
	void*			 m_attrRelatedObjectsDBT;
	void*			 m_attrRelatingType;

	SdaiEntity		 m_entityRelAssociatesClassification;
	void*			 m_attrRelatedObjectsAC;
	void*		     m_attrRelatingClassification;
};

void CPropertiesView::AddRelationshipsOf(SdaiInstance instance)
{
	RelationshipsVisitor v(*this, instance);
	RDF::CModelChecker::VisitAllInstances(globalIfcModel, v);
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

void CPropertiesView::AddClassification(SdaiInstance classificationSelect)
{
	const wchar_t* clsName = nullptr;
	sdaiGetAttrBN(classificationSelect, (char*) L"Name", sdaiUNICODE, &clsName);

	CString descr = GetInstanceDescription(classificationSelect);

	auto pPropClassif = new CMFCPropertyGridProperty(clsName);
	pPropClassif->SetDescription(descr);

	m_wndProps.AddProperty(pPropClassif);
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
