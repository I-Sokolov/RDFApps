#pragma once



#include "treeitems.h"



STRUCT_TREE_ITEM	* CreateTreeItem_ifcObject(
							int_t				ifcModel,
							int_t				ifcObjectInstance,
							STRUCT_TREE_ITEM	* parent,
							int_t				depth
						);

void				CreateTreeItem_ifcProject(
							int_t				ifcModel
						);

void				CreateTreeItem_ifcGroup(
							int_t				ifcModel
						);

STRUCT_TREE_ITEM	** CreateTreeItem_ifcSpaceBoundary(
							int_t				ifcModel,
							int_t				ifcObjectInstance,
							STRUCT_TREE_ITEM	* parent,
							STRUCT_TREE_ITEM	** pChild
						);

void				CreateTreeItem_ifcSpaceBoundary(
							int_t				ifcModel
						);

void				CreateTreeItem_nonReferencedIfcItems(
							int_t				ifcModel
						);
