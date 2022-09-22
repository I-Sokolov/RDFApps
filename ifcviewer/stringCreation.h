#ifndef STRING_CREATION_H
#define STRING_CREATION_H


#include "generic.h"
#include "headerInfo.h"


CString		NestedPropertyValue(
					int_t			ifcEntityInstance,
					const wchar_t	* propertyName,
					int_t			propertyType,
					int_t			* ifcAggregate
				);

CString		CreateToolTipText(
					int_t		ifcEntityInstance,
					int_t		entity,
					int_t		* pIndex
				);

CString		CreateToolTipText(
					int_t		ifcEntityInstance
				);

wchar_t		* getTreeItemName_property(
					STRUCT__PROPERTY				* ifcProperty
				);

//wchar_t		* getTreeItemName_headerSet(
//					STRUCT__HEADER__SET				* ifcHeaderSet
//				);

wchar_t		* getTreeItemName_propertySet(
					STRUCT__PROPERTY__SET			* ifcPropertySet
				);

wchar_t		* GetTreeItemName_ifcInstance(
//					int_t							ifcModel,
					int_t							ifcInstance
				);

wchar_t		* GetTreeItemName_ifcEntity(
//					int_t							ifcModel,
					int_t							ifcEntity
				);


#endif