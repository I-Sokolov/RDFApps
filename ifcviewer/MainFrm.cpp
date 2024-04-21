#include "stdafx.h"
#include "ifcviewer.h"

#include "MainFrm.h"
#include "LeftPane.h"
#include "RightPane.h"
#include "InstanceInfoPane.h"
#include "ifcviewerDoc.h"
#include "IDSCheckDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern	STRUCT__IFC__OBJECT	* highLightedIfcObject;

bool	showFaces = true, showWireFrame = true, showLines = true, showPoints = true, enableOnOver = true;

MOUSE_BEHAVIOUR		mouseBehaviour = MB_DEFAULT;

//		14, 15, 16, 17, 18, 19
//
//		 0   0   0   0   0   0		ENUM_ENCODING_DEFAULT
//		 0   0   1   0   0   0		ENUM_ENCODING_WINDOWS_1250
//		 0   0   0   1   0   0		ENUM_ENCODING_WINDOWS_1251
//		 0   0   1   1   0   0		ENUM_ENCODING_WINDOWS_1252
//		 0   0   0   0   1   0		ENUM_ENCODING_WINDOWS_1253
//		 0   0   1   0   1   0		ENUM_ENCODING_WINDOWS_1254
//		 0   0   0   1   1   0		ENUM_ENCODING_WINDOWS_1255
//		 0   0   1   1   1   0		ENUM_ENCODING_WINDOWS_1256
//		 0   0   0   0   0   1		ENUM_ENCODING_WINDOWS_1257
//		 0   0   1   0   0   1		ENUM_ENCODING_WINDOWS_1258
//		 1   0   0   0   0   0		ENUM_ENCODING_ISO8859_1
//		 1   0   1   0   0   0		ENUM_ENCODING_ISO8859_2
//		 1   0   0   1   0   0		ENUM_ENCODING_ISO8859_3
//		 1   0   1   1   0   0		ENUM_ENCODING_ISO8859_4
//		 1   0   0   0   1   0		ENUM_ENCODING_ISO8859_5
//		 1   0   1   0   1   0		ENUM_ENCODING_ISO8859_6
//		 1   0   0   1   1   0		ENUM_ENCODING_ISO8859_7
//		 1   0   1   1   1   0		ENUM_ENCODING_ISO8859_8
//		 1   0   0   0   0   1		ENUM_ENCODING_ISO8859_9
//		 1   0   1   0   0   1		ENUM_ENCODING_ISO8859_10
//		 1   0   0   1   0   1		ENUM_ENCODING_ISO8859_11
//		 1   0   1   1   0   1		ENUM_ENCODING_ISO8859_12
//		 1   0   0   0   1   1		ENUM_ENCODING_ISO8859_13
//		 1   0   1   0   1   1		ENUM_ENCODING_ISO8859_14
//		 1   0   0   1   1   1		ENUM_ENCODING_ISO8859_15
//		 1   0   1   1   1   1		ENUM_ENCODING_ISO8859_16
//		 0   1   0   0   0   0		ENUM_ENCODING_MACINTOSH_CENTRAL_EUROPEAN
//		 1   1   0   0   0   0		ENUM_ENCODING_SHIFT_JIS_X_213
//

__int64		encodingSetting = 0,
			encodingMask = flagbit14 + flagbit15 + flagbit16 + flagbit17 + flagbit18 + flagbit19;




// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_VIEWPOINTS, &CMainFrame::OnViewPoints)
	ON_COMMAND(ID_VIEW_VIEWLINES, &CMainFrame::OnViewLines)
	ON_COMMAND(ID_VIEW_VIEWFACES, &CMainFrame::OnViewFaces)
	ON_COMMAND(ID_VIEW_VIEWWIREFRAME, &CMainFrame::OnViewWireframe)
	ON_COMMAND(ID_VIEW_ONOVER, &CMainFrame::OnViewOnover)
	ON_COMMAND(ID_VIEW_RESETTOFRONT, &CMainFrame::OnResetFront)
	ON_COMMAND(ID_VIEW_RESETTOSIDE, &CMainFrame::OnResetSide)

	ON_COMMAND(ID_MOUSEBEHAVIOUR_DEFAULT, &CMainFrame::OnMouseBehaviourDefault)
	ON_COMMAND(ID_MOUSEBEHAVIOUR_ZOOMONLY, &CMainFrame::OnMouseBehaviourZoom)
	ON_COMMAND(ID_MOUSEBEHAVIOUR_PANONLY, &CMainFrame::OnMouseBehaviourPan)
	ON_COMMAND(ID_MOUSEBEHAVIOUR_ROTATEONLY, &CMainFrame::OnMouseBehaviourRotate)

	ON_COMMAND(ID_ENCODING_DEFAULT, &CMainFrame::OnEncodingDefault)
	ON_COMMAND(ID_ENCODING_WINDOWS_1250, &CMainFrame::OnEncodingWindows1250)
	ON_COMMAND(ID_ENCODING_WINDOWS_1251, &CMainFrame::OnEncodingWindows1251)
	ON_COMMAND(ID_ENCODING_WINDOWS_1252, &CMainFrame::OnEncodingWindows1252)
	ON_COMMAND(ID_ENCODING_WINDOWS_1253, &CMainFrame::OnEncodingWindows1253)
	ON_COMMAND(ID_ENCODING_WINDOWS_1254, &CMainFrame::OnEncodingWindows1254)
	ON_COMMAND(ID_ENCODING_WINDOWS_1255, &CMainFrame::OnEncodingWindows1255)
	ON_COMMAND(ID_ENCODING_WINDOWS_1256, &CMainFrame::OnEncodingWindows1256)
	ON_COMMAND(ID_ENCODING_WINDOWS_1257, &CMainFrame::OnEncodingWindows1257)
	ON_COMMAND(ID_ENCODING_WINDOWS_1258, &CMainFrame::OnEncodingWindows1258)
	ON_COMMAND(ID_ENCODING_ISO8859_1, &CMainFrame::OnEncodingISO8859_1)
	ON_COMMAND(ID_ENCODING_ISO8859_2, &CMainFrame::OnEncodingISO8859_2)
	ON_COMMAND(ID_ENCODING_ISO8859_3, &CMainFrame::OnEncodingISO8859_3)
	ON_COMMAND(ID_ENCODING_ISO8859_4, &CMainFrame::OnEncodingISO8859_4)
	ON_COMMAND(ID_ENCODING_ISO8859_5, &CMainFrame::OnEncodingISO8859_5)
	ON_COMMAND(ID_ENCODING_ISO8859_6, &CMainFrame::OnEncodingISO8859_6)
	ON_COMMAND(ID_ENCODING_ISO8859_7, &CMainFrame::OnEncodingISO8859_7)
	ON_COMMAND(ID_ENCODING_ISO8859_8, &CMainFrame::OnEncodingISO8859_8)
	ON_COMMAND(ID_ENCODING_ISO8859_9, &CMainFrame::OnEncodingISO8859_9)
	ON_COMMAND(ID_ENCODING_ISO8859_10, &CMainFrame::OnEncodingISO8859_10)
	ON_COMMAND(ID_ENCODING_ISO8859_11, &CMainFrame::OnEncodingISO8859_11)
	ON_COMMAND(ID_ENCODING_ISO8859_13, &CMainFrame::OnEncodingISO8859_13)
	ON_COMMAND(ID_ENCODING_ISO8859_14, &CMainFrame::OnEncodingISO8859_14)
	ON_COMMAND(ID_ENCODING_ISO8859_15, &CMainFrame::OnEncodingISO8859_15)
	ON_COMMAND(ID_ENCODING_ISO8859_16, &CMainFrame::OnEncodingISO8859_16)
	ON_COMMAND(ID_ENCODING_MACINTOSH_CENTRAL_EUROPEAN, &CMainFrame::OnEncodingMacintosCentralEuropean)
	ON_COMMAND(ID_ENCODING_SHIFT_JIS_X_213, &CMainFrame::OnEncodingShiftJIS_X_213)
	ON_COMMAND(ID_VIEW_MODELCHECKER, &CMainFrame::OnViewModelChecker)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MODELCHECKER, &CMainFrame::OnUpdateViewModelChecker)
	ON_COMMAND(ID_VIEW_IDSCHECKER, &CMainFrame::OnViewIDSChecker)
	ON_COMMAND(ID_VIEW_PSDCHECKER, &CMainFrame::OnViewPSDChecker)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if (!m_wndSplitter.CreateStatic(this, 1, 2)) {
		return FALSE;
	}

	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_wndSplitter.IdFromRowCol(0, 0))) {
		return FALSE;
	}

	if (!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CLeftPane), CSize(200, 200), pContext)  ||
		!m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CInstanceInfoPane), CSize(200, 200), pContext)
		) {
		m_wndSplitter.DestroyWindow();
		return	FALSE;
	}

	if ( //!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftPane), CSize(200, 200), pContext)  ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CRightPane), CSize(200, 200), pContext)
		) {
		m_wndSplitter.DestroyWindow();
		return	FALSE;
	}

	m_wndSplitter.SetColumnInfo(0, 400, 30);
	m_wndSplitter.SetColumnInfo(0, 600, 30);
	m_wndSplitter2.SetRowInfo(0, 400, 30);
	m_wndSplitter2.SetRowInfo(1, 400, 30);

	return	true;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers



