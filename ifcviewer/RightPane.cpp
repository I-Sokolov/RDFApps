#include "stdafx.h"
#include "RightPane.h"
#include "afxdialogex.h"

#include "camera.h"
#include "IFCEngineInteract.h"

#include <list>



extern	int_t						ifcModelGlobalTmp;
extern	bool						showFaces, showWireFrame, showLines, showPoints, enableOnOver;
extern	std::list<D3DMATERIAL9*>	mtrls;
extern	MOUSE_BEHAVIOUR				mouseBehaviour;


STRUCT__IFC__OBJECT	* highLightedIfcObject = nullptr;
VECTOR3				vPickRayDir, vPickRayOrig;
VECTOR3				center;
double				size;
D3DMATERIAL9		mtrlBlack, mtrlRed;


// RightPane dialog

IMPLEMENT_DYNCREATE(CRightPane, CFormView)

CRightPane::CRightPane()
	: CFormView(CRightPane::IDD)
{

}

CRightPane::~CRightPane()
{
	CleanupIfcFile();
}

void CRightPane::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRightPane, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TTN_NEEDTEXT, OnGetInfoTip)
END_MESSAGE_MAP()

// RightPane diagnostics


void CRightPane::OnGetInfoTip(NMHDR * pNMHDR, LRESULT * pResult)
{
	pNMHDR = pNMHDR;
	pResult = pResult;
//	LPNMTVGETINFOTIP pGetInfoTip = (LPNMTVGETINFOTIP)pNMHDR;

/*	STRUCT__BASE					* baseItem = (STRUCT__BASE *) GetTreeCtrl().GetItemData(pGetInfoTip->hItem);
	STRUCT__PROPERTY				* propertyItem;
	STRUCT__PROPERTY__SET			* propertySetItem;
	STRUCT__SELECTABLE__TREEITEM	* selectableTreeitem;

	int_t	ifcInstance = 0;
	CString strItemTxt = 0;//m_TreeCtrl.GetItemText(pGetInfoTip->hItem);
	switch  (baseItem->structType) {
		case  STRUCT_TYPE_SELECTABLE_TREEITEM:
			selectableTreeitem = (STRUCT__SELECTABLE__TREEITEM *) baseItem;
			if	(selectableTreeitem->ifcItem  &&  selectableTreeitem->ifcItem->ifcInstance) {
				ifcInstance = selectableTreeitem->ifcItem->ifcInstance;
			} else {
				ifcInstance = selectableTreeitem->ifcInstance;
			}
			break;
		case  STRUCT_TYPE_HEADER_SET:
			break;
		case  STRUCT_TYPE_PROPERTY:
			propertyItem = (STRUCT__PROPERTY *) baseItem;
			ifcInstance = propertyItem->ifcInstance;
			break;
		case  STRUCT_TYPE_PROPERTY_SET:
			propertySetItem = (STRUCT__PROPERTY__SET *) baseItem;
			ifcInstance = propertySetItem->ifcInstance;
			break;
		default:
			ASSERT(false);
			break;
	}

	if	(ifcInstance) {
		strItemTxt = CreateToolTipText(ifcInstance);
	}

	if	(strItemTxt  &&  strItemTxt[0]) {
		STRCPY(pGetInfoTip->pszText, "ABCDe");		
	}
*/
	*pResult = 0;
}

#ifdef _DEBUG
void CRightPane::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightPane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG



extern	STRUCT__IFC__OBJECT				* ifcObjectsLinkedList;

int_t		noVertices = 0, noIndices = 0;

bool		initialized = false;

CPosition	gCubePosition(0,0,0);	// Position of cube in the world
int_t		iZoomMouseX, iZoomMouseY;

int_t		DirectXStatus = 0,
			render_browse_type = IDS_ROTATE;

const float kCameraMoveAmt = 0.002f; // Amount to move camera by
const float kMaxAngle = 89.0f;

D3DXVECTOR3		m_vectorOrigin;


float		* pVerticesDeviceBuffer;
int32_t		* pIndicesDeviceBuffer;




void	VECTOR3CROSS(VECTOR3 *pOut, const  VECTOR3 * pV1, const VECTOR3 * pV2)
{
	VECTOR3	v;

	v.x = pV1->y * pV2->z - pV1->z * pV2->y;
	v.y = pV1->z * pV2->x - pV1->x * pV2->z;
	v.z = pV1->x * pV2->y - pV1->y * pV2->x;

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;
}

