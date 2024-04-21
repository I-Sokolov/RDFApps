
#include "pch.h"
#include "PSDChecker.h"

using namespace RDF::PSD;

/// <summary>
/// 
/// </summary>
static void ToUpper(std::string& str)
{
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = (char)toupper(str[i]);
    }
}

/// <summary>
/// 
/// </summary>
static bool StoD(const std::string& sval, double& dval)
{
    bool ok = false;
    try {
        size_t pos = 0;
        dval = stod(sval, &pos);
        if (pos == sval.length()) {
            ok = true;
        }
    }
    catch (...) {
    }
    return ok;
}

/// <summary>
/// 
/// </summary>
static void printInstance(std::string& str, SdaiInstance inst)
{
    if (inst) {
        auto ent = sdaiGetInstanceType(inst);
        auto entName = engiGetEntityName(ent, sdaiSTRING);
        auto stepId = internalGetP21Line(inst);

        char buff[80];
        sprintf(buff, "#%ld=", (long)stepId);
        str.append (buff);
        str.append (entName);

        if (auto attrName = sdaiGetAttrDefinition(ent, "Name")) {
            const char* name = NULL;
            if (sdaiGetAttr(inst, attrName, sdaiSTRING, &name) && name) {
                str.push_back('(');
                str.append (name);
                str.push_back(')');
            }
        }

        str.push_back(' ');
    }
}


/// <summary>
/// 
/// </summary>
void Console::formatOutput(ErrorLevel level, const char* message, SdaiInstance inst1, SdaiInstance inst2, SdaiInstance inst3)
{
    std::string str;

    //#ifndef _DEBUG
    if ((int)level < 0) {
        return;
    }
    //#endif // !_DEBUG

    switch (level) {
        case ErrorLevel::Error: str.append("ERROR "); break;
        case ErrorLevel::Warning: str.append("Warning "); break;
        case ErrorLevel::OK: break;
        case ErrorLevel::DebugTrace: str.append("      - "); break;
        default: assert(false);
    }

    str.append(message);
    str.push_back(':');
    str.push_back(' ');

    printInstance(str, inst1);
    printInstance(str, inst2);
    printInstance(str, inst3);

    writeString(str.c_str());
}

/// <summary>
/// 
/// </summary>
struct DefConsole : public Console
{
    void writeString(const char* msg) override
    {
        printf("  %s\n", msg);
    }
};

static DefConsole s_defaultConsole;

/// <summary>
/// 
/// </summary>
Checker::Checker(const char* psdFolder, Console* console)
    : m_psdFolder(psdFolder)
    , m_console(console ? *console : s_defaultConsole)
    , m_level (ErrorLevel::OK)
{
}

/// <summary>
/// 
/// </summary>
Checker::~Checker()
{

}

/// <summary>
/// 
/// </summary>
void Checker::Check(const char* ifcFilePath)
{
    auto model = sdaiOpenModelBN(0, ifcFilePath, "");
    if (model) {
        Check(model);
    }
    else {
        AddLogMsg(ErrorLevel::Error, "Cann't read IFC file");
    }
}

#define INIT_ENTITY(Name) m_entity##Name = sdaiGetEntity(m_model, "Ifc" #Name); assert(m_entity##Name);

#define INIT_ATTR(Ent,Attr,EntAttr) m_attr##EntAttr = sdaiGetAttrDefinition(m_entity##Ent, #Attr); assert(m_attr##EntAttr);

