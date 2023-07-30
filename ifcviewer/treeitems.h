#pragma once



#include "generic.h"
#include "unit.h"



struct STRUCT__IFC__OBJECT;

enum ENUM_TREE_ITEM_TYPES {
	TREE_ITEM_CONTAINS = 1,
	TREE_ITEM_NOTREFERENCED,
	TREE_ITEM_DECOMPOSEDBY,
	TREE_ITEM_GROUPEDBY,
	TREE_ITEM_GROUPS,
	TREE_ITEM_SPACEBOUNDARIES,
	TREE_ITEM_IFCINSTANCE,
	TREE_ITEM_IFCENTITY,
	TREE_ITEM_GEOMETRY,
	TREE_ITEM_PROPERTIES,
	TREE_ITEM_PROPERTY,
	TREE_ITEM_PROPERTYSET,
	TREE_ITEM_TEXT
};

enum ENUM_TREE_ITEM_SELECT_STATE {
	TI_CHECKED = 1,
	TI_UNCHECKED = 3,
	TI_PARTLY_CHECKED = 2,
	TI_NONE = 5
};

struct STRUCT_TREE_ITEM {
	ENUM_TREE_ITEM_TYPES			type;

	HTREEITEM						hTreeItem;
	const wchar_t					* nameBuffer;

	STRUCT_TREE_ITEM				* parent;
	STRUCT_TREE_ITEM				* child;
	STRUCT_TREE_ITEM				* next;
};

struct STRUCT_TREE_ITEM_SELECTABLE : STRUCT_TREE_ITEM {
	ENUM_TREE_ITEM_SELECT_STATE		selectState;
};

struct STRUCT_TREE_ITEM_IFCINSTANCE : STRUCT_TREE_ITEM_SELECTABLE {
	int_t							ifcModel;
	int_t							ifcInstance;

	STRUCT__IFC__OBJECT				* ifcObject;
};

struct STRUCT_TREE_ITEM_IFCENTITY : STRUCT_TREE_ITEM_SELECTABLE {
	int_t							ifcModel;
	int_t							ifcEntity;
};

struct STRUCT_TREE_ITEM_GEOMETRY : STRUCT_TREE_ITEM_SELECTABLE {
};

struct STRUCT_TREE_ITEM_NOTREFERENCED : STRUCT_TREE_ITEM_SELECTABLE {
};

struct STRUCT_TREE_ITEM_PROPERTIES : STRUCT_TREE_ITEM {
};

struct STRUCT_TREE_ITEM_PROPERTYSET : STRUCT_TREE_ITEM {
	int_t							ifcInstance;
};

struct STRUCT_TREE_ITEM_PROPERTY : STRUCT_TREE_ITEM {
	int_t							ifcInstance;
};

struct STRUCT_TREE_ITEM_TEXT : STRUCT_TREE_ITEM {
	bool							allocated;
};

struct STRUCT_TREE_ITEM_GROUPEDBY : STRUCT_TREE_ITEM_SELECTABLE {
};

struct STRUCT_TREE_ITEM_GROUPS : STRUCT_TREE_ITEM_SELECTABLE {
};

struct STRUCT_TREE_ITEM_DECOMPOSEDBY : STRUCT_TREE_ITEM_SELECTABLE {
};

struct STRUCT_TREE_ITEM_CONTAINS : STRUCT_TREE_ITEM_SELECTABLE {
};

struct STRUCT_TREE_ITEM_SPACEBOUNDARIES : STRUCT_TREE_ITEM_SELECTABLE {
};



STRUCT_TREE_ITEM	* CreateTreeItem__GEOMETRY(
							STRUCT_TREE_ITEM			* parent
						);

STRUCT_TREE_ITEM	* CreateTreeItem__PROPERTIES(
							STRUCT_TREE_ITEM			* parent
						);

STRUCT_TREE_ITEM	* CreateTreeItem__CONTAINS(
							STRUCT_TREE_ITEM			* parent
						);

STRUCT_TREE_ITEM	* CreateTreeItem__GROUPEDBY(
							STRUCT_TREE_ITEM			* parent
						);

STRUCT_TREE_ITEM	* CreateTreeItem__GROUPS(
							STRUCT_TREE_ITEM			* parent
						);

STRUCT_TREE_ITEM	* CreateTreeItem__DECOMPOSEDBY(
							STRUCT_TREE_ITEM			* parent,
							const wchar_t				* decompositionType
						);


STRUCT_TREE_ITEM	* CreateTreeItem__SPACEBOUNDARIES(
							STRUCT_TREE_ITEM			* parent
						);

STRUCT_TREE_ITEM	* CreateTreeItem__NOTREFERENCED(
							STRUCT_TREE_ITEM			* parent
						);

STRUCT_TREE_ITEM	* CreateTreeItem__IFCINSTANCE_model(
							STRUCT_TREE_ITEM			* parent,
							int_t						ifcModel,
							int_t						ifcInstance
						);

STRUCT_TREE_ITEM	* CreateTreeItem__IFCINSTANCE_spaceBoundary(
							STRUCT_TREE_ITEM			* parent,
							int_t						ifcModel,
							int_t						ifcInstance
						);

STRUCT_TREE_ITEM	* CreateTreeItem__IFCINSTANCE_nonReferenced(
							STRUCT_TREE_ITEM			* parent,
							int_t						ifcModel,
							int_t						ifcInstance
						);

STRUCT_TREE_ITEM	* CreateTreeItem__IFCINSTANCE_group(
							STRUCT_TREE_ITEM			* parent,
							int_t						ifcModel,
							int_t						ifcInstance
						);

STRUCT_TREE_ITEM	* CreateTreeItem__IFCENTITY(
							STRUCT_TREE_ITEM			* parent,
							int_t						ifcModel,
							int_t						ifcEntity
						);

STRUCT_TREE_ITEM	* CreateTreeItem__TEXT(
							STRUCT_TREE_ITEM			* parent,
							wchar_t						* text
						);

STRUCT_TREE_ITEM	* CreateTreeItem__ALLOCATEDTEXT(
							STRUCT_TREE_ITEM			* parent,
							wchar_t						* text
						);

STRUCT_TREE_ITEM	* CreateTreeItem__PROPERTYSET(
							STRUCT_TREE_ITEM			* parent,
							wchar_t						* text,
							int_t						ifcInstance
						);

const wchar_t		* GetTreeItemName(
							STRUCT_TREE_ITEM			* treeItem
						);

void				CreateTreeItems(
							STRUCT_TREE_ITEM			* parent,
							STRUCT__PROPERTY__SET		* propertySets
						);

void				InitializeTreeItems(
							STRUCT_TREE_ITEM_SELECTABLE	* treeItemSelectable,
							bool						* pChecked,
							bool						* pUnchecked
						);
