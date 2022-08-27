#pragma once

#include "ifcengine/include/engdef.h"
#include "treeitems.h"


#define		STRUCT_TYPE_MATERIAL				200
#define		STRUCT_TYPE_OBJECT_COLOR			201
#define		STRUCT_TYPE_HEADER_SET				202
#define		STRUCT_TYPE_IFC_ITEM				205
#define		STRUCT_TYPE_SELECTABLE_TREEITEM		206



struct STRUCT__COLOR;							//		color
struct STRUCT__SIUNIT;


struct STRUCT_MATERIALS;
struct STRUCT_TREE_ITEM_IFCINSTANCE;

enum ENUM_TREE_ITEM_SELECT_STATE;


struct VECTOR3 {
	double							x;
	double							y;
	double							z;
};

struct STRUCT__IFC__OBJECT {
	int_t							ifcInstance;
	int_t							ifcEntity;

	ENUM_TREE_ITEM_SELECT_STATE		selectState;

	wchar_t							* entityName;
	bool							hide;
	int_t							segmentationParts;
	STRUCT_TREE_ITEM_IFCINSTANCE	* treeItemModel;
	STRUCT_TREE_ITEM_IFCINSTANCE	* treeItemSpaceBoundary;
	STRUCT_TREE_ITEM_IFCINSTANCE	* treeItemNonReferenced;

	STRUCT_MATERIALS				* materials;
	STRUCT__IFC__OBJECT				* next;

	VECTOR3							vecMin;
	VECTOR3							vecMax;

	int_t							noVertices;
	float							* __vertices;

	int_t							vertexOffset;

	int_t							noPrimitivesForPoints;
	int32_t							* indicesForPoints;
	int_t							indexOffsetForPoints;

	int_t							noPrimitivesForLines;
	int32_t							* indicesForLines;
	int_t							indexOffsetForLines;

	int_t							noPrimitivesForFaces;
	int32_t							* indicesForFaces;
	int_t							indexOffsetForFaces;

	int_t							noPrimitivesForWireFrame;
	int32_t							* indicesForLinesWireFrame;
	int_t							indexOffsetForWireFrame;
};



bool	equals(wchar_t * txtI, wchar_t * txtII);
bool	equals(char * txtI, char * txtII);