void CMainFrame::OnViewPoints()
{
	if	(showPoints) {
		this->GetMenu()->CheckMenuItem(ID_VIEW_VIEWPOINTS, MF_UNCHECKED);
		showPoints = false;
	} else {
		this->GetMenu()->CheckMenuItem(ID_VIEW_VIEWPOINTS, MF_CHECKED);
		showPoints = true;
	}

	GetRightPane()->SendMessage(IDS_UPDATE_RIGHT_PANE, 0, 0);
}



void CMainFrame::OnViewLines()
{
	if	(showLines) {
		this->GetMenu()->CheckMenuItem(ID_VIEW_VIEWLINES, MF_UNCHECKED);
		showLines = false;
	} else {
		this->GetMenu()->CheckMenuItem(ID_VIEW_VIEWLINES, MF_CHECKED);
		showLines = true;
	}

	GetRightPane()->SendMessage(IDS_UPDATE_RIGHT_PANE, 0, 0);
}

void CMainFrame::OnViewFaces()
{
	if	(showFaces) {
		this->GetMenu()->CheckMenuItem(ID_VIEW_VIEWFACES, MF_UNCHECKED);
		showFaces = false;
	} else {
		this->GetMenu()->CheckMenuItem(ID_VIEW_VIEWFACES, MF_CHECKED);
		showFaces = true;
	}

	GetRightPane()->SendMessage(IDS_UPDATE_RIGHT_PANE, 0, 0);
}


void CMainFrame::OnViewWireframe()
{
	if	(showWireFrame) {
		this->GetMenu()->CheckMenuItem(ID_VIEW_VIEWWIREFRAME, MF_UNCHECKED);
		showWireFrame = false;
	} else {
		this->GetMenu()->CheckMenuItem(ID_VIEW_VIEWWIREFRAME, MF_CHECKED);
		showWireFrame = true;
	}

	GetRightPane()->SendMessage(IDS_UPDATE_RIGHT_PANE, 0, 0);
}


