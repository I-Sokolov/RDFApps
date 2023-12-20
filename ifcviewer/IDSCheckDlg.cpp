// IDSCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "afxdialogex.h"
#include "IDSCheckDlg.h"
#include "IDS.h"
#include "IFCEngineInteract.h"


class Console : public RDF::IDS::Console
{
public:
	Console (CString& text):m_text(text){}

	virtual void out(const char* sz) override
	{
		m_text.Append(CString(sz));
	}

	CString& m_text;
};

// CIDSCheckDlg dialog

IMPLEMENT_DYNAMIC(CIDSCheckDlg, CDialogEx)

CIDSCheckDlg::CIDSCheckDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IDSCHECKER, pParent)
	, m_strText(_T(""))
{

}

CIDSCheckDlg::~CIDSCheckDlg()
{
}

void CIDSCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_strText);
}


BEGIN_MESSAGE_MAP(CIDSCheckDlg, CDialogEx)
END_MESSAGE_MAP()


// CIDSCheckDlg message handlers


BOOL CIDSCheckDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	bool ok = false;

	if (globalIfcModel) {

		const TCHAR szFilter[] = _T("IDS (*.ids)|*.ids||");
		CFileDialog dlg(TRUE, _T("ids"), NULL, OFN_FILEMUSTEXIST | OFN_READONLY, szFilter, this);
		if (dlg.DoModal() != IDOK) {
			EndDialog(IDCANCEL);
			return TRUE;
		}

		CString idsFilePath = dlg.GetPathName();

		RDF::IDS::File ids;
		if (ids.Read(idsFilePath)) {
			Console output(m_strText);
			
			ok = ids.Check(globalIfcModel, false, RDF::IDS::MsgLevel::All, &output);			
		}
		else {
			m_strText.Format(L"Failed to read IDS file : %s", idsFilePath.GetString());
		}
	}
	else {
		m_strText = "No open model to check";
	}

	CString res;
	res.Format(L"Result: %s\r\n\r\n", ok ? L"OK" : L"FAIL");

	m_strText = res + m_strText;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
