#include "stdafx.h"
#include "stringCreation.h"


CString	NestedPropertyValue(int_t ifcEntityInstance, wchar_t * propertyName, int_t propertyType, int_t * ifcAggregate)
{
	CString	rValue = 0;

	if (ifcAggregate) {
		ASSERT(ifcEntityInstance == 0 && propertyName == 0 && propertyType == 0);
		engiGetAggrType(ifcAggregate, &propertyType);
	}
	else {
		ASSERT(ifcEntityInstance  &&  propertyName  &&  propertyType);
	}

	wchar_t	* buffer = nullptr, buff[512];

	int_t	ifcInstance = 0, lineNo, *ifcInstanceAggr, ifcInstanceAggrCnt, integer;

	wchar_t	* nominalValue = nullptr,
			* typePath = nullptr;
	int_t	* nominalValueADB = nullptr;

	switch (propertyType) {
	case  sdaiADB:
		sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiUNICODE, &nominalValue);
		if (nominalValue) {
			sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiADB, &nominalValueADB);
			typePath = (wchar_t*)sdaiGetADBTypePath(nominalValueADB, sdaiUNICODE);
		}

		if (nominalValue) {
			rValue += typePath;
			rValue += " (";
			rValue += nominalValue;
			rValue += ")";
		}
		else {
			rValue += "Ø";
		}
		break;
	case  sdaiAGGR:
		rValue += "(";
		ifcInstanceAggr = 0;
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char *)propertyName, sdaiAGGR, &ifcInstanceAggr);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiAGGR, &ifcInstanceAggr);
		}
		if (ifcInstanceAggr) {
			ifcInstanceAggrCnt = sdaiGetMemberCount(ifcInstanceAggr);

			if (ifcInstanceAggrCnt > 0) {
				rValue += NestedPropertyValue(0, 0, 0, ifcInstanceAggr);
				if (ifcInstanceAggrCnt > 1) {
					rValue += ", ...";
				}
			}
			else {
				ASSERT(ifcInstanceAggrCnt == 0);
				rValue += " Ø ";
			}
		}
		else {
			rValue += "???";
		}
		rValue += ")";
		break;
	case  sdaiBINARY:
	case  sdaiBOOLEAN:
	case  sdaiENUM:
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiUNICODE, &buffer);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiUNICODE, &buffer);
		}

		if (buffer  &&  buffer[0]) {
			rValue += buffer;
		}
		else {
			rValue += "Ø";
		}
		break;
	case  sdaiINSTANCE:
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char *)propertyName, sdaiINSTANCE, &ifcInstance);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiINSTANCE, &ifcInstance);
		}
		if (ifcInstance) {
			lineNo = internalGetP21Line(ifcInstance);
			rValue += "#";
#ifdef	WIN64
			_i64tow_s(lineNo, buff, 500, 10);
#else
			_itow_s(lineNo, buff, 10);
#endif
			rValue += buff;
		}
		else {
			rValue += "Ø";
		}
		break;
	case  sdaiINTEGER:
		integer = 0;
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char *)propertyName, sdaiINTEGER, &integer);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiINTEGER, &integer);
		}
#ifdef	WIN64
		_i64tow_s(integer, buff, 500, 10);
#else
		_itow_s(integer, buff, 10);
#endif
		rValue += buff;
		break;
	case  sdaiLOGICAL:
	case  sdaiREAL:
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiUNICODE, &buffer);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiUNICODE, &buffer);
		}

		if (buffer  &&  buffer[0]) {
			rValue += buffer;
		}
		else {
			rValue += "Ø";
		}
		break;
	case  sdaiSTRING:
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiUNICODE, &buffer);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiUNICODE, &buffer);
		}

		rValue += '\'';
		rValue += buffer;
		rValue += '\'';
		break;
	default:
		rValue += "<NOT IMPLEMENTED>";
		break;
	}

	return	rValue;
}

CString	CreateToolTipText(int_t ifcEntityInstance, int_t entity, int_t * pIndex)
{
	if (entity) {
		CString	rValue = CreateToolTipText(ifcEntityInstance, engiGetEntityParent(entity), pIndex);

		wchar_t	* entityName = nullptr;
		engiGetEntityName(entity, sdaiUNICODE, (const char**)&entityName);
		rValue += entityName;
		rValue += "\n";

		int_t	argCnt = engiGetEntityNoArguments(entity);
		while ((*pIndex) < argCnt) {
			wchar_t	* propertyName = nullptr;
			engiGetEntityArgumentName(entity, (*pIndex), sdaiUNICODE, (const char**)&propertyName);

			int_t	propertyType = 0;
			engiGetEntityArgumentType(entity, (*pIndex), &propertyType);
			rValue += "  ";
			rValue += propertyName;
			rValue += " : ";

			rValue += NestedPropertyValue(ifcEntityInstance, propertyName, propertyType, 0);

			rValue += "\n";
			(*pIndex)++;
		}

		return	rValue;
	}

	return	0;
}