void CMainFrame::OnViewOnover()
{
	if	(enableOnOver) {
		this->GetMenu()->CheckMenuItem(ID_VIEW_ONOVER, MF_UNCHECKED);
		enableOnOver = false;
		highLightedIfcObject = nullptr;
	} else {
		this->GetMenu()->CheckMenuItem(ID_VIEW_ONOVER, MF_CHECKED);
		enableOnOver = true;
	}

	GetRightPane()->SendMessage(IDS_UPDATE_RIGHT_PANE, 0, 0);
}

void CMainFrame::OnResetFront()
{
	GetRightPane()->SendMessage(IDS_RESET_FRONT, 0, 0);
}

void CMainFrame::OnResetSide()
{
	GetRightPane()->SendMessage(IDS_RESET_SIDE, 0, 0);
}

void CMainFrame::OnMouseBehaviourDefault()
{
	mouseBehaviour = MB_DEFAULT;
}

void CMainFrame::OnMouseBehaviourZoom()
{
	mouseBehaviour = MB_ZOOM;
}

void CMainFrame::OnMouseBehaviourPan()
{
	mouseBehaviour = MB_PAN;
}

void CMainFrame::OnMouseBehaviourRotate()
{
	mouseBehaviour = MB_ROTATE;
}

void CMainFrame::DisableAll()
{
	this->GetMenu()->CheckMenuItem(ID_ENCODING_DEFAULT, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1250, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1251, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1252, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1253, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1254, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1255, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1256, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1257, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1258, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_1, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_2, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_3, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_4, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_5, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_6, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_7, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_8, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_9, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_10, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_11, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_13, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_14, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_15, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_16, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_MACINTOSH_CENTRAL_EUROPEAN, MF_UNCHECKED);
	this->GetMenu()->CheckMenuItem(ID_ENCODING_SHIFT_JIS_X_213, MF_UNCHECKED);
}

void CMainFrame::OnEncodingDefault()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_DEFAULT, MF_CHECKED);

	//		 0   0   0   0   0   0		ENUM_ENCODING_DEFAULT
	encodingSetting = 0 + 0 + 0 + 0 + 0 + 0;
}

void CMainFrame::OnEncodingWindows1250()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1250, MF_CHECKED);

	//		 0   0   1   0   0   0		ENUM_ENCODING_WINDOWS_1250
	encodingSetting = 0 + 0 + flagbit16 + 0 + 0 + 0;
}

void CMainFrame::OnEncodingWindows1251()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1251, MF_CHECKED);

	//		 0   0   0   1   0   0		ENUM_ENCODING_WINDOWS_1251
	encodingSetting = 0 + 0 + 0 + flagbit17 + 0 + 0;
}

void CMainFrame::OnEncodingWindows1252()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1252, MF_CHECKED);

	//		 0   0   1   1   0   0		ENUM_ENCODING_WINDOWS_1252
	encodingSetting = 0 + 0 + flagbit16 + flagbit17 + 0 + 0;
}

void CMainFrame::OnEncodingWindows1253()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1253, MF_CHECKED);

	//		 0   0   0   0   1   0		ENUM_ENCODING_WINDOWS_1253
	encodingSetting = 0 + 0 + 0 + 0 + flagbit18 + 0;
}

void CMainFrame::OnEncodingWindows1254()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1254, MF_CHECKED);

	//		 0   0   1   0   1   0		ENUM_ENCODING_WINDOWS_1254
	encodingSetting = 0 + 0 + flagbit16 + 0 + flagbit18 + 0;
}

void CMainFrame::OnEncodingWindows1255()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1255, MF_CHECKED);

	//		 0   0   0   1   1   0		ENUM_ENCODING_WINDOWS_1255
	encodingSetting = 0 + 0 + 0 + flagbit17 + flagbit18 + 0;
}

void CMainFrame::OnEncodingWindows1256()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1256, MF_CHECKED);

	//		 0   0   1   1   1   0		ENUM_ENCODING_WINDOWS_1256
	encodingSetting = 0 + 0 + flagbit16 + flagbit17 + flagbit18 + 0;
}

