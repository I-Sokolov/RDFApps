// ModelChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "ModelChecker.h"

static int CheckModel(const char* filePath, const char* expressSchemaFilePath);
static int RunSmokeTests();


int main(int argc, char* argv[])
{
    printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<RDFExpressModelChecker>\n");


    int level = 0;

    if (argc > 1) {
        level = CheckModel(argv[1], argc > 2 ? argv[2] : NULL);
    }
    else {
        level = RunSmokeTests();
    }

    printf("\t<Finished errorLevel='%d' />\n", level);
    printf("</RDFExpressModelChecker>\n");

    return level;
}




static int CheckModel(const char* filePath, const char* expressSchemaFilePath)
{
    printf("\t<CheckModel file='%s'", filePath);
    
    if (expressSchemaFilePath)
        printf(" schema='%s'>\n", expressSchemaFilePath);
    else
        printf(" embedded_schema='true'>\n");

    int result = 0;

    SdaiModel model = sdaiOpenModelBN(NULL, filePath, expressSchemaFilePath ? expressSchemaFilePath : "");
    if (model) {
        RDF::CModelChecker cheker;
        result = cheker.CheckModel(model);
        //sdaiCloseModel(model);
    }
    else {
        printf("\t\t<Failure>Can not open model</Failure>\n");
        result = -13;
    }

    printf("\t</CheckModel>\n");
    return result;
}

static int RunSmokeTests()
{
    printf("\t<TestInvalidParameters>\n");

    RDF::CModelChecker checker;
    
    int result = checker.CheckModel((int_t)&checker);
    printf("\t\t<Finished errorLevel='%d' />\n", result);

    result = checker.CheckInstance((int_t)&result);
    printf("\t\t<Finished errorLevel='%d' />\n", result);

    printf("\t</TestInvalidParameters>\n");

    // test model with cases
    //
    result = CheckModel("SmokeTests.ifc", NULL);
    return result;
}
