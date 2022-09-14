
// ifcviewerDoc.cpp : implementation of the CifcviewerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ifcviewer.h"
#endif

#include "ifcviewerDoc.h"
#include "LeftPane.h"
#include "RightPane.h"
#include "ifcengine\include\engdef.h"
#include "ifcengine\include\ifcengine.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern	int_t	globalIfcModel;

extern	wchar_t	ifcFileName[512];



// CifcviewerDoc

IMPLEMENT_DYNCREATE(CifcviewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CifcviewerDoc, CDocument)
END_MESSAGE_MAP()


// CifcviewerDoc construction/destruction

CifcviewerDoc::CifcviewerDoc()
{
	// TODO: add one-time construction code here

}

CifcviewerDoc::~CifcviewerDoc()
{
}

BOOL CifcviewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CifcviewerDoc serialization

void	charcpy(wchar_t * txtI, CString txtII, int_t maxChar)
{
	int32_t	i = 0;
	while  (txtII[i]  && i < maxChar-1) {
		txtI[i] = txtII[i];
		i++;
	}
	txtI[i] = 0;
}

bool	ParseIfcFile(
				CWnd	* pParent
			);

void CifcviewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		// TODO: add storing code here
	} else {
		charcpy(ifcFileName, ar.m_strFileName, 512);


//		InitializeDeviceBuffer();

//		...
		auto vp = GetFirstViewPosition();
		while (vp){
			auto v = GetNextView(vp);
			v->SendMessage(IDS_LOAD_IFC, 0, 0);
		}
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CifcviewerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CifcviewerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CifcviewerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CifcviewerDoc diagnostics

#ifdef _DEBUG
void CifcviewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CifcviewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

CWnd* CifcviewerDoc::GetPane(CRuntimeClass* pClass)
{
	auto viewpos = GetFirstViewPosition();
	while (viewpos) {
		auto view = GetNextView(viewpos);
		if (view->GetRuntimeClass() == pClass) {
			return view;
		}
	}
	ASSERT(false);
	return NULL;
}

CLeftPane* CifcviewerDoc::GetModelTreeView()
{
	return DYNAMIC_DOWNCAST(CLeftPane, GetPane(RUNTIME_CLASS(CLeftPane)));
}


// CifcviewerDoc commands
