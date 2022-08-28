#pragma once


#include "ifcengine\include\ifcengine.h"
#include "ifcengine\include\engine.h"
#include "GenericTreeItem.h"

extern	int_t	globalIfcModel;

bool				ParseIfcFile(
							CWnd				* pParent
						);

void				CleanupIfcFile(
						);

STRUCT_TREE_ITEM	* CreateTreeItem_ifcObject(
							int_t				ifcModel,
							int_t				ifcObjectInstance,
							STRUCT_TREE_ITEM	* parent,
							int_t				depth
						);
