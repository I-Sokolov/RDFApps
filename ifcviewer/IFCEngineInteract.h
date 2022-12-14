#pragma once


#include "ifcengine\include\ifcengine.h"
#include "ifcengine\include\engine.h"
#include "GenericTreeItem.h"

extern	int_t	globalIfcModel;

bool				ParseIfcFile(
							CWnd				* pParent
						);

void				CleanupIfcFile(
						);

STRUCT_TREE_ITEM	* CreateTreeItem_ifcObject(
							int_t				ifcModel,
							int_t				ifcObjectInstance,
							STRUCT_TREE_ITEM	* parent,
							int_t				depth
						);


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
		CString str(name);
		return engiGetEntityAttributeIndexEx(ent, (char*)str.GetBuffer(), true, false);
	}
	else {
		return -1;
	}
}

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

static const char* GetIssueId(ValidationIssue* issue)
{
	auto type = validateGetIssueType(issue);
	switch (type) {
		case ValidationIssueType::WrongNumberOfArguments:		return "WrongNumberOfArguments";
		case ValidationIssueType::WrongArgumentType:			return "WrongArgumentType";
		case ValidationIssueType::MissedNonOptionalArgument:	return "MissedNonOptionalArgument";
		case ValidationIssueType::UnexpectedStar:				return "UnexpectedStar";
		case ValidationIssueType::ExpectedAggregation:			return "ExpectedAggregation";
		case ValidationIssueType::UnexpectedAggregation:		return "UnexpectedAggregation";
		case ValidationIssueType::WrongAggregationSize:			return "WrongAggregationSize";
		case ValidationIssueType::UnexpectedValueType:			return "UnexpectedValueType";
		case ValidationIssueType::UnresolvedReference:			return "UnresolvedReference";
		case ValidationIssueType::AbstractEntity:				return "AbstractEntity";
		case ValidationIssueType::InternalError:				return "InternalError";
		case ValidationIssueType::UniqueRuleViolation:			return "UniqueRuleViolation";
		case ValidationIssueType::AggrElementValueNotUnique:	return "AggrElementValueNotUnique";
		case ValidationIssueType::InvalidParameter:				return "InvalidParameter";
		case ValidationIssueType::MissedComplexInstanceEntity:	return "MissedComplexInstanceEntity";
		case ValidationIssueType::WhereRuleViolation:			return "WhereRuleViolation";
		default:
			assert(0);
			return "Unknown";
	}
}

extern void CollectReferencingInstancesRecursive(std::set<SdaiInstance>& referencingInstances, SdaiInstance referencedInstance, SdaiEntity* searchEntities /*NULL-terminated array*/);
