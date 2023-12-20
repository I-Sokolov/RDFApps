#pragma once
#include "afxdialogex.h"


// CIDSCheckDlg dialog

class CIDSCheckDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIDSCheckDlg)

public:
	CIDSCheckDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CIDSCheckDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IDSCHECKER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
	virtual BOOL OnInitDialog();
};
