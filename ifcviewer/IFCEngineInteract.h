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

extern const char* GetEntityName(ValidationIssue* issue);
extern const char* GetAttrName(ValidationIssue* issue);
extern int_t GetAttrIndex(ValidationIssue* issue);
extern int64_t GetStepId(ValidationIssue* issue);
extern const char* GetIssueId(ValidationIssue* issue);

extern void CollectReferencingInstancesRecursive(std::set<SdaiInstance>& referencingInstances, SdaiInstance referencedInstance, SdaiEntity* searchEntities /*NULL-terminated array*/);
