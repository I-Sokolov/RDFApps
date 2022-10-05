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
	void CheckInsertReferencingInstance(int_t instance, int_t referencedInstance, HTREEITEM hParent, int& childCounter, int childLimit);

private:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	afx_msg void OnDeleteitemReferenceTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingReferenceTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkReferenceTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedReferenceTree(NMHDR* pNMHDR, LRESULT* pResult);

private:
	void InsertRegularItem(int_t instance, int_t* aggregation, void* attr, bool inverseReference, HTREEITEM hParent);
	void InsertAddMoreItem(HTREEITEM hParent);

	int AddChildItemsWithNewLimit(HTREEITEM hItem, TreeItemData& data);
	void IncreaseChildLimit(HTREEITEM hAddItem);

	void InsertReferencedInstances(HTREEITEM hItem, TreeItemData& data);
	void InsertReferencingInstances(HTREEITEM hItem, TreeItemData& data);
	void InsertAggregationElements(HTREEITEM hItem, TreeItemData& data);

	HTREEITEM FindParentWithInstance(HTREEITEM hParent, int_t instance);


private:
	CTreeCtrl	m_wndTree;
	CImageList	m_refDir;
	int_t		m_rootInstance;

public:
	afx_msg void OnGetinfotipReferenceTree(NMHDR* pNMHDR, LRESULT* pResult);
};
