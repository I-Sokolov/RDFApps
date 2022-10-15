#pragma once

#include "ifcviewerDoc.h"

// CIssuesView view

class CIssuesView : public CListView
{
	DECLARE_DYNCREATE(CIssuesView)

public:
	struct Issue
	{
		RDF::ModelChecker::IssueID		issueId;
		int64_t							stepId;
		CString							entityName;
		CString							attrName;
		int_t							attrIndex;
		std::list<int_t>				arrgegation;
		RDF::ModelChecker::ErrorLevel	level;
		CString							text;

		const std::set<int_t>& RelatedInstances();

	private:
		bool                            m_relatedInsancesCollected = false;
		std::set<int_t>                 m_relatedInstances;
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
	void GetIssues(RDF::ModelChecker::CheckResults* results);

private:
	IssueList						 m_lstIssues;
};


