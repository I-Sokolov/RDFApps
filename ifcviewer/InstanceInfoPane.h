#pragma once


// CInstanceInfoPane view

class CInstanceInfoPane : public CTabView
{
	DECLARE_DYNCREATE(CInstanceInfoPane)

protected:
	CInstanceInfoPane();           // protected constructor used by dynamic creation
	virtual ~CInstanceInfoPane();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