/// <summary>
/// 
/// </summary>
void Checker::InitEntities()
{
    INIT_ENTITY(Root);
    INIT_ENTITY(PropertyDefinition);
    INIT_ENTITY(PropertySetDefinition);
    INIT_ENTITY(PropertySet);
    INIT_ENTITY(ElementQuantity);
    if (m_ifcVersion > IfcVersion::Ifc2x3) {
        INIT_ENTITY(PropertySetTemplate);
    }
    INIT_ENTITY(RelDefinesByProperties);
    INIT_ENTITY(Object);
    INIT_ENTITY(TypeObject);
    INIT_ENTITY(Property);
    INIT_ENTITY(PhysicalQuantity);
    INIT_ENTITY(PropertySingleValue);
    INIT_ENTITY(NamedUnit);
    INIT_ENTITY(DerivedUnit);
    INIT_ENTITY(MonetaryUnit);
    INIT_ENTITY(PropertyEnumeratedValue);
    INIT_ENTITY(PropertyTableValue);
    INIT_ENTITY(PropertyListValue);
    INIT_ENTITY(PropertyBoundedValue);

    INIT_ATTR(Root, Name, Root_Name);
    INIT_ATTR(PropertySet, HasProperties, PropertySet_HasProperties);
    INIT_ATTR(ElementQuantity, Quantities, ElementQuantity_Quantities);
    INIT_ATTR(RelDefinesByProperties, RelatingPropertyDefinition, RelDefinesByProperties_RelatingPropertyDefinition);
    INIT_ATTR(RelDefinesByProperties, RelatedObjects, RelDefinesByProperties_RelatedObjects);
    INIT_ATTR(TypeObject, HasPropertySets, TypeObject_HasPropertySets);
    INIT_ATTR(Property, Name, Property_Name);
    INIT_ATTR(PhysicalQuantity, Name, PhysicalQuantity_Name);
    INIT_ATTR(PropertySingleValue, NominalValue, PropertySingleValue_NominalValue);
    INIT_ATTR(PropertySingleValue, Unit, PropertySingleValue_Unit);
    INIT_ATTR(NamedUnit, UnitType, NamedUnit_UnitType);
    INIT_ATTR(DerivedUnit, UnitType, DerivedUnit_UnitType);
    INIT_ATTR(PropertyEnumeratedValue, EnumerationValues, PropertyEnumeratedValue_EnumerationValues);
    INIT_ATTR(PropertyTableValue, DefiningValues, PropertyTableValue_DefiningValues);
    INIT_ATTR(PropertyTableValue, DefinedValues, PropertyTableValue_DefinedValues);
    INIT_ATTR(PropertyTableValue, DefiningUnit, PropertyTableValue_DefiningUnit);
    INIT_ATTR(PropertyTableValue, DefinedUnit, PropertyTableValue_DefinedUnit);
    INIT_ATTR(PropertyListValue, ListValues, PropertyListValue_ListValues);
    INIT_ATTR(PropertyListValue, Unit, PropertyListValue_Unit);
    INIT_ATTR(PropertyBoundedValue, UpperBoundValue, PropertyBoundedValue_UpperBoundValue);
    INIT_ATTR(PropertyBoundedValue, LowerBoundValue, PropertyBoundedValue_LowerBoundValue);
    INIT_ATTR(PropertyBoundedValue, Unit, PropertyBoundedValue_Unit);
}

/// <summary>
/// 
/// </summary>
void Checker::AddLogMsg(ErrorLevel level, const char* message, SdaiInstance inst1, SdaiInstance inst2, SdaiInstance inst3)
{
    if (level > m_level) {
        m_level = level;
    }

    m_console.formatOutput(level, message, inst1, inst2, inst3);
}


/// <summary>
/// 
/// </summary>
std::string Checker::Format(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char buff[256];
    vsnprintf(buff, 255, format, args);

    va_end(args);

    return buff;
}

/// <summary>
/// 
/// </summary>
IfcVersion Checker::GetIfcVersion(SdaiModel model)
{
    const char* schemaName = nullptr;
    GetSPFFHeaderItem(model, 9, 0, sdaiSTRING, &schemaName);
    assert(schemaName);

    std::string str(schemaName);
    ToUpper(str);

    if (strstr(str.c_str(), "IFC4X3")) {
        return IfcVersion::Ifc4x3;
    }
    else if (strstr(str.c_str(), "IFC4")) {
        return IfcVersion::Ifc4;
    }
    else if (strstr(str.c_str(), "IFC2X3")) {
        return IfcVersion::Ifc2x3;
    }
    else {
        assert(false);
        std::string msg = "Unsupported IFC version ";
        msg += schemaName;
        AddLogMsg(ErrorLevel::Error, msg.c_str());
        return IfcVersion::Ifc4x3;
    }
}
;
/// <summary>
/// 
/// </summary>
void Checker::Check(SdaiModel model)
{
    m_model = model;
    m_ifcVersion = GetIfcVersion(model);

    auto saveUnicode = getStringUnicode();
    setStringUnicode(0);

    InitEntities();

    CheckPropertyDefinitions();
    CheckObjectsApplicability();
    CheckTypes();

    setStringUnicode(saveUnicode);

    m_model = NULL;
    m_ifcVersion = IfcVersion::Ifc2x3;
}

