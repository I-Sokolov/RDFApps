#pragma once


// CIssuesView view

class CIssuesView : public CListView
{
	DECLARE_DYNCREATE(CIssuesView)

protected:
	CIssuesView();           // protected constructor used by dynamic creation
	virtual ~CIssuesView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


