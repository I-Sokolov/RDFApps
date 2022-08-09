#pragma once


#include "parseXML.h"


int				GetElementCnt(
						STRUCT_ELEMENT_LIST	* elements,
						char				* tag
					);

STRUCT_ELEMENT	* GetElement(
						STRUCT_ELEMENT_LIST	* elements,
						int					index,
						char				* tag
					);

char			* GetAttribute(
						STRUCT_ELEMENT		* element,
						char				* key
					);
