// ModelChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

static int CheckModel(const char* filePath, const char* expressSchemaFilePath);

static void MessageOut(const char* format, ...);


int main(int argc, char* argv[])
{
    printf("RDF step model checker\n\n");

    if (argc != 2 && argc != 3) {
        printf("Usage: ModelChecker.exe <filepath> [<expressSchemaFilePath>]\n\n");
        return -13;
    }

    int level = CheckModel(argv[1], argc == 3 ? argv[2] : NULL);

    printf("ModelChecker finished\n\n");

    return level;
}

static void MessageOut(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}


static int CheckModel(const char* filePath, const char* expressSchemaFilePath)
{
    MessageOut("Checking file %s\n", filePath);
    
    if (expressSchemaFilePath)
        MessageOut("against schema %s", expressSchemaFilePath);
    else
        MessageOut("against embedded schema");

    SdaiModel model = sdaiOpenModelBN(NULL, filePath, expressSchemaFilePath);
    if (!model) {
        MessageOut("Can not open model\n");
        return -13;
    }

    sdaiCloseModel(model);
}

