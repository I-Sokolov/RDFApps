#include "stdafx.h"
#include "GenericTreeItem.h"
#include "generic.h"

#include <math.h>


extern	STRUCT__IFC__OBJECT	* ifcObjectsLinkedList;



STRUCT__IFC__OBJECT	* CreateIfcObject(int_t ifcEntity, int_t ifcInstance, wchar_t * entityName, bool hide, int_t segmentationParts)
{
	STRUCT__IFC__OBJECT	* ifcObject = new STRUCT__IFC__OBJECT;

	if (hide) {
		ifcObject->selectState = TI_UNCHECKED;
	} else {
		ifcObject->selectState = TI_CHECKED;
	}

	ifcObject->next = nullptr;

	ifcObject->entityName = entityName;
	ifcObject->hide = hide;
	ifcObject->segmentationParts = segmentationParts;
	ifcObject->treeItemModel = nullptr;
	ifcObject->treeItemSpaceBoundary = nullptr;
	ifcObject->treeItemNonReferenced = nullptr;

	ifcObject->materials = 0;

	ifcObject->vecMin.x = 0;
	ifcObject->vecMin.y = 0;
	ifcObject->vecMin.z = 0;
	ifcObject->vecMax.x = 0;
	ifcObject->vecMax.y = 0;
	ifcObject->vecMax.z = 0;

	ifcObject->ifcInstance = ifcInstance;
	ifcObject->ifcEntity = ifcEntity;
	ifcObject->noVertices = 0;
	ifcObject->__vertices = 0;

	ifcObject->vertexOffset = 0;

	ifcObject->noPrimitivesForPoints = 0;
	ifcObject->indicesForPoints = 0;
	ifcObject->indexOffsetForPoints = 0;

	ifcObject->noPrimitivesForLines = 0;
	ifcObject->indicesForLines = 0;
	ifcObject->indexOffsetForLines = 0;

	ifcObject->noPrimitivesForFaces = 0;
	ifcObject->indicesForFaces = 0;
	ifcObject->indexOffsetForFaces = 0;

	ifcObject->noPrimitivesForWireFrame = 0;
	ifcObject->indicesForLinesWireFrame = 0;
	ifcObject->indexOffsetForWireFrame = 0;

	return	ifcObject;
}

//
//	Cleanup
//

void	RemoveColor(STRUCT__COLOR * color)
{
	color = color;
	//delete(color);
}

void	RemoveIfcProperty(STRUCT__PROPERTY * ifcProperty)
{
	if (ifcProperty->name) {
		delete[] ifcProperty->name;
	}
	if (ifcProperty->description) {
		delete[] ifcProperty->description;
	}

	if (ifcProperty->nominalValue) {
		delete[] ifcProperty->nominalValue;
	}
	if (ifcProperty->areaValue) {
		delete[] ifcProperty->areaValue;
	}
	if (ifcProperty->unitName) {
		delete[] ifcProperty->unitName;
	}

	delete[] ifcProperty->nameBuffer;
	delete(ifcProperty);
}

void	RemoveNestedIfcProperty(STRUCT__PROPERTY * ifcProperty)
{
	while  (ifcProperty) {
		STRUCT__PROPERTY	* ifcPropertyToRemove = ifcProperty;
		ifcProperty = ifcProperty->next;

		RemoveIfcProperty(ifcPropertyToRemove);
	}
}

void	RemoveIfcPropertySet(STRUCT__PROPERTY__SET * ifcPropertySet)
{
	if	(ifcPropertySet->name) {
		delete[] ifcPropertySet->name;
	}
	if	(ifcPropertySet->description) {
		delete[] ifcPropertySet->description;
	}
	delete[] ifcPropertySet->nameBuffer;

	delete(ifcPropertySet);
}

void	RemoveNestedIfcPropertySet(STRUCT__PROPERTY__SET * ifcPropertySet)
{
	while  (ifcPropertySet) {
		STRUCT__PROPERTY__SET	* ifcPropertySetToRemove = ifcPropertySet;
		ifcPropertySet = ifcPropertySet->next;

		RemoveIfcPropertySet(ifcPropertySetToRemove);
	}
}

void	DeleteIfcObject(STRUCT__IFC__OBJECT * ifcObject)
{
	if	(ifcObject->__vertices) {
		delete[] ifcObject->__vertices;
	}
	if (ifcObject->indicesForPoints) {
		delete[] ifcObject->indicesForPoints;
	}
	if (ifcObject->indicesForLines) {
		delete[] ifcObject->indicesForLines;
	}
	if (ifcObject->indicesForFaces) {
		delete[] ifcObject->indicesForFaces;
	}
	if (ifcObject->indicesForLinesWireFrame) {
		delete[] ifcObject->indicesForLinesWireFrame;
	}

	STRUCT_MATERIALS	* materials = ifcObject->materials;
	while (materials) {
		STRUCT_MATERIALS	* materialToRemove = materials;
		materials = materials->next;

		delete  materialToRemove;
	}

	delete	ifcObject;
}

void	DeleteIfcObjects()
{
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		STRUCT__IFC__OBJECT	* ifcObjectToRemove = ifcObject;
		ifcObject = ifcObject->next;
	
		DeleteIfcObject(ifcObjectToRemove);
	}
	ifcObjectsLinkedList = nullptr;
}

void	DeleteTreeItem(STRUCT_TREE_ITEM * treeItem)
{
	switch (treeItem->type) {
		case TREE_ITEM_CONTAINS:
		case TREE_ITEM_NOTREFERENCED:
		case TREE_ITEM_GROUPS:
		case TREE_ITEM_GROUPEDBY:
		case TREE_ITEM_DECOMPOSEDBY:
		case TREE_ITEM_SPACEBOUNDARIES:
		case TREE_ITEM_PROPERTIES:
		case TREE_ITEM_GEOMETRY:
			break;
		case TREE_ITEM_IFCENTITY:
		case TREE_ITEM_IFCINSTANCE:
		case TREE_ITEM_PROPERTYSET:
		case TREE_ITEM_PROPERTY:
			if (treeItem->nameBuffer) {
				delete[] treeItem->nameBuffer;
			}
			break;
//		case TREE_ITEM_PROPERTYSET:
//		case TREE_ITEM_PROPERTY:
//			break;
		case TREE_ITEM_TEXT:
			ASSERT(treeItem->nameBuffer);
			if (((STRUCT_TREE_ITEM_TEXT*)treeItem)->allocated) {
				delete[] treeItem->nameBuffer;
			}
			break;
		default:
			ASSERT(false);
			break;
	}

	delete	treeItem;
}

void	DeleteTreeItems(STRUCT_TREE_ITEM * treeItem)
{
	while  (treeItem) {
		DeleteTreeItems(treeItem->child);

		STRUCT_TREE_ITEM	* treeItemToRemove = treeItem;
		treeItem = treeItem->next;

		DeleteTreeItem(treeItemToRemove);
	}
}
 