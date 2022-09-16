#pragma once


// CModelCheckDlg dialog

class CModelCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CModelCheckDlg)

public:
	CModelCheckDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CModelCheckDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODELCHECK };
#endif

	void OnNewModel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	void FormatIssueList();
	void FillIssueList();

	static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamMe);

	void OnActivateListItem(int iItem);

private:
	CListCtrl m_wndIssueList;

	int       m_nSortColumn;
	bool      m_bSortAscending;

public:
	afx_msg void OnDeleteitemIssuelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickIssuelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkIssuelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickIssuelist(NMHDR* pNMHDR, LRESULT* pResult);
};

