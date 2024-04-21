// ModelChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "ifcengine.h"

#define ASSERT assert
#define INDENT "\t\t"

static int64_t GetStepId(ValidationIssue issue)
{
    auto inst = validateGetInstance(issue);
    if (inst) {
        return internalGetP21Line(inst);
    }
    else {
        return -1;
    }
}

static const char* GetEntityName(ValidationIssue issue)
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

static const char* GetAttrName(ValidationIssue issue)
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


static int_t GetAttrIndex(ValidationIssue issue)
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

static ExpressID GetInstanceRelatesd(ValidationIssue issue)
{
    auto inst = validateGetInstanceRelated(issue);
    if (inst) {
        return internalGetP21Line(inst);
    }
    else {
        return 0;
    }
}

/// <summary>
/// 
/// </summary>
const char* GetIssueType(enum_validation_type type)
{
    switch (type) {
        case enum_validation_type::__NONE: return "__NONE";
        case enum_validation_type::__KNOWN_ENTITY: return "__UNKNOWN_ENTITY";
        case enum_validation_type::__NO_OF_ARGUMENTS: return "__NO_OF_ARGUMENTS";
        case enum_validation_type::__ARGUMENT_EXPRESS_TYPE: return "__ARGUMENT_EXPRESS_TYPE";
        case enum_validation_type::__ARGUMENT_PRIM_TYPE: return "__ARGUMENT_PRIM_TYP";
        case enum_validation_type::__REQUIRED_ARGUMENTS: return "__REQUIRED_ARGUMENT";
        case enum_validation_type::__ARRGEGATION_EXPECTED: return "__ARRGEGATION_EXPECTED";
        case enum_validation_type::__AGGREGATION_NOT_EXPECTED: return "__AGGREGATION_NOT_EXPECTED";
        case enum_validation_type::__AGGREGATION_SIZE: return "__AGGREGATION_SIZE";
        case enum_validation_type::__AGGREGATION_UNIQUE: return "__AGGREGATION_UNIQUE";
        case enum_validation_type::__COMPLEX_INSTANCE: return "__COMPLEX_INSTANCE";
        case enum_validation_type::__REFERENCE_EXISTS: return "__REFERENCE_EXISTS";
        case enum_validation_type::__ABSTRACT_ENTITY: return "__ABSTRACT_ENTITY";
        case enum_validation_type::__WHERE_RULE: return "__WHERE_RULE";
        case enum_validation_type::__UNIQUE_RULE: return "__UNIQUE_RULE";
        case enum_validation_type::__STAR_USAGE: return "__STAR_USAGE";
        case enum_validation_type::__CALL_ARGUMENT: return "__CALL_ARGUMENT";
        case enum_validation_type::__INTERNAL_ERROR: return "__INTERNAL_ERROR";
        default: assert(0); return "unknown";
    }
}

/// <summary>
/// Issue reporting 
/// </summary>
static void PrintIssue(ValidationIssue issue)
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

    ExpressID id2 = GetInstanceRelatesd(issue);
    if (id2 > 0)
    {
        printf(" relatedInstance='#%lld'", id2);
    }

    auto issueId = validateGetIssueType(issue);
    auto level = validateGetIssueLevel(issue);
    printf(" issueId='%s' issueLevel='%I64d'>\n", GetIssueType(issueId), level);

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
static ValidationIssueLevel CheckModelsByWC(const char* filePathWC, const char* expressSchemaFilePath)
{
    ValidationIssueLevel res = 0;

    const auto directory = std::filesystem::path{ filePathWC }.parent_path();

    WIN32_FIND_DATA ffd;
    auto hFind = FindFirstFile(filePathWC, &ffd);
    if (hFind != INVALID_HANDLE_VALUE) {
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
    }
    return res;
}

/// <summary>
/// 
/// </summary>
static ValidationIssueLevel CheckIfcFilesRecursively(const char* directory, const char* expressSchemaFilePath)
{
    ValidationIssueLevel res = 0;

    std::filesystem::path wcIfc (directory);
    wcIfc.append("*.ifc");
    CheckModelsByWC(wcIfc.string().c_str(), expressSchemaFilePath);

    //subfolders
    std::filesystem::path wcAll(directory);
    wcAll.append("*");
    WIN32_FIND_DATA ffd;
    auto hFind = FindFirstFile(wcAll.string().c_str(), &ffd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(ffd.cFileName, ".") && strcmp(ffd.cFileName, "..")) {
                    std::filesystem::path path(directory);
                    path.append(ffd.cFileName);
                    CheckIfcFilesRecursively(path.string().c_str(), expressSchemaFilePath);
                }
            }
        } while (FindNextFile(hFind, &ffd) != 0);

        FindClose(hFind);
    }
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

        /*
        validateSetOptions(-1, -1, true, 0, 0);
        if (argc > 3 && _stricmp(argv[3], "--all")) {
            validateSetOptions(-1, -1, false, 0, 0);
        }*/

        int level = -13;
        
        auto path = argv[1];
        auto schema = argc > 2 ? argv[2] : NULL;

        if (std::filesystem::is_directory(path)) {
            level = (int)CheckIfcFilesRecursively(path, schema);
        }
        else {
            level = (int)CheckModelsByWC(path, schema);
        }

        printf("\t<Finished errorLevel='%d' />\n", level);
        printf("</RDFExpressModelChecker>\n");

        return (int)level;
    }
    else {
        printf("USAGE: %s <CheckFileWildCard> [<SchemaFile>]\n", argv[0]);
        return -1;
    }
}
