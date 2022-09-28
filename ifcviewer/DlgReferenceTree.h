#pragma once


// CDlgReferenceTree dialog

struct TreeItemData;

class CDlgReferenceTree : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgReferenceTree)

public:
	CDlgReferenceTree(int_t instance, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgReferenceTree();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REFERENCEVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void CheckInsertReferencingInstance(int_t instance, int_t referencedInstance, HTREEITEM hParent);

private:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	afx_msg void OnDeleteitemReferenceTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingReferenceTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedReferenceTree(NMHDR* pNMHDR, LRESULT* pResult);

private:
	void InsertTreeItem(int_t instance, int_t* aggregation, int_t attr, bool inverseReference, HTREEITEM hParent);
	void InsertReferencedInstances(HTREEITEM hItem, TreeItemData* pData);
	void InsertReferencingInstances(HTREEITEM hItem, TreeItemData* pData);
	void InsertAggregationElements(HTREEITEM hItem, TreeItemData* pData);

	bool AggregationContainsInstance(int_t* aggregation, int_t checkInstance = 0);

	HTREEITEM FindParentWithInstance(HTREEITEM hParent, int_t instance);


private:
	CTreeCtrl	m_wndTree;
	CImageList	m_refDir;
	int_t		m_rootInstance;

};
