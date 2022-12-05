// ModelChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "ModelCheckerAPI.h"

#define ASSERT assert
#define INDENT "\t\t"


/// <summary>
/// Issue reporting 
/// </summary>
static void PrintIssue(RDF::ModelChecker::IssueInfo* issue)
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
        printf(" attributeIndex='%lld'", (int64_t)attrIndex);
    }

    auto aggrLevel = RDF::ModelChecker::AggrLevel(issue);
    auto aggrIndArray = RDF::ModelChecker::AggrIndArray(issue);
    for (int_t i = 0; i < aggrLevel; i++) {
        if (i == 0) {
            printf(" aggregationIndex='%lld'", (int64_t)aggrIndArray[i]);
        }
        else {
            printf(" aggregationIndex_%lld='%lld'", (int64_t)i, (int64_t)aggrIndArray[i]);
        }
    }

    auto issueId = RDF::ModelChecker::IssueId(issue);
    auto level = RDF::ModelChecker::Level(issue);
    printf(" issueId='%d' issueLevel='%d'>\n", (int)issueId, level);

    auto text = RDF::ModelChecker::Description(issue);
    if (text) {
        printf(INDENT INDENT "%s\n", text);
    }

    printf(INDENT "</Issue>\n");
}

/// <summary>
/// 
/// </summary>
static RDF::ModelChecker::ErrorLevel CheckModel(const char* filePath, const char* expressSchemaFilePath)
{
    printf("\t<CheckModel file='%s'", filePath);
    
    if (expressSchemaFilePath)
        printf(" schema='%s'>\n", expressSchemaFilePath);
    else
        printf(" embedded_schema='true'>\n");

    RDF::ModelChecker::ErrorLevel result = 0;

    SdaiModel model = sdaiOpenModelBN(NULL, filePath, expressSchemaFilePath ? expressSchemaFilePath : "");
    if (model) {
        auto checks = RDF::ModelChecker::CheckModel(model);
        for (auto issue = RDF::ModelChecker::FirstIssue(checks); issue; issue = RDF::ModelChecker::NextIssue(issue)) {
            PrintIssue(issue);
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


/// <summary>
/// 
/// </summary>
static RDF::ModelChecker::ErrorLevel CheckModels(const char* filePathWC, const char* expressSchemaFilePath)
{
    RDF::ModelChecker::ErrorLevel res = 0;

    const auto directory = std::filesystem::path{ filePathWC }.parent_path();

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
            res += CheckModel(filePath.string().c_str(), expressSchemaFilePath);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);

    return res;
}

/// <summary>
/// 
/// </summary>
int main(int argc, char* argv[])
{
    if (argc > 1) {
        printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        printf("<RDFExpressModelChecker>\n");

        auto   level = CheckModels(argv[1], argc > 2 ? argv[2] : NULL);

        printf("\t<Finished errorLevel='%d' />\n", level);
        printf("</RDFExpressModelChecker>\n");

        return level;
    }
    else {
        printf("USAGE: %s <CheckFileWildCard> [<SchemaFile>]\n", argv[0]);
        return -1;
    }
}
