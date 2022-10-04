#include "stdafx.h"
#include "ifcviewer.h"
#include "LeftPane.h"
#include "RightPane.h"
#include "ifcviewerDoc.h"
#include "generic.h"
#include "unit.h"
#include "stringCreation.h"
#include "DlgReferenceTree.h"

#include "ifcengine\include\ifcengine.h"


CImageList						* pImageList = nullptr;

extern	STRUCT_TREE_ITEM		* topTreeItem, * topModelTreeItem, * topSpaceBoundaryTreeItem, * topNonReferencedTreeItem, * topGroupTreeItem;
extern	STRUCT__IFC__OBJECT		* ifcObjectsLinkedList,
								* highLightedIfcObject;


// CLeftPane

IMPLEMENT_DYNCREATE(CLeftPane, CTreeView)

CLeftPane::CLeftPane(CWnd* pParent /*=NULL*/)
{
	pParent = pParent;
}

CLeftPane::~CLeftPane()
{
	if (pImageList) {
		pImageList->DeleteImageList();
		delete  pImageList;
		pImageList = nullptr;
	}
}

void CLeftPane::DoDataExchange(CDataExchange* pDX)
{
	CTreeView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLeftPane, CTreeView)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
	ON_NOTIFY_REFLECT(TVN_GETINFOTIP, OnGetInfoTip)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnBeforeExpand)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelectionChanged) 
	ON_COMMAND(ID_VIEW_REFERENCES, &CLeftPane::OnViewReferences)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REFERENCES, &CLeftPane::OnUpdateViewReferences)
END_MESSAGE_MAP()


void CLeftPane::OnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVGETINFOTIP	pGetInfoTip = (LPNMTVGETINFOTIP)pNMHDR;
	STRUCT_TREE_ITEM	* treeItem = (STRUCT_TREE_ITEM*) GetTreeCtrl().GetItemData(pGetInfoTip->hItem);

	int_t	ifcInstance = 0;
	switch (treeItem->type) {
		case TREE_ITEM_CONTAINS:
		case TREE_ITEM_NOTREFERENCED:
		case TREE_ITEM_GROUPS:
		case TREE_ITEM_GROUPEDBY:
		case TREE_ITEM_DECOMPOSEDBY:
		case TREE_ITEM_SPACEBOUNDARIES:
		case TREE_ITEM_IFCENTITY:
			break;
		case TREE_ITEM_IFCINSTANCE:
			ifcInstance = ((STRUCT_TREE_ITEM_IFCINSTANCE*) treeItem)->ifcInstance;
			break;
		case TREE_ITEM_GEOMETRY:
		case TREE_ITEM_PROPERTIES:
			ifcInstance = ((STRUCT_TREE_ITEM_IFCINSTANCE*) treeItem->parent)->ifcInstance;
			break;
		case TREE_ITEM_PROPERTYSET:
			ifcInstance = ((STRUCT_TREE_ITEM_PROPERTYSET*) treeItem)->ifcInstance;
		case TREE_ITEM_TEXT:
			break;
		case TREE_ITEM_PROPERTY:
			ifcInstance = ((STRUCT_TREE_ITEM_PROPERTY*) treeItem)->ifcInstance;
			break;
		default:
			ASSERT(false);
			break;
	}

	CString	strItemTxt = 0;
	if	(ifcInstance) {
		strItemTxt = CreateToolTipText(ifcInstance);
	}

	if	(strItemTxt  &&  strItemTxt[0]) {
		strItemTxt = strItemTxt.Left(pGetInfoTip->cchTextMax - 1);
		StrCpyW(pGetInfoTip->pszText, strItemTxt);
	}

	*pResult = 0;
}

void CLeftPane::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	if (pImageList == nullptr) {
		pImageList = new CImageList();
		pImageList->Create(16, 16, ILC_COLOR4, 6, 6);

		CBitmap bitmap;

		bitmap.LoadBitmap(IDB_PROPERTY_SET);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();

		bitmap.LoadBitmap(IDB_SELECTED_ALL);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();

		bitmap.LoadBitmap(IDB_SELECTED_PART);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();

		bitmap.LoadBitmap(IDB_SELECTED_NONE);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();

		bitmap.LoadBitmap(IDB_PROPERTY);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();

		bitmap.LoadBitmap(IDB_NONE);
		pImageList->Add(&bitmap, (COLORREF)0x000000);
		bitmap.DeleteObject();
	}

	CTreeCtrl *tst = &GetTreeCtrl();

	::SetWindowLong(*tst, GWL_STYLE, TVS_SHOWSELALWAYS|TVS_EDITLABELS|TVS_LINESATROOT|TVS_HASLINES|TVS_HASBUTTONS|TVS_INFOTIP|::GetWindowLong(*tst, GWL_STYLE));

	GetTreeCtrl().SetImageList(pImageList, TVSIL_NORMAL);
	GetTreeCtrl().DeleteAllItems();

	BuildTreeItem(topTreeItem);
}

