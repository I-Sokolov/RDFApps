#include "stdafx.h"
#include "containsdecomposedby.h"

extern	STRUCT_TREE_ITEM	* topTreeItem, * topModelTreeItem, * topSpaceBoundaryTreeItem, * topNonReferencedTreeItem, * topGroupTreeItem;
extern	STRUCT__IFC__OBJECT	* ifcObjectsLinkedList;

STRUCT__SIUNIT	* unitsGlobal = nullptr;

extern	int_t			ifcSpace_TYPE;


static void AddChild(STRUCT_TREE_ITEM* parent, STRUCT_TREE_ITEM* child)
{
	if (parent == child) {
		return;
	}

	if (!parent->child) {
		parent->child = child;
	}
	else {
		auto last = parent->child;
		while (last->next) {
			last = last->next;
		}
		last->next = child;
	}
}

static void AddDecompositionChild(
	SdaiModel			ifcModel,
	STRUCT_TREE_ITEM*	parent,
	const wchar_t*		decompositionTypeName,
	int_t				depth,
	STRUCT_TREE_ITEM*&	decomposedByTreeItem,
	STRUCT_TREE_ITEM**& ppChild,
	SdaiInstance  		instChild
	)
{
	if (instChild) {
		if (decomposedByTreeItem == nullptr) {
			decomposedByTreeItem = CreateTreeItem__DECOMPOSEDBY(parent, decompositionTypeName);
			ppChild = &decomposedByTreeItem->child;
		}

		(*ppChild) = CreateTreeItem_ifcObject(ifcModel, instChild, decomposedByTreeItem, depth + 1);
		ppChild = &(*ppChild)->next;
	}
}

static void PopulateTreeItems_ifcObjectDecomposedBy(
	SdaiModel			ifcModel,
	SdaiInstance		ifcObjectInstance,
	STRUCT_TREE_ITEM*	parent,
	int_t				depth,
	const wchar_t*		decompositionTypeName,
	const wchar_t*		decompositionInverseAttribute,
	const wchar_t*		relationTargetAttribute
)
{
	STRUCT_TREE_ITEM* decomposedByTreeItem = nullptr, ** ppChild = nullptr;

#if PRODUCE_FLAT_TREE
	decomposedByTreeItem = parent;
	ppChild = &decomposedByTreeItem->child;
	while (*ppChild)
		ppChild = &((*ppChild)->next);
#endif

	int_t* ifcRelDecomposesInstances = nullptr, ifcRelDecomposesInstancesCnt = 0;
	sdaiGetAttrBN(ifcObjectInstance, (char*)decompositionInverseAttribute, sdaiAGGR, &ifcRelDecomposesInstances);

	if (ifcRelDecomposesInstances) {
		ifcRelDecomposesInstancesCnt = sdaiGetMemberCount(ifcRelDecomposesInstances);
		for (int_t j = 0; j < ifcRelDecomposesInstancesCnt; ++j) {

			int_t ifcRelDecomposesInstance = 0;
			sdaiGetAggrByIndex(ifcRelDecomposesInstances, j, sdaiINSTANCE, &ifcRelDecomposesInstance);

			SdaiAggr childInstances = nullptr;
			SdaiInstance childInst = 0;
			if (sdaiGetAttrBN(ifcRelDecomposesInstance, (char*)relationTargetAttribute, sdaiAGGR, &childInstances)) {
				int_t	childCnt = sdaiGetMemberCount(childInstances);
				for (int_t k = 0; k < childCnt; ++k) {
					sdaiGetAggrByIndex(childInstances, k, sdaiINSTANCE, &childInst);
					AddDecompositionChild(ifcModel, parent, decompositionTypeName, depth, decomposedByTreeItem, ppChild, childInst);
				}
			}
			else if (sdaiGetAttrBN(ifcRelDecomposesInstance, (char*)relationTargetAttribute, sdaiINSTANCE, &childInst)) {
				AddDecompositionChild(ifcModel, parent, decompositionTypeName, depth, decomposedByTreeItem, ppChild, childInst);
			}
			else ASSERT(0);
		}
	}

	if (decomposedByTreeItem) {
		AddChild(parent, decomposedByTreeItem);
	}
}

