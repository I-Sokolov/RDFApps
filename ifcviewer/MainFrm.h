#pragma once

#include	"ModelCheckDlg.h"

class CLeftPane;
class CRightPane;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitter2;
	CModelCheckDlg	m_wndModelChecker;

public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	bool SelectInstance(int_t instance);

public:
	CLeftPane*  GetLeftPane();
	CRightPane* GetRightPane();

protected:  // control bar embedded members
	CStatusBar        m_wndStatusBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnViewModelChecker();
	afx_msg void OnUpdateViewModelChecker(CCmdUI* pCmdUI);
	afx_msg void OnViewPoints();
	afx_msg void OnViewLines();
	afx_msg void OnViewFaces();
	afx_msg void OnViewWireframe();
	afx_msg void OnViewOnover();
	afx_msg void OnResetFront();
	afx_msg void OnResetSide();

	afx_msg void OnMouseBehaviourDefault();
	afx_msg void OnMouseBehaviourZoom();
	afx_msg void OnMouseBehaviourPan();
	afx_msg void OnMouseBehaviourRotate();

	afx_msg void DisableAll();
	afx_msg void OnEncodingDefault();
	afx_msg void OnEncodingWindows1250();
	afx_msg void OnEncodingWindows1251();
	afx_msg void OnEncodingWindows1252();
	afx_msg void OnEncodingWindows1253();
	afx_msg void OnEncodingWindows1254();
	afx_msg void OnEncodingWindows1255();
	afx_msg void OnEncodingWindows1256();
	afx_msg void OnEncodingWindows1257();
	afx_msg void OnEncodingWindows1258();
	afx_msg void OnEncodingISO8859_1();
	afx_msg void OnEncodingISO8859_2();
	afx_msg void OnEncodingISO8859_3();
	afx_msg void OnEncodingISO8859_4();
	afx_msg void OnEncodingISO8859_5();
	afx_msg void OnEncodingISO8859_6();
	afx_msg void OnEncodingISO8859_7();
	afx_msg void OnEncodingISO8859_8();
	afx_msg void OnEncodingISO8859_9();
	afx_msg void OnEncodingISO8859_10();
	afx_msg void OnEncodingISO8859_11();
	afx_msg void OnEncodingISO8859_13();
	afx_msg void OnEncodingISO8859_14();
	afx_msg void OnEncodingISO8859_15();
	afx_msg void OnEncodingISO8859_16();
	afx_msg void OnEncodingMacintosCentralEuropean();
	afx_msg void OnEncodingShiftJIS_X_213();
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
};