double	VECTOR3DOT(const VECTOR3 * pV1, const VECTOR3 * pV2)
{
	return	pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

void	VECTOR3NORMALIZE(VECTOR3 * pOut, const VECTOR3 * pV)
{
	double	locSize;

	locSize = pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;

	if  (locSize) {
		pOut->x = pV->x / sqrt(locSize);
		pOut->y = pV->y / sqrt(locSize);
		pOut->z = pV->z / sqrt(locSize);
	}
}

bool	IntersectTriangle(VECTOR3 * v0, VECTOR3 * v1, VECTOR3 * v2, double * pDistance)
{
	VECTOR3	edge1, edge2, pvec, tvec, qvec;
	edge1.x = v1->x - v0->x;
	edge1.y = v1->y - v0->y;
	edge1.z = v1->z - v0->z;
	edge2.x = v2->x - v0->x;
	edge2.y = v2->y - v0->y;
	edge2.z = v2->z - v0->z;

	// Begin calculating determinant - also used to calculate U parameter
    VECTOR3CROSS(&pvec, &vPickRayDir, &edge2);

    // If determinant is near zero, ray lies in plane of triangle
	double	det = VECTOR3DOT(&edge1, &pvec), u, v, eps = 0.000001;
	tvec.x = vPickRayOrig.x - v0->x;
	tvec.y = vPickRayOrig.y - v0->y;
	tvec.z = vPickRayOrig.z - v0->z;

	if	(det > -eps  &&  det < eps) { return  false; }
 
	u = VECTOR3DOT(&tvec, &pvec) / det;
	if	(u < 0  ||  u > 1) { return  false; }
 
    // Prepare to test V parameter
    D3DXVECTOR3 ;
    VECTOR3CROSS(&qvec, &tvec, &edge1);

    // Calculate V parameter and test bounds
    v = VECTOR3DOT(&vPickRayDir, &qvec) / det;
	if	(v < 0  ||  u + v > 1) { return  false; }

    // Calculate t, scale parameters, ray intersects triangle
    double  distance = VECTOR3DOT(&edge2, &qvec) / det;

	(* pDistance) = distance;
	return	true;
}

void	checkPickBoundingBox(STRUCT__IFC__OBJECT * ifcObject, int_t * arrayOfIfcItems, double * arrayOfDistances, int_t * pArraySize)
{
	double			distance = 0, minDistance = 0;
	bool			intersect = false;

	VECTOR3			* vecMin = &ifcObject->vecMin, * vecMax = &ifcObject->vecMax,
					vec0, vec1, vec2;
	//
	//	z = Min (bottom), RHS = downwards
	//
	vec0.x = vecMin->x;
	vec0.y = vecMin->y;
	vec0.z = vecMin->z;
	vec1.x = vecMax->x;
	vec1.y = vecMin->y;
	vec1.z = vecMin->z;
	vec2.x = vecMin->x;
	vec2.y = vecMax->y;
	vec2.z = vecMin->z;
	if	(IntersectTriangle(&vec0, &vec2, &vec1, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}
	vec0.x = vecMax->x;
	vec0.y = vecMax->y;
	if	(IntersectTriangle(&vec0, &vec1, &vec2, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}

	//
	//	z = Max (top), RHS = upwards
	//
	vec0.x = vecMin->x;
	vec0.y = vecMin->y;
	vec0.z = vecMax->z;
	vec1.x = vecMax->x;
	vec1.y = vecMin->y;
	vec1.z = vecMax->z;
	vec2.x = vecMin->x;
	vec2.y = vecMax->y;
	vec2.z = vecMax->z;
	if	(IntersectTriangle(&vec0, &vec1, &vec2, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}
	vec0.x = vecMax->x;
	vec0.y = vecMax->y;
	if	(IntersectTriangle(&vec0, &vec2, &vec1, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}

	//
	//	y = Min (front)
	//
	vec0.x = vecMin->x;
	vec0.y = vecMin->y;
	vec0.z = vecMin->z;
	vec1.x = vecMax->x;
	vec1.y = vecMin->y;
	vec1.z = vecMin->z;
	vec2.x = vecMin->x;
	vec2.y = vecMin->y;
	vec2.z = vecMax->z;
	if	(IntersectTriangle(&vec0, &vec1, &vec2, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}
	vec0.x = vecMax->x;
	vec0.z = vecMax->z;
	if	(IntersectTriangle(&vec0, &vec2, &vec1, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}

	//
	//	y = Max (back)
	//
	vec0.x = vecMin->x;
	vec0.y = vecMax->y;
	vec0.z = vecMin->z;
	vec1.x = vecMax->x;
	vec1.y = vecMax->y;
	vec1.z = vecMin->z;
	vec2.x = vecMin->x;
	vec2.y = vecMax->y;
	vec2.z = vecMax->z;
	if	(IntersectTriangle(&vec0, &vec2, &vec1, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}
	vec0.x = vecMax->x;
	vec0.z = vecMax->z;
	if	(IntersectTriangle(&vec0, &vec1, &vec2, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}

	//
	//	x = Min (left)
	//
	vec0.x = vecMin->x;
	vec0.y = vecMin->y;
	vec0.z = vecMin->z;
	vec1.x = vecMin->x;
	vec1.y = vecMax->y;
	vec1.z = vecMin->z;
	vec2.x = vecMin->x;
	vec2.y = vecMin->y;
	vec2.z = vecMax->z;
	if	(IntersectTriangle(&vec0, &vec2, &vec1, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}
	vec0.y = vecMax->y;
	vec0.z = vecMax->z;
	if	(IntersectTriangle(&vec0, &vec1, &vec2, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}

	//
	//	x = Max (right)
	//
	vec0.x = vecMax->x;
	vec0.y = vecMin->y;
	vec0.z = vecMin->z;
	vec1.x = vecMax->x;
	vec1.y = vecMax->y;
	vec1.z = vecMin->z;
	vec2.x = vecMax->x;
	vec2.y = vecMin->y;
	vec2.z = vecMax->z;
	if	(IntersectTriangle(&vec0, &vec1, &vec2, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}
	vec0.y = vecMax->y;
	vec0.z = vecMax->z;
	if	(IntersectTriangle(&vec0, &vec2, &vec1, &distance)) {
		if	(intersect) {
			if	(minDistance > distance) {
				minDistance = distance;
			}
		} else {
			minDistance = distance;
			intersect = true;
		}
	}

	if	(intersect) {
		arrayOfIfcItems[(* pArraySize)] = (int_t) ifcObject;
		arrayOfDistances[(* pArraySize)] = minDistance;
		if	((* pArraySize) < 99) {
			(* pArraySize)++;
		}
	}
}

bool	checkPick(STRUCT__IFC__OBJECT * ifcObject, double * pDistance)
{
	VECTOR3		vec0, vec1, vec2;
	bool		intersect = false;
	int_t		i = 0;
	double		distance = 0;
	if	(ifcObject->noPrimitivesForFaces) {
		while  (i < ifcObject->noPrimitivesForFaces) {
			vec0.x = (ifcObject->__vertices[6 * ifcObject->indicesForFaces[3 * i + 0] + 0] - center.x) / size;
			vec0.y = (ifcObject->__vertices[6 * ifcObject->indicesForFaces[3 * i + 0] + 1] - center.y) / size;
			vec0.z = (ifcObject->__vertices[6 * ifcObject->indicesForFaces[3 * i + 0] + 2] - center.z) / size;
			vec1.x = (ifcObject->__vertices[6 * ifcObject->indicesForFaces[3 * i + 1] + 0] - center.x) / size;
			vec1.y = (ifcObject->__vertices[6 * ifcObject->indicesForFaces[3 * i + 1] + 1] - center.y) / size;
			vec1.z = (ifcObject->__vertices[6 * ifcObject->indicesForFaces[3 * i + 1] + 2] - center.z) / size;
			vec2.x = (ifcObject->__vertices[6 * ifcObject->indicesForFaces[3 * i + 2] + 0] - center.x) / size;
			vec2.y = (ifcObject->__vertices[6 * ifcObject->indicesForFaces[3 * i + 2] + 1] - center.y) / size;
			vec2.z = (ifcObject->__vertices[6 * ifcObject->indicesForFaces[3 * i + 2] + 2] - center.z) / size;
			if	(IntersectTriangle(&vec0, &vec2, &vec1, &distance)) {
				if	(intersect) {
					if	((*pDistance) > distance) {
						(*pDistance) = distance;
					}
				} else {
					(*pDistance) = distance;
					intersect = true;
				}
			}
			i++;
		}
	}

	return	intersect;
}

void	checkPickBoundingBoxNested(int_t * arrayOfIfcItems, double * arrayOfDistances, int_t * pArraySize)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;

	while  (ifcObject) {
		if	(ifcObject->treeItemModel  &&  ifcObject->treeItemModel->selectState == TI_CHECKED) {
			checkPickBoundingBox(ifcObject, arrayOfIfcItems, arrayOfDistances, pArraySize);
		}
		ifcObject = ifcObject->next;
	}
}

void	CRightPane::onHoverOverItem(int32_t iMouseX, int32_t iMouseY)
{
	STRUCT__IFC__OBJECT	* hoverOverIfcObject = 0;

	D3DXMATRIX		matWorld, matProj, matView, m;
	D3DXVECTOR3		v;

	g_pd3dDevice->GetTransform( D3DTS_WORLD, &matWorld );
	g_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

	// Compute the vector of the pick ray in screen space
	v.x =  ( ( ( 2.0f * iMouseX ) / m_iWidth  ) - 1 ) / matProj._11;
	v.y = -( ( ( 2.0f * iMouseY ) / m_iHeight ) - 1 ) / matProj._22;
	v.z =  1.0f;

	// Get the inverse view matrix
	g_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );

	D3DXMatrixMultiply( &matView, &matWorld, &matView );
	D3DXMatrixInverse( &m, NULL, &matView );

	// Transform the screen space pick ray into 3D space
	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	VECTOR3NORMALIZE(&vPickRayDir,&vPickRayDir);
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;

	int_t	arrayOfIfcItems[100];
	double	arrayOfDistances[100];
	int_t	arraySize = 0;

	checkPickBoundingBoxNested(arrayOfIfcItems, arrayOfDistances, &arraySize);

	if	(arraySize) {
		while  (hoverOverIfcObject == 0  &&  arraySize) {
			int_t i = 0, selectedIndex = 0;
			while  (i < arraySize) {
				if	(arrayOfDistances[selectedIndex] > arrayOfDistances[i]) {
					selectedIndex = i;
				}
				i++;
			}

			double	distance = 0;
			if	(checkPick((STRUCT__IFC__OBJECT *) arrayOfIfcItems[selectedIndex], &distance)) {
				hoverOverIfcObject = (STRUCT__IFC__OBJECT *) arrayOfIfcItems[selectedIndex];
			} else {
				arraySize--;
				arrayOfIfcItems[selectedIndex] = arrayOfIfcItems[arraySize];
				arrayOfDistances[selectedIndex] = arrayOfDistances[arraySize];
			}
		}
	}

	if	(highLightedIfcObject != hoverOverIfcObject) {
		highLightedIfcObject = hoverOverIfcObject;
		if	(initialized) {
			Render(false);
		}
	}
}



// RightPane message handlers

void CRightPane::OnSize(UINT nType, int32_t cx, int32_t cy) 
{
	SetScrollSizes( MM_TEXT, CSize(cx, cy) );

	CFormView::OnSize(nType, cx, cy);

	if	(initialized) {
		CRect rc;

		m_iWidth = cx;
		m_iHeight = cy;

		// Save static reference to the render window
		CWnd* pGroup = GetDlgItem(IDC_RENDERWINDOW);
		pGroup->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		pGroup->GetClientRect(&rc);
		pGroup->MapWindowPoints(this, &rc);

//		pGroup->EnableToolTips(true);
//		pGroup->OnEvent(...

		m_hwndRenderWindow = GetDlgItem(IDC_RENDERWINDOW)->GetSafeHwnd();

		InitializeDevice(&rc);
		InitializeDeviceBuffer();

		Render(false);
	}
}

void	CRightPane::Pan(int32_t iMouseX, int32_t iMouseY)
{
	CVector		vector;	// Used to hold camera's forward vector
	CPosition	eye;	// Used to hold camera's eye

	vector = gCamera->getCameraX();
	eye = gCamera->getEye();

	eye -= vector * (((float) (iMouseX-iZoomMouseX)) / 200);
	gCubePosition -=  vector * (((float) (iMouseX-iZoomMouseX)) / 200);

	iZoomMouseX = iMouseX;

	gCamera->setEye(eye);
	gCamera->setTarget(gCubePosition);
	//
	vector = gCamera->getCameraY();
	eye = gCamera->getEye();

	eye += vector * (((float) (iMouseY-iZoomMouseY)) / 200);
	gCubePosition +=  vector * (((float) (iMouseY-iZoomMouseY)) / 200);

	iZoomMouseY = iMouseY;

	gCamera->setEye(eye);
	gCamera->setTarget(gCubePosition);

	if  (initialized) {
		Render(false);
	}
}

void	CRightPane::Rotate(int32_t iMouseX, int32_t iMouseY)
{
	CVector		vector;	// Used to hold camera's forward vector
	CPosition	eye;	// Used to hold camera's eye

	double	pitchAmt = 0.0f,
			amt = (iMouseX - iZoomMouseX) * kCameraMoveAmt * 300;
	gCamera->rotateY(((float) amt * 3.14159265f / 180.0f), gCubePosition);
	iZoomMouseX = iMouseX;

	amt = -(iMouseY - iZoomMouseY) * kCameraMoveAmt * 300;
	// Cap pitch
	if(pitchAmt + amt < -kMaxAngle)
	{
		amt = -kMaxAngle - pitchAmt;
		pitchAmt = -kMaxAngle;
	}
	else if(pitchAmt + amt > kMaxAngle)
	{
		amt = kMaxAngle - pitchAmt;
		pitchAmt = kMaxAngle;
	}
	else
	{
		pitchAmt += amt;
	}
	// Pitch the camera up/down
	gCamera->pitch(((float) amt * 3.14159265f / 180.0f), gCubePosition);
	iZoomMouseY = iMouseY;

	if  (initialized) {
		Render(false);
	}
}

void	CRightPane::Zoom(int32_t iMouseX, int32_t iMouseY)
{
	iMouseX = iMouseX;

	CVector		vector;	// Used to hold camera's forward vector
	CPosition	eye;	// Used to hold camera's eye

	vector = gCamera->getCameraZ();
	eye = gCamera->getEye();

	eye += vector * (((float) (iMouseY-iZoomMouseY))/50);
	//gCubePos +=  vec * (((float) (iMouseY-iZoomMouseY))/50);

	if	(eye.z > -0.001) {
		eye.z = -0.001f;
	}
	iZoomMouseY = iMouseY;

	gCamera->setEye(eye);
	gCamera->setTarget(gCubePosition); // Set the camera to look at the cube

	if  (initialized) {
		Render(false);
	}
}

LRESULT CRightPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	int32_t	iMouseX = LOWORD(lParam),
			iMouseY = HIWORD(lParam);

	switch  (message)
	{
		case IDS_LOAD_IFC:
			Render(true);
			if (ParseIfcFile(this) == false) {
				this->MessageBoxW(L"File cannot be loaded, most probably the IFC schema is missing or not correct IFC2x3 / IFC4 file.");
			}
			InitializeDeviceBuffer();
			if (initialized) {
				Render(false);
			}
			break;
		case IDS_UPDATE:
			InitializeDeviceBuffer();
			if	(initialized) {
				Render(false);
			}
			break;
		case WM_MBUTTONDOWN:
			break;
		case WM_LBUTTONDOWN:
			iZoomMouseX = iMouseX;
			iZoomMouseY = iMouseY;
			onHoverOverItem(iMouseX, iMouseY);
			if (highLightedIfcObject) {
				this->GetWindow(GW_HWNDPREV)->SendMessage(IDS_SELECT_ITEM, 0, 0);
			}
			break;
		case WM_RBUTTONDOWN:
			iZoomMouseX = iMouseX;
			iZoomMouseY = iMouseY;
			onHoverOverItem(iMouseX, iMouseY);
			if (highLightedIfcObject) {
				this->GetWindow(GW_HWNDPREV)->SendMessage(IDS_SELECT_ITEM, 0, 0);

				HMENU	hMenu = ::CreatePopupMenu();
				wchar_t	menuItemString[255];
				menuItemString[0] = 'D';
				menuItemString[1] = 'i';
				menuItemString[2] = 's';
				menuItemString[3] = 'a';
				menuItemString[4] = 'b';
				menuItemString[5] = 'l';
				menuItemString[6] = 'e';
				menuItemString[7] = ' ';
				int_t k = 8, j = 0;
				while  (highLightedIfcObject->entityName[j]) {
					menuItemString[k++] = highLightedIfcObject->entityName[j++];
				}
				menuItemString[k] = 0;
				::AppendMenu(hMenu, 0, 1, menuItemString);
				::AppendMenu(hMenu, 0, 2, L"export geometry as *.rdf (default)");
				::AppendMenu(hMenu, 0, 3, L"export geometry as *.rdf (clean mapping, i.e. option bit10 / 1024)");

				DWORD	posa = GetMessagePos();
				CPoint	pta(LOWORD(posa), HIWORD(posa));

				int_t sel = ::TrackPopupMenuEx(hMenu, 
					TPM_CENTERALIGN | TPM_RETURNCMD,
					pta.x,
					pta.y,
					m_hWnd,
					NULL);
				::DestroyMenu(hMenu);

				switch (sel) {
					case  1:
						this->GetWindow(GW_HWNDPREV)->SendMessage(IDS_DISABLE_ITEM, 0, 0);
						highLightedIfcObject = 0;
						break;
					case  2:
						{
							int	express_id = (int)internalGetP21Line(highLightedIfcObject->ifcInstance);

							char strFilter[] = { "Text Files (*.rdf)|*.rdf|" }, buff[512];
							_itoa_s(express_id, buff, 512, 10);
							CFileDialog FileDlg(false, CString(".rdf"), CString("item #") + CString(buff) + CString(".rdf"), 0, CString(strFilter));

							if (FileDlg.DoModal() == IDOK)
							{
								cleanMemory(ifcModelGlobalTmp, 1);
//								setFilter(ifcModelGlobalTmp, 1024, 1024);

								int64_t	owlInstance = 0;
								createGeometryConversion(highLightedIfcObject->ifcInstance, &owlInstance);

//								setFilter(ifcModelGlobalTmp, 0, 1024);

								CString	fileName = FileDlg.GetFolderPath() + "\\" + FileDlg.GetFileName();

								char	fileNameChar[512];
								int		i = 0;
								while (fileName[i]) {
									fileNameChar[i] = (char)fileName[i];
									i++;
								}
								fileNameChar[i] = 0;
								SaveModel(ifcModelGlobalTmp, fileNameChar);

								cleanMemory(ifcModelGlobalTmp, 1);
							}
						}
						break;
					case  3:
						{
							int	express_id = (int)internalGetP21Line(highLightedIfcObject->ifcInstance);

							char strFilter[] = { "Text Files (*.rdf)|*.rdf|" }, buff[512];
							_itoa_s(express_id, buff, 512, 10);
							CFileDialog FileDlg(false, CString(".rdf"), CString("item #") + CString(buff) + CString(".rdf"), 0, CString(strFilter));

							if (FileDlg.DoModal() == IDOK)
							{
								cleanMemory(ifcModelGlobalTmp, 1);
								setFilter(ifcModelGlobalTmp, 1024, 1024);

								int64_t	owlInstance = 0;
								createGeometryConversion(highLightedIfcObject->ifcInstance, &owlInstance);

								setFilter(ifcModelGlobalTmp, 0, 1024);

								CString	fileName = FileDlg.GetFolderPath() + "\\" + FileDlg.GetFileName();

								char	fileNameChar[512];
								int		i = 0;
								while (fileName[i]) {
									fileNameChar[i] = (char)fileName[i];
									i++;
								}
								fileNameChar[i] = 0;
								SaveModel(ifcModelGlobalTmp, fileNameChar);

								cleanMemory(ifcModelGlobalTmp, 1);
							}
						}
						break;
					default:
						break;
				}
			}
			break;
		case WM_MOUSEMOVE:
			//
			//	Mouse moved
			//
			switch (mouseBehaviour) {
				case  MB_DEFAULT:
					if (MK_RBUTTON&wParam)
					{
						Pan(iMouseX, iMouseY);
					}

//					if	(WM_MBUTTONUP&wParam  ||  WM_MBUTTONDOWN&wParam  ||  MK_XBUTTON1&wParam  ||  MK_XBUTTON2&wParam) {
//					}

					if	(MK_MBUTTON&wParam) 
					{
						Zoom(iMouseX, iMouseY);
					}
					if	(MK_LBUTTON&wParam) 
					{
						Rotate(iMouseX, iMouseY);
					}
					break;
				case  MB_PAN:
					if ((MK_RBUTTON&wParam) ||
						(MK_MBUTTON&wParam) ||
						(MK_LBUTTON&wParam)) {
						Pan(iMouseX, iMouseY);
					}
					break;
				case  MB_ROTATE:
					if ((MK_RBUTTON&wParam) ||
						(MK_MBUTTON&wParam) ||
						(MK_LBUTTON&wParam)) {
						Rotate(iMouseX, iMouseY);
					}
					break;
				case  MB_ZOOM:
					if ((MK_RBUTTON&wParam) ||
						(MK_MBUTTON&wParam) ||
						(MK_LBUTTON&wParam)) {
						Zoom(iMouseX, iMouseY);
					}
					break;
				default:
					ASSERT(false);
					break;
			}

			highLightedIfcObject = nullptr;
			if	(enableOnOver) {
				onHoverOverItem(iMouseX, iMouseY);
			}
			break;
		case IDS_RESET_FRONT:
			m_vectorOrigin.x = 0;
			m_vectorOrigin.y = 0;
			m_vectorOrigin.z = 0;
			gCubePosition.x = 0;
			gCubePosition.y = 0;
			gCubePosition.z = 0;
			gCamera->setFront(1);
			gCamera->setTarget(gCubePosition); // Set the camera to look at the cube

			if  (initialized) {
				Render(false);
			}
			break;
		case IDS_RESET_SIDE:
			m_vectorOrigin.x = 0;
			m_vectorOrigin.y = 0;
			m_vectorOrigin.z = 0;
			gCubePosition.x = 0;
			gCubePosition.y = 0;
			gCubePosition.z = 0;
			gCamera->setSide(1);
			gCamera->setTarget(gCubePosition); // Set the camera to look at the cube

			if  (initialized) {
				Render(false);
			}
			break;
		case  WM_PAINT:
		case  IDS_UPDATE_RIGHT_PANE:
			if  (initialized) {
				Render(false);
			}
			break;
		case 7:
		case 8:
		case 32:
		case 132:
		case 312:
		case  641:
		case  867:
			break;
		case 1:
		case 5:
		case 129:
		case 131:
		case 3:
		case 48:
		case 272:
		case 70:
		case 71:
		case 24:
		case 868:
		case 133:
		case 20:
		case 310:
		case 514:
			break;
		case 25:
		case 33:
			if  (initialized) {
				Render(false);
			}
			break;
		default:
			break;
	}

	return	CFormView::WindowProc(message, wParam, lParam);
}

void CRightPane::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	if	(!initialized) {
		g_pD3D       = NULL;
		g_pd3dDevice = NULL;
		g_pVB        = NULL;
		g_pIB        = NULL;

		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

		m_hwndRenderWindow = GetDlgItem(IDC_RENDERWINDOW)->GetSafeHwnd();

		CRect rc;
		this->GetWindowRect( &rc );

		m_iWidth = rc.Width();
		m_iHeight = rc.Height();

		CWnd* pGroup = GetDlgItem(IDC_RENDERWINDOW);
		pGroup->SetWindowPos(NULL, 0, 0, m_iWidth, m_iHeight, SWP_NOZORDER);

		pGroup->EnableToolTips(true);

		InitializeDevice(nullptr);

		InitializeDeviceBuffer();

		initialized = true;

//		EnableToolTips(TRUE);
	}
}

void CRightPane::InitializeDevice(CRect * rc)
{
	rc = rc;

	if	(!DirectXStatus) {
		// Reset the device
		if	(false) {//g_pd3dDevice) {
//			D3DPRESENT_PARAMETERS	presentationParameters;
/*
			d3dpp.BackBufferHeight = rc->bottom - rc->top;
			d3dpp.BackBufferWidth = rc->right - rc->left;
//			presentationParameters.BackBufferHeight = 1000;
//			presentationParameters.BackBufferWidth = 1800;

			g_pd3dDevice->Reset(&d3dpp);

			g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// setup the vertex shader 
			if	(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX)) {
				DirectXStatus = -1;
				ASSERT(false);
				return;
			}
//	*/
/////			g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
		else {
			if( g_pVB != nullptr ) {
				if( FAILED( g_pVB->Release() ) ) {
					DirectXStatus = -1;
					ASSERT(false);
					return;
				}
			}

			if( g_pIB != nullptr ) {
				if( FAILED( g_pIB->Release() ) ) {
					DirectXStatus = -1;
					ASSERT(false);
					return;
				}
			}

			if( g_pd3dDevice != nullptr ) {
				if( FAILED( g_pd3dDevice->Release() ) ) {
					DirectXStatus = -1;
					ASSERT(false);
					return;
				}
			}

			if( g_pD3D != nullptr ) {
				if( FAILED( g_pD3D->Release() ) ) {
					DirectXStatus = -1;
					ASSERT(false);
					return;
				}
			}

			// Create the D3D object.
			if	( nullptr == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) {
				DirectXStatus = -1;
				ASSERT(false);
				return;
			}

			//
			//	Specific for DirectX 8.0
			//

			D3DDISPLAYMODE d3ddm;
			if	(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
				DirectXStatus = -1;
				ASSERT(false);
				return;
			}

			ZeroMemory( &d3dpp, sizeof(d3dpp) );
			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat = d3ddm.Format;
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

			DWORD qualityLevels = 0;
			int_t antiAlias = 16;
			while  (antiAlias > 0) {
				if	(SUCCEEDED(g_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
					 D3DDEVTYPE_HAL, d3dpp.BackBufferFormat, true,
					 (D3DMULTISAMPLE_TYPE)antiAlias, &qualityLevels))) {
					d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)antiAlias;
					d3dpp.MultiSampleQuality = qualityLevels-1;
					d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
					break;
				}
				--antiAlias;
			}

			// Create the D3DDevice
			if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwndRenderWindow,
											  D3DCREATE_HARDWARE_VERTEXPROCESSING,
											  &d3dpp, &g_pd3dDevice ) ) )
			{
				if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwndRenderWindow,
												  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
												  &d3dpp, &g_pd3dDevice ) ) ) {
					DirectXStatus = -1;
					ASSERT(false);
					return;
				}
			}

//			g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//NONE);
			g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// setup the vertex shader 
			if	(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX)) {
				DirectXStatus = -1;
				ASSERT(false);
				return;
			}
		}
	}
}