void	CLeftPane::BuildTreeItem(STRUCT_TREE_ITEM * treeItem)
{
	while  (treeItem) {
		TV_INSERTSTRUCT		tvstruct;
		if	(treeItem->parent) {
			ASSERT(treeItem->parent->hTreeItem);
			tvstruct.hParent = treeItem->parent->hTreeItem;
			tvstruct.hInsertAfter = TVI_LAST;
		} else {
			tvstruct.hParent = nullptr;
			tvstruct.hInsertAfter = TVI_FIRST;
		}
		tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
		if	(treeItem->child) {
tvstruct.item.cChildren = 1;
		}
 else {
	 tvstruct.item.cChildren = 0;

	 if (treeItem->type == TREE_ITEM_PROPERTIES) {
		 ASSERT(treeItem->parent  &&  treeItem->parent->type == TREE_ITEM_IFCINSTANCE);
		 int_t	ifcModel = ((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItem->parent)->ifcModel,
			 ifcInstance = ((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItem->parent)->ifcInstance;
		 if (IfcInstanceHasProperties(ifcModel, ifcInstance)) {
			 tvstruct.item.cChildren = 1;
		 }
	 }
 }

 tvstruct.item.pszText = GetTreeItemName(treeItem);

 switch (treeItem->type) {
 case TREE_ITEM_CONTAINS:
 case TREE_ITEM_NOTREFERENCED:
 case TREE_ITEM_GROUPEDBY:
 case TREE_ITEM_DECOMPOSEDBY:
 case TREE_ITEM_SPACEBOUNDARIES:
 case TREE_ITEM_IFCENTITY:
 case TREE_ITEM_IFCINSTANCE:
 case TREE_ITEM_GEOMETRY:
	 switch (((STRUCT_TREE_ITEM_SELECTABLE*)treeItem)->selectState) {
	 case TI_CHECKED:
		 tvstruct.item.iImage = ITEM_CHECKED;
		 tvstruct.item.iSelectedImage = ITEM_CHECKED;
		 break;
	 case TI_UNCHECKED:
		 tvstruct.item.iImage = ITEM_UNCHECKED;
		 tvstruct.item.iSelectedImage = ITEM_UNCHECKED;
		 break;
	 case TI_PARTLY_CHECKED:
		 tvstruct.item.iImage = ITEM_PARTLY_CHECKED;
		 tvstruct.item.iSelectedImage = ITEM_PARTLY_CHECKED;
		 break;
	 case TI_NONE:
		 tvstruct.item.iImage = ITEM_NONE;
		 tvstruct.item.iSelectedImage = ITEM_NONE;
		 break;
	 default:
		 ASSERT(false);
		 break;
	 }
	 break;
 case TREE_ITEM_PROPERTIES:
 case TREE_ITEM_PROPERTYSET:
 case TREE_ITEM_TEXT:
 case TREE_ITEM_GROUPS:
	 tvstruct.item.iImage = ITEM_PROPERTY_SET;
	 tvstruct.item.iSelectedImage = ITEM_PROPERTY_SET;
	 break;
 case TREE_ITEM_PROPERTY:
	 tvstruct.item.iImage = ITEM_PROPERTY;
	 tvstruct.item.iSelectedImage = ITEM_PROPERTY;
	 break;
 default:
	 ASSERT(false);
	 break;
 }
 tvstruct.item.lParam = (LPARAM)treeItem;

 ASSERT(treeItem->hTreeItem == 0);
 treeItem->hTreeItem = GetTreeCtrl().InsertItem(&tvstruct);

 treeItem = treeItem->next;
	}
}

void	CLeftPane::Expand(
						STRUCT_TREE_ITEM		* treeItem
					)
{
	if (treeItem) {
		Expand(treeItem->parent);

		if (treeItem->hTreeItem) {
			GetTreeCtrl().Expand(treeItem->hTreeItem, TVE_EXPAND);
		}
		else {
			ASSERT(false);
		}
	}
}

bool	CLeftPane::SelectTreeItem(
						STRUCT__IFC__OBJECT		* ifcObject,
						STRUCT_TREE_ITEM		* treeItem
					)
{
	while (treeItem) {
		if ((treeItem->type == TREE_ITEM_IFCINSTANCE) &&
			(((STRUCT_TREE_ITEM_IFCINSTANCE*) treeItem)->ifcObject == ifcObject)) {
			Expand(treeItem);

			if (treeItem->hTreeItem) {
				GetTreeCtrl().SetFocus();
				GetTreeCtrl().SelectItem(treeItem->hTreeItem);
			} else {
				ASSERT(false);
			}

			return	true;
		}

		if (SelectTreeItem(ifcObject, treeItem->child)) {
			return	true;
		}

		treeItem = treeItem->next;
	}

	return	false;
}

void	CLeftPane::UpdateParent(
				STRUCT_TREE_ITEM_SELECTABLE	* parentSelectableTreeitem
			)
{
	ASSERT(parentSelectableTreeitem);

	if (parentSelectableTreeitem->type == TREE_ITEM_GROUPS) {
		return;
	}

	bool	checked = false, unchecked = false;

	STRUCT_TREE_ITEM_SELECTABLE	* treeItemSelectable = (STRUCT_TREE_ITEM_SELECTABLE*) parentSelectableTreeitem->child;
	while (treeItemSelectable) {
		switch (treeItemSelectable->type) {
			case TREE_ITEM_CONTAINS:
			case TREE_ITEM_DECOMPOSEDBY:
			case TREE_ITEM_GROUPEDBY:
			case TREE_ITEM_SPACEBOUNDARIES:
			case TREE_ITEM_IFCINSTANCE:
			case TREE_ITEM_IFCENTITY:
			case TREE_ITEM_GEOMETRY:
				{
					switch (treeItemSelectable->selectState) {
						case  TI_CHECKED:
							checked = true;
							break;
						case  TI_PARTLY_CHECKED:
							checked = true;
							unchecked = true;
							break;
						case  TI_UNCHECKED:
							unchecked = true;
							break;
						case TI_NONE:
							break;
						default:
							ASSERT(false);
							break;
					}
				}
				break;
			case TREE_ITEM_PROPERTIES:
			case TREE_ITEM_PROPERTYSET:
			case TREE_ITEM_PROPERTY:
			case TREE_ITEM_TEXT:
				break;
			default:
				ASSERT(false);
				break;
		}
		treeItemSelectable = (STRUCT_TREE_ITEM_SELECTABLE*) treeItemSelectable->next;
	}

	ASSERT(checked  ||  unchecked);

	ENUM_TREE_ITEM_SELECT_STATE	selectState;
	if (checked) {
		if (unchecked) {
			selectState = TI_PARTLY_CHECKED;
		} else {
			selectState = TI_CHECKED;
		}
	} else {
		ASSERT(unchecked);
		selectState = TI_UNCHECKED;
	}

	if (parentSelectableTreeitem->selectState != selectState) {
		parentSelectableTreeitem->selectState = selectState;
		if (parentSelectableTreeitem->hTreeItem) {
			GetTreeCtrl().SetItemImage(parentSelectableTreeitem->hTreeItem, selectState, selectState);
		} else {
			ASSERT(false);
		}
		if (parentSelectableTreeitem->parent) {
			UpdateParent((STRUCT_TREE_ITEM_SELECTABLE*) parentSelectableTreeitem->parent);
		}
	}
}

void	CLeftPane::UpdateChildren(
						STRUCT_TREE_ITEM			* parentTreeitem,
						ENUM_TREE_ITEM_SELECT_STATE	selectState
					)
{
	ASSERT(parentTreeitem);
	ASSERT(selectState == TI_CHECKED || selectState == TI_UNCHECKED);

	STRUCT_TREE_ITEM_SELECTABLE	* treeItemSelectable = (STRUCT_TREE_ITEM_SELECTABLE*)parentTreeitem->child;
	while (treeItemSelectable) {
		switch (treeItemSelectable->type) {
			case TREE_ITEM_CONTAINS:
			case TREE_ITEM_DECOMPOSEDBY:
			case TREE_ITEM_GROUPEDBY:
			case TREE_ITEM_SPACEBOUNDARIES:
			case TREE_ITEM_IFCENTITY:
//#if !PRODUCE_FLAT_TREE
			case TREE_ITEM_IFCINSTANCE:
//#endif 
				{
					if (treeItemSelectable->selectState != selectState  &&  treeItemSelectable->selectState != TI_NONE) {
						treeItemSelectable->selectState = selectState;
						if (treeItemSelectable->hTreeItem) {
							GetTreeCtrl().SetItemImage(treeItemSelectable->hTreeItem, selectState, selectState);
						}
						ASSERT(treeItemSelectable->child);
						UpdateChildren(treeItemSelectable, selectState);
					}
				}
				break;
#if 0 //PRODUCE_FLAT_TREE
			case TREE_ITEM_IFCINSTANCE:
			{
				if (treeItemSelectable->selectState != selectState && treeItemSelectable->selectState != TI_NONE) {
					auto pInstance = (STRUCT_TREE_ITEM_IFCINSTANCE*) treeItemSelectable;
					if (pInstance->ifcObject) {
						//	THIS IS NOT ALWAYS TRUE AS SPACEBOUNDARY TREE CONTAINS THE SAME OBJECT MORE THAN ONCE							ASSERT(((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject->selectState == treeItemSelectable->selectState);
						pInstance->ifcObject->selectState = selectState;
						treeItemSelectable->selectState = selectState;
						if (treeItemSelectable->hTreeItem) {
							GetTreeCtrl().SetItemImage(treeItemSelectable->hTreeItem, selectState, selectState);
						}
					}
					else {
						ASSERT(treeItemSelectable->selectState == TI_NONE);
					}
				}
				else {
					auto pInstance = (STRUCT_TREE_ITEM_IFCINSTANCE*) treeItemSelectable;
					if (pInstance->ifcObject) {
						ASSERT(pInstance->ifcObject->selectState == treeItemSelectable->selectState);
					}
					else {
						ASSERT(treeItemSelectable->selectState == TI_NONE);
					}
				}

				if (treeItemSelectable->child == nullptr) {
					UpdateChildren(treeItemSelectable, selectState);
				}
			}
			break;
#endif 
			case TREE_ITEM_GEOMETRY:
				{
					if (treeItemSelectable->selectState != selectState  &&  treeItemSelectable->selectState != TI_NONE) {
						if (((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject) {
//	THIS IS NOT ALWAYS TRUE AS SPACEBOUNDARY TREE CONTAINS THE SAME OBJECT MORE THAN ONCE							ASSERT(((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject->selectState == treeItemSelectable->selectState);
							((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject->selectState = selectState;
							treeItemSelectable->selectState = selectState;
							if (treeItemSelectable->hTreeItem) {
								GetTreeCtrl().SetItemImage(treeItemSelectable->hTreeItem, selectState, selectState);
							}
						} else {
							ASSERT(treeItemSelectable->selectState == TI_NONE);
						}
						ASSERT(treeItemSelectable->child == nullptr);
					} else {
						if (((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject) {
							ASSERT(((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject->selectState == treeItemSelectable->selectState);
						} else {
							ASSERT(treeItemSelectable->selectState == TI_NONE);
						}
					}
				}
				break;
			case TREE_ITEM_PROPERTIES:
			case TREE_ITEM_PROPERTYSET:
			case TREE_ITEM_PROPERTY:
			case TREE_ITEM_TEXT:
				break;
			default:
				ASSERT(false);
				break;
		}
		treeItemSelectable = (STRUCT_TREE_ITEM_SELECTABLE*) treeItemSelectable->next;
	}
}

void	CLeftPane::UpdateTree(
						STRUCT_TREE_ITEM	* treeItem,
						bool				* pChecked,
						bool				* pUnchecked
					)
{
	ASSERT(treeItem && (*pChecked) == false && (*pUnchecked) == false);

	STRUCT_TREE_ITEM_SELECTABLE	* treeItemSelectable = (STRUCT_TREE_ITEM_SELECTABLE*) treeItem->child;
	while (treeItemSelectable) {
		switch (treeItemSelectable->type) {
			case TREE_ITEM_CONTAINS:
			case TREE_ITEM_DECOMPOSEDBY:
			case TREE_ITEM_GROUPEDBY:
			case TREE_ITEM_SPACEBOUNDARIES:
			case TREE_ITEM_IFCINSTANCE:
			case TREE_ITEM_IFCENTITY:
				{
					bool	checked = false, unchecked = false;
					if (treeItemSelectable->child) {
						UpdateTree(
								treeItemSelectable,
								&checked,
								&unchecked
							);
					} else {
						ASSERT(treeItemSelectable->type == TREE_ITEM_IFCINSTANCE);
						ASSERT(treeItemSelectable->selectState == TI_NONE);
					}

					ENUM_TREE_ITEM_SELECT_STATE	selectState;
					if (checked) {
						(*pChecked) = true;
						if (unchecked) {
							(*pUnchecked) = true;
							selectState = TI_PARTLY_CHECKED;
						} else {
							selectState = TI_CHECKED;
						}
					} else {
						if (unchecked) {
							(*pUnchecked) = true;
							selectState = TI_UNCHECKED;
						} else {
							selectState = TI_NONE;
						}
					}

					if (treeItemSelectable->selectState != selectState) {
						treeItemSelectable->selectState = selectState;
						if (treeItemSelectable->hTreeItem) {
							GetTreeCtrl().SetItemImage(treeItemSelectable->hTreeItem, selectState, selectState);
						}
					}
				}
				break;
			case TREE_ITEM_GEOMETRY:
				{
					ASSERT((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent);

					if (((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject) {
						ENUM_TREE_ITEM_SELECT_STATE	selectState = ((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject->selectState;
						if (treeItemSelectable->selectState != selectState) {
							treeItemSelectable->selectState = selectState;
							if (treeItemSelectable->hTreeItem) {
								GetTreeCtrl().SetItemImage(treeItemSelectable->hTreeItem, selectState, selectState);
							}
						}

						switch (treeItemSelectable->selectState) {
							case TI_CHECKED:
								(*pChecked) = true;
								break;
							case TI_UNCHECKED:
								(*pUnchecked) = true;
								break;
							case TI_NONE:
								break;
							default:
								ASSERT(false);
								break;
						}
					} else {
						ASSERT(treeItemSelectable->selectState == TI_NONE);
					}
				}
				break;
			case TREE_ITEM_PROPERTIES:
			case TREE_ITEM_PROPERTYSET:
			case TREE_ITEM_PROPERTY:
			case TREE_ITEM_TEXT:
				break;
			default:
				ASSERT(false);
				break;
		}
		treeItemSelectable = (STRUCT_TREE_ITEM_SELECTABLE*)treeItemSelectable->next;
	}
}

void	CLeftPane::UpdateTree(
						STRUCT_TREE_ITEM_SELECTABLE	* treeItemSelectable
					)
{
	bool	checked = false, unchecked = false;
	UpdateTree(
			treeItemSelectable,
			&checked,
			&unchecked
		);

	if (treeItemSelectable->type != TREE_ITEM_GROUPS) {
		ENUM_TREE_ITEM_SELECT_STATE	selectState;
		if (checked) {
			if (unchecked) {
				selectState = TI_PARTLY_CHECKED;
			} else {
				selectState = TI_CHECKED;
			}
		} else {
			if (unchecked) {
				selectState = TI_UNCHECKED;
			} else {
				selectState = TI_NONE;
			}
		}

		if (treeItemSelectable->selectState != selectState) {
			treeItemSelectable->selectState = selectState;
			if (treeItemSelectable->hTreeItem) {
				GetTreeCtrl().SetItemImage(treeItemSelectable->hTreeItem, selectState, selectState);
			}
		}
	}
}

void	CLeftPane::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	pNMHDR = pNMHDR;
	pResult = pResult;

	DWORD	posa = GetMessagePos();
	CPoint	pta(LOWORD(posa), HIWORD(posa));

	HMENU	hMenu = ::CreatePopupMenu();
	int_t	i = 1;

	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		bool	visualized = false, realGeometry = false;
		int_t	currentIfcEntity = ifcObject->ifcEntity;
		int32_t	flags = 0;
		while (ifcObject  &&  ifcObject->ifcEntity == currentIfcEntity) {
			if ((visualized == false) && 
				(ifcObject->selectState == TI_CHECKED)) {
				visualized = true;
				flags |= MF_CHECKED;
			}
			if ((realGeometry == false) &&
				(ifcObject->noPrimitivesForPoints || ifcObject->noPrimitivesForLines || ifcObject->noPrimitivesForFaces || ifcObject->noPrimitivesForWireFrame)) {
				realGeometry = true;
			}
			ifcObject = ifcObject->next;
		}
		if	(!realGeometry) {
			flags |= MF_DISABLED;
		}
		wchar_t	* entityName = 0;
		engiGetEntityName(currentIfcEntity, sdaiUNICODE, (const char**) &entityName);
		::AppendMenu(hMenu, flags, i++, entityName);
	}

	int_t cmdViewReferences = -1;
	auto instance = GetInstance(&pta);
	if (instance) {
		cmdViewReferences = i++;
		::AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		CString strViewRefereneces;
		strViewRefereneces.LoadString(IDS_VIEW_REFERENCES);
		::AppendMenu(hMenu, 0, cmdViewReferences, strViewRefereneces);
	}

	int_t sel = ::TrackPopupMenuEx(hMenu, 
		TPM_CENTERALIGN | TPM_RETURNCMD,
		pta.x,//pt.x + r.right,
		pta.y,//pt.y + r.top,
		GetTreeCtrl(),
		NULL);
	::DestroyMenu(hMenu);

	if (sel == cmdViewReferences) {
		OpenRefereneceTree(instance);
	}
	else if (sel > 0) {
		ifcObject = ifcObjectsLinkedList;
		while  (ifcObject) {
			int_t	currentIfcEntity = ifcObject->ifcEntity;
			sel--;
			if	(sel == 0) {
				STRUCT__IFC__OBJECT	* ifcStartingObject = ifcObject;
				bool	visualized = false;
				while  (ifcObject  &&  ifcObject->ifcEntity == currentIfcEntity) {
					if	(ifcObject->selectState == TI_CHECKED) {
						visualized = true;
					}
					ifcObject = ifcObject->next;
				}
				ifcObject = ifcStartingObject;
				while  (ifcObject  &&  ifcObject->ifcEntity == currentIfcEntity) {
					if	(visualized) {
						if	(ifcObject->selectState == TI_CHECKED) {
							ifcObject->selectState = TI_UNCHECKED;
						}
					} else {
						if (ifcObject->selectState == TI_UNCHECKED) {
							ifcObject->selectState = TI_CHECKED;
						}
					}
					ifcObject = ifcObject->next;
				}
			} else {
				while  (ifcObject  &&  ifcObject->ifcEntity == currentIfcEntity) {
					ifcObject = ifcObject->next;
				}
			}
		}

		//
		//	Update Left Pane
		//
		if (topModelTreeItem) {
			UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*)topModelTreeItem);
		}
		if (topSpaceBoundaryTreeItem) {
			UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*)topSpaceBoundaryTreeItem);
		}
		if (topNonReferencedTreeItem) {
			UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*)topNonReferencedTreeItem);
		}
		if (topGroupTreeItem) {
			if (topGroupTreeItem->child  &&  topGroupTreeItem->child->hTreeItem) {
				UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*) topGroupTreeItem);
			}
		}

		//
		//	Update Right Pane
		//
		GetRightPane()->SendMessage(IDS_UPDATE_RIGHT_PANE, 0, 0);
	}
}

STRUCT_TREE_ITEM	* TopTreeItem(
							STRUCT_TREE_ITEM	* treeItem
						)
{
	if (treeItem->parent) {
		return	TopTreeItem(treeItem->parent);
	}
	return	treeItem;
}

void	CLeftPane::UpdateAllTrees(
						STRUCT_TREE_ITEM_SELECTABLE		* treeItemSelectable,
						ENUM_TREE_ITEM_SELECT_STATE		selectState
					)
{
	ASSERT(treeItemSelectable->selectState != selectState);

	switch (treeItemSelectable->type) {
		case TREE_ITEM_CONTAINS:
		case TREE_ITEM_DECOMPOSEDBY:
		case TREE_ITEM_GROUPEDBY:
		case TREE_ITEM_NOTREFERENCED:
		case TREE_ITEM_SPACEBOUNDARIES:
		case TREE_ITEM_IFCINSTANCE:
		case TREE_ITEM_IFCENTITY:
		case TREE_ITEM_GEOMETRY:
			break;
		default:
			ASSERT(false);
			break;
	}

	treeItemSelectable->selectState = selectState;
	if (treeItemSelectable->type == TREE_ITEM_GEOMETRY) {
		if (((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject) {
			((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject->selectState = selectState;
		}
	}
	GetTreeCtrl().SetItemImage(treeItemSelectable->hTreeItem, selectState, selectState);

	//
	//	Update Children
	//
	UpdateChildren(treeItemSelectable, selectState);

	//
	//	Update Parents
	//
	if (treeItemSelectable->parent) {
		UpdateParent((STRUCT_TREE_ITEM_SELECTABLE*)treeItemSelectable->parent);
	}

	//
	//	Update trees (not needed in case of non-referenced items selection)
	//
	if (topGroupTreeItem  &&  TopTreeItem(treeItemSelectable) == topGroupTreeItem) {
		if (topModelTreeItem) {
			UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*)topModelTreeItem);
		}
		if (topSpaceBoundaryTreeItem) {
			UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*)topSpaceBoundaryTreeItem);
		}
		if (topNonReferencedTreeItem) {
			UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*)topNonReferencedTreeItem);
		}
	}
	else {
		if (topModelTreeItem  &&  TopTreeItem(treeItemSelectable) != topModelTreeItem) {
			UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*)topModelTreeItem);
		}

		if (topSpaceBoundaryTreeItem) {
			UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*)topSpaceBoundaryTreeItem);
		}

		if (topGroupTreeItem) {
			if (topGroupTreeItem->child  &&  topGroupTreeItem->child->hTreeItem) {
				UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*)topGroupTreeItem);
			}
		}
	}

	GetRightPane()->SendMessage(IDS_UPDATE_RIGHT_PANE, 0, 0);
}


