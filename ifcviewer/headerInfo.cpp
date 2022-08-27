#include "stdafx.h"
#include "headerInfo.h"


wchar_t	* CombineText(
				wchar_t	* textI,
				wchar_t	* textII
			)
{
	size_t	lenTextI = 0, lenTextII = 0;
	if (textI) {
		lenTextI = wcslen(textI);
	}
	if (textII) {
		lenTextII = wcslen(textII);
	}

	wchar_t	* text = new wchar_t[lenTextI + 4 + lenTextII + 2];

	if (textI) {
		memcpy(&text[0], textI, lenTextI * sizeof(wchar_t));
	}
	text[lenTextI + 0] = ' ';
	text[lenTextI + 1] = '=';
	text[lenTextI + 2] = ' ';
	text[lenTextI + 3] = '\'';
	if (textII) {
		memcpy(&text[lenTextI + 4], textII, lenTextII * sizeof(wchar_t));
	}
	text[lenTextI + 4 + lenTextII + 0] = '\'';
	text[lenTextI + 4 + lenTextII + 1] = 0;

	return	text;
}


//
//		Get Header Info
//


STRUCT_TREE_ITEM	* GetHeaderDescription(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerDescription = CreateTreeItem__TEXT(parent, L"Set of Descriptions"),
						** ppHeader = &headerDescription->child;

	wchar_t	* text = nullptr;
	int_t	i = 0;

	if	(!GetSPFFHeaderItem(ifcModel, 0, i, sdaiUNICODE, (char **) &text)) {
		while  (!GetSPFFHeaderItem(ifcModel, 0, i++, sdaiUNICODE, (char **) &text)) {
			(* ppHeader) = CreateTreeItem__ALLOCATEDTEXT(headerDescription, CombineText(L"Description", text));
			ppHeader = &(* ppHeader)->next;
			text = nullptr;
		}
	}

	return	headerDescription;
}

STRUCT_TREE_ITEM	* GetImplementationLevel(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerImplementationLevel = nullptr;

	wchar_t	* text = nullptr;

	GetSPFFHeaderItem(ifcModel, 1, 0, sdaiUNICODE, (char **) &text);
	headerImplementationLevel = CreateTreeItem__ALLOCATEDTEXT(parent, CombineText(L"ImplementationLevel", text));

	return	headerImplementationLevel;
}

STRUCT_TREE_ITEM	* GetName(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerName = 0;

	wchar_t	* text = nullptr;

	GetSPFFHeaderItem(ifcModel, 2, 0, sdaiUNICODE, (char **) &text);
	headerName = CreateTreeItem__ALLOCATEDTEXT(parent, CombineText(L"Name", text));

	return	headerName;
}

STRUCT_TREE_ITEM	* GetTimeStamp(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerTimeStamp = nullptr;

	wchar_t	* text = nullptr;

	GetSPFFHeaderItem(ifcModel, 3, 0, sdaiUNICODE, (char **) &text);
	headerTimeStamp = CreateTreeItem__ALLOCATEDTEXT(parent, CombineText(L"TimeStamp", text));

	return	headerTimeStamp;
}

STRUCT_TREE_ITEM	* GetAuthor(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerAuthor = CreateTreeItem__TEXT(parent, L"Set of Authors"),
						** ppHeader = &headerAuthor->child;

	wchar_t	* text = nullptr;
	int_t	i = 0;

	if	(!GetSPFFHeaderItem(ifcModel, 4, i, sdaiUNICODE, (char **) &text)) {
		while  (!GetSPFFHeaderItem(ifcModel, 4, i++, sdaiUNICODE, (char **) &text)) {
			(*ppHeader) = CreateTreeItem__ALLOCATEDTEXT(headerAuthor, CombineText(L"Author", text));
			ppHeader = &(* ppHeader)->next;
			text = 0;
		}
	}

	return	headerAuthor;
}

STRUCT_TREE_ITEM	* GetOrganization(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerOrganization = CreateTreeItem__TEXT(parent, L"Set of Organizations"),
						** ppHeader = &headerOrganization->child;

	wchar_t	* text = nullptr;
int_t	i = 0;

if (!GetSPFFHeaderItem(ifcModel, 5, i, sdaiUNICODE, (char **)&text)) {
	while (!GetSPFFHeaderItem(ifcModel, 5, i++, sdaiUNICODE, (char **)&text)) {
		(*ppHeader) = CreateTreeItem__ALLOCATEDTEXT(headerOrganization, CombineText(L"Organization", text));
		ppHeader = &(*ppHeader)->next;
		text = 0;
	}
}

return	headerOrganization;
}

