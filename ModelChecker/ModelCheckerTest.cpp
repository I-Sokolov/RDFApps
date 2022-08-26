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

static int_t r1[] = {1};
static int_t r3[] = {3};
static int_t r4[] = {4};
static int_t r6[] = {6};

static RDF::CModelChecker::IssueInfo rExpectedIssuesIFC2x3[] =
{
    //id   class                    attrName                    ind     aggrLev/aggrInd         Issue
    {84,    "IFCCARTESIANPOINTLIST2D",NULL,                     -1,     0,NULL,         RDF::CModelChecker::IssueID::WrongNumberOfArguments},
    {51,    "IfcProductDefinitionShape","Representations",      2,      1,r3,           RDF::CModelChecker::IssueID::UnresolvedReference},
    {74,    "IfcPolyLoop",          "Polygon",                  0,      1,r1,           RDF::CModelChecker::IssueID::UnresolvedReference},
    {110,   "IfcProject",           "GlobalId",                 0,      0,NULL,         RDF::CModelChecker::IssueID::MissedNonOptionalArgument},
    {111,   "IfcProject",           "ObjectType",               4,      0,NULL,         RDF::CModelChecker::IssueID::UnexpectedStar},
    {112,   "IfcProject",           "OwnerHistory",             1,      0,NULL,         RDF::CModelChecker::IssueID::UnexpectedAggregation},
    {113,   "IfcProject",           NULL,                       -1,     0,NULL,         RDF::CModelChecker::IssueID::WrongNumberOfArguments},
    {114,   "IfcProject",           NULL,                       -1,     0,NULL,         RDF::CModelChecker::IssueID::WrongNumberOfArguments},
    {115,   "IfcProject",           "RepresentationContexts",   7,      0,NULL,         RDF::CModelChecker::IssueID::ExpectedAggregation},
    {116,   "IfcProject",           "OwnerHistory",             1,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {120,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {121,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {122,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {123,   "IfcOwnerHistory",      "CreationDate",             7,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {124,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {125,   "IfcOwnerHistory",      "ChangeAction",             3,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {126,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {130,   "IfcMeasureWithUnit",   "ValueComponent",           0,      0,NULL,         RDF::CModelChecker::IssueID::WrongArgumentType},
    {170,   "IfcUnitAssignment",    "Units",                    0,      1,r4,           RDF::CModelChecker::IssueID::WrongArgumentType},
    {170,   "IfcUnitAssignment",    "Units",                    0,      1,r6,           RDF::CModelChecker::IssueID::UnexpectedValueType},
    {171,   "IfcUnitAssignment",    "Units",                    0,      1,r4,           RDF::CModelChecker::IssueID::UnexpectedValueType},
    {172,   "IfcUnitAssignment",    "Units",                    0,      1,r4,           RDF::CModelChecker::IssueID::UnexpectedAggregation},
    {230,   "IfcSite",              "RefLatitude",              9,      0,NULL,         RDF::CModelChecker::IssueID::WrongAggregationSize},
    {231,   "IfcSite",              "RefLatitude",              9,      0,NULL,         RDF::CModelChecker::IssueID::WrongAggregationSize},
};


static RDF::CModelChecker::IssueInfo rExpectedIssuesIFC4[] =
{
    {14,    "IfcShapeRepresentation",   "ContextOfItems",       0,      0,NULL,         RDF::CModelChecker::IssueID::MissedNonOptionalArgument}
};

//
// smoke test run and check
//

struct CheckExpectedIssuses : public RDF::CModelChecker::ModelCheckerLog
{
    CheckExpectedIssuses(RDF::CModelChecker::IssueInfo* rExpectedIssuesIFC2x3, int nExpectedIssues) : m_rExpectedIssues(rExpectedIssuesIFC2x3), m_nExpectedIssues(nExpectedIssues) {}

    virtual void ReportIssue(RDF::CModelChecker::IssueInfo& issue) override;

private:
    RDF::CModelChecker::IssueInfo* m_rExpectedIssues;
    int m_nExpectedIssues;
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

    CheckExpectedIssuses log(rExpectedIssuesIFC2x3, _countof(rExpectedIssuesIFC2x3));
    result = CheckModel("SmokeTestsIFC2x3.ifc", NULL, &log);

    //all expected issues are reported
    for (auto expected : rExpectedIssuesIFC2x3) {
        ASSERT(expected.stepId == -1);
    }

    CheckExpectedIssuses log2(rExpectedIssuesIFC4, _countof(rExpectedIssuesIFC4));
    result = CheckModel("SmokeTestsIFC4.ifc", NULL, &log2);

    //all expected issues are reported
    for (auto expected : rExpectedIssuesIFC2x3) {
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
    for (int i = 0; i < m_nExpectedIssues; i++) {
        auto& expected = m_rExpectedIssues[i];
        if (expected.stepId == issue.stepId && expected.attrIndex == issue.attrIndex && expected.aggrLevel == issue.aggrLevel) {
            assert(expected.issueId == issue.issueId);
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


