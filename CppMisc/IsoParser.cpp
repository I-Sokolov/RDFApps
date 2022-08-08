
#include "stdafx.h"

void IsoParser(void)
{
	FILE* fp = NULL;
	fopen_s(&fp, R"(O:\DevArea\RDF\RDFWrappers\EXPRESS\IFC2X3_TC1.exp)", "rt");
	assert(fp); if (!fp) return;
	
	STRUCT_ISO10303_11_SCHEMA* schema = ISO10303_11_in(fp);

	fclose(fp);
}