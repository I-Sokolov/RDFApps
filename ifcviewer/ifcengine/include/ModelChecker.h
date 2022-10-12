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

#ifdef IFCENGINE_MODEL_CHECKER

#pragma once

#ifndef IFCENGINE_EXPORT
#ifdef _MSC_VER
#define IFCENGINE_EXPORT __declspec(dllexport) 
#else
#define IFCENGINE_EXPORT
#endif
#endif

namespace RDF
{
    class IFCENGINE_EXPORT CModelChecker
    {
    public:
        typedef unsigned int ErrorLevel;

        enum class IssueID
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

        struct IssueInfo
        {
            int64_t     stepId;         //step ID of the STEP record where the issue is happend or -1
            const char  * entity;       //entity name or NULL
            const char  * attrName;     //attribute name or NULL
            int_t       attrIndex;      //0-based index of direct attribute (position in STEP record) or -1
            int_t       aggrLevel;      //specifies nesting level of aggregation or 0
            int_t       * aggrIndArray; //array of indecies for each aggregation lsize is aggrLevel

            IssueID     issueId;
            ErrorLevel  level;
            const char  * text;         //description text
        };

        struct IFCENGINE_EXPORT ModelCheckerLog //subclass if you want to change reporting
        {
            //default implementation prints xml to output stream
            virtual void ReportIssue(IssueInfo& issue);
        };

        struct IFCENGINE_EXPORT ModelCheckerProgress //subclass if you want to change progress indication
        {
            //default implementation prints to output stream
            virtual void SetLimit(int64_t limit);
            virtual void Increase(int_t step = 1);
            virtual void Finish();

            int64_t m_limit;
            int64_t m_pos;

            ModelCheckerProgress () : m_limit(0), m_pos(0) {}
        };

    public:
        static ErrorLevel CheckModel(SdaiModel model, ModelCheckerLog* pLog = nullptr, ModelCheckerProgress* pProgress = nullptr);

        static ErrorLevel CheckInstance(SdaiInstance instance, ModelCheckerLog* pLog = nullptr, ModelCheckerProgress* pProgress = nullptr);
        
        static void CollectReferencingInstancesRecursive (std::set<SdaiInstance>& referencingInstances, SdaiInstance referencedInstance, SdaiEntity* searchEntities /*NULL-terminated array*/);
    };
}

#endif // IFCENGINE_MODEL_CHECKER

