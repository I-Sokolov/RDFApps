#pragma once

#include "ifcviewerDoc.h"

// CIssuesView view

class CIssuesView : public CListView
{
	DECLARE_DYNCREATE(CIssuesView)

public:
	struct Issue
	{
		RDF::CModelChecker::IssueInfo	info;
		CString							descr;
	};

	typedef std::list<Issue> IssueList;

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

	CifcviewerDoc* GetViewerDoc() { return DYNAMIC_DOWNCAST(CifcviewerDoc, GetDocument()); }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

private:
	IssueList m_lstIssues;
};


