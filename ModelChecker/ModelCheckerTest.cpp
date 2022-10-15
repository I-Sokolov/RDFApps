// ModelChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define IFCENGINE_MODEL_CHECKER
#include "ModelChecker.h"

#define ASSERT assert
#define INDENT "\t\t"

struct IssueHandler
{
    virtual void OnIssue(RDF::ModelChecker::IssueInfo* issue) = NULL;
};

static RDF::ModelChecker::ErrorLevel CheckModel(const char* filePath, const char* expressSchemaFilePath, IssueHandler* pIssueHandler);
static RDF::ModelChecker::ErrorLevel CheckModels(const char* filePath, const char* expressSchemaFilePath);
static RDF::ModelChecker::ErrorLevel RunSmokeTests();


int main(int argc, char* argv[])
{
    printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<RDFExpressModelChecker>\n");


    int level = 0;

    if (argc > 1) {
        level = CheckModels(argv[1], argc > 2 ? argv[2] : NULL);
    }
    else {
        level = RunSmokeTests();
    }

    printf("\t<Finished errorLevel='%d' />\n", level);
    printf("</RDFExpressModelChecker>\n");

    return level;
}

/// <summary>
/// Issue reporting 
/// </summary>
struct PrintIssue : public IssueHandler
{
    virtual void OnIssue(RDF::ModelChecker::IssueInfo* issue) override
    {
        printf(INDENT "<Issue");

        auto stepId = RDF::ModelChecker::StepId(issue);
        if (stepId > 0) {
            printf(" stepId='#%lld'", stepId);
        }

        auto entity = RDF::ModelChecker::EntityName(issue);
        if (entity) {
            printf(" entity='%s'", entity);
        }

        auto attrName = RDF::ModelChecker::AttrName(issue);
        if (attrName) {
            printf(" attribute='%s'", attrName);
        }

        auto attrIndex = RDF::ModelChecker::AttrIndex(issue);
        if (attrIndex >= 0) {
            printf(" attributeIndex='%lld'", (int64_t) attrIndex);
        }

        auto aggrLevel = RDF::ModelChecker::AggrLevel(issue);
        auto aggrIndArray = RDF::ModelChecker::AggrIndArray(issue);
        for (int_t i = 0; i < aggrLevel; i++) {
            if (i == 0) {
                printf(" aggregationIndex='%lld'", (int64_t) aggrIndArray[i]);
            }
            else {
                printf(" aggregationIndex_%lld='%lld'", (int64_t) i, (int64_t) aggrIndArray[i]);
            }
        }

        auto issueId = RDF::ModelChecker::IssueId(issue);
        auto level = RDF::ModelChecker::Level(issue);
        printf(" issueId='%d' issueLevel='%d'>\n", (int) issueId, level);

        auto text = RDF::ModelChecker::Description(issue);
        if (text) {
            printf(INDENT INDENT "%s\n", text);
        }

        printf(INDENT "</Issue>\n");
    }
};

static RDF::ModelChecker::ErrorLevel CheckModels(const char* filePathWC, const char* expressSchemaFilePath)
{
    RDF::ModelChecker::ErrorLevel res = 0;

    const auto directory = std::filesystem::path {filePathWC}.parent_path();

    WIN32_FIND_DATA ffd;
    auto hFind = FindFirstFile(filePathWC, &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("\t\t<Failure call='FindFirstFile'>%s</Failure>\n", filePathWC);
        return -13;
    }

    do {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            //_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
        }
        else {
            std::filesystem::path filePath(directory);
            filePath.append(ffd.cFileName);
            res += CheckModel (filePath.string().c_str(), expressSchemaFilePath, NULL);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);

    return res;
}


static RDF::ModelChecker::ErrorLevel CheckModel(const char* filePath, const char* expressSchemaFilePath, IssueHandler* pLog)
{
    printf("\t<CheckModel file='%s'", filePath);
    
    if (expressSchemaFilePath)
        printf(" schema='%s'>\n", expressSchemaFilePath);
    else
        printf(" embedded_schema='true'>\n");

    int result = 0;

    SdaiModel model = sdaiOpenModelBN(NULL, filePath, expressSchemaFilePath ? expressSchemaFilePath : "");
    if (model) {
        auto checks = RDF::ModelChecker::CheckModel(model);
        for (auto issue = RDF::ModelChecker::FirstIssue(checks); issue; issue = RDF::ModelChecker::NextIssue(issue)) {
            pLog->OnIssue(issue);
            result = max(result, RDF::ModelChecker::Level(issue));
        }
        RDF::ModelChecker::DisposeResults(checks);
        //sdaiCloseModel(model);
    }
    else {
        printf("\t\t<Failure call='sdaiOpenModelBN'>%s</Failure>\n", filePath);
        result = -13;
    }

    printf("\t</CheckModel>\n");
    return result;
}