static void PopulateTreeItems_ifcObjectDecomposedBy(
							int_t				ifcModel,
							int_t				ifcObjectInstance,
							STRUCT_TREE_ITEM	* parent,
							int_t				depth
						)
{
	PopulateTreeItems_ifcObjectDecomposedBy(ifcModel, ifcObjectInstance, parent, depth, L"Aggregates", L"IsDecomposedBy", L"RelatedObjects");

	PopulateTreeItems_ifcObjectDecomposedBy(ifcModel, ifcObjectInstance, parent, depth, L"Nests", L"IsNestedBy", L"RelatedObjects");

	PopulateTreeItems_ifcObjectDecomposedBy(ifcModel, ifcObjectInstance, parent, depth, L"Openings", L"HasOpenings", L"RelatedOpeningElement");

	PopulateTreeItems_ifcObjectDecomposedBy(ifcModel, ifcObjectInstance, parent, depth, L"Projections", L"HasProjections", L"RelatedFeatureElement");
}

static void PopulateTreeItems_ifcObjectContains(
							int_t				ifcModel,
							int_t				ifcObjectInstance,
							STRUCT_TREE_ITEM	* parent,
							int_t				depth
						)
{
	STRUCT_TREE_ITEM	* containsTreeItem = nullptr, ** ppChild = nullptr;

#if PRODUCE_FLAT_TREE
	containsTreeItem = parent;
	ppChild = &containsTreeItem->child;
	while (*ppChild)
		ppChild = &((*ppChild)->next);
#endif

	int_t	* ifcRelContainedInSpatialStructureInstances = nullptr;
	sdaiGetAttrBN(ifcObjectInstance, (char*) L"ContainsElements", sdaiAGGR, &ifcRelContainedInSpatialStructureInstances);
	int_t	ifcRelContainedInSpatialStructureInstancesCnt = sdaiGetMemberCount(ifcRelContainedInSpatialStructureInstances);

	for (int_t i = 0; i < ifcRelContainedInSpatialStructureInstancesCnt; ++i) {
		int_t	ifcRelContainedInSpatialStructureInstance = 0;
		sdaiGetAggrByIndex(ifcRelContainedInSpatialStructureInstances, i, sdaiINSTANCE, &ifcRelContainedInSpatialStructureInstance);
		
			int_t	* ifcObjectInstances = nullptr;
			sdaiGetAttrBN(ifcRelContainedInSpatialStructureInstance, (char*)L"RelatedElements", sdaiAGGR, &ifcObjectInstances);

			int_t	ifcObjectInstancesCnt = sdaiGetMemberCount(ifcObjectInstances);
			if (ifcObjectInstancesCnt) {
				if (containsTreeItem == nullptr) {
					containsTreeItem = CreateTreeItem__CONTAINS(parent);
					ppChild = &containsTreeItem->child;
				}

				for (int_t k = 0; k < ifcObjectInstancesCnt; ++k) {
					ifcObjectInstance = 0;
					sdaiGetAggrByIndex(ifcObjectInstances, k, sdaiINSTANCE, &ifcObjectInstance);

					(*ppChild) = CreateTreeItem_ifcObject(ifcModel, ifcObjectInstance, containsTreeItem, depth + 1);
					ppChild = &(*ppChild)->next;
				}
			}
	}

	if (containsTreeItem) {
		AddChild(parent, containsTreeItem);
	}
}



