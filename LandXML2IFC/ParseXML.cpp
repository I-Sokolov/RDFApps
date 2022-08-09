// LandXML2IFC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "parseXML.h"

#include "parse.h"

#include "assert.h"



extern	bool	eof;

extern	int		TOTAL_LINES_READ,
				TOTAL_CHARS_READ;


STRUCT_ELEMENT_LIST* ParseElements(char* parentTag);

char	* ParseWord()
{
	char	name[512];
	switch  (name[0] = *GetByte()) {
		CASE_ENTITY_CHAR
		CASE_LOWER_CASE
		CASE_UPPER_CASE
			{
				int i = 1;
				while (!eof) {
					switch (name[i] = *GetByte()) {
						CASE_ENTITY_CHAR
						CASE_LOWER_CASE
						CASE_UPPER_CASE
						CASE_NUMBER
						case  ':':
							i++;
							break;
						default:
							switch (name[i]) {
								CASE_SPACE
								case  ':':
								case  '>':
								case  '=':
								case  '/':
									break;
								default:
									assert(false);
									break;
							}
							name[i] = 0;

							UndoGetByte();
							return	(char*) memcpy(new char[strlen(name) + 1], name, (strlen(name) + 1) * sizeof(char));
					}
				}
				assert(false);
			}
			break;
		default:
			assert(false);
			break;
	}

	return	nullptr;
}

STRUCT_TAG	* newTAG(char * name)
{
	STRUCT_TAG	* myTag = new STRUCT_TAG();

	myTag->name = name;

	return	myTag;
}

STRUCT_ELEMENT	* newELEMENT(char * tag)
{
	STRUCT_ELEMENT	* myElement = new STRUCT_ELEMENT();

	myElement->tag = newTAG(tag);

	return	myElement;
}

STRUCT_ATTRIBUTE	* newATTRIBUTE(char * key)
{
	STRUCT_ATTRIBUTE	* myAttribute = new STRUCT_ATTRIBUTE();

	myAttribute->key = key;

	return	myAttribute;
}

STRUCT_ATTRIBUTE	* ParseAttribute()
{
	STRUCT_ATTRIBUTE	* myAttribute = nullptr;
	while (!eof) {
		char	myChar = 0;
		switch (myChar = *GetByte()) {
			CASE_SPACE
				break;
			CASE_ENTITY_CHAR
			CASE_LOWER_CASE
			CASE_UPPER_CASE
				UndoGetByte();
				myAttribute = newATTRIBUTE(ParseWord());
				break;
			case  '=':
				if (myAttribute) {
					while (!eof) {
						switch (myChar = *GetByte()) {
							CASE_SPACE
								break;
							case  '"':
								while (myChar = *GetByte()) {
									if (myChar == '"') {
										return	myAttribute;
									}
									else {
										if (myAttribute->value) {
											char	* value = new char[strlen(myAttribute->value) + 2];
											memcpy(value, myAttribute->value, strlen(myAttribute->value) * sizeof(char));
											value[strlen(myAttribute->value) + 0] = myChar;
											value[strlen(myAttribute->value) + 1] = 0;
											delete[] myAttribute->value;
											myAttribute->value = value;
										}
										else {
											myAttribute->value = new char[2];
											myAttribute->value[0] = myChar;
											myAttribute->value[1] = 0;
										}
									}
								}
								break;
							default:
								assert(false);
								break;
						}
					}
				}
				assert(false);
				break;
			default:
				assert(false);
				break;
		}
	}

	assert(false);
	return	nullptr;
}

STRUCT_ATTRIBUTE_LIST	* ParseAttributes()
{
	STRUCT_ATTRIBUTE_LIST	* myAttributeList = nullptr,
							** pMyAttributeList = &myAttributeList;

	while (!eof) {
		char	myChar = 0;
		switch (myChar = *GetByte()) {
			CASE_SPACE
				break;
			CASE_ENTITY_CHAR
			CASE_LOWER_CASE
			CASE_UPPER_CASE
				UndoGetByte();
				(*pMyAttributeList) = new STRUCT_ATTRIBUTE_LIST();
				(*pMyAttributeList)->attribute = ParseAttribute();
				pMyAttributeList = &(*pMyAttributeList)->next;
				break;
			case  '/':
			case  '>':
				UndoGetByte();
				return	myAttributeList;
			default:
				assert(false);
				break;
		}
	}

	assert(false);
	return	nullptr;
}

