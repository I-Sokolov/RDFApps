
// ifcviewer.h : main header file for the ifcviewer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CifcviewerApp:
// See ifcviewer.cpp for the implementation of this class
//

class CifcviewerApp : public CWinApp
{
public:
	CifcviewerApp();
	~CifcviewerApp();


// Overrides
public:
	virtual BOOL InitApplication();
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnViewFaces();
	afx_msg void OnViewWireFrame();
};

extern CifcviewerApp theApp;
