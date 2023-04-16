#pragma once


#include "resource.h"

#include "GenericTreeItem.h"

#include "dx9\include\d3d9.h"
#include "dx9\include\d3dx9.h"


#define		D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL)

enum	MOUSE_BEHAVIOUR {
	MB_DEFAULT,
	MB_ZOOM,
	MB_PAN,
	MB_ROTATE
};
typedef struct CUSTOMVERTEX {
	float	x;
	float	y;
	float	z;
	float	nx;
	float	ny;
	float	nz;
}	customvertex;

// RightPane dialog

class CRightPane : public CFormView
{
	DECLARE_DYNCREATE(CRightPane)

protected:
	CRightPane();           // protected constructor used by dynamic creation
	virtual ~CRightPane();

public:
	enum { IDD = IDD_RIGHTPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	HWND	m_hwndRenderWindow;

	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg	void OnGetInfoTip(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()

	LPDIRECT3D9             g_pD3D;			// Used to create the D3DDevice
	LPDIRECT3DDEVICE9       g_pd3dDevice;	// Our rendering device
	LPDIRECT3DVERTEXBUFFER9 g_pVB;			// Buffer to hold vertices
	LPDIRECT3DINDEXBUFFER9  g_pIB;			// Buffer to hold vertices
	LPDIRECT3DTEXTURE9      g_pTexture;

    D3DPRESENT_PARAMETERS	d3dpp;

	D3DXVECTOR3		m_vecOrigin;

	int32_t		m_iWidth;
	int32_t		m_iHeight;

	void	RenderPoints();
	void	RenderLines();
	void	RenderWireFrame();
	void	RenderFacesHighLighted();
	void	RenderFaces(D3DMATERIAL9 * mtrl);

	void	onHoverOverItem(int32_t iMouseX, int32_t iMouseY);

	void	InitializeDevice(CRect * rc);
	void	InitializeDeviceBuffer();
	void	Render(bool clean);
	bool	SetupLights();
	bool	SetupMatrices();

	void	Pan(int32_t iMouseX, int32_t iMouseY);
	void	Rotate(int32_t iMouseX, int32_t iMouseY);
	void	Zoom(int32_t iMouseX, int32_t iMouseY);

	CWnd* GetLeftPane();
};
