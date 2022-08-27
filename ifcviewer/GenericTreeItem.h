#pragma once



#include "material.h"
#include "generic.h"
#include "headerInfo.h"

#include "d3d9types.h"

#define		ITEM_PROPERTY_SET					0
#define		ITEM_CHECKED						1
#define		ITEM_PARTLY_CHECKED					2
#define		ITEM_UNCHECKED						3
#define		ITEM_PROPERTY						4
#define		ITEM_NONE							5
#define		ITEM_UNKNOWN						6
#define		ITEM_CHECKED_TO_ADJUST				10
#define		ITEM_UNCHECKED_TO_ADJUST			12

#define		TYPE_IFCOBJECT_NON_VISIBLE			100
#define		TYPE_IFCOBJECT_VISIBLE				101
#define		TYPE_IFCOBJECT_PROPERTY				102
#define		TYPE_IFCOBJECT_PROPERTYSET			103
#define		TYPE_IFCOBJECT_UNKNOWN				104



static int_t flagbit0  = 1;          // 2^^0    0000.0000..0000.0001
static int_t flagbit1  = 2;          // 2^^1    0000.0000..0000.0010
static int_t flagbit2  = 4;          // 2^^2    0000.0000..0000.0100
static int_t flagbit3  = 8;          // 2^^3    0000.0000..0000.1000

static int_t flagbit4  = 16;         // 2^^4    0000.0000..0001.0000
static int_t flagbit5  = 32;         // 2^^5    0000.0000..0010.0000
static int_t flagbit6  = 64;         // 2^^6    0000.0000..0100.0000
static int_t flagbit7  = 128;        // 2^^7    0000.0000..1000.0000

static int_t flagbit8  = 256;        // 2^^8    0000.0001..0000.0000
static int_t flagbit9  = 512;        // 2^^9    0000.0010..0000.0000
static int_t flagbit10 = 1024;       // 2^^10   0000.0100..0000.0000
static int_t flagbit11 = 2048;       // 2^^11   0000.1000..0000.0000

static int_t flagbit12 = 4096;       // 2^^12   0001.0000..0000.0000
static int_t flagbit13 = 8192;       // 2^^13   0010.0000..0000.0000
static int_t flagbit14 = 16384;      // 2^^14   0100.0000..0000.0000
static int_t flagbit15 = 32768;      // 2^^15   1000.0000..0000.0000

static int_t flagbit16 = 65536;		// 2^^16   0000.0000..0000.0001  0000.0000..0000.0000
static int_t flagbit17 = 131072;	// 2^^17   0000.0000..0000.0010  0000.0000..0000.0000
static int_t flagbit18 = 262144;	// 2^^18   0000.0000..0000.0100  0000.0000..0000.0000
static int_t flagbit19 = 524288;	// 2^^19   0000.0000..0000.1000  0000.0000..0000.0000

static int_t flagbit24 = 16777216;
static int_t flagbit25 = 33554432;
static int_t flagbit26 = 67108864;
static int_t flagbit27 = 134217728;



STRUCT__COLOR					* CreateColor();

//STRUCT__HEADER__SET				* CreateHeaderSet(STRUCT__HEADER__SET * parent, wchar_t * name, wchar_t * value);
STRUCT__PROPERTY				* CreateIfcProperty(int_t ifcInstance, wchar_t * name, wchar_t * description);
STRUCT__PROPERTY__SET			* CreateIfcPropertySet(int_t ifcInstance, wchar_t * name, wchar_t * description);
STRUCT__IFC__OBJECT				* CreateIfcObject(int_t ifcEntity, int_t ifcInstance, wchar_t * entityName, bool hide, int_t segmentationParts);

void							RemoveColor(STRUCT__COLOR * color);
void							DeleteIfcObjects();
void							DeleteTreeItems(STRUCT_TREE_ITEM * treeItem);