STRUCT_TREE_ITEM	* CreateTreeItem_ifcObject(
							int_t				ifcModel,
							int_t				ifcObjectInstance,
							STRUCT_TREE_ITEM	* parent,
							int_t				depth
						)
{
	if (depth == 30) {
		return	nullptr;
	}

	STRUCT_TREE_ITEM	* treeItem = CreateTreeItem__IFCINSTANCE_model(parent, ifcModel, ifcObjectInstance);

	AddChild(treeItem, CreateTreeItem__GEOMETRY(treeItem));
#if !PRODUCE_FLAT_TREE
	AddChild (treeItem, CreateTreeItem__PROPERTIES(treeItem));
#endif

	PopulateTreeItems_ifcObjectDecomposedBy(ifcModel, ifcObjectInstance, treeItem, depth);

	PopulateTreeItems_ifcObjectContains(ifcModel, ifcObjectInstance, treeItem, depth);
	
	return	treeItem;
}

void	CreateTreeItem_ifcProject(
				int_t				ifcModel
			)
{
	int_t	* ifcProjectInstances = sdaiGetEntityExtentBN(ifcModel, (char*) L"IFCPROJECT"),
			noIfcProjectInstances = sdaiGetMemberCount(ifcProjectInstances);
	for (int_t i = 0; i < noIfcProjectInstances; ++i) {
		int_t	ifcProjectInstance = 0;
		sdaiGetAggrByIndex(ifcProjectInstances, i, sdaiINSTANCE, &ifcProjectInstance);

		unitsGlobal = GetUnits(ifcModel, ifcProjectInstance);

		STRUCT_TREE_ITEM	* treeItemIfcProject = CreateTreeItem_ifcObject(
															ifcModel,
															ifcProjectInstance,
															nullptr,
															0
														);
		ASSERT(treeItemIfcProject->next == nullptr);

		topModelTreeItem = treeItemIfcProject;

		treeItemIfcProject->next = topTreeItem;
		topTreeItem = treeItemIfcProject;
	}
}


























STRUCT_TREE_ITEM	* CreateTreeItem_ifcGroup(
							int_t				ifcModel,
							int_t				ifcGroupInstance,
							STRUCT_TREE_ITEM	* parent
						)
{
	STRUCT_TREE_ITEM	* treeItem = CreateTreeItem__IFCINSTANCE_group(parent, ifcModel, ifcGroupInstance);

	int_t	ifcRelAssignsToGroupInstance = 0;
	sdaiGetAttrBN(ifcGroupInstance, (char*)L"IsGroupedBy", sdaiINSTANCE, &ifcRelAssignsToGroupInstance);
	if (ifcRelAssignsToGroupInstance) {
		int_t	* ifcObjectDefinitionInstanceSet = nullptr;
		sdaiGetAttrBN(ifcRelAssignsToGroupInstance, (char*)L"RelatedObjects", sdaiAGGR, &ifcObjectDefinitionInstanceSet);

		treeItem->child = CreateTreeItem__GROUPEDBY(treeItem);
		STRUCT_TREE_ITEM	** pTreeItemGroups = &treeItem->child->child;

		int_t	ifcObjectDefinitionInstanceSetCnt = sdaiGetMemberCount(ifcObjectDefinitionInstanceSet);

		if (ifcObjectDefinitionInstanceSetCnt) {
			int_t	* ifcInstanceArray = new int_t[ifcObjectDefinitionInstanceSetCnt],
					* ifcInstanceEntityArray = new int_t[ifcObjectDefinitionInstanceSetCnt];
			for (int_t i = 0; i < ifcObjectDefinitionInstanceSetCnt; i++) {
				int_t	ifcObjectDefinitionInstance = 0;
				sdaiGetAggrByIndex(ifcObjectDefinitionInstanceSet, i, sdaiINSTANCE, &ifcObjectDefinitionInstance);

				ifcInstanceArray[i] = ifcObjectDefinitionInstance;
				ifcInstanceEntityArray[i] = sdaiGetInstanceType(ifcObjectDefinitionInstance);
				ASSERT(ifcInstanceEntityArray[i]);
			}

			for (int_t i = 0; i < ifcObjectDefinitionInstanceSetCnt; i++) {
				if (ifcInstanceEntityArray[i]) {
					int_t j = i, ifcInstanceEntity = ifcInstanceEntityArray[i];

					(*pTreeItemGroups) = CreateTreeItem__IFCENTITY(treeItem->child, ifcModel, ifcInstanceEntity);
					STRUCT_TREE_ITEM	** pTreeItemInstances = &(*pTreeItemGroups)->child;

					while (j < ifcObjectDefinitionInstanceSetCnt) {
						if (ifcInstanceEntityArray[j] == ifcInstanceEntity) {
							(*pTreeItemInstances) = CreateTreeItem__IFCINSTANCE_group((*pTreeItemGroups), ifcModel, ifcInstanceArray[j]);
							(*pTreeItemInstances)->child = CreateTreeItem__GEOMETRY((*pTreeItemInstances));
							(*pTreeItemInstances)->child->next = CreateTreeItem__PROPERTIES((*pTreeItemInstances));

							pTreeItemInstances = &(*pTreeItemInstances)->next;

							ifcInstanceEntityArray[j] = 0;
						}
						j++;
					}

					pTreeItemGroups = &(*pTreeItemGroups)->next;
				}
				int_t	ifcObjectDefinitionInstance = 0;
				sdaiGetAggrByIndex(ifcObjectDefinitionInstanceSet, i, sdaiINSTANCE, &ifcObjectDefinitionInstance);

				ifcInstanceArray[i] = ifcObjectDefinitionInstance;
				ifcInstanceEntityArray[i] = sdaiGetInstanceType(ifcObjectDefinitionInstance);
				ASSERT(ifcInstanceEntityArray[i]);
			}

			delete[] ifcInstanceArray;
			delete[] ifcInstanceEntityArray;
		}
	} else {
		ASSERT(false);
	}

	return	treeItem;
}