bool	isEqual(wchar_t * txtI, wchar_t * txtII)
{
	if (txtI && txtII) {
		int_t	i = 0;
		while (txtI[i] && txtI[i] == txtII[i]) { i++; }
		if (txtI[i] == 0 && txtII[i] == 0) {
			return	true;
		}
	}
	return	false;
}

void	GetDimensions(VECTOR3 * min, VECTOR3 * max, bool * pInitMinMax)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while (ifcObject) {
		if (ifcObject->noVertices &&
			!isEqual(ifcObject->entityName, L"IfcAlignmentSegment") &&
			(ifcObject->noPrimitivesForFaces  ||  ifcObject->noPrimitivesForLines)) {
			if (ifcObject->noPrimitivesForFaces) {
				int index1 = (int) ifcObject->indicesForFaces[0];
				ifcObject->vecMin.x = ifcObject->__vertices[6 * index1 + 0];
				ifcObject->vecMax.x = ifcObject->vecMin.x;
				ifcObject->vecMin.y = ifcObject->__vertices[6 * index1 + 1];
				ifcObject->vecMax.y = ifcObject->vecMin.y;
				ifcObject->vecMin.z = ifcObject->__vertices[6 * index1 + 2];
				ifcObject->vecMax.z = ifcObject->vecMin.z;
				int_t	i = 0;
				while (i < 3 * ifcObject->noPrimitivesForFaces) {
					int index2 = (int)ifcObject->indicesForFaces[i];
					if (ifcObject->vecMin.x > ifcObject->__vertices[6 * index2 + 0]) { ifcObject->vecMin.x = ifcObject->__vertices[6 * index2 + 0]; }
					if (ifcObject->vecMax.x < ifcObject->__vertices[6 * index2 + 0]) { ifcObject->vecMax.x = ifcObject->__vertices[6 * index2 + 0]; }
					if (ifcObject->vecMin.y > ifcObject->__vertices[6 * index2 + 1]) { ifcObject->vecMin.y = ifcObject->__vertices[6 * index2 + 1]; }
					if (ifcObject->vecMax.y < ifcObject->__vertices[6 * index2 + 1]) { ifcObject->vecMax.y = ifcObject->__vertices[6 * index2 + 1]; }
					if (ifcObject->vecMin.z > ifcObject->__vertices[6 * index2 + 2]) { ifcObject->vecMin.z = ifcObject->__vertices[6 * index2 + 2]; }
					if (ifcObject->vecMax.z < ifcObject->__vertices[6 * index2 + 2]) { ifcObject->vecMax.z = ifcObject->__vertices[6 * index2 + 2]; }
					i++;
				}
			}
			else {
				ASSERT(ifcObject->noPrimitivesForLines);
				int index1 = (int)ifcObject->indicesForLines[0];
				ifcObject->vecMin.x = ifcObject->__vertices[6 * index1 + 0];
				ifcObject->vecMax.x = ifcObject->vecMin.x;
				ifcObject->vecMin.y = ifcObject->__vertices[6 * index1 + 1];
				ifcObject->vecMax.y = ifcObject->vecMin.y;
				ifcObject->vecMin.z = ifcObject->__vertices[6 * index1 + 2];
				ifcObject->vecMax.z = ifcObject->vecMin.z;
				int_t	i = 0;
				while (i < 2 * ifcObject->noPrimitivesForLines) {
					int index2 = (int)ifcObject->indicesForLines[i];
					if (ifcObject->vecMin.x > ifcObject->__vertices[6 * index2 + 0]) { ifcObject->vecMin.x = ifcObject->__vertices[6 * index2 + 0]; }
					if (ifcObject->vecMax.x < ifcObject->__vertices[6 * index2 + 0]) { ifcObject->vecMax.x = ifcObject->__vertices[6 * index2 + 0]; }
					if (ifcObject->vecMin.y > ifcObject->__vertices[6 * index2 + 1]) { ifcObject->vecMin.y = ifcObject->__vertices[6 * index2 + 1]; }
					if (ifcObject->vecMax.y < ifcObject->__vertices[6 * index2 + 1]) { ifcObject->vecMax.y = ifcObject->__vertices[6 * index2 + 1]; }
					if (ifcObject->vecMin.z > ifcObject->__vertices[6 * index2 + 2]) { ifcObject->vecMin.z = ifcObject->__vertices[6 * index2 + 2]; }
					if (ifcObject->vecMax.z < ifcObject->__vertices[6 * index2 + 2]) { ifcObject->vecMax.z = ifcObject->__vertices[6 * index2 + 2]; }
					i++;
				}
			}

			if	( (ifcObject->vecMin.x > -1000000000  &&  ifcObject->vecMax.x < 1000000000)  &&
				  (ifcObject->vecMin.y > -1000000000  &&  ifcObject->vecMax.y < 1000000000)  &&
				  (ifcObject->vecMin.z > -1000000000  &&  ifcObject->vecMax.z < 1000000000) ) {
				if	((*pInitMinMax) == false) {
					min->x = ifcObject->vecMin.x;
					max->x = ifcObject->vecMax.x;
					min->y = ifcObject->vecMin.y;
					max->y = ifcObject->vecMax.y;
					min->z = ifcObject->vecMin.z;
					max->z = ifcObject->vecMax.z;
					(*pInitMinMax) = true;
				} else {
					if	(min->x > ifcObject->vecMin.x) {
						min->x = ifcObject->vecMin.x;
					}
					if	(max->x < ifcObject->vecMax.x) {
						max->x = ifcObject->vecMax.x;
					}
					if	(min->y > ifcObject->vecMin.y) {
						min->y = ifcObject->vecMin.y;
					}
					if	(max->y < ifcObject->vecMax.y) {
						max->y = ifcObject->vecMax.y;
					}
					if	(min->z > ifcObject->vecMin.z) {
						min->z = ifcObject->vecMin.z;
					}
					if	(max->z < ifcObject->vecMax.z) {
						max->z = ifcObject->vecMax.z;
					}
				}
			}
		}
		ifcObject = ifcObject->next;
	}
}

