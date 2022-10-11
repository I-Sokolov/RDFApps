
// ifcviewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ifcviewer.h"
#include "MainFrm.h"

#include "ifcviewerDoc.h"
#include "ifcviewerView.h"

#include "ifcengine/include/engdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



extern	wchar_t	* ifcSchemaName_IFC2x3_TC1, * ifcSchemaName_IFC4x3;


// CifcviewerApp

BEGIN_MESSAGE_MAP(CifcviewerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CifcviewerApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CifcviewerApp construction

CifcviewerApp::CifcviewerApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ifcviewer.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CifcviewerApp object

CifcviewerApp theApp;


// CifcviewerApp initialization

BOOL CifcviewerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CifcviewerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CifcviewerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.

	//ifcViewer initialization specific: it can not open file before first show winodow, 
	//but need call to ProsessShellCommand before showing window
	CString fileToOpen;
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen) {
		fileToOpen = cmdInfo.m_strFileName;
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
	}
	//
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	if (!fileToOpen.IsEmpty()) {
		return OpenDocumentFile(fileToOpen)!=nullptr;
	}

	return TRUE;
}

int CifcviewerApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CifcviewerApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CifcviewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CifcviewerApp::InitApplication() 
{
	int_t	i = (int_t) wcslen(this->m_pszHelpFilePath) - (int_t) wcslen(this->m_pszExeName) - 4;

	ASSERT(ifcSchemaName_IFC2x3_TC1 == nullptr && ifcSchemaName_IFC4x3 == nullptr && i > 0);

	ifcSchemaName_IFC2x3_TC1 = new wchar_t[i+wcslen(L"IFC2X3_TC1.exp")+1];
	memcpy(&ifcSchemaName_IFC2x3_TC1[0], this->m_pszHelpFilePath, i * sizeof(wchar_t));
	memcpy(&ifcSchemaName_IFC2x3_TC1[i], L"IFC2X3_TC1.exp", (wcslen(L"IFC2X3_TC1.exp") + 1) * sizeof(wchar_t));

	ifcSchemaName_IFC4x3 = new wchar_t[i + wcslen(L"IFC4x3_RC2.exp") + 1];
	memcpy(&ifcSchemaName_IFC4x3[0], this->m_pszHelpFilePath, i * sizeof(wchar_t));
	memcpy(&ifcSchemaName_IFC4x3[i], L"IFC4x3_RC2.exp", (wcslen(L"IFC4x3_RC2.exp") + 1) * sizeof(wchar_t));

	return	CWinApp::InitApplication();
}

CifcviewerApp::~CifcviewerApp()
{
	if (ifcSchemaName_IFC2x3_TC1 != nullptr) {
		delete[] ifcSchemaName_IFC2x3_TC1;
	}
	if (ifcSchemaName_IFC4x3 != nullptr) {
		delete[] ifcSchemaName_IFC4x3;
	}
}

// CifcviewerApp message handlers