STRUCT_TREE_ITEM	** CreateTreeItem_ifcGroupRecursively(
							int_t				ifcModel,
							int_t				ifcParentEntity,
							STRUCT_TREE_ITEM	** pTreeItem
						)
{
	int_t	* ifcEntityExtend = sdaiGetEntityExtent(ifcModel, ifcParentEntity),
			cnt = sdaiGetMemberCount(ifcEntityExtend);

	for (int_t i = 0; i < cnt; ++i) {
		int_t	ifcParentEntityInstance = 0;
		sdaiGetAggrByIndex(ifcEntityExtend, i, sdaiINSTANCE, &ifcParentEntityInstance);

		(*pTreeItem) = CreateTreeItem_ifcGroup(
								ifcModel,
								ifcParentEntityInstance,
								nullptr
							);
		ASSERT((*pTreeItem)->next == nullptr);
		pTreeItem = &(*pTreeItem)->next;
	}

	cnt = engiGetEntityCount(ifcModel);
	for (int_t i = 0; i < cnt; i++) {
		int_t ifcEntity = engiGetEntityElement(ifcModel, i);
		if (engiGetEntityParent(ifcEntity) == ifcParentEntity) {
			pTreeItem = CreateTreeItem_ifcGroupRecursively(ifcModel, ifcEntity, pTreeItem);
		}
	}

	return	pTreeItem;
}

void	CreateTreeItem_ifcGroup(
				int_t				ifcModel
			)
{
	STRUCT_TREE_ITEM	* treeItem = nullptr,
						** pTreeItem = &treeItem;
	CreateTreeItem_ifcGroupRecursively(
			ifcModel,
			sdaiGetEntity(ifcModel, (char*)L"IFCGROUP"),
			pTreeItem
		);

	if (treeItem) {
		STRUCT_TREE_ITEM	* tTreeItem = CreateTreeItem__GROUPS(nullptr);
		topGroupTreeItem = tTreeItem;

		tTreeItem->child = treeItem;
		while (treeItem) {
			ASSERT(treeItem->parent == nullptr);
			treeItem->parent = tTreeItem;

			treeItem = treeItem->next;
		}

		tTreeItem->next = topTreeItem;
		topTreeItem = tTreeItem;
	}
}

