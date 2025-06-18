
#include "pch.h"
#include "RDFImportPLY.h"
#include "ImportPLY.h"

extern "C" OwlInstance RDFImportPLY(
    const char* filePathPLY,
    OwlModel     model,
    const char* textureFolder,
    const char* textureBasePath,
    char*       errorBuff //[512]
)
{
    ImportPLY importer(model, textureFolder, textureBasePath, errorBuff);
    return importer.Import(filePathPLY);
}

