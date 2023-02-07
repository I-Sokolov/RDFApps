
// ifcviewerDoc.h : interface of the CifcviewerDoc class
//
#pragma once

class CLeftPane;

class CifcviewerDoc : public CDocument
{
protected: // create from serialization only
	CifcviewerDoc();
	DECLARE_DYNCREATE(CifcviewerDoc)

// Attributes
public:
	CWnd* GetPane(CRuntimeClass* pClass);
	CLeftPane* GetModelTreeView();

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CifcviewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	enum class UpdateHint : LPARAM
	{
		SetActiveInstance = 14,
		ValidationResults = 15
	};

	class ActiveInstanceHint : public CObject
	{
	public:
		ActiveInstanceHint(int_t instance) : m_instance(instance) {}
		int_t GetIntstance() { return m_instance; }
	private:
		int_t m_instance;
	};

	class ValidationResultsHint : public CObject
	{
	public:
		ValidationResultsHint(ValidationResults results) : m_results(results) {}
		ValidationResults GetResults() { return m_results; }
	private:
		ValidationResults m_results;
	};

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
