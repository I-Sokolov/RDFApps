//
//  Author:  Peter Bonsma
//  $Date:  $
//  $Revision:  $
//  Project: IFC Engine Library
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@rdf.bg
//

#pragma once

#ifndef IFCENGINE_EXPORT
    #ifdef _MSC_VER
        #ifdef  IFCENGINE_TOOLBOX_EX
            #define IFCENGINE_EXPORT __declspec(dllexport) //MicroSoft specififc for DLL export
        #else
            #define IFCENGINE_EXPORT __declspec(dllimport) //MicroSoft specififc for DLL import
        #endif
    #else //_MSC_VER
        #define IFCENGINE_EXPORT extern 
    #endif //else _MSC_VER
#endif

namespace RDF
{
    namespace ModelChecker
    {
        typedef unsigned int ErrorLevel;

        enum class IssueID : unsigned char
        {
            Undef,
            WrongNumberOfArguments,
            WrongArgumentType,
            MissedNonOptionalArgument,
            UnexpectedStar,
            ExpectedAggregation,
            UnexpectedAggregation,
            WrongAggregationSize,
            UnexpectedValueType,
            UnresolvedReference,
            AbstractEntity,
            InternalError,
            UniqueRuleViolation,
            AggrElementValueNotUnique,
            InvalidParameter,
            MissedComplexInstanceEntity
        };

        struct CheckResults;
        struct IssueInfo;

        struct IProgress
        {
            virtual void SetLimit(int64_t limit) = 0;
            virtual void Increase(int64_t step) = 0;
            virtual void Finish() = 0;
        };

        extern IFCENGINE_EXPORT CheckResults* CheckModel(SdaiModel model, IProgress* pProgress = nullptr);

        extern IFCENGINE_EXPORT CheckResults* CheckInstance(SdaiInstance instance);

        extern IFCENGINE_EXPORT void         DisposeResults(CheckResults* results);

        extern IFCENGINE_EXPORT IssueInfo*   FirstIssue(CheckResults* results);
        extern IFCENGINE_EXPORT IssueInfo*   NextIssue(IssueInfo* issue);

        extern IFCENGINE_EXPORT IssueID      IssueId(IssueInfo* issue);
        extern IFCENGINE_EXPORT int64_t      StepId(IssueInfo* issue);       //step ID of the STEP record where the issue is happend or -1
        extern IFCENGINE_EXPORT const char*  EntityName(IssueInfo* issue);   //entity name or NULL
        extern IFCENGINE_EXPORT const char*  AttrName(IssueInfo* issue);     //attribute name or NULL
        extern IFCENGINE_EXPORT int_t        AttrIndex(IssueInfo* issue);    //0-based index of direct attribute (position in STEP record) or -1
        extern IFCENGINE_EXPORT int_t        AggrLevel(IssueInfo* issue);    //specifies nesting level of aggregation or 0
        extern IFCENGINE_EXPORT const int_t* AggrIndArray(IssueInfo* issue); //array of indecies for each aggregation lsize is aggrLevel
        extern IFCENGINE_EXPORT ErrorLevel   Level(IssueInfo* issue);
        extern IFCENGINE_EXPORT const char*  Description(IssueInfo* issue);  //description text


        //they are not exactly relevant to Model Checker and are temporary here
        extern IFCENGINE_EXPORT void         CollectReferencingInstancesRecursive(std::set<SdaiInstance>& referencingInstances, SdaiInstance referencedInstance, SdaiEntity* searchEntities /*NULL-terminated array*/);
    }
}