CString	CreateToolTipText(int_t ifcEntityInstance)
{
	CString	rValue = 0;
	int_t	index = 0, lineNo, ifcEntity = sdaiGetInstanceType(ifcEntityInstance);
	wchar_t	buff[512];

	lineNo = internalGetP21Line(ifcEntityInstance);
	rValue += "#";
#ifdef	WIN64
	_i64tow_s(lineNo, buff, 500, 10);
#else
	_itow_s(lineNo, buff, 10);
#endif
	rValue += buff;
	rValue += " = ";

	wchar_t	* entityName = nullptr;
	engiGetEntityName(ifcEntity, sdaiUNICODE, (const char **)&entityName);

	rValue += entityName;
	rValue += "(...)\n\n";

	rValue += CreateToolTipText(ifcEntityInstance, ifcEntity, &index);

	return	rValue;
}

wchar_t	* getTreeItemName_property(STRUCT__PROPERTY * ifcProperty)
{
	size_t	i = 0;
	if (ifcProperty->name) {
		memcpy(&ifcProperty->nameBuffer[i], ifcProperty->name, wcslen(ifcProperty->name) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcProperty->name);
	}
	else {
		ifcProperty->nameBuffer[i++] = '<';
		ifcProperty->nameBuffer[i++] = 'n';
		ifcProperty->nameBuffer[i++] = 'a';
		ifcProperty->nameBuffer[i++] = 'm';
		ifcProperty->nameBuffer[i++] = 'e';
		ifcProperty->nameBuffer[i++] = '>';
	}
	ifcProperty->nameBuffer[i++] = ' ';
	ifcProperty->nameBuffer[i++] = '=';
	ifcProperty->nameBuffer[i++] = ' ';
	if (ifcProperty->nominalValue) {
		memcpy(&ifcProperty->nameBuffer[i], ifcProperty->nominalValue, wcslen(ifcProperty->nominalValue) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcProperty->nominalValue);
		ifcProperty->nameBuffer[i++] = ' ';
	}
	else if (ifcProperty->lengthValue) {
		memcpy(&ifcProperty->nameBuffer[i], ifcProperty->lengthValue, wcslen(ifcProperty->lengthValue) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcProperty->lengthValue);
		ifcProperty->nameBuffer[i++] = ' ';
	}
	else if (ifcProperty->areaValue) {
		memcpy(&ifcProperty->nameBuffer[i], ifcProperty->areaValue, wcslen(ifcProperty->areaValue) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcProperty->areaValue);
		ifcProperty->nameBuffer[i++] = ' ';
	}
	else if (ifcProperty->volumeValue) {
		memcpy(&ifcProperty->nameBuffer[i], ifcProperty->volumeValue, wcslen(ifcProperty->volumeValue) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcProperty->volumeValue);
		ifcProperty->nameBuffer[i++] = ' ';
	}
	else if (ifcProperty->countValue) {
		memcpy(&ifcProperty->nameBuffer[i], ifcProperty->countValue, wcslen(ifcProperty->countValue) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcProperty->countValue);
		ifcProperty->nameBuffer[i++] = ' ';
	}
	else if (ifcProperty->weigthValue) {
		memcpy(&ifcProperty->nameBuffer[i], ifcProperty->weigthValue, wcslen(ifcProperty->weigthValue) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcProperty->weigthValue);
		ifcProperty->nameBuffer[i++] = ' ';
	}
	else if (ifcProperty->timeValue) {
		memcpy(&ifcProperty->nameBuffer[i], ifcProperty->timeValue, wcslen(ifcProperty->timeValue) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcProperty->timeValue);
		ifcProperty->nameBuffer[i++] = ' ';
	}
	if (ifcProperty->unitName) {
		memcpy(&ifcProperty->nameBuffer[i], ifcProperty->unitName, wcslen(ifcProperty->unitName) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcProperty->unitName);
		ifcProperty->nameBuffer[i++] = ' ';
	}
	if (ifcProperty->description) {
		ifcProperty->nameBuffer[i++] = '(';
		if (ifcProperty->description) {
			ifcProperty->nameBuffer[i++] = '\'';
			memcpy(&ifcProperty->nameBuffer[i], ifcProperty->description, wcslen(ifcProperty->description) * sizeof(wchar_t) / sizeof(char));
			i += wcslen(ifcProperty->description);
			ifcProperty->nameBuffer[i++] = '\'';
		}
		else {
			ifcProperty->nameBuffer[i++] = '<';
			ifcProperty->nameBuffer[i++] = 'd';
			ifcProperty->nameBuffer[i++] = 'e';
			ifcProperty->nameBuffer[i++] = 's';
			ifcProperty->nameBuffer[i++] = 'c';
			ifcProperty->nameBuffer[i++] = 'r';
			ifcProperty->nameBuffer[i++] = 'i';
			ifcProperty->nameBuffer[i++] = 'p';
			ifcProperty->nameBuffer[i++] = 't';
			ifcProperty->nameBuffer[i++] = 'i';
			ifcProperty->nameBuffer[i++] = 'o';
			ifcProperty->nameBuffer[i++] = 'n';
			ifcProperty->nameBuffer[i++] = '>';
		}
		ifcProperty->nameBuffer[i++] = ')';
	}
	ifcProperty->nameBuffer[i] = 0;
	return	ifcProperty->nameBuffer;
}

