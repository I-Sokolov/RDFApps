#pragma once

extern "C" OwlInstance RDFImportPLY(
    const char*  filePathPLY,
    OwlModel     model,
    const char*  textureFolder,
    const char*  textureBasePath,
    char*        errorBuff //[512]
);
