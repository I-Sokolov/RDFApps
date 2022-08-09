// LandXML2IFC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "parseLandXML.h"

#include "assert.h"




int	GetElementCnt(
				STRUCT_ELEMENT_LIST	* elements,
				char				* tag
			)
{
	int	cnt = 0;
	while (elements) {
		if (elements->element) {
			if (equals(elements->element->tag->name, tag)) {
				cnt++;
			}
			else {
				cnt += GetElementCnt(elements->element->elements, tag);
			}
		}
		elements = elements->next;
	}
	return	cnt;
}

STRUCT_ELEMENT	* GetElement(
						STRUCT_ELEMENT_LIST	* elements,
						int					* pIndex,
						char				* tag
					)
{
	while (elements) {
		if (elements->element) {
			if (equals(elements->element->tag->name, tag)) {
				if ((*pIndex) == 0) {
					return	elements->element;
				}
				(*pIndex)--;
			}
			else {
				STRUCT_ELEMENT	* rValue = GetElement(elements->element->elements, pIndex, tag);
				if (rValue) {
					return	rValue;
				}
			}
		}
		elements = elements->next;
	}
	return	nullptr;
}

STRUCT_ELEMENT	* GetElement(
						STRUCT_ELEMENT_LIST	* elements,
						int					index,
						char				* tag
					)
{
	return	GetElement(
					elements,
					&index,
					tag
				);
}

char	* GetAttribute(
				STRUCT_ELEMENT	* element,
				char			* key
			)
{
	STRUCT_ATTRIBUTE_LIST	* attributes = element->attributes;
	while (attributes) {
		if (attributes->attribute &&
			equals(attributes->attribute->key, key)) {
			return	attributes->attribute->value;
		}
		attributes = attributes->next;
	}
	return	nullptr;
}