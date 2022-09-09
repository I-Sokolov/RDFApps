#pragma once



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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


