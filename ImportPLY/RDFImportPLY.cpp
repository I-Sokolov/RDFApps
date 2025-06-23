
#include "pch.h"
#include "RDFImportPLY.h"
#include "ImportPLY.h"

extern "C" OwlInstance RDFImportPLY(
    const char* filePathPLY,
    OwlModel     model,
    char*       errorBuff //[512]
)
{
    ImportPLY importer(model, errorBuff);
    return importer.Import(filePathPLY);
}