/*
wchar_t	* getTreeItemName_headerSet(STRUCT__HEADER__SET * ifcHeaderSet)
{
	size_t	i = 0;
	if (ifcHeaderSet->name) {
		memcpy(&ifcHeaderSet->nameBuffer[i], ifcHeaderSet->name, wcslen(ifcHeaderSet->name) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcHeaderSet->name);
	} else {
		ifcHeaderSet->nameBuffer[i++] = '<';
		ifcHeaderSet->nameBuffer[i++] = 'n';
		ifcHeaderSet->nameBuffer[i++] = 'a';
		ifcHeaderSet->nameBuffer[i++] = 'm';
		ifcHeaderSet->nameBuffer[i++] = 'e';
		ifcHeaderSet->nameBuffer[i++] = '>';
	}
	if (!ifcHeaderSet->child) {
		ifcHeaderSet->nameBuffer[i++] = ' ';
		ifcHeaderSet->nameBuffer[i++] = '=';
		ifcHeaderSet->nameBuffer[i++] = ' ';
		if (ifcHeaderSet->value) {
			ifcHeaderSet->nameBuffer[i++] = '\'';
			memcpy(&ifcHeaderSet->nameBuffer[i], ifcHeaderSet->value, wcslen(ifcHeaderSet->value) * sizeof(wchar_t) / sizeof(char));
			i += wcslen(ifcHeaderSet->value);
			ifcHeaderSet->nameBuffer[i++] = '\'';
		} else {
			ifcHeaderSet->nameBuffer[i++] = '<';
			ifcHeaderSet->nameBuffer[i++] = 'v';
			ifcHeaderSet->nameBuffer[i++] = 'a';
			ifcHeaderSet->nameBuffer[i++] = 'l';
			ifcHeaderSet->nameBuffer[i++] = 'u';
			ifcHeaderSet->nameBuffer[i++] = 'e';
			ifcHeaderSet->nameBuffer[i++] = '>';
		}
	}
	ifcHeaderSet->nameBuffer[i] = 0;
	return	ifcHeaderSet->nameBuffer;
}*/