void	GetBufferSizes_ifc(int_t * pVBuffSize)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while (ifcObject) {
		if ((ifcObject->ifcInstance)  &&
			(ifcObject->noVertices)  &&
			(ifcObject->noPrimitivesForPoints || ifcObject->noPrimitivesForLines || ifcObject->noPrimitivesForFaces || ifcObject->noPrimitivesForWireFrame)) {
			ifcObject->vertexOffset = (int_t)(*pVBuffSize);

			(*pVBuffSize) += ifcObject->noVertices;
		}
		ifcObject = ifcObject->next;
	}
}

void	GetBufferSizes_ifcPoints(int_t * pVBuffSize, int_t * pIBuffSize)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while (ifcObject) {
		if (ifcObject->ifcInstance  &&  ifcObject->noVertices  &&  ifcObject->noPrimitivesForPoints) {
			ifcObject->indexOffsetForPoints = (int_t)(*pIBuffSize);

			(*pVBuffSize) += ifcObject->noVertices;
			(*pIBuffSize) += 1 * ifcObject->noPrimitivesForPoints;
		}
		ifcObject = ifcObject->next;
	}
}

void	GetBufferSizes_ifcLines(int_t * pVBuffSize, int_t * pIBuffSize)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while (ifcObject) {
		if (ifcObject->ifcInstance  &&  ifcObject->noVertices  &&  ifcObject->noPrimitivesForLines) {
			ifcObject->indexOffsetForLines = (int_t)(*pIBuffSize);

			(*pVBuffSize) += ifcObject->noVertices;
			(*pIBuffSize) += 2 * ifcObject->noPrimitivesForLines;
		}
		ifcObject = ifcObject->next;
	}
}