//
// smoke-test expected issues
//

static int_t r1[] = {1};
static int_t r2[] = {2};
static int_t r3[] = {3};
static int_t r4[] = {4};
static int_t r6[] = {6};
static int_t r7[] = {7};
static int_t r9[] = {9};
static int_t r32[] = {3,2};

struct IssueInfo
{
    int64_t                     stepId;
    const char*                 entity;
    const char*                 attrName;
    int_t                       attrInd;
    int_t                       aggrLevel;
    int_t*                      aggrIndArray;
    RDF::ModelChecker::IssueID  issueId;
};

static IssueInfo rExpectedIssuesIFC2x3[] =
{
    //id   class                    attrName                    ind     aggrLev/aggrInd         Issue
    {84,    "IFCCARTESIANPOINTLIST2D",NULL,                     -1,     0,NULL,         RDF::ModelChecker::IssueID::WrongNumberOfArguments},
    {51,    "IfcProductDefinitionShape","Representations",      2,      1,r3,           RDF::ModelChecker::IssueID::UnresolvedReference},
    {74,    "IfcPolyLoop",          "Polygon",                  0,      1,r1,           RDF::ModelChecker::IssueID::UnresolvedReference},
    {110,   "IfcProject",           "GlobalId",                 0,      0,NULL,         RDF::ModelChecker::IssueID::MissedNonOptionalArgument},
    {111,   "IfcProject",           "ObjectType",               4,      0,NULL,         RDF::ModelChecker::IssueID::UnexpectedStar},
    {112,   "IfcProject",           "OwnerHistory",             1,      0,NULL,         RDF::ModelChecker::IssueID::UnexpectedAggregation},
    {113,   "IfcProject",           NULL,                       -1,     0,NULL,         RDF::ModelChecker::IssueID::WrongNumberOfArguments},
    {114,   "IfcProject",           NULL,                       -1,     0,NULL,         RDF::ModelChecker::IssueID::WrongNumberOfArguments},
    {115,   "IfcProject",           "RepresentationContexts",   7,      0,NULL,         RDF::ModelChecker::IssueID::ExpectedAggregation},
    {116,   "IfcProject",           "OwnerHistory",             1,      0,NULL,         RDF::ModelChecker::IssueID::WrongArgumentType},
    {120,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::ModelChecker::IssueID::WrongArgumentType},
    {121,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::ModelChecker::IssueID::WrongArgumentType},
    {122,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::ModelChecker::IssueID::WrongArgumentType},
    {123,   "IfcOwnerHistory",      "CreationDate",             7,      0,NULL,         RDF::ModelChecker::IssueID::WrongArgumentType},
    {124,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::ModelChecker::IssueID::WrongArgumentType},
    {125,   "IfcOwnerHistory",      "ChangeAction",             3,      0,NULL,         RDF::ModelChecker::IssueID::WrongArgumentType},
    {126,   "IfcOwnerHistory",      "OwningUser",               0,      0,NULL,         RDF::ModelChecker::IssueID::WrongArgumentType},
    {130,   "IfcMeasureWithUnit",   "ValueComponent",           0,      0,NULL,         RDF::ModelChecker::IssueID::WrongArgumentType},
    {170,   "IfcUnitAssignment",    "Units",                    0,      1,r6,           RDF::ModelChecker::IssueID::UnexpectedValueType},
    {170,   "IfcUnitAssignment",    "Units",                    0,      1,r4,           RDF::ModelChecker::IssueID::WrongArgumentType},
    {171,   "IfcUnitAssignment",    "Units",                    0,      1,r4,           RDF::ModelChecker::IssueID::UnexpectedValueType},
    {172,   "IfcUnitAssignment",    "Units",                    0,      1,r4,           RDF::ModelChecker::IssueID::UnexpectedAggregation},
    {230,   "IfcSite",              "RefLatitude",              9,      0,NULL,         RDF::ModelChecker::IssueID::WrongAggregationSize},
    {231,   "IfcSite",              "RefLatitude",              9,      0,NULL,         RDF::ModelChecker::IssueID::WrongAggregationSize},
    {6,     "IfcApplication",       "Version",                  1,      0,NULL,         RDF::ModelChecker::IssueID::UniqueRuleViolation}
};