STRUCT_TREE_ITEM	* CreateTreeItem_ifcSpaceBoundary_SPACE(
							int_t				ifcModel,
							int_t				ifcSpaceInstance,
							STRUCT_TREE_ITEM	* parent
						)
{
	int_t	* ifcRelSpaceBoundaryInstanceSet = nullptr;
	sdaiGetAttrBN(ifcSpaceInstance, (char*)L"BoundedBy", sdaiAGGR, &ifcRelSpaceBoundaryInstanceSet);

	int_t	ifcObjectDefinitionInstanceSetCnt = sdaiGetMemberCount(ifcRelSpaceBoundaryInstanceSet);
	if (ifcObjectDefinitionInstanceSetCnt) {
		STRUCT_TREE_ITEM	* treeItem = CreateTreeItem__SPACEBOUNDARIES(parent),
							** pChild = &treeItem->child;

		for (int_t i = 0; i < ifcObjectDefinitionInstanceSetCnt; i++) {
			int_t	ifcRelSpaceBoundaryInstance = 0;
			sdaiGetAggrByIndex(ifcRelSpaceBoundaryInstanceSet, i, sdaiINSTANCE, &ifcRelSpaceBoundaryInstance);

			(*pChild) = CreateTreeItem__IFCINSTANCE_spaceBoundary(treeItem, ifcModel, ifcRelSpaceBoundaryInstance);
			(*pChild)->child = CreateTreeItem__GEOMETRY((*pChild));

			int_t	ifcObjectInstance = 0;
			sdaiGetAttrBN(ifcRelSpaceBoundaryInstance, (char*)L"RelatedBuildingElement", sdaiINSTANCE, &ifcObjectInstance);
			if (ifcObjectInstance) {
				(*pChild)->child->next = CreateTreeItem__SPACEBOUNDARIES((*pChild));
				(*pChild)->child->next->child = CreateTreeItem__IFCINSTANCE_spaceBoundary((*pChild)->child->next, ifcModel, ifcObjectInstance);
				(*pChild)->child->next->child->child = CreateTreeItem__GEOMETRY((*pChild)->child->next->child);
				(*pChild)->child->next->child->child->next = CreateTreeItem__PROPERTIES((*pChild)->child->next->child);
			}

			pChild = &(*pChild)->next;
		}

		return	treeItem;
	}

	return	nullptr;
}

static STRUCT_TREE_ITEM** CreateTreeItem_ifcSpaceBoundaryDecomposedBy(
	int_t				ifcModel,
	int_t				ifcObjectInstance,
	STRUCT_TREE_ITEM* parent,
	STRUCT_TREE_ITEM** pChild
)
{
	int_t* ifcRelDecomposesInstances = nullptr, ifcRelDecomposesInstancesCnt;
	sdaiGetAttrBN(ifcObjectInstance, (char*)L"IsDecomposedBy", sdaiAGGR, &ifcRelDecomposesInstances);

	if (ifcRelDecomposesInstances) {
		ifcRelDecomposesInstancesCnt = sdaiGetMemberCount(ifcRelDecomposesInstances);
		for (int_t j = 0; j < ifcRelDecomposesInstancesCnt; ++j) {
			int_t ifcRelDecomposesInstance = 0;
			sdaiGetAggrByIndex(ifcRelDecomposesInstances, j, sdaiINSTANCE, &ifcRelDecomposesInstance);

			int_t* ifcObjectInstances = nullptr;
			sdaiGetAttrBN(ifcRelDecomposesInstance, (char*)L"RelatedObjects", sdaiAGGR, &ifcObjectInstances);

			int_t	ifcObjectInstancesCnt = sdaiGetMemberCount(ifcObjectInstances);
			if (ifcObjectInstancesCnt) {
				for (int_t k = 0; k < ifcObjectInstancesCnt; ++k) {
					ifcObjectInstance = 0;
					sdaiGetAggrByIndex(ifcObjectInstances, k, sdaiINSTANCE, &ifcObjectInstance);

					pChild = CreateTreeItem_ifcSpaceBoundary(ifcModel, ifcObjectInstance, parent, pChild);
				}
			}
		}
	}

	return	pChild;
}

