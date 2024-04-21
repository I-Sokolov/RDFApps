// IDSCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ifcviewer.h"
#include "afxdialogex.h"
#include "IDSCheckDlg.h"
#include "IDS.h"
#include "PSDChecker.h"
#include "IFCEngineInteract.h"


class IDSConsole : public RDF::IDS::Console
{
public:
	IDSConsole (CString& text):m_text(text){}

	virtual void out(const char* sz) override
	{
		m_text.Append(CString(sz));
	}

	CString& m_text;
};

struct PSDConsole : public RDF::PSD::Console
{
	PSDConsole (CString& text) : m_text(text){}
	
	virtual void writeString(const char* msg) override
	{
		m_text.Append(CString(msg));
		m_text.Append(L"\r\n");
	}

	CString& m_text;
};

// CIDSCheckDlg dialog

IMPLEMENT_DYNAMIC(CIDSCheckDlg, CDialogEx)

CIDSCheckDlg::CIDSCheckDlg(Check check, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IDSCHECKER, pParent)
	, m_strText(_T(""))
	, m_check (check)
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

	bool show = true;

	if (globalIfcModel) {

		switch (m_check) {
			case Check::IDS:
				show = DoIDS(globalIfcModel, m_strText);
				break;
			case Check::PSD:
				show = DoPSD(globalIfcModel, m_strText);
				break;
		}
	}
	else {
		m_strText = "No open model to check";
	}


	if (show) {
		UpdateData(FALSE);
	}
	else {
		EndDialog(IDCANCEL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool CIDSCheckDlg::DoIDS(SdaiModel model, CString& log)
{
	const TCHAR szFilter[] = _T("IDS (*.ids)|*.ids||");
	CFileDialog dlg(TRUE, _T("ids"), NULL, OFN_FILEMUSTEXIST | OFN_READONLY, szFilter, this);
	if (dlg.DoModal() != IDOK) {
		return false;
	}

	CString idsFilePath = dlg.GetPathName();

	RDF::IDS::File ids;
	if (ids.Read(idsFilePath)) {
		IDSConsole output(log);

		bool ok = ids.Check(model, false, RDF::IDS::MsgLevel::All, &output);

		CString res;
		res.Format(L"Result: %s\r\n\r\n", ok ? L"OK" : L"FAIL");

		log.Insert(0, res);
	}
	else {
		log.Format(L"Failed to read IDS file : %s", idsFilePath.GetString());
	}

	return true;
}

bool CIDSCheckDlg::DoPSD(SdaiModel model, CString& log)
{
	CString exeFile;
	AfxGetModuleFileName(NULL, exeFile);

	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[4 * _MAX_PATH];
	wchar_t fname[_MAX_PATH];
	wchar_t ext[_MAX_PATH];
	_wsplitpath_s(exeFile, drive, dir, fname, ext);

	wchar_t psdPath[4 * MAX_PATH];
	_wmakepath_s(psdPath, drive, dir, L"PSD", NULL);
	 
	if (0!=_waccess(psdPath, 0)) {
		log.Format(L"ERROR. Folder does not exist: %s\n\nCopy PSD with subfolder for each schema to this location", psdPath);
		AfxMessageBox(log, MB_ICONSTOP);
		return false;
	}

	CStringA pathUTF8(psdPath);

	PSDConsole console(log);
	RDF::PSD::Checker psd(pathUTF8, &console);
	
	psd.Check(model);	
	log.Insert(0, L"\r\n\r\n");

	auto err = psd.GetErrorLevel();
	switch (err) {
		case RDF::PSD::ErrorLevel::OK:
			log.Insert(0, L"No issues found");
			break;
		case RDF::PSD::ErrorLevel::Warning:
			log.Insert(0, L"There are warnings");
			break;
		case RDF::PSD::ErrorLevel::Error:
			log.Insert(0, L"There are errors");
			break;
		default:
			ASSERT(FALSE);
	}
	return true;
}