void CMainFrame::OnEncodingWindows1257()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1257, MF_CHECKED);

	//		 0   0   0   0   0   1		ENUM_ENCODING_WINDOWS_1257
	encodingSetting = 0 + 0 + 0 + 0 + 0 + flagbit19;
}

void CMainFrame::OnEncodingWindows1258()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_WINDOWS_1258, MF_CHECKED);

	//		 0   0   1   0   0   1		ENUM_ENCODING_WINDOWS_1258
	encodingSetting = 0 + 0 + flagbit16 + 0 + 0 + flagbit19;
}

void CMainFrame::OnEncodingISO8859_1()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_1, MF_CHECKED);

	//		 1   0   0   0   0   0		ENUM_ENCODING_ISO8859_1
	encodingSetting = flagbit14 + 0 + 0 + 0 + 0 + 0;
}

void CMainFrame::OnEncodingISO8859_2()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_2, MF_CHECKED);

	//		 1   0   1   0   0   0		ENUM_ENCODING_ISO8859_2
	encodingSetting = flagbit14 + 0 + flagbit16 + 0 + 0 + 0;
}

void CMainFrame::OnEncodingISO8859_3()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_3, MF_CHECKED);

	//		 1   0   0   1   0   0		ENUM_ENCODING_ISO8859_3
	encodingSetting = flagbit14 + 0 + 0 + flagbit17 + 0 + 0;
}

void CMainFrame::OnEncodingISO8859_4()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_4, MF_CHECKED);

	//		 1   0   1   1   0   0		ENUM_ENCODING_ISO8859_4
	encodingSetting = flagbit14 + 0 + flagbit16 + flagbit17 + 0 + 0;
}

void CMainFrame::OnEncodingISO8859_5()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_5, MF_CHECKED);

	//		 1   0   0   0   1   0		ENUM_ENCODING_ISO8859_5
	encodingSetting = flagbit14 + 0 + 0 + 0 + flagbit18 + 0;
}

void CMainFrame::OnEncodingISO8859_6()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_6, MF_CHECKED);

	//		 1   0   1   0   1   0		ENUM_ENCODING_ISO8859_6
	encodingSetting = flagbit14 + 0 + flagbit16 + 0 + flagbit18 + 0;
}

void CMainFrame::OnEncodingISO8859_7()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_7, MF_CHECKED);

	//		 1   0   0   1   1   0		ENUM_ENCODING_ISO8859_7
	encodingSetting = flagbit14 + 0 + 0 + flagbit17 + flagbit18 + 0;
}

void CMainFrame::OnEncodingISO8859_8()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_8, MF_CHECKED);

	//		 1   0   1   1   1   0		ENUM_ENCODING_ISO8859_8
	encodingSetting = flagbit14 + 0 + flagbit16 + flagbit17 + flagbit18 + 0;
}

void CMainFrame::OnEncodingISO8859_9()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_9, MF_CHECKED);

	//		 1   0   0   0   0   1		ENUM_ENCODING_ISO8859_9
	encodingSetting = flagbit14 + 0 + 0 + 0 + 0 + flagbit19;
}

void CMainFrame::OnEncodingISO8859_10()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_10, MF_CHECKED);

	//		 1   0   1   0   0   1		ENUM_ENCODING_ISO8859_10
	encodingSetting = flagbit14 + 0 + flagbit16 + 0 + 0 + flagbit19;
}

void CMainFrame::OnEncodingISO8859_11()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_11, MF_CHECKED);

	//		 1   0   0   1   0   1		ENUM_ENCODING_ISO8859_11
	encodingSetting = flagbit14 + 0 + 0 + flagbit17 + 0 + flagbit19;
}

void CMainFrame::OnEncodingISO8859_13()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_13, MF_CHECKED);

	//		 1   0   0   0   1   1		ENUM_ENCODING_ISO8859_13
	encodingSetting = flagbit14 + 0 + 0 + 0 + flagbit18 + flagbit19;
}

