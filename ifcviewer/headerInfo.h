#pragma once



#include	"treeitems.h"
//#include	"ifcengine\include\engdef.h"
//#include	"ifcengine\include\ifcengine.h"

/*
typedef struct STRUCT__HEADER__SET {
	int_t							structType;

	wchar_t							* name;
	wchar_t							* value;

	HTREEITEM						hTreeItem;

	wchar_t							* nameBuffer;

	STRUCT__HEADER__SET				* child;
	STRUCT__HEADER__SET				* next;
}	STRUCT__HEADER__SET;
*/


STRUCT_TREE_ITEM	* GetHeaderInfo(
							int_t				ifcModel
						);

void				DeleteHeaderInfo(
							STRUCT_TREE_ITEM	* headerInfo
						);
