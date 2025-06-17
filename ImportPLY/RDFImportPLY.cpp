
#include "pch.h"
#include "RDFImportPLY.h"

extern "C" OwlInstance RDFImportPLY(
    const char* filePathPLY,
    OwlModel     model,
    const char* textureFolder,
    const char* textureBasePath,
    char*       errorBuff //[512]
)
{
    if (errorBuff) {
        strcpy(errorBuff, "Not implemented");
    }
    return NULL;
}

