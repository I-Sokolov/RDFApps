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

private:
	CListCtrl m_wndIssueList;
};