STRUCT_ELEMENT	* ParseElement()
{
	bool	closeDirectly = false;
	switch (*GetByte()) {
		case  '!':
			closeDirectly = true;
			break;
		CASE_ENTITY_CHAR
		CASE_LOWER_CASE
		CASE_UPPER_CASE
			{
				UndoGetByte();
			
				STRUCT_ELEMENT	* myElement = newELEMENT(ParseWord());

				myElement->attributes = ParseAttributes();

				switch (*GetByte()) {
					case  '/':
						{
							char	myChar = *GetByte();
							assert(myChar == '>');
						}
						return	myElement;
					case  '>':
						assert(myElement->elements == nullptr);
						myElement->elements = ParseElements(myElement->tag->name);
						return	myElement;
					default:
						assert(false);
						break;
				}
			}
			break;
		case  '?':
			while (*GetByte() != '?' && !eof) {
			}
			if (*GetByte() != '>') {
				assert(false);
			}
			return	newELEMENT((char*) "Header");
		default:
			assert(false);
			break;
	}

	assert(false);
	return	nullptr;
}

bool	equals(char * txtI, char * txtII)
{
	if (txtI && txtII) {
		int i = 0;
		while (txtI[i] && txtI[i] == txtII[i]) { i++; }
		if (txtI[i] == 0 && txtII[i] == 0) { return true; }
	}
	return  false;
}

STRUCT_ELEMENT_LIST	* ParseElements(char * parentTag)
{
	STRUCT_ELEMENT_LIST	* myElementList = nullptr,
						** pMyElementList = &myElementList;

	while (!eof) {
		char	myChar = 0;
		switch (myChar = *GetByte()) {
			CASE_SPACE
				break;
			case  '<':
				myChar = *GetByte();
				if (myChar == '/') {
					assert(parentTag);
					char	* tag = ParseWord();
					assert(equals(tag, parentTag));
					myChar = *GetByte();
					assert(myChar == '>');
					return	myElementList;
				}
				else {
					UndoGetByte();
					(*pMyElementList) = new STRUCT_ELEMENT_LIST();
					(*pMyElementList)->element = ParseElement();
					pMyElementList = &(*pMyElementList)->next;
				}
				break;
			CASE_NUMBER
			case  '-':
				{
					(*pMyElementList) = new STRUCT_ELEMENT_LIST();
					(*pMyElementList)->value = new char[2];
					(*pMyElementList)->value[0] = myChar;
					(*pMyElementList)->value[1] = 0;

					bool	skip = false;
					while (skip == false) {
						switch (myChar = *GetByte()) {
							case  '<':
								UndoGetByte();
								skip = true;
								break;
							CASE_SPACE
								skip = true;
								break;
							CASE_NUMBER
							case  '-':
							case  '.':
								{
									char	* value = new char[strlen((*pMyElementList)->value) + 2];
									memcpy(value, (*pMyElementList)->value, strlen((*pMyElementList)->value) * sizeof(char));
									value[strlen((*pMyElementList)->value) + 0] = myChar;
									value[strlen((*pMyElementList)->value) + 1] = 0;
									delete[] (*pMyElementList)->value;
									(*pMyElementList)->value = value;
								}
								break;
							default:
								assert(false);
								break;
						}
					}
					pMyElementList = &(*pMyElementList)->next;
				}
				break;
			case  '_':
			CASE_LOWER_CASE
			CASE_UPPER_CASE
				{
					(*pMyElementList) = new STRUCT_ELEMENT_LIST();
					(*pMyElementList)->value = new char[2];
					(*pMyElementList)->value[0] = myChar;
					(*pMyElementList)->value[1] = 0;

					bool	skip = false;
					while (skip == false) {
						switch (myChar = *GetByte()) {
							case  '<':
								UndoGetByte();
								skip = true;
								break;
							default:
								{
									char	* value = new char[strlen((*pMyElementList)->value) + 2];
									memcpy(value, (*pMyElementList)->value, strlen((*pMyElementList)->value) * sizeof(char));
									value[strlen((*pMyElementList)->value) + 0] = myChar;
									value[strlen((*pMyElementList)->value) + 1] = 0;
									delete[] (*pMyElementList)->value;
									(*pMyElementList)->value = value;
								}
								break;
						}
					}
				}
				break;
			default:
				if (!eof) {
					assert(false);
				}
				break;
		}
	}

	assert(parentTag == nullptr);
	return	myElementList;
}

STRUCT_ELEMENT_LIST	* ParseXML(
							char	* fileName
						)
{
	FILE	* fp = nullptr;
	fopen_s(&fp, fileName, "r");
	if (fp) {
		TOTAL_LINES_READ = 0;
		TOTAL_CHARS_READ = 0;

		InitGetByte(fp);

		STRUCT_ELEMENT_LIST	* elements = ParseElements(nullptr);

		fclose(fp);

		return	elements;
	}

	return	nullptr;
}