void	CLeftPane::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	pNMHDR = pNMHDR;

	STRUCT_TREE_ITEM_SELECTABLE	* treeItemSelectable;

	DWORD		pos = GetMessagePos();
	CPoint		pt(LOWORD(pos), HIWORD(pos));
	GetTreeCtrl().ScreenToClient(&pt);

	HTREEITEM	hItem = GetTreeCtrl().GetFirstVisibleItem();
	bool		updated = false;

	while  (hItem) {
		CRect r;
		GetTreeCtrl().GetItemRect(hItem, &r, true);
		r.right = r.left-4;
		r.left = r.left-18;

		if (r.PtInRect(pt))
		{
//			bool	quit = false;

			treeItemSelectable = (STRUCT_TREE_ITEM_SELECTABLE *)GetTreeCtrl().GetItemData(hItem);

			if (treeItemSelectable) {
				ASSERT(treeItemSelectable->hTreeItem);
				switch (treeItemSelectable->type) {
					case TREE_ITEM_CONTAINS:
					case TREE_ITEM_DECOMPOSEDBY:
					case TREE_ITEM_GROUPEDBY:
					case TREE_ITEM_NOTREFERENCED:
					case TREE_ITEM_SPACEBOUNDARIES:
					case TREE_ITEM_IFCINSTANCE:
					case TREE_ITEM_IFCENTITY:
					case TREE_ITEM_GEOMETRY:
						{
							ENUM_TREE_ITEM_SELECT_STATE	selectState = TI_NONE;
							switch (treeItemSelectable->selectState) {
								case  TI_CHECKED:
								case  TI_PARTLY_CHECKED:
									selectState = TI_UNCHECKED;
									break;
								case  TI_UNCHECKED:
									selectState = TI_CHECKED;
									break;
								case TI_NONE:
									break;
								default:
									ASSERT(false);
									break;
							}

							if (selectState != TI_NONE) {
								UpdateAllTrees(
										treeItemSelectable,
										selectState
									);
							}
						}
						break;
					case TREE_ITEM_PROPERTIES:
					case TREE_ITEM_PROPERTYSET:
					case TREE_ITEM_PROPERTY:
					case TREE_ITEM_TEXT:
						break;
					default:
						ASSERT(false);
						break;
				}
			}
		}

		hItem = GetTreeCtrl().GetNextVisibleItem(hItem);
	}

	//
	//	Update right window
	//

	if	(updated) {
		GetRightPane()->SendMessage(IDS_UPDATE_RIGHT_PANE, 0, 0);
	}

	*pResult = 0;
}