/// <summary>/// 
/// </summary>
void Checker::CheckObjectsApplicability()
{
    auto rels = xxxxGetEntityAndSubTypesExtent(m_model, m_entityRelDefinesByProperties);
    
    int_t i = 0;
    SdaiInstance rel = NULL;
    while (sdaiGetAggrByIndex(rels, i++, sdaiINSTANCE, &rel)) {
        SdaiInstance propDefinition = NULL;
        if (sdaiGetAttr(rel, m_attrRelDefinesByProperties_RelatingPropertyDefinition, sdaiINSTANCE, &propDefinition)) {
            if (sdaiIsKindOf(propDefinition, m_entityPropertySetDefinition)) {
                SdaiAggr objects = NULL;
                if (sdaiGetAttr(rel, m_attrRelDefinesByProperties_RelatedObjects, sdaiAGGR, &objects)) {
                    SdaiInstance obj = NULL;
                    int_t j = 0;
                    while (sdaiGetAggrByIndex(objects, j++, sdaiINSTANCE, &obj)) {
                        if (sdaiIsKindOf(obj, m_entityObject)) {
                            CheckApplicability(obj, propDefinition);
                        }
                        else {
                            AddLogMsg(ErrorLevel::Error, "RelatedObjects must be a list of IfcObject", rel, obj);
                        }
                    }
                }
            }
            else {
                AddLogMsg(ErrorLevel::Error, "RelatingPropetyDefinition must be IfcPropertySetDefinition", rel, propDefinition);
            }
        } 
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckTypes()
{
    auto types = xxxxGetEntityAndSubTypesExtent(m_model, m_entityTypeObject);

    int_t i = 0;
    SdaiInstance type = NULL;
    while (sdaiGetAggrByIndex(types, i++, sdaiINSTANCE, &type)) {
        SdaiAggr psets = NULL;
        if (sdaiGetAttr(type, m_attrTypeObject_HasPropertySets, sdaiINSTANCE, &psets)) {
            SdaiInstance pset = NULL;
            int_t j = 0;
            while (sdaiGetAggrByIndex(psets, j++, sdaiINSTANCE, &pset)) {
                if (sdaiIsKindOf(pset, m_entityPropertySetDefinition)) {
                    CheckApplicability(type, pset);
                    CheckPropertyDefinition(pset);
                }
                else {
                    AddLogMsg(ErrorLevel::Error, "HasPropertySet must be a set of IfcPropertySetDefinition", type, pset);
                }
            }

        }
    }
}

void Checker::CheckApplicability(SdaiInstance obj, SdaiInstance pset)
{
    AddLogMsg(ErrorLevel::DebugTrace, "Cheking applicability", obj, pset);

    assert(sdaiIsKindOfBN(obj, "IfcObjectDefinition"));
    assert(sdaiIsKindOfBN(pset, "IfcPropertySetDefinition"));

    const char* psetName = nullptr;
    if (sdaiGetAttr(pset, m_attrRoot_Name, sdaiSTRING, &psetName)) {
        auto psd = GetPSD(psetName);
        if (psd) {
            if (!psd->IsClassApplicable(obj)) {
                AddLogMsg(ErrorLevel::Error, "Property set definition is not applicable to the instance", pset, obj); 
            }
        }
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckPropertyDefinitions()
{
    auto psets = xxxxGetEntityAndSubTypesExtent(m_model, m_entityPropertyDefinition);
    SdaiInstance pset = NULL;
    int_t i = 0;
    while (sdaiGetAggrByIndex(psets, i++, sdaiINSTANCE, &pset)) {
        CheckPropertyDefinition(pset);
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckPropertyDefinition(SdaiInstance pdef)
{
    AddLogMsg(ErrorLevel::DebugTrace, __FUNCTION__, pdef);

    assert(sdaiIsKindOf(pdef, m_entityPropertyDefinition));

    const char* psetName = NULL;
    sdaiGetAttr(pdef, m_attrRoot_Name, sdaiSTRING, &psetName);
    if (psetName) {
        auto psd = GetPSD(psetName);
        if (psd) {
            if (sdaiIsKindOf(pdef, m_entityPropertySet)) {
                CheckPropertySet(pdef, *psd);
            }
            else if (sdaiIsKindOf(pdef, m_entityElementQuantity)) {
                CheckElementQuantity(pdef, *psd);
            }
            else if (sdaiIsKindOf(pdef, m_entityPropertySetTemplate)) {
                //do nothing?
            }
            else {
                AddLogMsg(ErrorLevel::Error, "Unsupported property definition class", pdef);
                assert(false);
            }
        }
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckPropertySet(SdaiInstance pset, PropertySetDef& psd)
{
    assert(sdaiIsKindOf(pset, m_entityPropertySet));

    SdaiAggr props = NULL;
    sdaiGetAttr(pset, m_attrPropertySet_HasProperties, sdaiAGGR, &props);

    SdaiInstance prop = NULL;
    int_t i = 0;
    while (sdaiGetAggrByIndex(props, i++, sdaiINSTANCE, &prop)) {
        CheckProperty(pset, prop, psd);
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckProperty(SdaiInstance pset, SdaiInstance prop, PropertySetDef& psd)
{
    AddLogMsg(ErrorLevel::DebugTrace, __FUNCTION__, pset, prop);

    assert(sdaiIsKindOf(prop, m_entityProperty));

    const char* propName = NULL;
    sdaiGetAttr(prop, m_attrProperty_Name, sdaiSTRING, &propName);

    if (auto pdef = psd.GetPropertyDef(propName)) {
        switch (pdef->m_typeProperty) {
            case PropertyDef::TypeProperty::SingleValue:
                CheckPropertySingleValue(pset, prop, *pdef);
                break;
            case PropertyDef::TypeProperty::EnumeratedValue:
                CheckPropertyEnumeratedValue(pset, prop, *pdef);
                break;
            case PropertyDef::TypeProperty::BoundedValue:
                CheckPropertyBoundedValue(pset, prop, *pdef);
                break;
            case PropertyDef::TypeProperty::TableValue:
                CheckPropertyTableValue(pset, prop, *pdef);
                break;
            case PropertyDef::TypeProperty::ReferenceValue:
                CheckPropertyReferenceValue(pset, prop, *pdef);
                break;
            case PropertyDef::TypeProperty::ListValue:
                CheckPropertyListValue(pset, prop, *pdef);
                break;
            case PropertyDef::TypeProperty::ComplexProperty:
                CheckPropertyComplexValue(pset, prop, *pdef);
                break;
            default:
                assert(!"Not implemented typeProperty check");
        }
    }
    else {
        AddLogMsg(ErrorLevel::Warning, "PSD does not contain property", pset, prop);
    }
}

/// <summary
///
/// </summary
void Checker::CheckPropertySingleValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef) 
{ 
    if (sdaiIsKindOf(prop, m_entityPropertySingleValue)) {
        CheckDataType(pset, prop, m_attrPropertySingleValue_NominalValue, pdef.m_dataType);
        CheckUnitType(pset, prop, m_attrPropertySingleValue_Unit, pdef.m_unitType);
    }
    else {
        AddLogMsg(ErrorLevel::Error, "Property must be an instance of entity IfcPropertySingleValue", pset, prop);
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckDataType(SdaiInstance pset, SdaiInstance prop, SdaiAttr attr, const std::string& dataType)
{
    if (dataType.length()) {
        SdaiADB nominalValue = NULL;
        sdaiGetAttr(prop, attr, sdaiADB, &nominalValue);
        if (nominalValue) {
            auto adbTypePath = sdaiGetADBTypePath(nominalValue, 0);
            if (_stricmp(dataType.c_str(), adbTypePath)) {
                const char* attrName = NULL;
                engiGetAttributeTraits(attr, &attrName, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                AddLogMsg(ErrorLevel::Error, Format("%s of property must be of type %s", attrName, dataType.c_str()).c_str(), pset, prop);
            }
        }
    }

}

/// <summary>
/// 
/// </summary>
void Checker::CheckUnitType(SdaiInstance pset, SdaiInstance prop, SdaiAttr attr, const std::string& unitType)
{
    if (unitType.length()) {
        SdaiInstance unit = NULL;
        if (sdaiGetAttr(prop, attr, sdaiINSTANCE, &unit)) {

            const char* uType = NULL;
            if (sdaiIsKindOf(unit, m_entityNamedUnit)) {
                sdaiGetAttr(unit, m_attrNamedUnit_UnitType, sdaiENUM, &uType);
            }
            else if (sdaiIsKindOf(unit, m_entityDerivedUnit)) {
                sdaiGetAttr(unit, m_attrDerivedUnit_UnitType, sdaiENUM, &uType);
            }
            else if (sdaiIsKindOf(unit, m_entityMonetaryUnit)) {

            }
            else {
                assert(!"Unknonw unit entity");
            }

            if (!uType || strcmp(uType, unitType.c_str())) {
                const char* attrName = NULL;
                engiGetAttributeTraits(attr, &attrName, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                AddLogMsg(ErrorLevel::Error, Format("%s of property must be of type %s", attrName, unitType.c_str()).c_str(), pset, prop);
            }
        }
    }

}

/// <summary>
/// 
/// </summary>
void Checker::CheckPropertyEnumeratedValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef)
{
    if (sdaiIsKindOf(prop, m_entityPropertyEnumeratedValue)) {
        int_t i = 0;
        SdaiAggr aggr = NULL;
        if (sdaiGetAttr(prop, m_attrPropertyEnumeratedValue_EnumerationValues, sdaiAGGR, &aggr)) {
            int_t N = sdaiGetMemberCount (aggr);
            for (; i < N; i++) {
                SdaiADB value = NULL;
                const char* strval = NULL;
                if (sdaiGetAggrByIndex(aggr, i++, sdaiADB, &value)) {
                    sdaiGetADBValue(value, sdaiSTRING, &strval);
                }
                if (!strval || pdef.m_enumListItems.find(strval) == pdef.m_enumListItems.end()) {
                    i = 0;
                    break;
                }
            }
        }

        if (!i) {
            std::string list;
            for (auto s : pdef.m_enumListItems) {
                if (!list.empty()) {
                    list += ", ";
                }
                list += s;
            }
            AddLogMsg(ErrorLevel::Error, Format("EnumerationValues must be a list [1:*] of elemenets from the set {%s}", list.c_str()).c_str(), pset, prop);
        }
    }
    else {
        AddLogMsg(ErrorLevel::Error, "Property must be an instance of entity IfcPropertySingleValue", pset, prop);
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckPropertyBoundedValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef)
{ 
    if (sdaiIsKindOf(prop, m_entityPropertyBoundedValue)) {

        CheckDataType(pset, prop, m_attrPropertyBoundedValue_LowerBoundValue, pdef.m_dataType);
        CheckDataType(pset, prop, m_attrPropertyBoundedValue_UpperBoundValue, pdef.m_dataType);
        CheckUnitType(pset, prop, m_attrPropertyBoundedValue_Unit, pdef.m_unitType);

        if (!pdef.m_lowerBoundValue.empty()) {
            if (pdef.m_lowerBoundValue != "?") {
                CheckBoundLimit(pset, prop, m_attrPropertyBoundedValue_LowerBoundValue, pdef.m_lowerBoundValue, true);
            }
        }
        if (!pdef.m_upperBoundValue.empty()) {
            if (pdef.m_upperBoundValue != "?") {
                CheckBoundLimit(pset, prop, m_attrPropertyBoundedValue_UpperBoundValue, pdef.m_upperBoundValue, false);
            }
        }
    }
    else {
        AddLogMsg(ErrorLevel::Error, "Property must be an instance of entity IfcPropertyBoundedValue", pset, prop);
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckBoundLimit(SdaiInstance pset, SdaiInstance prop, SdaiAttr attr, const std::string& bound, bool lower)
{
    double dbound = 0;
    if (!StoD(bound, dbound)) {
        AddLogMsg(ErrorLevel::Error, Format("Bound limit check is implemented only for real values but not for %s", bound.c_str()).c_str(), pset, prop);
        assert(!"TODO");
        return;
    }

    SdaiADB nominalValue = NULL;
    sdaiGetAttr(prop, attr, sdaiADB, &nominalValue);
    if (nominalValue) {
        double dval = 0;
        if (sdaiGetADBValue(nominalValue, sdaiREAL, &dval)) {
            bool ok = false;
            if (lower) {
                ok = (dbound - dval < DBLTOL);
            }
            else {
                ok = (dval - dbound < DBLTOL);
            }

            if (!ok) {
                const char* attrName = NULL;
                engiGetAttributeTraits(attr, &attrName, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                AddLogMsg(ErrorLevel::Error, Format("Value of %s is out of bound limit", attrName).c_str(), pset, prop);
            }
        }
        else {
            const char* attrName = NULL;
            engiGetAttributeTraits(attr, &attrName, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            AddLogMsg(ErrorLevel::Error, Format("Expected number value for %s", attrName).c_str(), pset, prop);
        }
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckPropertyTableValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef)
{ 
    if (sdaiIsKindOf(prop, m_entityPropertyTableValue)) {
        CheckListValue(pset, prop, m_attrPropertyTableValue_DefiningValues, pdef.m_definingValue);
        CheckListValue(pset, prop, m_attrPropertyTableValue_DefinedValues, pdef.m_definedValue);
        CheckUnitType(pset, prop, m_attrPropertyTableValue_DefiningUnit, pdef.m_definingValue.unitType);
        CheckUnitType(pset, prop, m_attrPropertyTableValue_DefinedUnit, pdef.m_definedValue.unitType);
    }
    else {
        AddLogMsg(ErrorLevel::Error, "Property must be an instance of entity IfcPropertyTableValue", pset, prop);
    }
}

void Checker::CheckListValue(SdaiInstance pset, SdaiInstance prop, SdaiAttr attr, PropertyDef::ListValue& def)
{
    SdaiAggr values = NULL;
    sdaiGetAttr(prop, attr, sdaiAGGR, &values);
    auto N = sdaiGetMemberCount(values);
    if (N > 0) {
        for (int_t i = 0; i < N; i++) {
            SdaiADB adbVal = NULL;
            sdaiGetAggrByIndex(values, i, sdaiADB, &adbVal);

            if (!def.dataType.empty()) {
                auto adbTypePath = sdaiGetADBTypePath(adbVal, 0);
                if (_stricmp(def.dataType.c_str(), adbTypePath)) {
                    const char* attrName = NULL;
                    engiGetAttributeTraits(attr, &attrName, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                    AddLogMsg(ErrorLevel::Error, Format("%s must be a list of %s", attrName, def.dataType.c_str()).c_str(), pset, prop);
                    break;
                }
            }

            if (!def.sitems.empty()) {
                bool match = false;

                //try as numbers
                if (!def.ditems.empty()) {
                    double dval = 0;
                    if (sdaiGetADBValue(adbVal, sdaiREAL, &dval)) {
                        if (def.ditems.find(dval) != def.ditems.end()) {
                            match = true;
                        }
                    }
                }

                //try as string
                if (!match) {
                    const char* strval = NULL;
                    sdaiGetADBValue(adbVal, sdaiSTRING, &strval);
                    if (!strval || def.sitems.find(strval) != def.sitems.end()) {
                        match = true;
                    }
                }

                //message
                if (!match){
                    std::string list;
                    for (auto s : def.sitems) {
                        if (!list.empty()) {
                            list += ", ";
                        }
                        list += s;
                    }
                    const char* attrName = NULL;
                    engiGetAttributeTraits(attr, &attrName, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                    AddLogMsg(ErrorLevel::Error, Format("%s must be a list [1:*] of elemenets from the set {%s}", attrName, list.c_str()).c_str(), pset, prop);
                    break;
                }
            }
        }

    }
    else {
        const char* attrName = NULL;
        engiGetAttributeTraits(attr, &attrName, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        AddLogMsg(ErrorLevel::Error, Format("%s must be a list [1:*] of IfcValue", attrName).c_str(), pset, prop);
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckPropertyReferenceValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& /*pdef*/)
{ 
    AddLogMsg(ErrorLevel::Error, "Property entity is not implemented, TODO", pset, prop);
    assert(!"not implemented");
}

/// <summary>
/// 
/// </summary>
void Checker::CheckPropertyListValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef) 
{ 
    if (sdaiIsKindOf(prop, m_entityPropertyListValue)) {
        CheckListValue(pset, prop, m_attrPropertyListValue_ListValues, pdef.m_listValue);
        CheckUnitType(pset, prop, m_attrPropertyListValue_Unit, pdef.m_listValue.unitType);
    }
    else {
        AddLogMsg(ErrorLevel::Error, "Property must be an instance of entity IfcPropertyListValue", pset, prop);
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckPropertyComplexValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& /*pdef*/)
{
    AddLogMsg(ErrorLevel::Error, "Property entity is not implemented, TODO", pset, prop);
    assert(!"not implemented");
}


/// <summary>
/// 
/// </summary>
void Checker::CheckElementQuantity(SdaiInstance qset, PropertySetDef& psd)
{
    assert(sdaiIsKindOf(qset, m_entityElementQuantity));

    SdaiAggr quants = NULL;
    sdaiGetAttr(qset, m_attrElementQuantity_Quantities, sdaiAGGR, &quants);

    SdaiInstance qto = NULL;
    int_t i = 0;
    while (sdaiGetAggrByIndex(quants, i++, sdaiINSTANCE, &qto)) {
        CheckQuantity(qset, qto, psd);
    }
}

/// <summary>
/// 
/// </summary>
void Checker::CheckQuantity(SdaiInstance qset, SdaiInstance qto, PropertySetDef& psd)
{
    AddLogMsg(ErrorLevel::DebugTrace, __FUNCTION__, qset, qto);

    assert(sdaiIsKindOf(qto, m_entityPhysicalQuantity));

    const char* qtoName = NULL;
    sdaiGetAttr(qto, m_attrPhysicalQuantity_Name, sdaiSTRING, &qtoName);

    if (auto pdef = psd.GetPropertyDef(qtoName)) {
        if (pdef->m_typeProperty == PropertyDef::TypeProperty::Quantity) {
            if (!sdaiIsKindOfBN(qto, pdef->m_qtoEntity.c_str())) {
                AddLogMsg(ErrorLevel::Error, Format("The quantity must be an instance of %s", pdef->m_qtoEntity.c_str()).c_str(), qset, qto);
            }
        }
        else {
            AddLogMsg(ErrorLevel::Error, "The property must not be a quantity", qset, qto);
        }
    }
    else {
        AddLogMsg(ErrorLevel::Warning, "PSD does not contain quantity", qset, qto);
    }

}

/// <summary>
/// 
/// </summary>
PropertySetDef* Checker::GetPSD(const char* psetName)
{
    auto& psds = m_PSDs[m_ifcVersion];

    auto& psd = psds[psetName];
    if (psd.GetLoadState() == PropertySetDef::LoadState::Not) {
        LoadPSD(psetName, psd);
    }
    return (psd.GetLoadState() == PropertySetDef::LoadState::OK) ? &psd : NULL;
}

/// <summary>
/// 
/// </summary>
static void AddPath(std::string& path, const char* name)
{
    if (path.back() != '\\' && path.back() != '/') {
        path.push_back('\\');
    }

    path.append(name);
}

/// <summary>
/// 
/// </summary>
void Checker::LoadPSD(const char* psetName, PropertySetDef& psd)
{
    std::string psdFilePath = m_psdFolder;

    switch (m_ifcVersion) {
        case IfcVersion::Ifc2x3:
            AddPath(psdFilePath, "IFC2x3");
            break;
        case IfcVersion::Ifc4:
            AddPath(psdFilePath, "IFC4");
            break;
        case IfcVersion::Ifc4x3:
            AddPath(psdFilePath, "IFC4x3");
            break;
    }

    AddPath(psdFilePath, psetName);

    psdFilePath.append(".xml");

    try {
        psd.Load(psdFilePath.c_str(), 0 == _strnicmp(psetName, "pset_", 4));
    }
    catch (exception& ex) {
        std::string msg = "Failed read IDS file (";
        msg += ex.what();
        msg += "): ";
        msg += psdFilePath;
        AddLogMsg(ErrorLevel::Error, msg.c_str());
    }
}

/// <summary>
/// 
/// </summary>
void PropertySetDef::Load(const char* psdFilePath, bool standard)
{
    m_loadState = PropertySetDef::LoadState::Tried;
    
    if (0 == _access(psdFilePath, 0)) {

        _xml::_document doc(nullptr);
        doc.load(psdFilePath);

        if (auto root = doc.getRoot()) {
            Load(*root);
            m_loadState = PropertySetDef::LoadState::OK;
        }
        else {
            throw exception("invalid xml: no root element");
        }
    }
    else if (standard) {
        throw exception("Possible Pset_ prefix is used for not standard property set");
    }
}

/// <summary>
/// 
/// </summary>
void PropertySetDef::Load(_xml::_element& root)
{
    for (auto section : root.children()) {
        auto& sectionName = section->getName();
        if (sectionName == "ApplicableClasses") {
            for (auto className : section->children()) {
                if (className->getName() == "ClassName") {
                    AddApplicableClass(*className);
                }
                else assert(false);
            }
        }
        else if (sectionName == "PropertyDefs") {
            for (auto propertyDef : section->children()) {
                if (propertyDef->getName() == "PropertyDef") {
                    m_defs.LoadPropertyDef(*propertyDef);
                }
                else assert(false);
            }
        }
        else if (sectionName == "QtoDefs") {
            for (auto qtoDef : section->children()) {
                if (qtoDef->getName() == "QtoDef") {
                    m_defs.LoadPropertyDef(*qtoDef);
                }
                else assert(false);
            }
        }
    }
}

/// <summary>
/// 
/// </summary>
void PropertySetDef::AddApplicableClass(_xml::_element& className)
{
    auto& name = className.getContent();
    m_applicableClassNames.push_back(name);
    m_model = NULL;
}

/// <summary>
/// 
/// </summary>
PropertyDef* PropertySetDef::GetPropertyDef(const char* name)
{
    auto it = m_defs.find(name);
    if (it != m_defs.end()) {
        return &(it->second);
    }
    else {
        return NULL;
    }
}

 /// <summary>
 /// 
 /// </summary>
bool PropertySetDef::IsClassApplicable(SdaiInstance inst)
{
    auto model = sdaiGetInstanceModel(inst);

    if (model != m_model) {
        m_applicableEntities.clear();

        for (auto className : m_applicableClassNames) {
            auto pos = className.find_last_of('/');
            if (pos != std::string::npos) {
                className = className.substr(pos+1);
            }
            auto entity = sdaiGetEntity(model, className.c_str());
            assert(entity);
            m_applicableEntities.insert(entity);
        }

        m_model = model;
    }

    for (auto entity : m_applicableEntities) {
        if (sdaiIsKindOf(inst, entity)) {
            return true;
        }
    }
    return false;
}

/// <summary>
/// 
/// </summary>
void PropertyDefs::LoadPropertyDef(_xml::_element& propertyDef)
{
    PropertyDef* def = NULL;

    for (auto elem : propertyDef.children()) {
        auto& elemName = elem->getName();
        if (elemName == "Name") {
            assert(!def);
            auto& propName = elem->getContent();
            def = &((*this)[propName]);
        }
        else if (elemName == "PropertyType") {
            assert(def); //expect Name element before
            if (def) {
                def->LoadType (*elem);
            }
        }
        else if (elemName == "QtoType") {
            assert(def); //expect Name element before
            if (def) {
                def->LoadQtoType(*elem);
            }
        }
    }
}

/// <summary>
/// 
/// </summary>
void PropertyDef::LoadType(_xml::_element& propertyType)
{
    for (auto typeProperty : propertyType.children()) {
        auto& typeName = typeProperty->getName();
        if (typeName == "TypePropertySingleValue") {
            m_typeProperty = TypeProperty::SingleValue;
            LoadTypeSingleOrBoundedValue(*typeProperty);
        }
        else if (typeName == "TypePropertyBoundedValue") {
            m_typeProperty = TypeProperty::BoundedValue;
            LoadTypeSingleOrBoundedValue(*typeProperty);
        }
        else if (typeName == "TypePropertyEnumeratedValue") {
            m_typeProperty = TypeProperty::EnumeratedValue;
            LoadTypeEnumeratedValue(*typeProperty);
        }
        else if (typeName == "TypePropertyTableValue") {
            m_typeProperty = TypeProperty::TableValue;
            LoadTypeTableValue(*typeProperty);
        }
        else if (typeName == "TypePropertyReferenceValue") {
            m_typeProperty = TypeProperty::ReferenceValue;
            LoadTypeReferenceValue(*typeProperty);
        }
        else if (typeName == "TypePropertyListValue") {
            m_typeProperty = TypeProperty::ListValue;
            LoadTypeListValue(*typeProperty);
        }
        else if (typeName == "TypeComplexProperty") {
            m_typeProperty = TypeProperty::ComplexProperty;
            LoadTypeComplexProperty(*typeProperty);
        }
        else {
            assert(false); //unsupported typeProperty
        }
    }
}

/// <summary>
/// 
/// </summary>
void PropertyDef::LoadQtoType(_xml::_element& qtoType)
{
    m_typeProperty = TypeProperty::Quantity;

    auto& type = qtoType.getContent();

    if (type == "Q_LENGTH") {
        m_qtoEntity = "IfcQuantityLength";
    }
    else if (type == "Q_AREA") {
        m_qtoEntity = "IfcQuantityArea";
    }
    else if (type == "Q_VOLUME") {
        m_qtoEntity = "IfcQuantityVolume";
    }
    else if (type == "Q_COUNT") {
        m_qtoEntity = "IfcQuantityCount";
    }
    else if (type == "Q_WEIGHT") {
        m_qtoEntity = "IfcQuantityWeight";
    }
    else if (type == "Q_TIME") {
        m_qtoEntity = "IfcQuantityTime";
    }
    else {
        assert(!"not implemented");
    }
}

/// <summary>
/// 
/// </summary>
void PropertyDef::GetAttrValue(std::string& val, _xml::_element& elem, const char* attrName, bool required)
{
    if (auto attr = elem.getAttribute(attrName)) {
        val = attr->getValue();
    }
    else {
        if (required) {
            assert(false);
        }
        val.clear();
    }
}

/// <summary>
/// 
/// </summary>
void PropertyDef::LoadTypeSingleOrBoundedValue(_xml::_element& typeProperty)
{
    for (auto elem : typeProperty.children()) {
        auto& elemName = elem->getName();
        if (elemName == "DataType") {
            GetAttrValue(m_dataType, *elem, "type", false);
        }
        else if (elemName == "UnitType") {
            GetAttrValue(m_unitType, *elem, "type");
        }
        else if (elemName == "ValueRangeDef") {
            LoadValueRangeDef(*elem);
        }
        else {
            assert(false);
        }
    }
}

/// <summary>
/// 
/// </summary>
void PropertyDef::LoadValueRangeDef(_xml::_element& range)
{
    for (auto elem : range.children()) {
        auto& elemName = elem->getName();
        if (elemName == "LowerBoundValue") {
            GetAttrValue(m_lowerBoundValue, *elem, "value");
        }
        else if (elemName == "UpperBoundValue") {
            GetAttrValue(m_upperBoundValue, *elem, "value");
        }
        else {
            assert(false);
        }
    }
}

/// <summary>
/// 
/// </summary>
void PropertyDef::LoadTypeEnumeratedValue(_xml::_element& typeProperty)
{
    for (auto list : typeProperty.children()) {
        auto& listName = list->getName();
        if (listName == "EnumList") {

            GetAttrValue(m_enumListName, *list, "name", false);
            
            for (auto enumItem : list->children()) {
                auto& enumItemName = enumItem->getName();
                if (enumItemName == "EnumItem") {
                    auto& item = enumItem->getContent();
                    m_enumListItems.insert(item);
                }
                else assert(false);
            }
        }
        else if (listName == "ConstantList") {
            for (auto constDef : list->children()) {
                if (constDef->getName() == "ConstantDef") {
                    for (auto elem : constDef->children()) {
                        if (elem->getName() == "Name") {
                            auto& item = elem->getContent();
                            m_enumListItems.insert(item);
                        }
                    }
                }
                else assert(false);
            }
        }
        else assert(false);
    }
}

/// <summary>
/// 
/// </summary>
void PropertyDef::LoadTypeReferenceValue(_xml::_element& typeProperty)
{
    GetAttrValue(m_reftype, typeProperty, "reftype", false);
    GetAttrValue(m_guid, typeProperty, "guid", false);
    GetAttrValue(m_URL, typeProperty, "URL", false);
    GetAttrValue(m_libraryname, typeProperty, "libraryname", false);
    GetAttrValue(m_sectionref, typeProperty, "sectionref", false);
}

/// <summary>
/// 
/// </summary>
void PropertyDef::LoadTypeTableValue(_xml::_element& typeProperty)
{
    for (auto child : typeProperty.children()) {
        auto& childName = child->getName();
        if (childName == "Expression") {
            m_Expression = child->getContent();
        }
        else if (childName == "DefiningValue") {
            LoadListValue(*child, m_definingValue);
        }
        else if (childName == "DefinedValue") {
            LoadListValue(*child, m_definedValue);
        }
        else {
            assert(false);
        }
    }
}


/// <summary>
/// 
/// </summary>
void PropertyDef::LoadListValue(_xml::_element& elemValue, ListValue& xxxValue)
{
    for (auto child : elemValue.children()) {
        auto& childName = child->getName();
        if (childName == "DataType") {
            GetAttrValue (xxxValue.dataType, *child, "type");
        }
        else if (childName == "UnitType") {
            GetAttrValue(xxxValue.unitType, *child, "type");
        }
        else if (childName == "Values") {
            bool numbers = true;

            for (auto item : child->children()) {
                auto& itemName = item->getName();
                if (itemName == "ValueItem") {
                    auto& sitem = item->getContent();
                    xxxValue.sitems.insert(sitem);
                    
                    if (numbers) {
                        double dval = 0;
                        numbers = StoD(sitem, dval);

                        if (numbers){
                            xxxValue.ditems.insert(dval);
                        }
                        else {
                            numbers = false;
                            xxxValue.ditems.clear();
                        }
                    }
                }
                else {
                    assert(false);
                }
            }
        }
        else {
            assert(false);
        }
    }
}

/// <summary>
/// 
/// </summary>
void PropertyDef::LoadTypeListValue(_xml::_element& typeProperty)
{
    for (auto child : typeProperty.children()) {
        auto& childName = child->getName();
        if (childName == "ListValue") {
            LoadListValue(*child, m_listValue);
        }
        else assert(false);
    }
}

/// <summary>
/// 
/// </summary>
void PropertyDef::LoadTypeComplexProperty(_xml::_element& typeProperty)
{
    GetAttrValue(m_complexName, typeProperty, "name");

    for (auto part : typeProperty.children()) {
        if (part->getName() == "PropertyDef") {
            m_complexParts.LoadPropertyDef(*part);
        }
        else {
            assert(false);
        }
    }
}
