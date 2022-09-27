#include "stdafx.h"
#include "treeitems.h"
#include "stringCreation.h"

#include <unordered_map>


extern	std::unordered_map<int_t, STRUCT__IFC__OBJECT *> allIfcObjects;


void	InitTreeItem(
				STRUCT_TREE_ITEM	* treeItem,
				STRUCT_TREE_ITEM	* parent
			)
{
	ASSERT(treeItem->type);

	treeItem->nameBuffer = nullptr;

	treeItem->hTreeItem = 0;

	treeItem->parent = parent;
	treeItem->child = nullptr;
	treeItem->next = nullptr;
}


void	InitTreeItemSelectable(
				STRUCT_TREE_ITEM_SELECTABLE	* treeItem,
				STRUCT_TREE_ITEM			* parent
			)
{
	InitTreeItem(treeItem, parent);
	treeItem->selectState = TI_CHECKED;
}


STRUCT_TREE_ITEM	* CreateTreeItem__NOTREFERENCED(
							STRUCT_TREE_ITEM	* parent
						)
{
	STRUCT_TREE_ITEM_NOTREFERENCED	* treeItem = new STRUCT_TREE_ITEM_NOTREFERENCED;

	treeItem->type = TREE_ITEM_NOTREFERENCED;

	InitTreeItemSelectable(treeItem, parent);

	treeItem->nameBuffer = L"not referenced";

	ASSERT(treeItem->parent == nullptr);

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__CONTAINS(
							STRUCT_TREE_ITEM	* parent
						)
{
	STRUCT_TREE_ITEM_CONTAINS	* treeItem = new STRUCT_TREE_ITEM_CONTAINS;

	treeItem->type = TREE_ITEM_CONTAINS;

	InitTreeItemSelectable(treeItem, parent);

	treeItem->nameBuffer = L"contains";

	ASSERT(treeItem->parent  &&  treeItem->parent->type == TREE_ITEM_IFCINSTANCE);

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__GROUPS(
							STRUCT_TREE_ITEM	* parent
						)
{
	STRUCT_TREE_ITEM_GROUPS	* treeItem = new STRUCT_TREE_ITEM_GROUPS;

	treeItem->type = TREE_ITEM_GROUPS;

	InitTreeItemSelectable(treeItem, parent);

	treeItem->nameBuffer = L"groups";

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__GROUPEDBY(
							STRUCT_TREE_ITEM	* parent
						)
{
	STRUCT_TREE_ITEM_GROUPEDBY	* treeItem = new STRUCT_TREE_ITEM_GROUPEDBY;

	treeItem->type = TREE_ITEM_GROUPEDBY;

	InitTreeItemSelectable(treeItem, parent);

	treeItem->nameBuffer = L"grouped by";

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__DECOMPOSEDBY(
							STRUCT_TREE_ITEM	* parent
						)
{
	STRUCT_TREE_ITEM_DECOMPOSEDBY	* treeItem = new STRUCT_TREE_ITEM_DECOMPOSEDBY;

	treeItem->type = TREE_ITEM_DECOMPOSEDBY;

	InitTreeItemSelectable(treeItem, parent);

	treeItem->nameBuffer = L"decomposition";

	ASSERT(treeItem->parent  &&  treeItem->parent->type == TREE_ITEM_IFCINSTANCE);

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__DECOMPOSEDBY__NESTED(
							STRUCT_TREE_ITEM	* parent
						)
{
	STRUCT_TREE_ITEM_DECOMPOSEDBY	* treeItem = new STRUCT_TREE_ITEM_DECOMPOSEDBY;

	treeItem->type = TREE_ITEM_DECOMPOSEDBY;

	InitTreeItemSelectable(treeItem, parent);

	treeItem->nameBuffer = L"decomposition (is nested)";

	ASSERT(treeItem->parent  &&  treeItem->parent->type == TREE_ITEM_IFCINSTANCE);

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__SPACEBOUNDARIES(
							STRUCT_TREE_ITEM		* parent
						)
{
	STRUCT_TREE_ITEM_SPACEBOUNDARIES	* treeItem = new STRUCT_TREE_ITEM_SPACEBOUNDARIES;

	treeItem->type = TREE_ITEM_SPACEBOUNDARIES;

	InitTreeItemSelectable(treeItem, parent);

	treeItem->nameBuffer = L"Space Boundaries";

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__GEOMETRY(
							STRUCT_TREE_ITEM	* parent
						)
{
	STRUCT_TREE_ITEM_GEOMETRY	* treeItem = new STRUCT_TREE_ITEM_GEOMETRY;

	treeItem->type = TREE_ITEM_GEOMETRY;

	InitTreeItemSelectable(treeItem, parent);

	treeItem->nameBuffer = L"geometry";

	ASSERT(treeItem->parent  &&  treeItem->parent->type == TREE_ITEM_IFCINSTANCE);

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__PROPERTIES(
							STRUCT_TREE_ITEM	* parent
						)
{
	STRUCT_TREE_ITEM_PROPERTIES	* treeItem = new STRUCT_TREE_ITEM_PROPERTIES;

	treeItem->type = TREE_ITEM_PROPERTIES;

	InitTreeItem(treeItem, parent);

	treeItem->nameBuffer = L"properties";

	ASSERT(treeItem->parent  &&  treeItem->parent->type == TREE_ITEM_IFCINSTANCE);

	return	treeItem;
}


STRUCT_TREE_ITEM	* CreateTreeItem__ALLOCATEDTEXT(
							STRUCT_TREE_ITEM	* parent,
							wchar_t				* text
						)
{
	STRUCT_TREE_ITEM_TEXT	* treeItem = new STRUCT_TREE_ITEM_TEXT;

	treeItem->type = TREE_ITEM_TEXT;

	InitTreeItem(treeItem, parent);

	treeItem->nameBuffer = text;
	treeItem->allocated = true;

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__TEXT(
							STRUCT_TREE_ITEM	* parent,
							wchar_t				* text
						)
{
	STRUCT_TREE_ITEM_TEXT	* treeItem = new STRUCT_TREE_ITEM_TEXT;

	treeItem->type = TREE_ITEM_TEXT;

	InitTreeItem(treeItem, parent);

	treeItem->nameBuffer = text;
	treeItem->allocated = false;

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__PROPERTYSET(
							STRUCT_TREE_ITEM	* parent,
							wchar_t				* text,
							int_t				ifcInstance
						)
{
	STRUCT_TREE_ITEM_PROPERTYSET	* treeItem = new STRUCT_TREE_ITEM_PROPERTYSET;

	treeItem->type = TREE_ITEM_PROPERTYSET;

	InitTreeItem(treeItem, parent);

	treeItem->nameBuffer = text;
	treeItem->ifcInstance = ifcInstance;

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__PROPERTY(
							STRUCT_TREE_ITEM	* parent,
							wchar_t				* text,
							int_t				ifcInstance
						)
{
	STRUCT_TREE_ITEM_PROPERTY	* treeItem = new STRUCT_TREE_ITEM_PROPERTY;

	treeItem->type = TREE_ITEM_PROPERTY;

	InitTreeItem(treeItem, parent);

	treeItem->nameBuffer = text;
	treeItem->ifcInstance = ifcInstance;

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__IFCINSTANCE_model(
							STRUCT_TREE_ITEM	* parent,
							int_t				ifcModel,
							int_t				ifcInstance
						)
{
	STRUCT_TREE_ITEM_IFCINSTANCE	* treeItem = new STRUCT_TREE_ITEM_IFCINSTANCE;

	treeItem->type = TREE_ITEM_IFCINSTANCE;

	InitTreeItemSelectable(treeItem, parent);

	//
	//	Init Tree Item IfcInstance
	//
	treeItem->ifcModel = ifcModel;
	treeItem->ifcInstance = ifcInstance;

	treeItem->ifcObject = allIfcObjects[ifcInstance];
	if (treeItem->ifcObject) {
		treeItem->ifcObject->treeItemModel = treeItem;
	}

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__IFCINSTANCE_spaceBoundary(
							STRUCT_TREE_ITEM	* parent,
							int_t				ifcModel,
							int_t				ifcInstance
						)
{
	STRUCT_TREE_ITEM_IFCINSTANCE	* treeItem = new STRUCT_TREE_ITEM_IFCINSTANCE;

	treeItem->type = TREE_ITEM_IFCINSTANCE;

	InitTreeItemSelectable(treeItem, parent);

	//
	//	Init Tree Item IfcInstance
	//
	treeItem->ifcModel = ifcModel;
	treeItem->ifcInstance = ifcInstance;

	treeItem->ifcObject = allIfcObjects[ifcInstance];
	if (treeItem->ifcObject) {
		treeItem->ifcObject->treeItemSpaceBoundary = treeItem;
	}

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__IFCINSTANCE_nonReferenced(
							STRUCT_TREE_ITEM	* parent,
							int_t				ifcModel,
							int_t				ifcInstance
						)
{
	STRUCT_TREE_ITEM_IFCINSTANCE	* treeItem = new STRUCT_TREE_ITEM_IFCINSTANCE;

	treeItem->type = TREE_ITEM_IFCINSTANCE;

	InitTreeItemSelectable(treeItem, parent);

	//
	//	Init Tree Item IfcInstance
	//
	treeItem->ifcModel = ifcModel;
	treeItem->ifcInstance = ifcInstance;

	treeItem->ifcObject = allIfcObjects[ifcInstance];
	if (treeItem->ifcObject) {
		ASSERT(treeItem->ifcObject->treeItemNonReferenced == nullptr);
		treeItem->ifcObject->treeItemNonReferenced = treeItem;
	}

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__IFCINSTANCE_group(
							STRUCT_TREE_ITEM	* parent,
							int_t				ifcModel,
							int_t				ifcInstance
						)
{
	STRUCT_TREE_ITEM_IFCINSTANCE	* treeItem = new STRUCT_TREE_ITEM_IFCINSTANCE;

	treeItem->type = TREE_ITEM_IFCINSTANCE;

	InitTreeItemSelectable(treeItem, parent);

	//
	//	Init Tree Item IfcInstance
	//
	treeItem->ifcModel = ifcModel;
	treeItem->ifcInstance = ifcInstance;

	treeItem->ifcObject = allIfcObjects[ifcInstance];
	if (treeItem->ifcObject) {
//		treeItem->ifcObject->treeItem... = treeItem;
	}

	return	treeItem;
}

STRUCT_TREE_ITEM	* CreateTreeItem__IFCENTITY(
							STRUCT_TREE_ITEM	* parent,
							int_t				ifcModel,
							int_t				ifcEntity
						)
{
	STRUCT_TREE_ITEM_IFCENTITY	* treeItem = new STRUCT_TREE_ITEM_IFCENTITY;

	treeItem->type = TREE_ITEM_IFCENTITY;

	InitTreeItemSelectable(treeItem, parent);

	//
	//	Init Tree Item IfcEntity
	//
	treeItem->ifcModel = ifcModel;
	treeItem->ifcEntity = ifcEntity;

	return	treeItem;
}

wchar_t	* GetTreeItemName(
				STRUCT_TREE_ITEM	* treeItem
			)
{
	switch (treeItem->type) {
		case TREE_ITEM_CONTAINS:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_NOTREFERENCED:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_GROUPEDBY:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_DECOMPOSEDBY:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_SPACEBOUNDARIES:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_IFCINSTANCE:
			treeItem->nameBuffer = GetTreeItemName_ifcInstance(
//										((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItem)->ifcModel,
										((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItem)->ifcInstance
									);
			break;
		case TREE_ITEM_IFCENTITY:
			treeItem->nameBuffer = GetTreeItemName_ifcEntity(
//										((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItem)->ifcModel,
										((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItem)->ifcInstance
									);
			break;
		case TREE_ITEM_GEOMETRY:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_GROUPS:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_PROPERTIES:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_PROPERTYSET:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_PROPERTY:
			ASSERT(treeItem->nameBuffer);
			break;
		case TREE_ITEM_TEXT:
			ASSERT(treeItem->nameBuffer);
			break;
		default:
			ASSERT(false);
			break;
	}
	
	return	treeItem->nameBuffer;
}

void	CreateTreeItems(
				STRUCT_TREE_ITEM		* parent,
				STRUCT__PROPERTY__SET	* propertySets
			)
{
	STRUCT_TREE_ITEM	** pChildPropertySet = &parent->child;
	while (propertySets) {
		(*pChildPropertySet) = CreateTreeItem__PROPERTYSET(parent, getTreeItemName_propertySet(propertySets), propertySets->ifcInstance);

		STRUCT__PROPERTY	* properties = propertySets->properties;
		STRUCT_TREE_ITEM	** pChildProperty = &(*pChildPropertySet)->child;
		while (properties) {
			(*pChildProperty) = CreateTreeItem__PROPERTY((*pChildPropertySet), getTreeItemName_property(properties), properties->ifcInstance);

			pChildProperty = &(*pChildProperty)->next;
			properties = properties->next;
		}
		
		pChildPropertySet = &(*pChildPropertySet)->next;
		propertySets = propertySets->next;
	}
}















void	InitializeTreeItems(
				STRUCT_TREE_ITEM_SELECTABLE	* treeItemSelectable,
				bool						* pChecked,
				bool						* pUnchecked
			)
{
	ASSERT((*pChecked) == false && (*pUnchecked) == false);
	while (treeItemSelectable) {
		switch (treeItemSelectable->type) {
			case TREE_ITEM_CONTAINS:
			case TREE_ITEM_NOTREFERENCED:
			case TREE_ITEM_DECOMPOSEDBY:
			case TREE_ITEM_GROUPEDBY:
			case TREE_ITEM_SPACEBOUNDARIES:
			case TREE_ITEM_IFCINSTANCE:
			case TREE_ITEM_IFCENTITY:
				{
					bool	checked = false, unchecked = false;
					if (treeItemSelectable->child) {
						InitializeTreeItems(
								(STRUCT_TREE_ITEM_SELECTABLE*) treeItemSelectable->child,
								&checked,
								&unchecked
							);
					} else {
						ASSERT(treeItemSelectable->type == TREE_ITEM_IFCINSTANCE);
					}
				
#if PRODUCE_FLAT_TREE //there is no geoemetry child item in this case
					if (treeItemSelectable->type == TREE_ITEM_IFCINSTANCE) {
						auto pInstance = (STRUCT_TREE_ITEM_IFCINSTANCE*) treeItemSelectable;
						if (pInstance->ifcInstance && sdaiIsKindOfBN(pInstance->ifcInstance, (char*)L"IfcProduct")) {
							switch (treeItemSelectable->selectState) {
								case TI_CHECKED:
									checked = true;
									break;
								case TI_UNCHECKED:
									unchecked = true;
									break;
								case TI_NONE:
									break;
								default:
									ASSERT(false);
									break;
							}

						}
					}
#endif

					if (checked) {
						(*pChecked) = true;
						if (unchecked) {
							(*pUnchecked) = true;
							treeItemSelectable->selectState = TI_PARTLY_CHECKED;
						} else {
							treeItemSelectable->selectState = TI_CHECKED;
						}
					} else {
						if (unchecked) {
							(*pUnchecked) = true;
							treeItemSelectable->selectState = TI_UNCHECKED;
						} else {
							treeItemSelectable->selectState = TI_NONE;
						}
					}
				}
				break;
			case TREE_ITEM_GEOMETRY:
				{
					ASSERT((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent);

					if (((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject) {
						treeItemSelectable->selectState = ((STRUCT_TREE_ITEM_IFCINSTANCE*)treeItemSelectable->parent)->ifcObject->selectState;
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
						treeItemSelectable->selectState = TI_NONE;
					}
				}
				break;
			case TREE_ITEM_GROUPS:
				{
					//
					//	Initialization could also happen at a later stage
					//
					bool	checked = false, unchecked = false;
					if (treeItemSelectable->child) {
						InitializeTreeItems(
								(STRUCT_TREE_ITEM_SELECTABLE*)treeItemSelectable->child,
								&checked,
								&unchecked
							);
					} else {
						ASSERT(false);
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