STRUCT_TREE_ITEM	* GetPreprocessorVersion(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerPreprocessorVersion = nullptr;

	wchar_t	* text = nullptr;

	GetSPFFHeaderItem(ifcModel, 6, 0, sdaiUNICODE, (char **)&text);
	headerPreprocessorVersion = CreateTreeItem__ALLOCATEDTEXT(parent, CombineText(L"PreprocessorVersion", text));

	return	headerPreprocessorVersion;
}

STRUCT_TREE_ITEM	* GetOriginatingSystem(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerOriginatingSystem = nullptr;

	wchar_t	* text = nullptr;

	GetSPFFHeaderItem(ifcModel, 7, 0, sdaiUNICODE, (char **)&text);
	headerOriginatingSystem = CreateTreeItem__ALLOCATEDTEXT(parent, CombineText(L"OriginatingSystem", text));

	return	headerOriginatingSystem;
}

STRUCT_TREE_ITEM	* GetAuthorization(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerAuthorization = nullptr;

	wchar_t	* text = nullptr;

	GetSPFFHeaderItem(ifcModel, 8, 0, sdaiUNICODE, (char **)&text);
	headerAuthorization = CreateTreeItem__ALLOCATEDTEXT(parent, CombineText(L"Authorization", text));

	return	headerAuthorization;
}

STRUCT_TREE_ITEM	* GetFileSchema(int_t ifcModel, STRUCT_TREE_ITEM * parent)
{
	STRUCT_TREE_ITEM	* headerFileSchema = CreateTreeItem__TEXT(parent, L"Set of FileSchemas"),
						** ppHeader = &headerFileSchema->child;

	wchar_t	* text = nullptr;
	int_t	i = 0;

	if (!GetSPFFHeaderItem(ifcModel, 9, i, sdaiUNICODE, (char **)&text)) {
		while (!GetSPFFHeaderItem(ifcModel, 9, i++, sdaiUNICODE, (char **)&text)) {
			(*ppHeader) = CreateTreeItem__ALLOCATEDTEXT(headerFileSchema, CombineText(L"FileSchema", text));
			ppHeader = &(*ppHeader)->next;
			text = 0;
		}
	}

	return	headerFileSchema;
}

STRUCT_TREE_ITEM	* GetHeaderInfo(
							int_t	ifcModel
						)
{
	STRUCT_TREE_ITEM	* headerFileSchema = CreateTreeItem__TEXT(nullptr, L"Header Info"),
						** ppHeader = &headerFileSchema->child;

	(*ppHeader) = GetHeaderDescription(ifcModel, headerFileSchema);
	ppHeader = &(*ppHeader)->next;
	(*ppHeader) = GetImplementationLevel(ifcModel, headerFileSchema);
	ppHeader = &(*ppHeader)->next;
	(*ppHeader) = GetName(ifcModel, headerFileSchema);
	ppHeader = &(*ppHeader)->next;
	(*ppHeader) = GetTimeStamp(ifcModel, headerFileSchema);
	ppHeader = &(*ppHeader)->next;
	(*ppHeader) = GetAuthor(ifcModel, headerFileSchema);
	ppHeader = &(*ppHeader)->next;
	(*ppHeader) = GetOrganization(ifcModel, headerFileSchema);
	ppHeader = &(*ppHeader)->next;
	(*ppHeader) = GetPreprocessorVersion(ifcModel, headerFileSchema);
	ppHeader = &(*ppHeader)->next;
	(*ppHeader) = GetOriginatingSystem(ifcModel, headerFileSchema);
	ppHeader = &(*ppHeader)->next;
	(*ppHeader) = GetAuthorization(ifcModel, headerFileSchema);
	ppHeader = &(*ppHeader)->next;
	(*ppHeader) = GetFileSchema(ifcModel, headerFileSchema);

	return	headerFileSchema;
}

void	DeleteHeaderInfo(STRUCT_TREE_ITEM * headerInfo)
{
	while (headerInfo) {
		DeleteHeaderInfo(headerInfo->child);

//		STRUCT_TREE_ITEM	* currentHeaderInfo = headerInfo;
//		if (headerInfo->name) {
//			delete[] headerInfo->name;
//		}
		
//		if (headerInfo->value) {
//			delete[] headerInfo->value;
//		}
		
//		delete[]  headerInfo->nameBuffer;
			
		headerInfo = headerInfo->next;

//		delete  currentHeaderInfo;
	}
}
