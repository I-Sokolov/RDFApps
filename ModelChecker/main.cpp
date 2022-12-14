// ModelChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "ifcengine.h"

#define ASSERT assert
#define INDENT "\t\t"

static int64_t GetStepId(ValidationIssue* issue)
{
    auto inst = validateGetInstance(issue);
    if (inst) {
        return internalGetP21Line(inst);
    }
    else {
        return -1;
    }
}

static const char* GetEntityName(ValidationIssue* issue)
{
    auto ent = validateGetEntity(issue);
    if (ent) {
        const char* name = 0;
        engiGetEntityName(ent, sdaiSTRING, &name);
        return name;
    }
    else {
        return NULL;
    }
}

static const char* GetAttrName(ValidationIssue* issue)
{
    auto attr = validateGetAttr(issue);
    if (attr) {
        const char* name = 0;
        engiGetAttributeTraits(attr, &name, 0, 0, 0, 0, 0, 0, 0);
        return name;
    }
    else {
        return NULL;
    }
}


static int_t GetAttrIndex(ValidationIssue* issue)
{
    auto ent = validateGetEntity(issue);
    const char* name = GetAttrName(issue);
    if (ent && name) {
        return engiGetEntityAttributeIndexEx(ent, name, true, false);
    }
    else {
        return -1;
    }
}


/// <summary>
/// Issue reporting 
/// </summary>
static void PrintIssue(ValidationIssue* issue)
{
    printf(INDENT "<Issue");

    auto stepId = GetStepId(issue);
    if (stepId > 0) {
        printf(" stepId='#%lld'", stepId);
    }

    auto entity = GetEntityName(issue);
    if (entity) {
        printf(" entity='%s'", entity);
    }

    auto attrName = GetAttrName(issue);
    if (attrName) {
        printf(" attribute='%s'", attrName);
    }

    auto attrIndex = GetAttrIndex(issue);
    if (attrIndex >= 0) {
        printf(" attributeIndex='%lld'", (int64_t)attrIndex);
    }

    auto aggrLevel = validateGetAggrLevel(issue);
    auto aggrIndArray = validateGetAggrIndArray(issue);
    for (int_t i = 0; i < aggrLevel; i++) {
        if (i == 0) {
            printf(" aggregationIndex='%lld'", (int64_t)aggrIndArray[i]);
        }
        else {
            printf(" aggregationIndex_%lld='%lld'", (int64_t)i, (int64_t)aggrIndArray[i]);
        }
    }

    auto issueId = validateGetIssueType(issue);
    auto level = validateGetIssueLevel(issue);
    printf(" issueId='%x' issueLevel='%I64d'>\n", (unsigned int)issueId, level);

    auto text = validateGetDescription(issue);
    if (text) {
        printf(INDENT INDENT "%s\n", text);
    }

    printf(INDENT "</Issue>\n");
}

/// <summary>
/// 
/// </summary>
static ValidationIssueLevel CheckModel(const char* filePath, const char* expressSchemaFilePath)
{
    printf("\t<CheckModel file='%s'", filePath);
    
    if (expressSchemaFilePath)
        printf(" schema='%s'>\n", expressSchemaFilePath);
    else
        printf(" embedded_schema='true'>\n");

    ValidationIssueLevel result = 0;

    SdaiModel model = sdaiOpenModelBN(NULL, filePath, expressSchemaFilePath ? expressSchemaFilePath : "");
    if (model) {
        auto checks = validateModel(model);
        for (auto issue = validateGetFirstIssue(checks); issue; issue = validateGetNextIssue(issue)) {
            PrintIssue(issue);
            result = max(result, validateGetIssueLevel(issue));
        }
        validateFreeResults(checks);
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
static ValidationIssueLevel CheckModels(const char* filePathWC, const char* expressSchemaFilePath)
{
    ValidationIssueLevel res = 0;

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

        printf("\t<Finished errorLevel='%I64d' />\n", level);
        printf("</RDFExpressModelChecker>\n");

        return (int)level;
    }
    else {
        printf("USAGE: %s <CheckFileWildCard> [<SchemaFile>]\n", argv[0]);
        return -1;
    }
}
