// InstanceInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "LeftPane.h"
#include "stringCreation.h"
#include "ifcviewerDoc.h"
#include "AttributesView.h"


// CInstanceInfo

IMPLEMENT_DYNCREATE(CAttributesView, CFormView)

CAttributesView::CAttributesView()
	: CFormView(IDD_PropertiesView)
{

}

CAttributesView::~CAttributesView()
{
}

void CAttributesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROPERTYGRID, m_wndProps);
}


BEGIN_MESSAGE_MAP(CAttributesView, CFormView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CInstanceInfo diagnostics

#ifdef _DEBUG
void CAttributesView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAttributesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInstanceInfo message handlers

int CAttributesView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CAttributesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMFCHeaderCtrl& header = m_wndProps.GetHeaderCtrl();
	ASSERT(2 == header.GetItemCount());

	HDITEMW item;
	memset(&item, 0, sizeof(item));
	item.mask = HDI_TEXT;
	item.pszText = L"Attribute";
	header.SetItem(0, &item);

	m_wndProps.EnableDescriptionArea(false);

	m_wndProps.RemoveAll();

	Invalidate();
}


void CAttributesView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	if (lHint == (LPARAM)CifcviewerDoc::UpdateHint::SetActiveInstance) {				
		auto pInstance = dynamic_cast<CifcviewerDoc::ActiveInstanceHint*> (pHint);

		m_wndProps.RemoveAll();

		if (pInstance) {
			auto instance = pInstance->GetIntstance();

			if (instance) {
				AddStepId(instance);

				int_t	entity = sdaiGetInstanceType(instance);
				ASSERT(entity); if (!entity) return;

				AddEntityName(entity);

				std::set<int_t> visitedEntities;
				AddAttributes(entity, instance, visitedEntities);
			}
		}
	}

	Invalidate();
}

void CAttributesView::AddStepId(int_t instance)
{
	int_t nId = internalGetP21Line(instance);
	CString strId;
	strId.Format(L"%lld", nId);

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(L"#", strId);
	pProp->AllowEdit(false);
	m_wndProps.AddProperty(pProp);
}

void CAttributesView::AddEntityName(int_t entity)
{
	wchar_t* entityName = nullptr;
	engiGetEntityName(entity, sdaiUNICODE, (const char**) &entityName);
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(L"Entity", entityName);
	pProp->AllowEdit(false);
	m_wndProps.AddProperty(pProp);
}

void CAttributesView::AddAttributes(int_t entity, int_t instance, std::set<int_t>& visitedEntities)
{
	if (!visitedEntities.insert(entity).second) {
		return; //already here
	}

	auto NP = engiGetEntityNoParents(entity);
	for (int_t ip = 0; ip < NP; ip++) {
		auto parent = engiGetEntityParentEx(entity, ip);
		AddAttributes(parent, instance, visitedEntities);
	}

	auto NA = engiGetEntityNoAttributesEx(entity, false, true);	
	if (NA) {
		const wchar_t* entityName = nullptr;
		engiGetEntityName(entity, sdaiUNICODE, (const char**) &entityName);

		auto pSet = new CMFCPropertyGridProperty(entityName);
		pSet->AllowEdit(false);

		for (int_t ia = 0; ia < NA; ia++) {
			auto attr = engiGetEntityAttributeByIndex(entity, ia, false, true);

			const char* attrName = nullptr;
			bool inverse = false;
			engiGetAttributeTraits(attr, &attrName, nullptr, &inverse, nullptr, nullptr, nullptr, nullptr, nullptr);

			CString attrIndName;
			if (!inverse) {
				auto ind = engiGetEntityAttributeIndexEx(entity, (const char*) (const wchar_t*) CString(attrName), true, false);
				attrIndName.Format(L"%lld: %hs", ind+1, attrName);
			}
			else {
				attrIndName.Format(L"INV: %hs", attrName);
			}

			auto propType = engiGetAttributeType(attr);
			auto value = NestedPropertyValue(instance, CString(attrName), propType, 0);

			CMFCPropertyGridProperty* pProp(new CMFCPropertyGridProperty(attrIndName, value));
			pProp->AllowEdit(false);
			pSet->AddSubItem(pProp);
		}

		m_wndProps.AddProperty(pSet);
	}
}