void CMainFrame::OnEncodingISO8859_14()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_14, MF_CHECKED);

	//		 1   0   1   0   1   1		ENUM_ENCODING_ISO8859_14
	encodingSetting = flagbit14 + 0 + flagbit16 + 0 + flagbit18 + flagbit19;
}

void CMainFrame::OnEncodingISO8859_15()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_15, MF_CHECKED);

	//		 1   0   0   1   1   1		ENUM_ENCODING_ISO8859_15
	encodingSetting = flagbit14 + 0 + 0 + flagbit17 + flagbit18 + flagbit19;
}

void CMainFrame::OnEncodingISO8859_16()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_ISO8859_16, MF_CHECKED);

	//		 1   0   1   1   1   1		ENUM_ENCODING_ISO8859_16
	encodingSetting = flagbit14 + 0 + flagbit16 + flagbit17 + flagbit18 + flagbit19;
}

void CMainFrame::OnEncodingMacintosCentralEuropean()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_MACINTOSH_CENTRAL_EUROPEAN, MF_CHECKED);

	//		 0   1   0   0   0   0		ENUM_ENCODING_MACINTOSH_CENTRAL_EUROPEAN
	encodingSetting = 0 + flagbit15 + 0 + 0 + 0 + 0;
}

void CMainFrame::OnEncodingShiftJIS_X_213()
{
	DisableAll();

	this->GetMenu()->CheckMenuItem(ID_ENCODING_SHIFT_JIS_X_213, MF_CHECKED);

	//		 1   1   0   0   0   0		ENUM_ENCODING_SHIFT_JIS_X_213
	encodingSetting = flagbit14 + flagbit15 + 0 + 0 + 0 + 0;
}

bool CMainFrame::SelectInstance(int_t instance)
{
	auto pTreeView = GetLeftPane();
	if (!pTreeView) return true;

	return pTreeView->SelectInstance(instance);
}


CLeftPane* CMainFrame::GetLeftPane()
	{
	auto pDoc = DYNAMIC_DOWNCAST(CifcviewerDoc, GetActiveDocument());
	if (pDoc)
		{
		return DYNAMIC_DOWNCAST(CLeftPane, pDoc->GetPane(RUNTIME_CLASS(CLeftPane)));
		}
	ASSERT(0);
	return nullptr;
	}

CRightPane* CMainFrame::GetRightPane()
{
	auto pDoc = DYNAMIC_DOWNCAST(CifcviewerDoc, GetActiveDocument());
	if (pDoc) {
		return DYNAMIC_DOWNCAST(CRightPane, pDoc->GetPane(RUNTIME_CLASS(CRightPane)));
	}
	ASSERT(0);
	return nullptr;
}


void CMainFrame::OnViewModelChecker()
{
	if (m_wndModelChecker.GetSafeHwnd() && m_wndModelChecker.IsWindowVisible()) {
		m_wndModelChecker.ShowWindow(SW_HIDE);
	}
	else {
		if (!m_wndModelChecker.GetSafeHwnd()) {
			m_wndModelChecker.Create(IDD_MODELCHECK);
		}
		m_wndModelChecker.ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnViewIDSChecker()
{
	CIDSCheckDlg dlg(CIDSCheckDlg::Check::IDS);
	dlg.DoModal();
}

void CMainFrame::OnViewPSDChecker()
{
	CIDSCheckDlg dlg (CIDSCheckDlg::Check::PSD);
	dlg.DoModal();
}

void CMainFrame::OnUpdateViewModelChecker(CCmdUI* pCmdUI)
{
	auto visible = m_wndModelChecker.GetSafeHwnd() && m_wndModelChecker.IsWindowVisible();
	pCmdUI->SetCheck(visible);
}


void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CFrameWnd::OnUpdateFrameTitle(bAddToTitle);

	m_wndModelChecker.OnNewModel();
}