void	CLeftPane::OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW	* pnmtv = (NMTREEVIEW*) pNMHDR;
	HTREEITEM	hItem = pnmtv->itemNew.hItem;

	pResult = 0;
	hItem = 0;

	auto instance = GetInstance();
	CifcviewerDoc::ActiveInstanceHint hint(instance);
	GetDocument()->UpdateAllViews(this, (LPARAM) CifcviewerDoc::UpdateHint::SetActiveInstance, &hint);

////////////////////////////////////////	ASSERT(false);
//	STRUCT__SELECTABLE__TREEITEM	* selectableTreeitem = (STRUCT__SELECTABLE__TREEITEM *) GetTreeCtrl().GetItemData(hItem);
//	if	(selectableTreeitem->structType == STRUCT_TYPE_SELECTABLE_TREEITEM) {
//		highLightedIfcObject = selectableTreeitem->ifcObject;
//		GetRightPane()->SendMessage(IDS_UPDATE_RIGHT_PANE, 0, 0);
//	}
}

void	CLeftPane::OnBeforeExpand(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW			* pnmtv = (NMTREEVIEW*) pNMHDR;
	HTREEITEM			hItem = pnmtv->itemNew.hItem;

	pResult = 0;

	STRUCT_TREE_ITEM	* treeItem = (STRUCT_TREE_ITEM *) GetTreeCtrl().GetItemData(hItem);

	if (treeItem->child  &&  treeItem->child->hTreeItem) {
		return;
	}

	if (treeItem->type == TREE_ITEM_PROPERTIES) {
		ASSERT(treeItem->child == nullptr);
		ASSERT(treeItem->parent  &&  treeItem->parent->type == TREE_ITEM_IFCINSTANCE);

		int_t	ifcModel = ((STRUCT_TREE_ITEM_IFCINSTANCE*) treeItem->parent)->ifcModel,
				ifcInstance = ((STRUCT_TREE_ITEM_IFCINSTANCE*) treeItem->parent)->ifcInstance;

		STRUCT__PROPERTY__SET	* propertySets = nullptr;
		CreateIfcInstanceProperties(ifcModel, &propertySets, ifcInstance, units);
		CreateTreeItems(treeItem, propertySets);

		DeleteIfcInstanceProperties(propertySets);
	} else if (treeItem->type == TREE_ITEM_GROUPS) {
		ASSERT(treeItem == topGroupTreeItem);
		UpdateTree((STRUCT_TREE_ITEM_SELECTABLE*) treeItem);
	}

	if (treeItem->child) {
		if (treeItem->child->hTreeItem == 0) {
			BuildTreeItem(treeItem->child);
		}
	} else {
		ASSERT(false);
	}
}