STRUCT_TREE_ITEM	** CreateTreeItem_ifcSpaceBoundaryContains(
							int_t				ifcModel,
							int_t				ifcObjectInstance,
							STRUCT_TREE_ITEM	* parent,
							STRUCT_TREE_ITEM	** pChild
						)
{
	int_t	* ifcRelContainedInSpatialStructureInstances = nullptr;
	sdaiGetAttrBN(ifcObjectInstance, (char*) L"ContainsElements", sdaiAGGR, &ifcRelContainedInSpatialStructureInstances);
	int_t	ifcRelContainedInSpatialStructureInstancesCnt = sdaiGetMemberCount(ifcRelContainedInSpatialStructureInstances);

	for (int_t i = 0; i < ifcRelContainedInSpatialStructureInstancesCnt; ++i) {
		int_t	ifcRelContainedInSpatialStructureInstance = 0;
		sdaiGetAggrByIndex(ifcRelContainedInSpatialStructureInstances, i, sdaiINSTANCE, &ifcRelContainedInSpatialStructureInstance);

			int_t	* ifcObjectInstances = nullptr;
			sdaiGetAttrBN(ifcRelContainedInSpatialStructureInstance, (char*)L"RelatedElements", sdaiAGGR, &ifcObjectInstances);

			int_t	ifcObjectInstancesCnt = sdaiGetMemberCount(ifcObjectInstances);
			if (ifcObjectInstancesCnt) {
				for (int_t k = 0; k < ifcObjectInstancesCnt; ++k) {
					ifcObjectInstance = 0;
					sdaiGetAggrByIndex(ifcObjectInstances, k, sdaiINSTANCE, &ifcObjectInstance);

					pChild = CreateTreeItem_ifcSpaceBoundary(ifcModel, ifcObjectInstance, parent, pChild);
				}
			}
	}

	return	pChild;
}

STRUCT_TREE_ITEM	** CreateTreeItem_ifcSpaceBoundary(
							int_t				ifcModel,
							int_t				ifcObjectInstance,
							STRUCT_TREE_ITEM	* parent,
							STRUCT_TREE_ITEM	** pChild
						)
{
	if (sdaiGetInstanceType(ifcObjectInstance) == ifcSpace_TYPE) {
		(*pChild) = CreateTreeItem__IFCINSTANCE_spaceBoundary(parent, ifcModel, ifcObjectInstance);

		(*pChild)->child = CreateTreeItem__GEOMETRY((*pChild));
		(*pChild)->child->next = CreateTreeItem__PROPERTIES((*pChild));
		(*pChild)->child->next->next = CreateTreeItem_ifcSpaceBoundary_SPACE(ifcModel, ifcObjectInstance, (*pChild));

		pChild = &(*pChild)->next;
	}

	pChild = CreateTreeItem_ifcSpaceBoundaryDecomposedBy(ifcModel, ifcObjectInstance, parent, pChild);
	pChild = CreateTreeItem_ifcSpaceBoundaryContains(ifcModel, ifcObjectInstance, parent, pChild);

	return	pChild;
}