wchar_t	* getTreeItemName_propertySet(STRUCT__PROPERTY__SET * ifcPropertySet)
{
	size_t	i = 0;
	if (ifcPropertySet->name) {
		ifcPropertySet->nameBuffer[i++] = '\'';
		memcpy(&ifcPropertySet->nameBuffer[i], ifcPropertySet->name, wcslen(ifcPropertySet->name) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcPropertySet->name);
		ifcPropertySet->nameBuffer[i++] = '\'';
	} else {
		ifcPropertySet->nameBuffer[i++] = '<';
		ifcPropertySet->nameBuffer[i++] = 'n';
		ifcPropertySet->nameBuffer[i++] = 'a';
		ifcPropertySet->nameBuffer[i++] = 'm';
		ifcPropertySet->nameBuffer[i++] = 'e';
		ifcPropertySet->nameBuffer[i++] = '>';
	}
	ifcPropertySet->nameBuffer[i++] = ' ';
	ifcPropertySet->nameBuffer[i++] = '(';
	if (ifcPropertySet->description) {
		ifcPropertySet->nameBuffer[i++] = '\'';
		memcpy(&ifcPropertySet->nameBuffer[i], ifcPropertySet->description, wcslen(ifcPropertySet->description) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcPropertySet->description);
		ifcPropertySet->nameBuffer[i++] = '\'';
	} else {
		ifcPropertySet->nameBuffer[i++] = '<';
		ifcPropertySet->nameBuffer[i++] = 'd';
		ifcPropertySet->nameBuffer[i++] = 'e';
		ifcPropertySet->nameBuffer[i++] = 's';
		ifcPropertySet->nameBuffer[i++] = 'c';
		ifcPropertySet->nameBuffer[i++] = 'r';
		ifcPropertySet->nameBuffer[i++] = 'i';
		ifcPropertySet->nameBuffer[i++] = 'p';
		ifcPropertySet->nameBuffer[i++] = 't';
		ifcPropertySet->nameBuffer[i++] = 'i';
		ifcPropertySet->nameBuffer[i++] = 'o';
		ifcPropertySet->nameBuffer[i++] = 'n';
		ifcPropertySet->nameBuffer[i++] = '>';
	}
	ifcPropertySet->nameBuffer[i++] = ')';
	ifcPropertySet->nameBuffer[i] = 0;
	return	ifcPropertySet->nameBuffer;
}

wchar_t	* GetTreeItemName_ifcInstance(
//				int_t	ifcModel,
				int_t	ifcInstance
			)
{
	wchar_t	* ifcType = nullptr,
			* name = nullptr,
			* description = nullptr;

	int_t	ifcInstanceType = sdaiGetInstanceType(ifcInstance);
	engiGetEntityName(ifcInstanceType, sdaiUNICODE, (const char**) &ifcType);

	if (ifcInstance) {
		sdaiGetAttrBN(ifcInstance, (char*) L"name", sdaiUNICODE, &name);
		sdaiGetAttrBN(ifcInstance, (char*) L"description", sdaiUNICODE, &description);
	}

	size_t	i = 0, size = 0;
	if (ifcType) {
		size += wcslen(ifcType);
	}
	if (name) {
		size += 2 + wcslen(name) + 1;
	} else {
		size += 0;
	}
	if (description) {
		size += 2 + 1 + wcslen(description) + 1 + 1;
	} else {
		size += 0;
	}
	size += 1;

	wchar_t	* result = new wchar_t[size];

	if (ifcType) {
		memcpy(&result[i], ifcType, wcslen(ifcType) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcType);
	}
	if (name) {
		result[i++] = ' ';
		result[i++] = '\'';
		memcpy(&result[i], name, wcslen(name) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(name);
		result[i++] = '\'';
	}
	if (description) {
		result[i++] = ' ';
		result[i++] = '(';
		if (description) {
			result[i++] = '\'';
			memcpy(&result[i], description, wcslen(description) * sizeof(wchar_t) / sizeof(char));
			i += wcslen(description);
			result[i++] = '\'';
		} else {
			result[i++] = '<';
			result[i++] = 'd';
			result[i++] = 'e';
			result[i++] = 's';
			result[i++] = 'c';
			result[i++] = 'r';
			result[i++] = 'i';
			result[i++] = 'p';
			result[i++] = 't';
			result[i++] = 'i';
			result[i++] = 'o';
			result[i++] = 'n';
			result[i++] = '>';
		}
		result[i++] = ')';
	}
	result[i] = 0;

	ASSERT(i + 1 == size);
	return	result;
}

wchar_t	* GetTreeItemName_ifcEntity(
//				int_t	ifcModel,
				int_t	ifcEntity
			)
{
	wchar_t	* ifcEntityName = nullptr;

	engiGetEntityName(ifcEntity, sdaiUNICODE, (const char**) &ifcEntityName);

	size_t	i = 0, size = 0;
	if (ifcEntityName) {
		size += wcslen(ifcEntityName);
	} else {
		ASSERT(false);
	}
	size += 1;

	wchar_t	* result = new wchar_t[size];

	if (ifcEntityName) {
		memcpy(&result[i], ifcEntityName, wcslen(ifcEntityName) * sizeof(wchar_t) / sizeof(char));
		i += wcslen(ifcEntityName);
	}
	result[i] = 0;

	ASSERT(i + 1 == size);
	return	result;
}