void	GetBufferSizes_ifcFaces(int_t * pVBuffSize, int_t * pIBuffSize)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		if	(ifcObject->ifcInstance  &&  ifcObject->noVertices  &&  ifcObject->noPrimitivesForFaces) {
			ifcObject->indexOffsetForFaces = (int_t) (*pIBuffSize);

			(*pVBuffSize) += ifcObject->noVertices;
			(*pIBuffSize) += 3 * ifcObject->noPrimitivesForFaces;
		}
		ifcObject = ifcObject->next;
	}
}

void	GetBufferSizes_ifcWireFrame(int_t * pVBuffSize, int_t * pIBuffSize)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		if	(ifcObject->ifcInstance  &&  ifcObject->noVertices  &&  ifcObject->noPrimitivesForWireFrame) {
			ifcObject->indexOffsetForWireFrame = (int_t) (*pIBuffSize);

			(*pVBuffSize) += ifcObject->noVertices;
			(*pIBuffSize) += 2 * ifcObject->noPrimitivesForWireFrame;
		}
		ifcObject = ifcObject->next;
	}
}

void	AdjustMinMax(VECTOR3 * centerPar, double sizePar)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		if	(ifcObject->ifcInstance  &&  ifcObject->noVertices) {
			ifcObject->vecMin.x = (ifcObject->vecMin.x - centerPar->x) / sizePar;
			ifcObject->vecMin.y = (ifcObject->vecMin.y - centerPar->y) / sizePar;
			ifcObject->vecMin.z = (ifcObject->vecMin.z - centerPar->z) / sizePar;

			ifcObject->vecMax.x = (ifcObject->vecMax.x - centerPar->x) / sizePar;
			ifcObject->vecMax.y = (ifcObject->vecMax.y - centerPar->y) / sizePar;
			ifcObject->vecMax.z = (ifcObject->vecMax.z - centerPar->z) / sizePar;
		}
		ifcObject = ifcObject->next;
	}
}

void	FillVertexBuffers_ifc(int_t * pVBuffSize, float * pVertices, VECTOR3 * centerPar, double sizePar)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		if ((ifcObject->ifcInstance) &&
			(ifcObject->noVertices) &&
			(ifcObject->noPrimitivesForPoints || ifcObject->noPrimitivesForLines || ifcObject->noPrimitivesForFaces || ifcObject->noPrimitivesForWireFrame)) {
			int_t	i = 0;
			while  (i < ifcObject->noVertices) {
				pVertices[6 * ((*pVBuffSize) + i) + 0] = (float) ((ifcObject->__vertices[6 * i + 0] - centerPar->x) / sizePar);
				pVertices[6 * ((*pVBuffSize) + i) + 1] = (float) ((ifcObject->__vertices[6 * i + 1] - centerPar->y) / sizePar);
				pVertices[6 * ((*pVBuffSize) + i) + 2] = (float) ((ifcObject->__vertices[6 * i + 2] - centerPar->z) / sizePar);
				pVertices[6 * ((*pVBuffSize) + i) + 3] = ifcObject->__vertices[6 * i + 3];
				pVertices[6 * ((*pVBuffSize) + i) + 4] = ifcObject->__vertices[6 * i + 4];
				pVertices[6 * ((*pVBuffSize) + i) + 5] = ifcObject->__vertices[6 * i + 5];
				i++;
			}

			ASSERT(ifcObject->vertexOffset == (int_t) (*pVBuffSize));

			(*pVBuffSize) += ifcObject->noVertices;
		}
		ifcObject = ifcObject->next;
	}
}

void	FillIndexBuffers_ifcFaces(int_t * pIBuffSize, int32_t * pIndices, D3DMATERIAL9 * mtrl)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		STRUCT_MATERIALS	* materials = ifcObject->materials;
		while (materials) {
			if	((ifcObject->ifcInstance)  &&
				 (ifcObject->noVertices)  &&
				 (ifcObject->noPrimitivesForFaces)  &&
				 (materials->material->MTRL == (void*) mtrl)) {
				int_t	i = 0;
				while  (i < materials->__noPrimitivesForFaces) {
					pIndices[(*pIBuffSize) + 3 * i + 0] = (int32_t) (ifcObject->indicesForFaces[3 * i + materials->__indexArrayOffset + 0] + ifcObject->vertexOffset);
					pIndices[(*pIBuffSize) + 3 * i + 1] = (int32_t) (ifcObject->indicesForFaces[3 * i + materials->__indexArrayOffset + 1] + ifcObject->vertexOffset);
					pIndices[(*pIBuffSize) + 3 * i + 2] = (int32_t) (ifcObject->indicesForFaces[3 * i + materials->__indexArrayOffset + 2] + ifcObject->vertexOffset);
					i++;
				}

				materials->__indexOffsetForFaces = (int_t) (*pIBuffSize);
				ASSERT(materials->__indexOffsetForFaces == (*pIBuffSize));

				(*pIBuffSize) += 3 * (int_t) materials->__noPrimitivesForFaces;
			}
			materials = materials->next;
		}
		ifcObject = ifcObject->next;
	}
}

void	FillIndexBuffers_ifcPoints(int_t * pIBuffSize, int32_t * pIndices)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while (ifcObject) {
		if ((ifcObject->ifcInstance) &&
			(ifcObject->noVertices) &&
			(ifcObject->noPrimitivesForPoints)) {
			int_t	i = 0;
			while (i < ifcObject->noPrimitivesForPoints) {
				pIndices[(*pIBuffSize) + 1 * i + 0] = (int32_t)(ifcObject->indicesForPoints[1 * i + 0] + ifcObject->vertexOffset);
				i++;
			}

			ifcObject->indexOffsetForPoints = (int_t)(*pIBuffSize);
			ASSERT(ifcObject->indexOffsetForPoints == (*pIBuffSize));

			(*pIBuffSize) += 1 * ifcObject->noPrimitivesForPoints;
		}
		ifcObject = ifcObject->next;
	}
}