static IssueInfo rExpectedIssuesIFC4[] =
{
    {14,    "IfcShapeRepresentation",   "ContextOfItems",       0,      0,NULL,         RDF::ModelChecker::IssueID::MissedNonOptionalArgument},
    {29,    "IfcIndexedPolyCurve",      "Segments",             1,      2,r32,          RDF::ModelChecker::IssueID::WrongArgumentType},
    {29,    "IfcIndexedPolyCurve",      "Segments",             1,      1,r2,           RDF::ModelChecker::IssueID::WrongAggregationSize},
    {7,     "IfcRelAggregates",         "RelatedObjects",       5,      1,r2,           RDF::ModelChecker::IssueID::UnresolvedReference},
    {3,     "IfcRelAggregates",         "RelatedObjects",       5,      1,r1,           RDF::ModelChecker::IssueID::AggrElementValueNotUnique},
    {19,    "IfcSpatialStructureElement",NULL,                  -1,     0,NULL,         RDF::ModelChecker::IssueID::AbstractEntity},
    {1,     "IfcRoot",                   "GlobalId",            0,      0,NULL,         RDF::ModelChecker::IssueID::UniqueRuleViolation},
    {21,    "IfcPropertyListValue",      "ListValues",          2,      1,r9,           RDF::ModelChecker::IssueID::WrongArgumentType},
    {21,    "IfcPropertyListValue",      "ListValues",          2,      1,r7,           RDF::ModelChecker::IssueID::WrongArgumentType}
};

//
// smoke test run and check
//

struct CheckExpectedIssuses : public PrintIssue
{
    CheckExpectedIssuses(IssueInfo* rExpectedIssuesIFC2x3, int nExpectedIssues) : m_rExpectedIssues(rExpectedIssuesIFC2x3), m_nExpectedIssues(nExpectedIssues) {}

    virtual void OnIssue(RDF::ModelChecker::IssueInfo* issue) override;

private:
    IssueInfo* m_rExpectedIssues;
    int m_nExpectedIssues;
};

static RDF::ModelChecker::ErrorLevel RunSmokeTests()
{
    printf("\t<TestInvalidParameters>\n");

    RDF::ModelChecker::CheckResults* checks = RDF::ModelChecker::CheckModel((int_t)&checks);
    auto issue = RDF::ModelChecker::FirstIssue(checks);
    auto level = RDF::ModelChecker::Level(issue);
    ASSERT(level == 100000);
    ASSERT(!RDF::ModelChecker::NextIssue(issue));
    printf("\t\t<Finished errorLevel='%d' />\n", level);
    RDF::ModelChecker::DisposeResults(checks);

    checks = RDF::ModelChecker::CheckInstance((int_t)checks);
    issue = RDF::ModelChecker::FirstIssue(checks);
    level = RDF::ModelChecker::Level(issue);
    ASSERT(level == 100000);
    ASSERT(!RDF::ModelChecker::NextIssue(issue));
    printf("\t\t<Finished errorLevel='%d' />\n", level);
    RDF::ModelChecker::DisposeResults(checks);

    printf("\t</TestInvalidParameters>\n");

    // test model with different cases of issues
    //

    CheckExpectedIssuses log(rExpectedIssuesIFC2x3, _countof(rExpectedIssuesIFC2x3));
    auto result = CheckModel("SmokeTestsIFC2x3.ifc", NULL, &log);

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

    return 0;
}


void CheckExpectedIssuses::OnIssue(RDF::ModelChecker::IssueInfo* issue)
{
    //base report
    __super::OnIssue(issue);

    //check issue expected
    bool found = false;
    for (int i = 0; i < m_nExpectedIssues; i++) {
        auto& expected = m_rExpectedIssues[i];
        if (expected.stepId == RDF::ModelChecker::StepId (issue) 
            && expected.attrInd == RDF::ModelChecker::AttrIndex (issue) 
            && expected.aggrLevel == RDF::ModelChecker::AggrLevel (issue)) {

            ASSERT(expected.issueId == RDF::ModelChecker::IssueId (issue));
            
            auto aggrLevel = RDF::ModelChecker::AggrLevel(issue);
            auto aggrIndArray = RDF::ModelChecker::AggrIndArray(issue);
            for (int i = 0; i < aggrLevel; i++) {
                ASSERT(expected.aggrIndArray[i] == aggrIndArray[i]);
            }
            
            found = true;
            expected.stepId = -1; //mark as reported
            break;
        }
    }
    ASSERT(found);
}


