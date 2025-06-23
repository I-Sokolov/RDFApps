#pragma once

extern "C" OwlInstance RDFImportPLY(
    const char*  filePathPLY,
    OwlModel     model,
    char*        errorBuff //[512]
);