void	FillIndexBuffers_ifcLines(int_t * pIBuffSize, int32_t * pIndices)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while (ifcObject) {
		if ((ifcObject->ifcInstance) &&
			(ifcObject->noVertices) &&
			(ifcObject->noPrimitivesForLines)) {
			int_t	i = 0;
			while (i < ifcObject->noPrimitivesForLines) {
				pIndices[(*pIBuffSize) + 2 * i + 0] = (int32_t)(ifcObject->indicesForLines[2 * i + 0] + ifcObject->vertexOffset);
				pIndices[(*pIBuffSize) + 2 * i + 1] = (int32_t)(ifcObject->indicesForLines[2 * i + 1] + ifcObject->vertexOffset);
				i++;
			}

			ifcObject->indexOffsetForLines = (int_t)(*pIBuffSize);
			ASSERT(ifcObject->indexOffsetForLines == (*pIBuffSize));

			(*pIBuffSize) += 2 * ifcObject->noPrimitivesForLines;
		}
		ifcObject = ifcObject->next;
	}
}

void	FillIndexBuffers_ifcWireFrame(int_t * pIBuffSize, int32_t * pIndices)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while (ifcObject) {
		if ((ifcObject->ifcInstance)  &&
			(ifcObject->noVertices)  &&
			(ifcObject->noPrimitivesForFaces)  &&
			(ifcObject->noPrimitivesForWireFrame)) {
			int_t	i = 0;
			while  (i < ifcObject->noPrimitivesForWireFrame) {
				pIndices[(*pIBuffSize) + 2 * i + 0] = (int32_t)(ifcObject->indicesForLinesWireFrame[2 * i + 0] + ifcObject->vertexOffset);
				pIndices[(*pIBuffSize) + 2 * i + 1] = (int32_t)(ifcObject->indicesForLinesWireFrame[2 * i + 1] + ifcObject->vertexOffset);
				i++;
			}

			ifcObject->indexOffsetForWireFrame = (int_t) (*pIBuffSize);
			ASSERT(ifcObject->indexOffsetForWireFrame == (*pIBuffSize));

			(*pIBuffSize) += 2 * ifcObject->noPrimitivesForWireFrame;
		}
		ifcObject = ifcObject->next;
	}
}

void CRightPane::InitializeDeviceBuffer()
{
	if	(ifcObjectsLinkedList) {
		VECTOR3	min, max;
		bool			initSizes = false;
		GetDimensions(&min, &max, &initSizes);

		if	(initSizes) {
			center.x = (max.x + min.x)/2;
			center.y = (max.y + min.y)/2;
			center.z = (max.z + min.z)/2;
			size = max.x - min.x;
			if	(size < max.y - min.y) { size = max.y - min.y; }
			if	(size < max.z - min.z) { size = max.z - min.z; }

//center.x = 0.;
//center.y = 0.;
//center.z = 0.;
//size = 1.;

			int_t	iBuffSize, maxVBuffSize = 0;
			{
				int_t	vBuffSize;
				GetBufferSizes_ifc(&maxVBuffSize);

				vBuffSize = 0;
				iBuffSize = 0;
				GetBufferSizes_ifcPoints(&vBuffSize, &iBuffSize);
				ASSERT(maxVBuffSize >= vBuffSize);

				vBuffSize = 0;
				//			iBuffSize = 0;
				GetBufferSizes_ifcLines(&vBuffSize, &iBuffSize);
				ASSERT(maxVBuffSize >= vBuffSize);

				vBuffSize = 0;
//				iBuffSize = 0;
				GetBufferSizes_ifcFaces(&vBuffSize, &iBuffSize);
				ASSERT(maxVBuffSize >= vBuffSize);

				//			tmpVBuffSize = vBuffSize;
				vBuffSize = 0;
				GetBufferSizes_ifcWireFrame(&vBuffSize, &iBuffSize);
				ASSERT(maxVBuffSize >= vBuffSize);
			}

			if	(!DirectXStatus) {
				if (maxVBuffSize) {
					if( FAILED( g_pd3dDevice->CreateVertexBuffer( ((int32_t) maxVBuffSize) * sizeof(CUSTOMVERTEX),
																   0, D3DFVF_CUSTOMVERTEX,
																   D3DPOOL_DEFAULT, &g_pVB, NULL ) ) ) {
						DirectXStatus = -1;
						ASSERT(false);
						return;
					}
				}

				if	(iBuffSize) {
					if( FAILED( g_pd3dDevice->CreateIndexBuffer( ((int32_t) iBuffSize) * sizeof(int32_t),
																  0, D3DFMT_INDEX32,
																  D3DPOOL_DEFAULT, &g_pIB, NULL ) ) ) {
						DirectXStatus = -1;
						ASSERT(false);
						return;
					}
				}

				if (FAILED(g_pVB->Lock(0, (int32_t)maxVBuffSize * 6 * sizeof(float), (void **)&pVerticesDeviceBuffer, 0))) {
					DirectXStatus = -1;
					ASSERT(false);
					return;
				}

				if( FAILED( g_pIB->Lock( 0, (int32_t) iBuffSize*sizeof(int32_t), (void **)&pIndicesDeviceBuffer, 0 ) ) ) {
					DirectXStatus = -1;
					ASSERT(false);
					return;
				}

				int_t vertexCnt = 0, indexCnt = 0;

				AdjustMinMax(&center, size);

				FillVertexBuffers_ifc(&vertexCnt, pVerticesDeviceBuffer, &center, size);
				FillIndexBuffers_ifcFaces(&indexCnt, pIndicesDeviceBuffer, 0);
				for  (std::list<D3DMATERIAL9*>::iterator it=mtrls.begin() ; it != mtrls.end(); ++it) {
					FillIndexBuffers_ifcFaces(&indexCnt, pIndicesDeviceBuffer, (*it));
				}
				ASSERT(maxVBuffSize == vertexCnt);
				FillIndexBuffers_ifcWireFrame(&indexCnt, pIndicesDeviceBuffer);
				FillIndexBuffers_ifcLines(&indexCnt, pIndicesDeviceBuffer);
				FillIndexBuffers_ifcPoints(&indexCnt, pIndicesDeviceBuffer);

				ASSERT(iBuffSize == indexCnt);

				if	(FAILED( g_pIB->Unlock())) {
					DirectXStatus = -1;
					ASSERT(false);
					return;
				}
		
				if	(FAILED( g_pVB->Unlock())) {
					DirectXStatus = -1;
					ASSERT(false);
					return;
				}
			}
		}
	}
}

void	CRightPane::RenderFacesHighLighted()
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while	(ifcObject) {
		if	(ifcObject->treeItemModel  &&  ifcObject->treeItemModel->selectState == TI_CHECKED  &&  ifcObject == highLightedIfcObject) {
			if	(ifcObject->noPrimitivesForFaces) {
				STRUCT_MATERIALS	* materials = ifcObject->materials;
				while  (materials) {
					g_pd3dDevice->DrawIndexedPrimitive(
							D3DPT_TRIANGLELIST,
							0,
							(int32_t) ifcObject->vertexOffset,
							(int32_t) ifcObject->noVertices,
							(int32_t) materials->__indexOffsetForFaces,
							(int32_t) materials->__noPrimitivesForFaces
						);
					materials = materials->next;
				}
			}
		}
		ifcObject = ifcObject->next;
	}
}

bool		facesToDraw = false;
long long	vertexOffset,
			indexOffsetForFaces,
			noPrimitivesForFaces;

void	CRightPane::RenderFaces(D3DMATERIAL9 * mtrl)
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while	(ifcObject) {
		STRUCT_MATERIALS	* materials = ifcObject->materials;
		while  (materials) {
			if	(ifcObject->selectState == TI_CHECKED  &&  ifcObject->noPrimitivesForFaces  &&  materials->material->MTRL == (void*) mtrl  &&  ifcObject != highLightedIfcObject) {
				if	(facesToDraw) {
					if	(indexOffsetForFaces + 3 * noPrimitivesForFaces == materials->__indexOffsetForFaces) {
						if	(ifcObject->vertexOffset < vertexOffset) {
							ASSERT(vertexOffset - ifcObject->vertexOffset >= 0);
							noVertices += (int_t) vertexOffset - ifcObject->vertexOffset;
							if	(noVertices < ifcObject->noVertices) {
								noVertices = ifcObject->noVertices;
							}
							vertexOffset = ifcObject->vertexOffset;
						} else {
							ASSERT(ifcObject->vertexOffset - vertexOffset >= 0);
							if	(noVertices < ifcObject->noVertices + ifcObject->vertexOffset - vertexOffset) {
								noVertices = ifcObject->noVertices + ifcObject->vertexOffset - (int_t) vertexOffset;
							}
						}
						noPrimitivesForFaces += materials->__noPrimitivesForFaces;
					} else {
						g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (int32_t) vertexOffset,  (int32_t) noVertices,  (int32_t) indexOffsetForFaces,  (int32_t) noPrimitivesForFaces);
						facesToDraw = false;
					}
				}

				if	(facesToDraw == false) {
					vertexOffset = ifcObject->vertexOffset;
					noVertices = ifcObject->noVertices;
					indexOffsetForFaces = materials->__indexOffsetForFaces;
					noPrimitivesForFaces = materials->__noPrimitivesForFaces;
					facesToDraw = true;
				}
			}
			materials = materials->next;
		}
		ifcObject = ifcObject->next;
	}
}

