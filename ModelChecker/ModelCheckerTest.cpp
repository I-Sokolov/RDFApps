// ModelChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "ModelChecker.h"

#define ASSERT assert

static int CheckModel(const char* filePath, const char* expressSchemaFilePath, RDF::CModelChecker::ModelCheckerLog* pLog);
static int RunSmokeTests();


int main(int argc, char* argv[])
{
    printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<RDFExpressModelChecker>\n");


    int level = 0;

    if (argc > 1) {
        level = CheckModel(argv[1], argc > 2 ? argv[2] : NULL, NULL);
    }
    else {
        level = RunSmokeTests();
    }

    printf("\t<Finished errorLevel='%d' />\n", level);
    printf("</RDFExpressModelChecker>\n");

    return level;
}




static int CheckModel(const char* filePath, const char* expressSchemaFilePath, RDF::CModelChecker::ModelCheckerLog* pLog)
{
    printf("\t<CheckModel file='%s'", filePath);
    
    if (expressSchemaFilePath)
        printf(" schema='%s'>\n", expressSchemaFilePath);
    else
        printf(" embedded_schema='true'>\n");

    int result = 0;

    SdaiModel model = sdaiOpenModelBN(NULL, filePath, expressSchemaFilePath ? expressSchemaFilePath : "");
    if (model) {
        result = RDF::CModelChecker::CheckModel(model, pLog);
        //sdaiCloseModel(model);
    }
    else {
        printf("\t\t<Failure>Can not open model</Failure>\n");
        result = -13;
    }

    printf("\t</CheckModel>\n");
    return result;
}

//
// smoke-test expected issues
//

static int_t r4[] = {4};
static int_t r6[] = {6};

static RDF::CModelChecker::IssueInfo rExpectedIssues[] =
{
    //id   class                    attrName                    ind     aggrLev/aggrInd         Issue
    {110,   "IfcProject",           "GlobalId",                 0,      0,NULL,         RDF::CModelChecker::IssueID::MissedNonOptionalArgument},
    {111,   "IfcProject",           "ObjectType",               4,      0,NULL,         RDF::CModelChecker::IssueID::UnexpectedStar},
    {112,   "IfcProject",           "OwnerHistory",             1,      0,NULL,         RDF::CModelChecker::IssueID::UnexpectedAggregation},
    {113,   "IfcProject",           NULL,                       -1,     0,NULL,         RDF::CModelChecker::IssueID::WrongNumberOfArguments},
    {114,   "IfcProject",           NULL,                       -1,     0,NULL,         RDF::CModelChecker::IssueID::WrongNumberOfArguments},
    {115,   "IfcProject",           "RepresentationContexts",   7,      0,NULL,         RDF::CModelChecker::IssueID::ExpectedAggregation},
    {120,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {121,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {122,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {123,   "IfcOwnerHistory",      "CreationDate",             7,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {124,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {170,   "IfcUnitAssignment",    "Units",                    0,      1,r4,           RDF::CModelChecker::IssueID::WrongArgumentType},
    {170,   "IfcUnitAssignment",    "Units",                    0,      1,r6,           RDF::CModelChecker::IssueID::UnexpectedValueType},
    {171,   "IfcUnitAssignment",    "Units",                    0,      1,r4,           RDF::CModelChecker::IssueID::UnexpectedValueType},
    {172,   "IfcUnitAssignment",    "Units",                    0,      1,r4,           RDF::CModelChecker::IssueID::WrongArgumentType},
    {230,   "IfcSite",              "RefLatitude",              9,      0,NULL,         RDF::CModelChecker::IssueID::WrongAggregationSize},
    {231,   "IfcSite",              "RefLatitude",              9,      0,NULL,         RDF::CModelChecker::IssueID::WrongAggregationSize},
};


//
// smoke test run and check
//

struct CheckExpectedIssuses : public RDF::CModelChecker::ModelCheckerLog
{
    virtual void ReportIssue(RDF::CModelChecker::IssueInfo& issue) override;
};

static int RunSmokeTests()
{
    printf("\t<TestInvalidParameters>\n");

    RDF::CModelChecker::ErrorLevel result = RDF::CModelChecker::CheckModel((int_t)&result);
    ASSERT(result == 100000);
    printf("\t\t<Finished errorLevel='%d' />\n", result);

    result = RDF::CModelChecker::CheckInstance((int_t)&result);
    ASSERT(result == 100000);
    printf("\t\t<Finished errorLevel='%d' />\n", result);

    printf("\t</TestInvalidParameters>\n");

    // test model with different cases of issues
    //
    CheckExpectedIssuses log;
    result = CheckModel("SmokeTests.ifc", NULL, &log);

    //all expected issues are reported
    for (auto expected : rExpectedIssues) {
        ASSERT(expected.stepId == -1);
    }

    return result;
}


void CheckExpectedIssuses::ReportIssue(RDF::CModelChecker::IssueInfo& issue)
{
    //base report
    __super::ReportIssue(issue);

    //check issue expected
    bool found = false;
    for (auto& expected : rExpectedIssues) {
        if (expected.stepId == issue.stepId && expected.attrIndex == issue.attrIndex && expected.aggrLevel == issue.aggrLevel) {
            for (int i = 0; i < issue.aggrLevel; i++) {
                ASSERT(expected.aggrIndArray[i] == issue.aggrIndArray[i]);
            }
            found = true;
            expected.stepId = -1; //mark as reported
            break;
        }
    }
    ASSERT(found);
}