LRESULT CLeftPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
//	STRUCT_TREE_ITEM	* treeitem;
//	HTREEITEM			hItem;
	switch  (message)
	{
		case IDS_LOAD_IFC:
			GetTreeCtrl().DeleteAllItems();
			break;
		case IDS_DISABLE_ITEM:
			if	(highLightedIfcObject) {
				UpdateAllTrees(highLightedIfcObject->treeItemModel, TI_UNCHECKED);
			}
			break;
		case IDS_SELECT_ITEM:
			if (SelectTreeItem(highLightedIfcObject, topModelTreeItem) == false) {
				SelectTreeItem(highLightedIfcObject, topTreeItem);
			}
			break;
		default:
			break;
	}

	return	CTreeView::WindowProc(message, wParam, lParam);
}

// CLeftPane diagnostics

#ifdef _DEBUG
void CLeftPane::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftPane::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG

CWnd* CLeftPane::GetRightPane()
{
	auto pDoc = DYNAMIC_DOWNCAST(CifcviewerDoc, GetDocument());
	if (pDoc) {
		return pDoc->GetPane(RUNTIME_CLASS(CRightPane));
	}
	ASSERT(0);
	return nullptr;
}

HTREEITEM CLeftPane::ItemFromScreenPoint(CPoint pt)
{
	auto& wndTree = GetTreeCtrl();

	wndTree.ScreenToClient(&pt);

	HTREEITEM hItem = wndTree.GetFirstVisibleItem();
	while (hItem) {
		CRect r;
		GetTreeCtrl().GetItemRect(hItem, &r, true);
		if (r.PtInRect(pt)) {
			return hItem;
		}

		hItem = wndTree.GetNextVisibleItem(hItem);
	}

	return NULL;
}

