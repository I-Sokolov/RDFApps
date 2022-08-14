// ModelChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "ModelChecker.h"

static int CheckModel(const char* filePath, const char* expressSchemaFilePath);


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




static int CheckModel(const char* filePath, const char* expressSchemaFilePath)
{
    printf("Checking file %s", filePath);
    
    if (expressSchemaFilePath)
        printf(" against schema %s\n\n", expressSchemaFilePath);
    else
        printf(" against embedded schema\n\n");

    SdaiModel model = sdaiOpenModelBN(NULL, filePath, expressSchemaFilePath ? expressSchemaFilePath : "");
    if (!model) {
        printf("Can not open model\n\n");
        return -13;
    }

    RDF::CModelChecker cheker;   
    auto res = cheker.CheckModel(model);

    //sdaiCloseModel(model);

    return (int)res;
}