void	CreateTreeItem_ifcSpaceBoundary(
				int_t				ifcModel
			)
{
	ifcSpace_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCSPACE");;

	if (sdaiGetMemberCount(sdaiGetEntityExtentBN(ifcModel, (char*)L"IFCRELSPACEBOUNDARY"))) {
		STRUCT_TREE_ITEM	* treeItemSpaceBoundaries = CreateTreeItem__SPACEBOUNDARIES(nullptr),
							** treeItemChild = &treeItemSpaceBoundaries->child;

		topSpaceBoundaryTreeItem = treeItemSpaceBoundaries;

		treeItemSpaceBoundaries->next = topTreeItem;
		topTreeItem = treeItemSpaceBoundaries;

		int_t	* ifcBuildingStoreyInstanceSet = sdaiGetEntityExtentBN(ifcModel, (char*)L"IFCBUILDINGSTOREY"),
				ifcBuildingStoreyInstanceSetCnt = sdaiGetMemberCount(ifcBuildingStoreyInstanceSet);
		for (int_t i = 0; i < ifcBuildingStoreyInstanceSetCnt; i++) {
			int_t	ifcBuildingStoreyInstance = 0;
			sdaiGetAggrByIndex(ifcBuildingStoreyInstanceSet, i, sdaiINSTANCE, &ifcBuildingStoreyInstance);

			(*treeItemChild) = CreateTreeItem__IFCINSTANCE_spaceBoundary(treeItemSpaceBoundaries, ifcModel, ifcBuildingStoreyInstance);

			CreateTreeItem_ifcSpaceBoundary(ifcModel, ifcBuildingStoreyInstance, (*treeItemChild), &(*treeItemChild)->child);

			treeItemChild = &(*treeItemChild)->next;
		}
	}
}
























void	CreateTreeItem_nonReferencedIfcItems(
				int_t				ifcModel
			)
{
	int_t	nonReferencedItemCnt = 0;
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		ASSERT(ifcObject->ifcInstance);
		if (ifcObject->treeItemModel == nullptr && ifcObject->treeItemSpaceBoundary == nullptr) {
			nonReferencedItemCnt++;
		}
		ifcObject = ifcObject->next;
	}

	if	(nonReferencedItemCnt) {
		wchar_t	buffer[100];
		_itow_s((int32_t) nonReferencedItemCnt, buffer, 100, 10);

		topNonReferencedTreeItem = CreateTreeItem__NOTREFERENCED(nullptr);// , L"non-referenced instances");
		STRUCT_TREE_ITEM	** pChild = &topNonReferencedTreeItem->child;

		topNonReferencedTreeItem->next = topTreeItem;
		topTreeItem = topNonReferencedTreeItem;

		ifcObject = ifcObjectsLinkedList;
		while (ifcObject) {
			int_t	ifcEntity = ifcObject->ifcEntity, cnt = 0;

			STRUCT__IFC__OBJECT	* ifcObjectCnt = ifcObject;
			while (ifcObjectCnt  &&  ifcObjectCnt->ifcEntity == ifcEntity) {
				if (ifcObjectCnt->treeItemModel == nullptr && ifcObjectCnt->treeItemSpaceBoundary == nullptr) {
					cnt++;
				}
				ifcObjectCnt = ifcObjectCnt->next;
			}

			if (cnt) {
				(*pChild) = CreateTreeItem__IFCENTITY(topNonReferencedTreeItem, ifcModel, ifcEntity);
				STRUCT_TREE_ITEM	** pSubChild = &(*pChild)->child;

				while (ifcObject  &&  ifcObject->ifcEntity == ifcEntity) {
					if (ifcObject->treeItemModel == nullptr && ifcObject->treeItemSpaceBoundary == nullptr) {
						(*pSubChild) = CreateTreeItem__IFCINSTANCE_nonReferenced((*pChild), ifcModel, ifcObject->ifcInstance);
						(*pSubChild)->child = CreateTreeItem__GEOMETRY((*pSubChild));
						(*pSubChild)->child->next = CreateTreeItem__PROPERTIES((*pSubChild));

						pSubChild = &(*pSubChild)->next;
					}

					ifcObject = ifcObject->next;
				}

				pChild = &(*pChild)->next;
			} else {
				ifcObject = ifcObjectCnt;
			}
		}
	}
}