bool		wireFrameToDraw = false, linesToDraw = false, pointsToDraw = false;
long long	indexOffsetForWireFrame,
			noPrimitivesForWireFrame;

long long	indexOffsetForLines,
			noPrimitivesForLines,
			indexOffsetForPoints,
			noPrimitivesForPoints;

void	CRightPane::RenderWireFrame()
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		if	(ifcObject->selectState == TI_CHECKED  &&  ifcObject->noPrimitivesForWireFrame) {
			if	(wireFrameToDraw) {
				if	(indexOffsetForWireFrame + 2 * noPrimitivesForWireFrame == ifcObject->indexOffsetForWireFrame) {
					if	(ifcObject->vertexOffset < vertexOffset) {
						ASSERT(vertexOffset - ifcObject->vertexOffset >= 0);
						noVertices += (int_t) vertexOffset - ifcObject->vertexOffset;
						if	(noVertices < ifcObject->noVertices) {
							noVertices = ifcObject->noVertices;
						}
						vertexOffset = ifcObject->vertexOffset;
					} else {
						ASSERT(ifcObject->vertexOffset - vertexOffset >= 0);
						if	(noVertices < ifcObject->noVertices + ifcObject->vertexOffset - vertexOffset) {
							noVertices = ifcObject->noVertices + ifcObject->vertexOffset - (int_t) vertexOffset;
						}
					}
					noPrimitivesForWireFrame += ifcObject->noPrimitivesForWireFrame;
				} else {
					g_pd3dDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, (int32_t) vertexOffset,  (int32_t) noVertices,  (int32_t) indexOffsetForWireFrame,  (int32_t) noPrimitivesForWireFrame);
					wireFrameToDraw = false;
				}
			}

			if	(wireFrameToDraw == false) {
				vertexOffset = ifcObject->vertexOffset;
				noVertices = ifcObject->noVertices;
				indexOffsetForWireFrame = ifcObject->indexOffsetForWireFrame;
				noPrimitivesForWireFrame = ifcObject->noPrimitivesForWireFrame;
				wireFrameToDraw = true;
			}
		}
		ifcObject = ifcObject->next;
	}
}

void	CRightPane::RenderLines()
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		if	(ifcObject->selectState == TI_CHECKED  &&  ifcObject->noPrimitivesForLines) {
			if	(linesToDraw) {
				if	(indexOffsetForLines + 2 * noPrimitivesForLines == ifcObject->indexOffsetForLines) {
					if	(ifcObject->vertexOffset < vertexOffset) {
						ASSERT(vertexOffset - ifcObject->vertexOffset >= 0);
						noVertices += (int_t) vertexOffset - ifcObject->vertexOffset;
						if	(noVertices < ifcObject->noVertices) {
							noVertices = ifcObject->noVertices;
						}
						vertexOffset = ifcObject->vertexOffset;
					} else {
						ASSERT(ifcObject->vertexOffset - vertexOffset >= 0);
						if	(noVertices < ifcObject->noVertices + ifcObject->vertexOffset - vertexOffset) {
							noVertices = ifcObject->noVertices + ifcObject->vertexOffset - (int_t) vertexOffset;
						}
					}
					noPrimitivesForLines += ifcObject->noPrimitivesForLines;
				} else {
					g_pd3dDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, (int32_t) vertexOffset, (int32_t) noVertices, (int32_t) indexOffsetForLines, (int32_t) noPrimitivesForLines);
					linesToDraw = false;
				}
			}

			if	(linesToDraw == false) {
				vertexOffset = ifcObject->vertexOffset;
				noVertices = ifcObject->noVertices;
				indexOffsetForLines = ifcObject->indexOffsetForLines;
				noPrimitivesForLines = ifcObject->noPrimitivesForLines;
				linesToDraw = true;
			}
		}
		ifcObject = ifcObject->next;
	}
}

void	CRightPane::RenderPoints()
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		if	(ifcObject->selectState == TI_CHECKED  &&  ifcObject->noPrimitivesForPoints) {
			if	(pointsToDraw) {
				if	(indexOffsetForPoints + 1 * noPrimitivesForPoints == ifcObject->indexOffsetForPoints) {
					if	(ifcObject->vertexOffset < vertexOffset) {
						ASSERT(vertexOffset - ifcObject->vertexOffset >= 0);
						noVertices += (int_t) vertexOffset - ifcObject->vertexOffset;
						if	(noVertices < ifcObject->noVertices) {
							noVertices = ifcObject->noVertices;
						}
						vertexOffset = ifcObject->vertexOffset;
					} else {
						ASSERT(ifcObject->vertexOffset - vertexOffset >= 0);
						if	(noVertices < ifcObject->noVertices + ifcObject->vertexOffset - vertexOffset) {
							noVertices = ifcObject->noVertices + ifcObject->vertexOffset - (int_t) vertexOffset;
						}
					}
					noPrimitivesForPoints += ifcObject->noPrimitivesForPoints;
				} else {
					g_pd3dDevice->DrawIndexedPrimitive(D3DPT_POINTLIST, 0, (int32_t) vertexOffset,  (int32_t) noVertices,  (int32_t) indexOffsetForPoints,  (int32_t) noPrimitivesForPoints);
					pointsToDraw = false;
				}
			}

			if	(pointsToDraw == false) {
				vertexOffset = ifcObject->vertexOffset;
				noVertices = ifcObject->noVertices;
				indexOffsetForPoints = ifcObject->indexOffsetForPoints;
				noPrimitivesForPoints = ifcObject->noPrimitivesForPoints;
				pointsToDraw = true;
			}
		}
		ifcObject = ifcObject->next;
	}
}

void	CRightPane::Render(bool clean)
{
	if	(!DirectXStatus) {
		// Clear the backbuffer and the zbuffer
		if( FAILED( g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
										 D3DCOLOR_XRGB(255,255,255), 1.0f, 0 ) ) ) {
			DirectXStatus = -1;
			return;
		}

		// Begin the scene
		if	(SUCCEEDED(g_pd3dDevice->BeginScene()))
		{
			// Setup the lights and materials
			if	(SetupLights()) {
				DirectXStatus = -1;
				return;
			}

			// Setup the world, view, and projection matrices
			if	(SetupMatrices()) {
				DirectXStatus = -1;
				return;
			}


			if	(g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX))) {
				DirectXStatus = -1;
				return;
			}

			if( g_pd3dDevice->SetIndices( g_pIB ) ) {
				DirectXStatus = -1;
				return;
			}

			if	(g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX)) {
				DirectXStatus = -1;
				return;
			}

			if	(showFaces  &&  clean == false) {
/*				g_pd3dDevice->SetMaterial(&mtrlDefault);
				RenderFaces(0);

				if	(facesToDraw) {
					g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (int32_t) vertexOffsetForFaces,  (int32_t) noVerticesForFaces,  (int32_t) indexOffsetForFaces,  (int32_t) noPrimitivesForFaces);
					facesToDraw = false;
				}	//	*/

				//
				//	First the non-transparent faces
				//

				for  (std::list<D3DMATERIAL9*>::iterator it=mtrls.begin() ; it != mtrls.end(); ++it) {
					if	((*it)->Ambient.a == 1) {
						g_pd3dDevice->SetMaterial(*it);
						RenderFaces(*it);

						if	(facesToDraw) {
							g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (int32_t) vertexOffset,  (int32_t) noVertices,  (int32_t) indexOffsetForFaces,  (int32_t) noPrimitivesForFaces);
							facesToDraw = false;
						}
					}
				}

				g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
				g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
				g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

				//
				//	Now the transparant faces
				//
				for  (std::list<D3DMATERIAL9*>::iterator it=mtrls.begin() ; it != mtrls.end(); ++it) {
					if	((*it)->Ambient.a < 1) {
						g_pd3dDevice->SetMaterial(*it);
						RenderFaces(*it);

						if	(facesToDraw) {
							g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (int32_t) vertexOffset,  (int32_t) noVertices,  (int32_t) indexOffsetForFaces,  (int32_t) noPrimitivesForFaces);
							facesToDraw = false;
						}
					}
				}

				g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

				g_pd3dDevice->SetMaterial(&mtrlRed);
				RenderFacesHighLighted();
			}

			if (showWireFrame  &&  clean == false) {
				g_pd3dDevice->SetMaterial(&mtrlBlack);

				RenderWireFrame();

				if	(wireFrameToDraw) {
					g_pd3dDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, (int32_t) vertexOffset,  (int32_t) noVertices,  (int32_t) indexOffsetForWireFrame,  (int32_t) noPrimitivesForWireFrame);
					wireFrameToDraw = false;
				}
			}

			if (showLines  &&  clean == false) {
				g_pd3dDevice->SetMaterial(&mtrlBlack);

				RenderLines();

				if (linesToDraw) {
					g_pd3dDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, (int32_t)vertexOffset, (int32_t)noVertices, (int32_t)indexOffsetForLines, (int32_t)noPrimitivesForLines);
					linesToDraw = false;
				}
			}

			if (showPoints  &&  clean == false) {
				g_pd3dDevice->SetMaterial(&mtrlBlack);

				RenderPoints();

				if (pointsToDraw) {
					g_pd3dDevice->DrawIndexedPrimitive(D3DPT_POINTLIST, 0, (int32_t)vertexOffset, (int32_t)noVertices, (int32_t)indexOffsetForPoints, (int32_t)noPrimitivesForPoints);
					pointsToDraw = false;
				}
			}

			// End the scene
			if( FAILED( g_pd3dDevice->EndScene() ) ) {
				DirectXStatus = -1;
				return;
			}
		}

		// Present the backbuffer contents to the display
		if( FAILED( g_pd3dDevice->Present( nullptr, nullptr, nullptr, nullptr ) ) ) {
			//DirectXStatus = -1;
			return;
		}
	}
}