SdaiInstance CLeftPane::GetInstance(const CPoint* ppt)
{
	HTREEITEM hItem = NULL;
	if (ppt) {
		hItem = ItemFromScreenPoint(*ppt);
	}
	else {
		hItem = GetTreeCtrl().GetSelectedItem();
	}

	if (!hItem) {
		return NULL;
	}

	auto data = GetTreeCtrl().GetItemData(hItem);
	auto pItem = (STRUCT_TREE_ITEM*) data;

	const wchar_t* attrName = NULL;

	while (pItem) {
		if (pItem->type == TREE_ITEM_IFCINSTANCE) {
			auto pInst = (STRUCT_TREE_ITEM_IFCINSTANCE*) pItem;
			if (attrName) {
				SdaiInstance inst = NULL;
				sdaiGetAttrBN(pInst->ifcInstance, (char*)attrName, sdaiINSTANCE, &inst);
				return inst;
			}
			else {
				return pInst->ifcInstance;
			}
		}
		else if (pItem->type == TREE_ITEM_GEOMETRY) {
			attrName = L"Representation";
		}
		pItem = pItem->parent;
	}

	return NULL;
}

STRUCT_TREE_ITEM_IFCINSTANCE* CLeftPane::FindInstance(int_t instance, HTREEITEM* pItem, HTREEITEM hStartFrom)
{
	if (!hStartFrom) {
		return nullptr;
	}

	auto dwData = GetTreeCtrl().GetItemData(hStartFrom);
	auto pData = (STRUCT_TREE_ITEM*) dwData;
	if (pData && pData->type == ENUM_TREE_ITEM_TYPES::TREE_ITEM_IFCINSTANCE) {
		auto pInst = (STRUCT_TREE_ITEM_IFCINSTANCE*) pData;
		if (pInst && pInst->ifcInstance == instance) {
			//FOUND!
			if (pItem)
				*pItem = hStartFrom;
			return pInst; //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		}
	}

	if (pData && pData->child) {
		if (pData->child->hTreeItem == 0) {
			BuildTreeItem(pData->child);
		}
	}

	STRUCT_TREE_ITEM_IFCINSTANCE* pFound = FindInstance(instance, pItem, GetTreeCtrl().GetNextSiblingItem(hStartFrom));
	if (pFound) {
		return pFound;
	}

	return FindInstance(instance, pItem, GetTreeCtrl().GetChildItem(hStartFrom));
}

bool CLeftPane::SelectInstance(int_t instance)
{
	if (!instance) {
		auto res = GetTreeCtrl().Select(nullptr, TVGN_CARET);
		return res;
	}

	HTREEITEM hItem = nullptr;
	auto pItem = FindInstance(instance, &hItem, GetTreeCtrl().GetRootItem());
	
	if (!pItem)
		return false; 

	auto res = GetTreeCtrl().Select(hItem, TVGN_CARET);

	return res;

/*	if (highLightedIfcObject) {
		this->GetWindow(GW_HWNDPREV)->SendMessage(IDS_SELECT_ITEM, 0, 0);
	}*/
}

void CLeftPane::OpenRefereneceTree(int_t instance)
{
	if (instance) {
		auto pDlg = new CDlgReferenceTree(instance, AfxGetMainWnd());
		pDlg->Create(IDD_REFERENCEVIEW);
		pDlg->ShowWindow(SW_SHOW);
	}
}


void CLeftPane::OnViewReferences()
{
	auto instance = GetInstance();
	OpenRefereneceTree(instance);
}

void CLeftPane::OnUpdateViewReferences(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetInstance()!=NULL);
}