bool	 CRightPane::SetupLights()
{
	mtrlBlack.Diffuse.r = mtrlBlack.Ambient.r = mtrlBlack.Specular.r = 0.0f;
	mtrlBlack.Diffuse.g = mtrlBlack.Ambient.g = mtrlBlack.Specular.g = 0.0f;
	mtrlBlack.Diffuse.b = mtrlBlack.Ambient.b = mtrlBlack.Specular.b = 0.0f;
	mtrlBlack.Diffuse.a = mtrlBlack.Ambient.a = mtrlBlack.Specular.a = 1.0f;
	mtrlBlack.Emissive.r = 0.0f;
	mtrlBlack.Emissive.g = 0.0f;
	mtrlBlack.Emissive.b = 0.0f;
	mtrlBlack.Emissive.a = 0.5f;
	mtrlBlack.Power = 0.5f;

	mtrlRed.Diffuse.r = mtrlRed.Ambient.r = mtrlRed.Specular.r = 0.4f;
	mtrlRed.Diffuse.g = mtrlRed.Ambient.g = mtrlRed.Specular.g = 0.05f;
	mtrlRed.Diffuse.b = mtrlRed.Ambient.b = mtrlRed.Specular.b = 0.05f;
	mtrlRed.Diffuse.a = mtrlRed.Ambient.a = mtrlRed.Specular.a = 1.0f;
	mtrlRed.Emissive.r = 0.1f;
	mtrlRed.Emissive.g = 0.02f;
	mtrlRed.Emissive.b = 0.02f;
	mtrlRed.Emissive.a = 0.5f;
	mtrlRed.Power = 0.5f;

	// Finally, turn on some ambient light.
    //g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );

    // Set up a white, directional light, with an oscillating direction.
    // Note that many lights may be active at a time (but each one slows down
    // the rendering of our scene). However, here we are just using one. Also,
    // we need to set the D3DRS_LIGHTING renderstate to enable lighting
    D3DXVECTOR3 vecDir;
    D3DLIGHT9 light;
    ZeroMemory(&light, sizeof(D3DLIGHT9));
    light.Type       = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 0.1f;
	light.Diffuse.g  = 0.1f;
	light.Diffuse.b  = 0.1f;
	light.Diffuse.a  = 0.1f;
	light.Specular.r = 0.3f;
	light.Specular.g = 0.3f;
	light.Specular.b = 0.3f;
	light.Specular.a = 0.1f;//0.5f;
	light.Ambient.r  = 0.7f;
	light.Ambient.g  = 0.7f;
	light.Ambient.b  = 0.7f;
	light.Ambient.a  = 0.1f;
    light.Position.x = (float) 2.0f;
    light.Position.y = (float) 2.0f;
    light.Position.z = (float) 0.0f;
    vecDir.x = -3.0f;
    vecDir.y = -6.0f;
    vecDir.z = -2.0f;
    D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
    light.Range       = 10.0f;

    D3DLIGHT9 light1;
    ZeroMemory(&light1, sizeof(D3DLIGHT9));
    light1.Type       = D3DLIGHT_DIRECTIONAL;
	light1.Diffuse.r  = 0.2f;
	light1.Diffuse.g  = 0.2f;
	light1.Diffuse.b  = 0.2f;
	light1.Diffuse.a  = 1.0f;
	light1.Specular.r = 0.2f;//0.3f;
	light1.Specular.g = 0.2f;//0.3f;
	light1.Specular.b = 0.2f;//0.3f;
	light1.Specular.a = 1.0f;//0.5f;
	light1.Ambient.r  = 0.2f;//0.7f;
	light1.Ambient.g  = 0.2f;//0.7f;
	light1.Ambient.b  = 0.2f;//0.7f;
	light1.Ambient.a  = 1.0f;
    light1.Position.x = (float) -1.0f;
    light1.Position.y = (float) -1.0f;
    light1.Position.z = (float) -0.5f;
    vecDir.x = 1.0f;
    vecDir.y = 1.0f;
    vecDir.z = 0.5f;
    D3DXVec3Normalize((D3DXVECTOR3*)&light1.Direction, &vecDir);
    light1.Range       = 2.0f;
	if	(FAILED(g_pd3dDevice->SetLight(0, &light1))) {
		DirectXStatus = -1;
		return	true;
	}

	if	(FAILED(g_pd3dDevice->LightEnable(0, TRUE))) {
		DirectXStatus = -1;
		return	true;
	}	//	*/

    D3DLIGHT9 light2;
    ZeroMemory(&light2, sizeof(D3DLIGHT9));
    light2.Type       = D3DLIGHT_DIRECTIONAL;
	light2.Diffuse.r  = 0.2f;
	light2.Diffuse.g  = 0.2f;
	light2.Diffuse.b  = 0.2f;
	light2.Diffuse.a  = 1.0f;
	light2.Specular.r = 0.2f;//0.3f;
	light2.Specular.g = 0.2f;//0.3f;
	light2.Specular.b = 0.2f;//0.3f;
	light2.Specular.a = 1.0f;//0.5f;
	light2.Ambient.r  = 0.2f;//0.7f;
	light2.Ambient.g  = 0.2f;//0.7f;
	light2.Ambient.b  = 0.2f;//0.7f;
	light2.Ambient.a  = 1.0f;
    light2.Position.x = (float) 1.0f;
    light2.Position.y = (float) 1.0f;
    light2.Position.z = (float) 0.5f;
    vecDir.x = -1.0f;
    vecDir.y = -1.0f;
    vecDir.z = -0.5f;
    D3DXVec3Normalize((D3DXVECTOR3*)&light2.Direction, &vecDir);
    light2.Range       = 2.0f;
	if	(FAILED(g_pd3dDevice->SetLight(1, &light2))) {
		DirectXStatus = -1;
		return	true;
	}

	g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00000000 );

	if	(FAILED(g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE))) {
		DirectXStatus = -1;
		return	true;
	}


    // Finally, turn on some ambient light.
//    g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00707070);

	return	false;
}

bool	CRightPane::SetupMatrices()
{
    // For our world matrix, we will just leave it as the identity
    D3DXMATRIX	matWorld;
    D3DXMatrixIdentity( &matWorld );
	
	matWorld._22 = -1.0f;

	D3DXVec3TransformCoord((D3DXVECTOR3 *) &matWorld._41, &m_vectorOrigin, &matWorld);

	if( FAILED( g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld ) ) ) {
		DirectXStatus = -1;
		return	true;
	}

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.

	double	counter = 0;
    D3DXVECTOR3 vEyePt(2.0f, 3 * ((float) sin(counter)), 4 * ((float) cos(counter)));
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVector(0.0f, 1.0f, 0.0f);
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVector );
	if( FAILED( g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView ) ) ) {
		DirectXStatus = -1;
		return	true;
	}

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4,  (float) m_iWidth/(float) m_iHeight, 0.03f, 10.0f );
	if( FAILED( g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj ) ) ) {
		DirectXStatus = -1;
		return	true;
	}

	D3DXMATRIXA16 matrix;

	// Create "D3D Vector" versions of our camera's eye, look at position, and up vector
	D3DXVECTOR3 eye(gCamera->getEye().z, gCamera->getEye().x, gCamera->getEye().y);
	D3DXVECTOR3 lookAt(gCamera->getTarget().z, gCamera->getTarget().x, gCamera->getTarget().y);
	D3DXVECTOR3 up(0, 0, 1); // The world's up vector

	// We create a matrix that represents our camera's view of the world.  Notice
	// the "LH" on the end of the function.  That says, "Hey create this matrix for
	// a left handed coordinate system".
	D3DXMatrixLookAtLH(&matrix, &eye, &lookAt, &up);
	if( FAILED( g_pd3dDevice->SetTransform( D3DTS_VIEW, &matrix ) ) ) {
		DirectXStatus = -1;
	}

	return	false;
}
