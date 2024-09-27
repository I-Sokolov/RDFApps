//
//  Author:  Peter Bonsma
//  $Date: 1999-12-31 23:59:59 +0000 (Wed, 31 Jan 1999) $
//  $Revision: 3999 $
//  Project: Engine Library
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@rdf.bg
//

#include "pch.h"

#include "IDS.h"
using namespace RDF::IDS;


/// <summary>
/// 
/// </summary>
#define GET_ATTR(name)                      \
    for (auto attr : elem.attributes()) {   \
    if (attr) {                             \
        auto attrName = attr->getName();    \
        if (attrName == #name) {            \
            m_##name = attr->getValue();    \
        }                                   

#define NEXT_ATTR(name)                     \
            else if (attrName == #name) {   \
            m_##name = attr->getValue();    \
        }

#define END_ATTR        \
            else { ctx.LogMsg(MsgLevel::Warning, "Unknown attribute '%s'", attrName.c_str()); } } }


/// <summary>
/// 
/// </summary>
#define GET_CHILD(name)                     \
    for (auto child : elem.children()) {    \
        if (child) {                        \
            auto&  tag= child->getName();   \
            if (tag == #name) {             \
                Read_##name(*child, ctx);   \
            }

#define NEXT_CHILD(name)                    \
            else if (tag == #name) {        \
                Read_##name(*child, ctx);   \
            }

#define GET_CHILD_MEMBER(name)              \
    for (auto child : elem.children()) {    \
        if (child) {                        \
            auto&  tag= child->getName();   \
            if (tag == #name) {             \
                m_##name.Read(*child, ctx); \
            }

#define NEXT_CHILD_MEMBER(name)             \
            else if (tag == #name) {        \
                m_##name.Read(*child, ctx); \
            }

#define END_CHILDREN \
            else { ctx.LogMsg(MsgLevel::Warning, "Unknown child element <%s>", tag.c_str()); } } }


/// <summary>
/// 
/// </summary>
#define ENTITY(EntityName)                      \
    private:                                    \
        SdaiEntity m_entity##EntityName = 0;    \
    public:                                     \
        SdaiEntity _##EntityName()              \
        {                                       \
            if (!m_entity##EntityName) {        \
                m_entity##EntityName = sdaiGetEntity(model, #EntityName);  \
                assert(m_entity##EntityName);   \
            }                                   \
            return m_entity##EntityName;        \
        }                                       

/// <summary>
/// 
/// </summary>
#define ATTR(Ent,Attr,EntAttr)                                          \
    private:                                                            \
        SdaiAttr    m_attribute_##EntAttr = 0;                          \
    public:                                                             \
        SdaiAttr    _##EntAttr ()                                       \
        {                                                               \
            if (!m_attribute_##EntAttr){                                \
                auto ent = _##Ent();                                    \
                m_attribute_##EntAttr = sdaiGetAttrDefinition(ent, #Attr);\
                assert(m_attribute_##EntAttr);                          \
            }                                                           \
            return m_attribute_##EntAttr;                               \
        }



/// <summary>
/// 
/// </summary>
class RDF::IDS::Context
{
public:
    enum class IfcVersion { NotItitialized, Ifc2x3, Ifc4, Ifc4x3, Unsupported };

public:
    Context(Console& con_, MsgLevel msgLevel_, bool stopAtFirstError_)
        : console(con_), msgLevel(msgLevel_), stopAtFirstError(stopAtFirstError_)
    {}
    ~Context() {}

public:
    void LogMsg(MsgLevel type, const char* format, ...);

    IfcVersion GetIfcVersion(const char** pstr = nullptr);

    double GetUnitScale(SdaiInstance unit, const char* unitKind);

    double Precision() { return 1e-6; }

public:
    Console& console;
    MsgLevel    msgLevel;
    bool        stopAtFirstError;

    SdaiModel       model = 0;
    SdaiInstance    currentInstane = 0;
    Specification* currentSpecification = nullptr;

private:
    IfcVersion m_ifcVersion = IfcVersion::NotItitialized;
    std::string m_ifcVersionStr;

    ENTITY(IfcRoot);
    ENTITY(IfcObjectDefinition);
    ENTITY(IfcRelAssociatesClassification);
    ENTITY(IfcRelAssociatesMaterial);
    ENTITY(IfcClassificationReference);
    ENTITY(IfcExternalReference);
    ENTITY(IfcExternalReferenceRelationship);
    ENTITY(IfcClassification);
    ENTITY(IfcClassificationNotation);
    ENTITY(IfcRelAssignsToGroup);
    ENTITY(IfcFeatureElementSubtraction);
    ENTITY(IfcRelVoidsElement);
    ENTITY(IfcElement);
    ENTITY(IfcRelFillsElement);
    ENTITY(IfcRelContainedInSpatialStructure);
    ENTITY(IfcRelAggregates);
    ENTITY(IfcRelNests);
    ENTITY(IfcObject);
    ENTITY(IfcTypeObject);
    ENTITY(IfcContext);

    ENTITY(IfcRelDefinesByProperties);
    ENTITY(IfcPropertySet);
    ENTITY(IfcElementQuantity);

    ENTITY(IfcRelDefinesByType);

    ENTITY(IfcProperty);
    ENTITY(IfcComplexProperty);
    ENTITY(IfcPropertyBoundedValue);
    ENTITY(IfcPropertyEnumeratedValue);
    ENTITY(IfcPropertyListValue);
    ENTITY(IfcPropertyReferenceValue);
    ENTITY(IfcPropertySingleValue);
    ENTITY(IfcPropertyTableValue);
    ENTITY(IfcPropertyEnumeration);

    ENTITY(IfcPhysicalQuantity);
    ENTITY(IfcPhysicalComplexQuantity);
    ENTITY(IfcPhysicalSimpleQuantity);
    ENTITY(IfcQuantityArea);
    ENTITY(IfcQuantityCount);
    ENTITY(IfcQuantityLength);
    ENTITY(IfcQuantityNumber);
    ENTITY(IfcQuantityTime);
    ENTITY(IfcQuantityVolume);
    ENTITY(IfcQuantityWeight);

    ENTITY(IfcDerivedUnit);
    ENTITY(IfcMonetaryUnit);
    ENTITY(IfcNamedUnit);
    ENTITY(IfcContextDependentUnit);
    ENTITY(IfcConversionBasedUnit);
    ENTITY(IfcSIUnit);
    ENTITY(IfcDerivedUnitElement);

    ENTITY(IfcMaterial);
    ENTITY(IfcMaterialLayer);
    ENTITY(IfcMaterialLayerSet);
    ENTITY(IfcMaterialProfile);
    ENTITY(IfcMaterialProfileSet);
    ENTITY(IfcMaterialConstituent);
    ENTITY(IfcMaterialConstituentSet);
    ENTITY(IfcMaterialLayerSetUsage);
    ENTITY(IfcMaterialProfileSetUsage);
    ENTITY(IfcMaterialList);


    ATTR(IfcRoot, Name, IfcRoot_Name);
    ATTR(IfcObjectDefinition, Decomposes, IfcObjectDefinition_Decomposes);
    ATTR(IfcObjectDefinition, Nests, IfcObjectDefinition_Nests);
    ATTR(IfcObjectDefinition, HasAssignments, IfcObjectDefinition_HasAssignments);
    ATTR(IfcObjectDefinition, HasAssociations, IfcObjectDefinition_HasAssociations);
    ATTR(IfcRelAssociatesClassification, RelatingClassification, IfcRelAssociatesClassification_RelatingClassification);
    ATTR(IfcRelAssociatesMaterial, RelatingMaterial, IfcRelAssociatesMaterial_RelatingMaterial);
    ATTR(IfcRelAggregates, RelatingObject, IfcRelAggregates_RelatingObject);
    ATTR(IfcRelNests, RelatingObject, IfcRelNests_RelatingObject);
    ATTR(IfcRelAssignsToGroup, RelatingGroup, IfcRelAssignsToGroup_RelatingGroup);
    ATTR(IfcFeatureElementSubtraction, VoidsElements, IfcFeatureElementSubtraction_VoidsElements);
    ATTR(IfcRelVoidsElement, RelatingBuildingElement, IfcRelVoidsElement_RelatingBuildingElement);
    ATTR(IfcElement, FillsVoids, IfcElement_FillsVoids);
    ATTR(IfcRelFillsElement, RelatingOpeningElement, IfcRelFillsElement_RelatingOpeningElement);
    ATTR(IfcRelContainedInSpatialStructure, RelatingStructure, IfcRelContainedInSpatialStructure_RelatingStructure);
    ATTR(IfcRelContainedInSpatialStructure, RelatedElements, IfcRelContainedInSpatialStructure_RelatedElements);
    ATTR(IfcExternalReferenceRelationship, RelatingReference, IfcExternalReferenceRelationship_RelatingReference);
    ATTR(IfcExternalReference, Identification, IfcExternalReference_Identification);
    ATTR(IfcExternalReference, ItemReference, IfcExternalReference_ItemReference);
    ATTR(IfcExternalReference, Location, IfcExternalReference_Location);
    ATTR(IfcClassificationReference, ReferencedSource, IfcClassificationReference_ReferencedSource);
    ATTR(IfcClassification, Name, IfcClassification_Name);
    ATTR(IfcClassification, Location, IfcClassification_Location);
    ATTR(IfcClassification, Specification, IfcClassification_Specification);
    ATTR(IfcClassification, Source, IfcClassification_Source);
    ATTR(IfcObject, IsDefinedBy, IfcObject_IsDefinedBy);
    ATTR(IfcObject, IsTypedBy, IfcObject_IsTypedBy);
    ATTR(IfcObject, ObjectType, IfcObject_ObjectType);
    ATTR(IfcTypeObject, HasPropertySets, IfcTypeObject_HasPropertySets);
    ATTR(IfcContext, IsDefinedBy, IfcContext_IsDefinedBy);
    ATTR(IfcRelDefinesByProperties, RelatingPropertyDefinition, IfcRelDefinesByProperties_RelatingPropertyDefinition);
    ATTR(IfcRelDefinesByType, RelatingType, IfcRelDefinesByType_RelatingType);
    ATTR(IfcPropertySet, HasProperties, IfcPropertySet_HasProperties);
    ATTR(IfcElementQuantity, Quantities, IfcElementQuantity_Quantities);
    ATTR(IfcProperty, Name, IfcProperty_Name);
    ATTR(IfcPhysicalQuantity, Name, IfcPhysicalQuantity_Name);
    ATTR(IfcComplexProperty, HasProperties, IfcComplexProperty_HasProperties);
    ATTR(IfcPhysicalSimpleQuantity, Unit, IfcPhysicalSimpleQuantity_Unit);
    ATTR(IfcQuantityArea, AreaValue, IfcQuantityArea_AreaValue);
    ATTR(IfcQuantityCount, CountValue, IfcQuantityCount_CountValue);
    ATTR(IfcQuantityLength, LengthValue, IfcQuantityLength_LengthValue);
    ATTR(IfcQuantityNumber, NumberValue, IfcQuantityNumber_NumberValue);
    ATTR(IfcQuantityTime, TimeValue, IfcQuantityTime_TimeValue);
    ATTR(IfcQuantityVolume, VolumeValue, IfcQuantityVolume_VolumeValue);
    ATTR(IfcQuantityWeight, WeightValue, IfcQuantityWeight_WeightValue);
    ATTR(IfcPropertySingleValue, NominalValue, IfcPropertySingleValue_NominalValue);
    ATTR(IfcPropertySingleValue, Unit, IfcPropertySingleValue_Unit);
    ATTR(IfcPropertyEnumeratedValue, EnumerationValues, IfcPropertyEnumeratedValue_EnumerationValues);
    ATTR(IfcPropertyEnumeratedValue, EnumerationReference, IfcPropertyEnumeratedValue_EnumerationReference);
    ATTR(IfcPropertyEnumeration, Unit, IfcPropertyEnumeration_Unit);
    ATTR(IfcPropertyBoundedValue, Unit, IfcPropertyBoundedValue_Unit);
    ATTR(IfcPropertyBoundedValue, UpperBoundValue, IfcPropertyBoundedValue_UpperBoundValue);
    ATTR(IfcPropertyBoundedValue, LowerBoundValue, IfcPropertyBoundedValue_LowerBoundValue);
    ATTR(IfcPropertyBoundedValue, SetPointValue, IfcPropertyBoundedValue_SetPointValue);
    ATTR(IfcPropertyListValue, Unit, IfcPropertyListValue_Unit);
    ATTR(IfcPropertyListValue, ListValues, IfcPropertyListValue_ListValues);
    ATTR(IfcPropertyTableValue, DefiningUnit, IfcPropertyTableValue_DefiningUnit);
    ATTR(IfcPropertyTableValue, DefiningValues, IfcPropertyTableValue_DefiningValues);
    ATTR(IfcPropertyTableValue, DefinedUnit, IfcPropertyTableValue_DefinedUnit);
    ATTR(IfcPropertyTableValue, DefinedValues, IfcPropertyTableValue_DefinedValues);
    ATTR(IfcDerivedUnit, Elements, IfcDerivedUnit_Elements);
    ATTR(IfcDerivedUnitElement, Unit, IfcDerivedUnitElement_Unit);
    ATTR(IfcDerivedUnitElement, Exponent, IfcDerivedUnitElement_Exponent);

    ATTR(IfcMaterial, Name, IfcMaterial_Name);
    ATTR(IfcMaterial, Category, IfcMaterial_Category);
    ATTR(IfcMaterialLayer, Name, IfcMaterialLayer_Name);
    ATTR(IfcMaterialLayer, Category, IfcMaterialLayer_Category);
    ATTR(IfcMaterialLayer, Material, IfcMaterialLayer_Material);
    ATTR(IfcMaterialLayerSet, LayerSetName, IfcMaterialLayerSet_LayerSetName);
    ATTR(IfcMaterialLayerSet, MaterialLayers, IfcMaterialLayerSet_MaterialLayers);
    ATTR(IfcMaterialLayerSetUsage, ForLayerSet, IfcMaterialLayerSetUsage_ForLayerSet);
    ATTR(IfcMaterialProfile, Name, IfcMaterialProfile_Name);
    ATTR(IfcMaterialProfile, Category, IfcMaterialProfile_Category);
    ATTR(IfcMaterialProfile, Material, IfcMaterialProfile_Material);
    ATTR(IfcMaterialProfileSet, Name, IfcMaterialProfileSet_Name);
    ATTR(IfcMaterialProfileSet, MaterialProfiles, IfcMaterialProfileSet_MaterialProfiles);
    ATTR(IfcMaterialProfileSetUsage, ForProfileSet, IfcMaterialProfileSetUsage_ForProfileSet);
    ATTR(IfcMaterialConstituent, Name, IfcMaterialConstituent_Name);
    ATTR(IfcMaterialConstituent, Category, IfcMaterialConstituent_Category);
    ATTR(IfcMaterialConstituent, Material, IfcMaterialConstituent_Material);
    ATTR(IfcMaterialConstituentSet, Name, IfcMaterialConstituentSet_Name);
    ATTR(IfcMaterialConstituentSet, MaterialConstituents, IfcMaterialConstituentSet_MaterialConstituents);
    ATTR(IfcMaterialList, Materials, IfcMaterialList_Materials);
};


/// <summary>
/// 
/// </summary>
static int StrICmp(const char* s1, const char* s2)
{
    return _stricmp(s1, s2);
}

/// <summary>
/// 
/// </summary>
static void ToUpper(std::string& str)
{
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = (char)toupper (str[i]);
    }
}

/// <summary>
/// 
/// </summary>
static bool IsAttrSuitable(SdaiAttr attr, SdaiInstance inst)
{
    SdaiEntity entity = 0;
    engiGetAttributeTraits(attr, (char**)0, &entity, 0, 0, 0, 0, 0);
    return sdaiIsKindOf(inst, entity);
}

/// <summary>
/// 
/// </summary>
enum class Cardinality { Required, Optional, Prohibited };
static Cardinality GetCardinality(
    const char* cardinality,
    MultiTypeValueCache& minOccur,
    MultiTypeValueCache& maxOccur,
    Cardinality def,
    Context& ctx
)
{
    if (cardinality && *cardinality) {
        if (!strcmp(cardinality, "required")) {
            return Cardinality::Required;
        }
        else if (!strcmp(cardinality, "prohibited")) {
            return Cardinality::Prohibited;
        }
        else if (!strcmp(cardinality, "optional")) {
            return Cardinality::Optional;
        }
        else {
            ctx.LogMsg (MsgLevel::Error, "Invalid IDS file, unallowed cardinality attribute value '%s'", cardinality);
            return Cardinality::Required;
        }
    }

    const char* min__ = nullptr;
    minOccur.Get(&min__);

    if (!min__ || !*min__) {
        return def;
    }

    SdaiInteger min_ = 0;
    minOccur.Get(&min_);
    if (min_ > 0) {
        return Cardinality::Required;
    }

    SdaiInteger max_ = MAXINT;
    const char* max__ = nullptr;
    maxOccur.Get(&max__);
    if (*max__) {
        maxOccur.Get(&max_);
    }

    if (max_ == 0)
        return Cardinality::Prohibited;

    return Cardinality::Optional;
}


/// <summary>
/// 
/// </summary>
static std::map<std::string, std::string> s_ifcDataTypesUnits;

static bool InitIfcDataTypesUnits()
{
#define IGNORE_1ST(a,b) b
    s_ifcDataTypesUnits["IFCAMOUNTOFSUBSTANCEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "AMOUNTOFSUBSTANCEUNIT");
    s_ifcDataTypesUnits["IFCAREADENSITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "AREADENSITYUNIT");
    s_ifcDataTypesUnits["IFCAREAMEASURE"] = IGNORE_1ST("IFCUNITENUM", "AREAUNIT");
    s_ifcDataTypesUnits["IFCDYNAMICVISCOSITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "DYNAMICVISCOSITYUNIT");
    s_ifcDataTypesUnits["IFCELECTRICCAPACITANCEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "ELECTRICCAPACITANCEUNIT");
    s_ifcDataTypesUnits["IFCELECTRICCHARGEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "ELECTRICCHARGEUNIT");
    s_ifcDataTypesUnits["IFCELECTRICCONDUCTANCEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "ELECTRICCONDUCTANCEUNIT");
    s_ifcDataTypesUnits["IFCELECTRICCURRENTMEASURE"] = IGNORE_1ST("IFCUNITENUM", "ELECTRICCURRENTUNIT");
    s_ifcDataTypesUnits["IFCELECTRICRESISTANCEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "ELECTRICRESISTANCEUNIT");
    s_ifcDataTypesUnits["IFCELECTRICVOLTAGEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "ELECTRICVOLTAGEUNIT");
    s_ifcDataTypesUnits["IFCENERGYMEASURE"] = IGNORE_1ST("IFCUNITENUM", "ENERGYUNIT");
    s_ifcDataTypesUnits["IFCFORCEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "FORCEUNIT");
    s_ifcDataTypesUnits["IFCFREQUENCYMEASURE"] = IGNORE_1ST("IFCUNITENUM", "FREQUENCYUNIT");
    s_ifcDataTypesUnits["IFCHEATFLUXDENSITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "HEATFLUXDENSITYUNIT");
    s_ifcDataTypesUnits["IFCHEATINGVALUEMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "HEATINGVALUEUNIT");
    s_ifcDataTypesUnits["IFCILLUMINANCEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "ILLUMINANCEUNIT");
    s_ifcDataTypesUnits["IFCIONCONCENTRATIONMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "IONCONCENTRATIONUNIT");
    s_ifcDataTypesUnits["IFCISOTHERMALMOISTURECAPACITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "ISOTHERMALMOISTURECAPACITYUNIT");
    s_ifcDataTypesUnits["IFCLENGTHMEASURE"] = IGNORE_1ST("IFCUNITENUM", "LENGTHUNIT");
    s_ifcDataTypesUnits["IFCLINEARVELOCITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "LINEARVELOCITYUNIT");
    s_ifcDataTypesUnits["IFCLUMINOUSFLUXMEASURE"] = IGNORE_1ST("IFCUNITENUM", "LUMINOUSFLUXUNIT");
    s_ifcDataTypesUnits["IFCLUMINOUSINTENSITYMEASURE"] = IGNORE_1ST("IFCUNITENUM", "LUMINOUSINTENSITYUNIT");
    s_ifcDataTypesUnits["IFCMASSDENSITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "MASSDENSITYUNIT");
    s_ifcDataTypesUnits["IFCMASSFLOWRATEMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "MASSFLOWRATEUNIT");
    s_ifcDataTypesUnits["IFCMASSMEASURE"] = IGNORE_1ST("IFCUNITENUM", "MASSUNIT");
    s_ifcDataTypesUnits["IFCMASSPERLENGTHMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "MASSPERLENGTHUNIT");
    s_ifcDataTypesUnits["IFCMODULUSOFELASTICITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "MODULUSOFELASTICITYUNIT");
    s_ifcDataTypesUnits["IFCMOISTUREDIFFUSIVITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "MOISTUREDIFFUSIVITYUNIT");
    s_ifcDataTypesUnits["IFCMOLECULARWEIGHTMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "MOLECULARWEIGHTUNIT");
    s_ifcDataTypesUnits["IFCMOMENTOFINERTIAMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "MOMENTOFINERTIAUNIT");
    s_ifcDataTypesUnits["IFCPHMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "PHUNIT");
    s_ifcDataTypesUnits["IFCPLANARFORCEMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "PLANARFORCEUNIT");
    s_ifcDataTypesUnits["IFCPLANEANGLEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "PLANEANGLEUNIT");
    s_ifcDataTypesUnits["IFCPOWERMEASURE"] = IGNORE_1ST("IFCUNITENUM", "POWERUNIT");
    s_ifcDataTypesUnits["IFCPRESSUREMEASURE"] = IGNORE_1ST("IFCUNITENUM", "PRESSUREUNIT");
    s_ifcDataTypesUnits["IFCRADIOACTIVITYMEASURE"] = IGNORE_1ST("IFCUNITENUM", "RADIOACTIVITYUNIT");
    s_ifcDataTypesUnits["IFCROTATIONALFREQUENCYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "ROTATIONALFREQUENCYUNIT");
    s_ifcDataTypesUnits["IFCSECTIONMODULUSMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "SECTIONMODULUSUNIT");
    s_ifcDataTypesUnits["IFCSOUNDPOWERMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "SOUNDPOWERUNIT");
    s_ifcDataTypesUnits["IFCSOUNDPRESSUREMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "SOUNDPRESSUREUNIT");
    s_ifcDataTypesUnits["IFCSPECIFICHEATCAPACITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "SPECIFICHEATCAPACITYUNIT");
    s_ifcDataTypesUnits["IFCTEMPERATURERATEOFCHANGEMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "TEMPERATURERATEOFCHANGEUNIT");
    s_ifcDataTypesUnits["IFCTHERMALCONDUCTIVITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "THERMALCONDUCTANCEUNIT");
    s_ifcDataTypesUnits["IFCTHERMODYNAMICTEMPERATUREMEASURE"] = IGNORE_1ST("IFCUNITENUM", "THERMODYNAMICTEMPERATUREUNIT");
    s_ifcDataTypesUnits["IFCTIMEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "TIMEUNIT");
    s_ifcDataTypesUnits["IFCTORQUEMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "TORQUEUNIT");
    s_ifcDataTypesUnits["IFCVAPORPERMEABILITYMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "VAPORPERMEABILITYUNIT");
    s_ifcDataTypesUnits["IFCVOLUMEMEASURE"] = IGNORE_1ST("IFCUNITENUM", "VOLUMEUNIT");
    s_ifcDataTypesUnits["IFCVOLUMETRICFLOWRATEMEASURE"] = IGNORE_1ST("IFCDERIVEDUNITENUM", "VOLUMETRICFLOWRATEUNIT");

    return true;
}
static bool s_InitIfcDataTypesUnits = InitIfcDataTypesUnits();


/// <summary>
/// 
/// </summary>
double Context::GetUnitScale(SdaiInstance unit, const char* unitKind)
{
    double scale = 1;
    if (unit) {
        scale = getUnitInstanceConversionFactor(unit, (char**)nullptr, nullptr, nullptr);
    }
    else if (unitKind) {
        scale = getProjectUnitConversionFactor(model, unitKind, nullptr, nullptr, nullptr);
    }

    if (scale < FLT_MIN) {
        scale = 1;
    }

    return scale;
}


/// <summary>
/// 
/// </summary>
Context::IfcVersion Context::GetIfcVersion(const char** pstr)
{
    if (m_ifcVersion == IfcVersion::NotItitialized) {
        assert(model);
        
        const char* schemaName = nullptr;
        GetSPFFHeaderItem(model, 9, 0, sdaiSTRING, &schemaName);
        assert(schemaName);

        m_ifcVersionStr = schemaName;
        ToUpper(m_ifcVersionStr);

        if (strstr(m_ifcVersionStr.c_str(), "IFC4X3")) {
            m_ifcVersion = IfcVersion::Ifc4x3;
        }
        else if (strstr(m_ifcVersionStr.c_str(), "IFC4")) {
            m_ifcVersion = IfcVersion::Ifc4;
        }
        else if (strstr(m_ifcVersionStr.c_str(), "IFC2X3")) {
            m_ifcVersion = IfcVersion::Ifc2x3;
        }
        else {
            m_ifcVersion = IfcVersion::Unsupported;
            assert(0);
            LogMsg(MsgLevel::NotImplemented, "Unknown IFC version %s", schemaName);
        }
    }

    if (pstr) {
        *pstr = m_ifcVersionStr.c_str();
    }

    return m_ifcVersion;
}

/// <summary>
/// 
/// </summary>
static SdaiInstance GetTypeObject(SdaiInstance inst, Context& ctx)
{
    if (sdaiIsKindOf(inst, ctx._IfcObject())) {
        SdaiInstance relType = 0;
        if (sdaiGetAttr(inst, ctx._IfcObject_IsTypedBy(), sdaiINSTANCE, &relType)) {
            SdaiInstance type = 0;
            if (sdaiGetAttr(relType, ctx._IfcRelDefinesByType_RelatingType(), sdaiINSTANCE, &type)) {
                return type;
            }
        }
    }
    return NULL;
}

/// <summary>
/// 
/// </summary>
class DefaultConsole : public Console
{
    virtual void out(const char* msg) override { printf("%s", msg); }
};


/// <summary>
/// 
/// </summary>
void Context::LogMsg (MsgLevel type, const char* format, ...)
{
    if (type < msgLevel) {
        return; //>>>>>>>>>>>>>
    }

    auto& log = console;

    //
    log.out("\t<");

    const char* msgType;
    switch (type) {
        case MsgLevel::Info:     msgType = "info"; break;
        case MsgLevel::Warning:  msgType = "Warning"; break;
        default:                 msgType = "ERROR"; break;
    }

    log.out(msgType);

    if (currentInstane > 0) {
        log.out(" stepId='#");
        auto id = internalGetP21Line(currentInstane);
        char str[64];
        sprintf(str, "%lld", id);
        log.out(str);
        log.out("'");
    }

    if (currentSpecification) {
        log.out(" specification='");
        auto name = currentSpecification->DisplayName();
        log.out(name.c_str());
        log.out("'");
    }

    //
    log.out(">\r\n\t\t");

    char msg[512];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, 511, format, args);
    va_end(args);

    log.out(msg);
    log.out("\r\n");

    //
    log.out("\t</");
    log.out(msgType);
    log.out(">\r\n");
}


/// <summary>
/// 
/// </summary>
template <typename TSrc> static bool ReadTpl(File& idsFile, TSrc idsSrc)
{
    bool ok = false;

    DefaultConsole con;
    Context ctx(con, MsgLevel::All, false);

    try {
        _xml::_document doc(nullptr);
        doc.load(idsSrc);

        if (auto root = doc.getRoot()) {
            idsFile.Read(*root, ctx);
            ok = true;
        }
    }
    catch (exception& ex) {
        ctx.LogMsg(MsgLevel::Error, "Failed read IDS file: %s", ex.what());
    }

    return ok;
}

/// <summary>
/// 
/// </summary>
bool File::Read(const char* idsFilePath)
{
    return ReadTpl(*this, idsFilePath);
}

/// <summary>
/// 
/// </summary>
bool File::Read(const wchar_t* idsFilePath)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string utf8 = converter.to_bytes(idsFilePath);

    return ReadTpl(*this, utf8.c_str());
}

/// <summary>
/// 
/// </summary>
bool File::Read(std::istream* idsStream)
{
    return ReadTpl(*this, idsStream);
}


/// <summary>
/// 
/// </summary>
void File::Read(_xml::_element& elem , Context& ctx)
{
    GET_CHILD(info)
    NEXT_CHILD(specifications)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
void File::Read_info(_xml::_element& elem, Context& ctx)
{
    GET_CHILD(title)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
void File::Read_specifications(_xml::_element& elem, Context& ctx)
{
    GET_CHILD(specification)
    END_CHILDREN
}


/// <summary>
/// 
/// </summary>
Specification::Specification(_xml::_element& elem, Context& ctx)
    : m_minOccursVal (m_minOccurs)
    , m_maxOccursVal (m_maxOccurs)
{
    GET_ATTR(name)
    NEXT_ATTR(minOccurs)
    NEXT_ATTR(maxOccurs)
    NEXT_ATTR(ifcVersion)
    NEXT_ATTR(identifier)
    NEXT_ATTR(description)
    NEXT_ATTR(instructions)
    END_ATTR

    GET_CHILD_MEMBER(applicability)
    NEXT_CHILD_MEMBER(requirements)
    END_CHILDREN
}


/// <summary>
/// 
/// </summary>
void Requirements::Read(_xml::_element& elem, Context& ctx)
{
    GET_ATTR(description)
    END_ATTR

    Facets::Read (elem, ctx);
}

/// <summary>
/// 
/// </summary>
void Facets::Read(_xml::_element& elem, Context& ctx)
{
    GET_CHILD(entity)
    NEXT_CHILD(partOf)
    NEXT_CHILD(classification)
    NEXT_CHILD(attribute)
    NEXT_CHILD(property)
    NEXT_CHILD(material)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
void Facet::Read(_xml::_element& elem, Context& ctx)
{
    GET_ATTR(cardinality)
    NEXT_ATTR(minOccurs)
    NEXT_ATTR(maxOccurs)
    NEXT_ATTR(datatype)
    NEXT_ATTR(relation)
    END_ATTR
}

/// <summary>
/// 
/// </summary>
void Facet::ResetCache()
{
    ResetCacheImpl();
}

/// <summary>
/// 
/// </summary>
bool Facet::Match(SdaiInstance inst, Context& ctx)
{
    Matched match = MatchImpl(inst, ctx);

    auto rq = GetCardinality(m_cardinality.c_str(), m_minOccursVal, m_maxOccursVal, Cardinality::Required, ctx);

    switch (rq) {
        case Cardinality::Required:
            return match == Matched::Yes;
            break;

        case Cardinality::Prohibited:
            return match == Matched::No || match == Matched::NotFound;
            break;

        case Cardinality::Optional:
            return match == Matched::Yes || match == Matched::NotFound;

        default:
            return false;
    }
}

/// <summary>
/// 
/// </summary>
void FacetEntity::Read(_xml::_element& elem, Context& ctx)
{
    Facet::Read(elem, ctx);

    GET_CHILD_MEMBER(name)
    NEXT_CHILD_MEMBER(predefinedType)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetPartOf::FacetPartOf(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(entity)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetClassification::FacetClassification(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(value)
    NEXT_CHILD_MEMBER(system)
    NEXT_CHILD_MEMBER(URI)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetAttribute::FacetAttribute(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(name)
    NEXT_CHILD_MEMBER(value)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetProperty::FacetProperty(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(propertySet)
    NEXT_CHILD_MEMBER(name)
    NEXT_CHILD_MEMBER(value)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetMaterial::FacetMaterial(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(value)
    END_CHILDREN
}


/// <summary>
/// 
/// </summary>
void IdsValue::Read(_xml::_element& elem, Context& ctx)
{
    m_isSet = true;

    GET_CHILD(simpleValue)
    NEXT_CHILD(restriction)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
Value::Value(_xml::_element& elem, Context& ctx)
    : m_val (m_value)
{
    GET_ATTR(value)
    NEXT_ATTR(fixed)
    END_ATTR
}

/// <summary>
/// 
/// </summary>
Restriction::Restriction(_xml::_element& elem, Context& ctx)
{
    GET_ATTR(base)
    END_ATTR

    GET_CHILD(enumeration)
    NEXT_CHILD(pattern)
    NEXT_CHILD(minInclusive)
    NEXT_CHILD(maxInclusive)
    NEXT_CHILD(minExclusive)
    NEXT_CHILD(maxExclusive)
    NEXT_CHILD(length)
    NEXT_CHILD(minLength)
    NEXT_CHILD(maxLength)
    END_CHILDREN
}


/// <summary>
/// 
/// </summary>
bool File::Check(SdaiModel model, bool stopAtFirstError, MsgLevel msgLevel, Console* output)
{
    bool ok = false;

    for (auto spec : m_specifications) {
        if (spec) {
            spec->ResetCache();
        }
    }

    DefaultConsole con;
    if (!output) {
        output = &con;
    }

    auto saveUnicode = getStringUnicode();
    setStringUnicode(0);

    Context ctx(*output, msgLevel, stopAtFirstError);
    ctx.model = model;

    ok = CheckInstances(ctx);

    if (ok || !ctx.stopAtFirstError) {
        if (!CheckSpecificationsUsed(ctx)) {
            ok = false;
        }
    }

    setStringUnicode(saveUnicode);
    ctx.model = 0;

    return ok;
}

/// <summary>
/// 
/// </summary>
bool File::CheckInstances(Context& ctx)
{
    bool ok = true;

    auto aggr = xxxxGetAllInstances(ctx.model);
    int i = 0;
    SdaiInstance inst = 0;
    while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &inst)) {

        ctx.currentInstane = inst;

        for (auto spec : m_specifications) {
            if (spec) {

                ctx.currentSpecification = spec;

                if (!spec->Check(inst, ctx)) {
                    ok = false;
                }

                ctx.currentSpecification = nullptr;
                if (!ok && ctx.stopAtFirstError) {
                    return false; //>>>>>>>>>>>>>>>>>>>>>
                }
            }
        }
        ctx.currentInstane = 0;
    }

    return ok;
}

/// <summary>
/// 
/// </summary>
bool File::CheckSpecificationsUsed(Context& ctx)
{
    bool ok = true;

    for (auto spec : m_specifications) {
        if (spec) {

            ctx.currentSpecification = spec;

            if (!spec->CheckUsed(ctx)) {
                ok = false;
            }

            ctx.currentSpecification = nullptr;
            if (!ok && ctx.stopAtFirstError) {
                return false; //>>>>>>>>>>>>>>>>>>>>>
            }
        }
    }

    return ok;
}

/// <summary>
/// 
/// </summary>
std::string Specification::DisplayName()
{
    std::string ret;

    if (!m_identifier.empty()) {
        ret = m_identifier;
    }

    if (!m_name.empty()) {
        if (ret.empty()) {
            ret = m_name;
        }
        else {
            ret += "(" + m_name + ")";
        }
    }

    if (ret.empty()) {
        ret = m_description;
    }

    return ret;
}


/// <summary>
/// 
/// </summary>
void Specification::ResetCache() 
{ 
    m_nOccurs = 0;
    m_ifcVersionChecked = false;

    m_applicability.ResetCache();
    m_requirements.ResetCache();
};

/// <summary>
/// 
/// </summary>
bool Specification::Check(SdaiInstance inst, Context& ctx)
{
    bool ok = true;

    if (SuitableIfcVersion(ctx)) {
        if (m_applicability.Match(inst, ctx)) {
            m_nOccurs++;

            auto rq = GetCardinality(NULL, m_minOccursVal, m_maxOccursVal, Cardinality::Optional, ctx);

            ok = m_requirements.Match(inst, ctx);

            if (rq == Cardinality::Prohibited) {
                ok = !ok;
            }

            if (ok) {
                ctx.LogMsg(MsgLevel::Info, "Checked ok");
            }
            else {
                ctx.LogMsg(MsgLevel::Error, "Instance does not match specification");
            }
        }
    }

    return ok;
}


/// <summary>
/// 
/// </summary>
bool Specification::CheckUsed(Context& ctx)
{
    bool ok = true;

    if (SuitableIfcVersion(ctx)) {
        
        auto rq = GetCardinality(NULL, m_minOccursVal, m_maxOccursVal, Cardinality::Optional, ctx);
        
        switch (rq) {
            case Cardinality::Required:
                if (m_nOccurs == 0) {
                    ok = false;
                    ctx.LogMsg(MsgLevel::Error, "Required specification never match");
                }
                break;
        }
    }

    ctx.LogMsg(MsgLevel::Info, "Specification was applied to %d instances", m_nOccurs);

    return ok;
}


/// <summary>
/// Function to split a string by whitespaces 
/// </summary>
static void splitString(const std::string& input, std::set<std::string>& result)
{
    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
        result.insert(token);
    }
}

/// <summary>
/// 
/// </summary>
bool Specification::SuitableIfcVersion(Context& ctx)
{
    //Each Specification may specify the IFC schema(s) that it applies to. This is purely for information purposes and a user may choose to audit the model with the specification and get a pass or fail result

    if (!m_ifcVersionChecked) {
        m_ifcVersionChecked = true;
        
        if (!m_ifcVersion.empty()) {

            std::set<std::string> all;
            splitString(m_ifcVersion, all);

            bool match = false;

            const char* ifcVersion = nullptr;
            switch (ctx.GetIfcVersion(&ifcVersion)) {
                case Context::IfcVersion::Ifc2x3:
                    match = all.find("IFC2x3") != all.end();
                    break;
                case Context::IfcVersion::Ifc4:
                    match = all.find("IFC4") != all.end();
                    break;
                case Context::IfcVersion::Ifc4x3:
                    match = all.find("IFC4x3") != all.end();
                    break;
                default:
                    assert(0);
                    ctx.LogMsg(MsgLevel::NotImplemented, "Model has unknown IFC version: %s", ifcVersion);
            }

            if (!match) {
                ctx.LogMsg(MsgLevel::Warning, "Specification is intended for %s but model has version %s", m_ifcVersion.c_str(), ifcVersion);
            }
        }
    }

    return true;
}


/// <summary>
/// 
/// </summary>
bool Facets::Match(SdaiInstance inst, Context& ctx)
{
    for (auto facet : m_facets) {
        if (!facet->Match(inst, ctx)) {
            return false;
        }
    }
    return true;
}

/// <summary>
/// 
/// </summary>
void FacetEntity::ResetCacheImpl()
{
    m_sdaiEntity = 0;
}

/// <summary>
/// 
/// </summary>
Facet::Matched FacetEntity::MatchImpl(SdaiInstance inst, Context& ctx)
{
    // check entity name
    //
    bool entityNameMatch = false;

    auto instType = sdaiGetInstanceType(inst);

    //doc says: The IFC Class must match exactly
    if (auto name = m_name.GetSimpleValue()) {
        if (!m_sdaiEntity) {
            //entities_must_be_specified_as_uppercase_strings
            std::string uppercase = name;
            ToUpper(uppercase);
            if (strcmp(name, uppercase.c_str())) {
                m_sdaiEntity = -1;
            }
            else {
                m_sdaiEntity = sdaiGetEntity(ctx.model, name);
                if (!m_sdaiEntity) {
                    m_sdaiEntity = -1;
                }
            }
        }

        entityNameMatch = (instType == m_sdaiEntity);
    }
    else {
        std::string instTypeName = engiGetEntityName(instType, sdaiSTRING);
        ToUpper (instTypeName);
        entityNameMatch = m_name.Match(instTypeName.c_str(), false, ctx);
    }


    if (!entityNameMatch) {
        return Facet::Matched::No; //>>>>>>>>>>>>>>>>>>>>>>>
    }
    
    //
    // check predefined type
    //
    if (m_predefinedType.IsSet() && !MatchPredefinedType(inst, ctx)) {
        return Matched::No;
    }

    //
    return Facet::Matched::Yes;
}

/// <summary>
/// 
/// </summary>
bool FacetEntity::MatchPredefinedType(SdaiInstance inst, Context& ctx)
{
    const char* predType = nullptr;
    sdaiGetAttrBN(inst, "PredefinedType", sdaiENUM, &predType);

    const wchar_t* objType = nullptr;
    if (!predType || !strcmp(predType, "USERDEFINED")) {
        sdaiGetAttrBN(inst, "ObjectType", sdaiUNICODE, &objType);
        if (!objType) {
            sdaiGetAttrBN(inst, "ElementType", sdaiUNICODE, &objType);
        }
        if (!objType) {
            sdaiGetAttrBN(inst, "ProcessType", sdaiUNICODE, &objType);
        }
    }

    if (objType) {
        return m_predefinedType.Match(objType, false, ctx); //>>>>>>>>>
    }
    else if (predType) {
        return m_predefinedType.Match(predType, false, ctx); //>>>>>>>>>>
    }
    else if (auto type = GetTypeObject(inst, ctx)) {
        return MatchPredefinedType(type, ctx); //>>>>>>>>>>
    }

    return false;
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::ResetCacheImpl()
{
    m_entity.ResetCache();
    m_navigations.clear();
}

/// <summary>
/// 
/// </summary>
Facet::Matched FacetPartOf::MatchImpl(SdaiInstance inst, Context& ctx)
{
    if (m_navigations.empty()) {
        FillParentsNavigators(ctx);
    }

    std::list<SdaiInstance> toCheckParents;
    toCheckParents.push_back(inst);

    while (!toCheckParents.empty()) {
        
        inst = toCheckParents.front();
        toCheckParents.pop_front();

        for (auto& nav : m_navigations) {
            std::list<SdaiInstance> follow;
            nav->Follow(inst, follow, ctx);
            for (auto parent : follow) {
                if (nav->canMatch && m_entity.Match(parent, ctx)) {
                    return Matched::Yes; //>>>>>>>>>>>>>>>>>>>>>>>>>
                }
                toCheckParents.push_back(parent);
            }
        }
    }

    return Matched::No;
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::FillParentsNavigators(Context& ctx)
{
    ToUpper(m_relation);

    if (m_relation.empty()) {
        assert(!"to test");
        CreateNavigator(ctx._IfcFeatureElementSubtraction_VoidsElements(), sdaiINSTANCE, NULL, ctx._IfcRelVoidsElement_RelatingBuildingElement(), ctx);
        CreateNavigator(ctx._IfcElement_FillsVoids(), sdaiAGGR, NULL, ctx._IfcRelFillsElement_RelatingOpeningElement(), ctx);
        CreateNavigator(ctx._IfcObjectDefinition_HasAssignments(), sdaiAGGR, ctx._IfcRelAssignsToGroup(), ctx._IfcRelAssignsToGroup_RelatingGroup(), ctx);

        if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
            CreateNavigatorBN2(ctx._IfcObjectDefinition_Decomposes(), sdaiAGGR, NULL, "RelatingObject", ctx);
        }
        else {
            CreateNavigator(ctx._IfcObjectDefinition_Decomposes(), sdaiAGGR, NULL, ctx._IfcRelAggregates_RelatingObject(), ctx);
            CreateNavigator(ctx._IfcObjectDefinition_Nests(), sdaiAGGR, NULL, ctx._IfcRelNests_RelatingObject(), ctx);
        }

        CreateNavigatorBN("ContainedInStructure", sdaiAGGR, NULL, ctx._IfcRelContainedInSpatialStructure_RelatingStructure(), ctx);
    }
    else if (m_relation == "IFCRELAGGREGATES") {
        if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
            assert(!"to test");
            CreateNavigator(ctx._IfcObjectDefinition_Decomposes(), sdaiAGGR, ctx._IfcRelAggregates(), ctx._IfcRelAggregates_RelatingObject(), ctx);
        }
        else {
            CreateNavigator(ctx._IfcObjectDefinition_Decomposes(), sdaiAGGR, NULL, ctx._IfcRelAggregates_RelatingObject(), ctx);
        }
    }
    else if (m_relation == "IFCRELNESTS") {
        if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
            CreateNavigator(ctx._IfcObjectDefinition_Decomposes(), sdaiAGGR, ctx._IfcRelNests(), ctx._IfcRelNests_RelatingObject(), ctx);
        }
        else {
            CreateNavigator(ctx._IfcObjectDefinition_Nests(), sdaiAGGR, NULL, ctx._IfcRelNests_RelatingObject(), ctx);
        }
    }
    else if (m_relation == "IFCRELASSIGNSTOGROUP") {
        CreateNavigator(ctx._IfcObjectDefinition_HasAssignments(), sdaiAGGR, ctx._IfcRelAssignsToGroup(), ctx._IfcRelAssignsToGroup_RelatingGroup(), ctx);
    }
    else if (m_relation == "IFCRELCONTAINEDINSPATIALSTRUCTURE") {
        CreateNavigatorBN("ContainedInStructure", sdaiAGGR, NULL, ctx._IfcRelContainedInSpatialStructure_RelatingStructure(), ctx);

        //parts of objects belongs to parents spatial structure
        CreateNavigator(ctx._IfcFeatureElementSubtraction_VoidsElements(), sdaiINSTANCE, NULL, ctx._IfcRelVoidsElement_RelatingBuildingElement(), ctx, false);
        CreateNavigator(ctx._IfcElement_FillsVoids(), sdaiAGGR, NULL, ctx._IfcRelFillsElement_RelatingOpeningElement(), ctx, false);

        if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
            assert(!"to test");
            CreateNavigatorBN2(ctx._IfcObjectDefinition_Decomposes(), sdaiAGGR, NULL, "RelatingObject", ctx, false);
        }
        else {
            CreateNavigator(ctx._IfcObjectDefinition_Decomposes(), sdaiAGGR, NULL, ctx._IfcRelAggregates_RelatingObject(), ctx, false);
            CreateNavigator(ctx._IfcObjectDefinition_Nests(), sdaiAGGR, NULL, ctx._IfcRelNests_RelatingObject(), ctx, false);
        }

    }
    else if (m_relation == "IFCRELVOIDSELEMENT") {
        assert(!"to test");
        CreateNavigator(ctx._IfcFeatureElementSubtraction_VoidsElements(), sdaiINSTANCE, NULL, ctx._IfcRelVoidsElement_RelatingBuildingElement(), ctx);
    }
    else if (m_relation == "IFCRELFILLSELEMENT") {
        assert(!"to test");
        CreateNavigator(ctx._IfcElement_FillsVoids(), sdaiAGGR, NULL, ctx._IfcRelFillsElement_RelatingOpeningElement(), ctx);
    }
    else {
        ctx.LogMsg(MsgLevel::NotImplemented, "Unsupported relationship %s", m_relation.c_str());
        assert(0);
    }
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::CreateNavigator(SdaiAttr attrRelation, SdaiPrimitiveType sdaiType, SdaiEntity relClass, SdaiAttr attrParent, Context&, bool canMatch)
{
    auto nav = new Navigator ();
    m_navigations.push_back(nav);

    nav->attrRelation = attrRelation;
    nav->sdaiType = sdaiType;
    nav->relClass = relClass;
    nav->attrParent = attrParent;
    nav->canMatch = canMatch;
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::CreateNavigatorBN(const char* attrRelation, SdaiPrimitiveType sdaiType, SdaiEntity relClass, SdaiAttr attrParent, Context&, bool canMatch)
{
    auto nav = new Navigator();
    m_navigations.push_back(nav);

    nav->attrRelationBN = attrRelation;
    nav->sdaiType = sdaiType;
    nav->relClass = relClass;
    nav->attrParent = attrParent;
    nav->canMatch = canMatch;
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::CreateNavigatorBN2(SdaiAttr attrRelation, SdaiPrimitiveType sdaiType, SdaiEntity relClass, const char* attrParent, Context&, bool canMatch)
{
    auto nav = new Navigator();
    m_navigations.push_back(nav);

    nav->attrRelation = attrRelation;
    nav->sdaiType = sdaiType;
    nav->relClass = relClass;
    nav->attrParentBN = attrParent;
    nav->canMatch = canMatch;
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::Navigator::Follow(SdaiInstance inst, std::list<SdaiInstance>& follow, Context&)
{
    if (sdaiType == sdaiAGGR) {
        SdaiAggr aggr = 0;
        
        if (attrRelation) {
            if (IsAttrSuitable(attrRelation, inst)) {
                sdaiGetAttr(inst, attrRelation, sdaiAGGR, &aggr);
            }
        }
        else if (attrRelationBN) {
            sdaiGetAttrBN(inst, attrRelationBN, sdaiAGGR, &aggr);
        }

        if (aggr) {
            int i = 0;
            SdaiInstance rel = 0;
            while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &rel)) {
                FollowRel(rel, follow);
            }
        }
    }
    else {
        SdaiInstance rel = 0;

        if (attrRelation) {
            if (IsAttrSuitable(attrRelation, inst)) {
                sdaiGetAttr(inst, attrRelation, sdaiINSTANCE, &rel);
            }
        }
        else if (attrRelationBN) {
            sdaiGetAttrBN(inst, attrRelationBN, sdaiINSTANCE, &rel);
        }

        FollowRel(rel, follow);
    }
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::Navigator::FollowRel(SdaiInstance rel, std::list<SdaiInstance>& follow)
{
    if (!rel) {
        return;
    }
    
    if (relClass) {
        if (!sdaiIsInstanceOf(rel, relClass)) {
            return;
        }
    }

    SdaiInstance inst = 0;
    if (attrParent) {
        sdaiGetAttr(rel, attrParent, sdaiINSTANCE, &inst);
    }
    else if (attrParentBN) {
        sdaiGetAttrBN(rel, attrParentBN, sdaiINSTANCE, &inst);
    }

    if (inst) {
        follow.push_back(inst);
    }
}

/// <summary>
/// 
/// </summary>
void FacetClassification::ResetCacheImpl()
{
}

/// <summary>
/// 
/// </summary>
Facet::Matched FacetClassification::MatchImpl(SdaiInstance inst, Context& ctx)
{
    References references;

    CollectIfcRelAssociatesClassification(inst, ctx, references);

    CollectIfcExternalReferenceRelationship(inst, ctx, references);

    if (auto type = GetTypeObject(inst, ctx)) {
        CollectIfcRelAssociatesClassification(type, ctx, references);
    }

    return Match (references, ctx) ? Matched::Yes : Matched::No;
}

/// <summary>
/// 
/// </summary>
void FacetClassification::CollectIfcRelAssociatesClassification(SdaiInstance inst, Context& ctx, References& references)
{
    //when inst is IfcPropertyDefinition or IfcObjectDefinition
    SdaiAggr aggrAssoc = 0;
    sdaiGetAttrBN(inst, "HasAssociations", sdaiAGGR, &aggrAssoc); 

    if (aggrAssoc) {
        SdaiInstance relAssoc = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(aggrAssoc, i++, sdaiINSTANCE, &relAssoc)) {

            auto entityAssoc = sdaiGetInstanceType(relAssoc);
            if (entityAssoc == ctx._IfcRelAssociatesClassification()) {

                SdaiInstance clsf = 0;
                sdaiGetAttr(relAssoc, ctx._IfcRelAssociatesClassification_RelatingClassification(), sdaiINSTANCE, &clsf);
                if (clsf) {

                    std::wstring system;
                    Reference reference;

                    HandleClassificationSelect(clsf, ctx, system, reference);
                    
                    references.insert(References::value_type(system, reference));
                }
            }
        }
    }
}

/// <summary>
/// 
/// </summary>
void FacetClassification::CollectIfcExternalReferenceRelationship(SdaiInstance inst, Context& ctx, References& references)
{
    //when inst is IfcResourceObjectSelect
    SdaiAggr aggrExternalRefs = 0;
    sdaiGetAttrBN(inst, "HasExternalReferences", sdaiAGGR, &aggrExternalRefs);

    if (aggrExternalRefs) {
        SdaiInstance relExternalRefRel = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(aggrExternalRefs, i++, sdaiINSTANCE, &relExternalRefRel)) {

            SdaiInstance externalRef = 0;
            sdaiGetAttr(relExternalRefRel, ctx._IfcExternalReferenceRelationship_RelatingReference(), sdaiINSTANCE, &externalRef);

            auto entityRef = sdaiGetInstanceType(externalRef);
            if (entityRef == ctx._IfcClassificationReference()) {

                std::wstring system;
                Reference reference;

                HandleClassificationSelect(externalRef, ctx, system, reference);

                references.insert (References::value_type (system, reference));
            }
        }
    }
}


/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassificationSelect(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference)
{
    auto clsfType = sdaiGetInstanceType(clsf);
    if (clsfType == ctx._IfcClassificationReference()) {
        HandleClassificationReference(clsf, ctx, system, reference);
        return;
    }

    if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
        if (clsfType == ctx._IfcClassificationNotation()) {
            HandleClassificationNotation(clsf, ctx, system, reference);
            return;
        }
    }
    else {
        if (clsfType == ctx._IfcClassification()) {
            HandleClassification(clsf, ctx, system, reference);
            return;
        }
    }

    auto clsfTypeName = engiGetEntityName(clsfType, sdaiSTRING);
    assert(clsfTypeName);
    ctx.LogMsg(MsgLevel::NotImplemented, "Not implemented IfcClassificationSelect type %s", clsfTypeName ? clsfTypeName : NULL);
    assert(0);
}

/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassificationReference(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference)
{
    //
    const wchar_t* item = nullptr;

    if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
        sdaiGetAttr(clsf, ctx._IfcExternalReference_ItemReference(), sdaiUNICODE, &item);
    }
    else {
        sdaiGetAttr(clsf, ctx._IfcExternalReference_Identification(), sdaiUNICODE, &item);
    }

    if (item) {
        reference.items.push_back(item);
    }

    //
    const wchar_t* uri = nullptr;
    sdaiGetAttr(clsf, ctx._IfcExternalReference_Location(), sdaiUNICODE, &uri);

    if (uri) {
        reference.URI.push_back(uri);
    }

    //
    SdaiInstance source = 0;
    sdaiGetAttr(clsf, ctx._IfcClassificationReference_ReferencedSource(), sdaiINSTANCE, &source);
    HandleClassificationSelect(source, ctx, system, reference);
}

/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassification(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(clsf, ctx._IfcClassification_Name(), sdaiUNICODE, &name);
    if (name) {
        assert(system.empty());
        system = name;
    }

    if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc4) {
        const wchar_t* source = nullptr;
        sdaiGetAttr(clsf, ctx._IfcClassification_Location(), sdaiUNICODE, &source);
        if (source) {
            reference.URI.push_back(source);
        }
    }

    if (ctx.GetIfcVersion() > Context::IfcVersion::Ifc4) {
        const wchar_t* source = nullptr;
        sdaiGetAttr(clsf, ctx._IfcClassification_Specification(), sdaiUNICODE, &source);
        if (source) {
            reference.URI.push_back(source);
        }
    }

    const wchar_t* source = nullptr;
    sdaiGetAttr(clsf, ctx._IfcClassification_Source(), sdaiUNICODE, &source);
    if (source) {
        reference.URI.push_back(source);
    }
}

/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassificationNotation(SdaiInstance /*clsf*/, Context& ctx, std::wstring& /*system*/, Reference& /*reference*/)
{
    ctx.LogMsg(MsgLevel::NotImplemented, "IfcClassificationNotation in facet classification");
    assert(!"TODO... if somebody uses");
}

/// <summary>
/// 
/// </summary>
bool FacetClassification::Match(References& references, Context& ctx)
{
    for (auto& ref : references) {
        if (m_system.Match(ref.first.c_str(), true, ctx)) {

            bool itemMatch = !m_value.IsSet();
            for (auto& item : ref.second.items) {
                itemMatch = m_value.Match(item.c_str(), true, ctx);
                if (itemMatch) {
                    break;
                }
            }

            if (itemMatch) {

                bool uriMatch = !m_URI.IsSet();
                for (auto& uri : ref.second.URI) {
                    uriMatch = m_URI.Match(uri.c_str(), true, ctx);
                }

                if (uriMatch) {
                    return true;
                }
            }
        }
    }

    return false;
}


/// <summary>
/// 
/// </summary>
Facet::Matched FacetAttribute::MatchImpl(SdaiInstance inst, Context& ctx)
{
    SdaiEntity ent = sdaiGetInstanceType(inst);

    if (auto attrName = m_name.GetSimpleValue()) {

        //explicitly specified attribute
        auto attr = sdaiGetAttrDefinition(ent, attrName);
        if (attr) {
            return MatchAttributeValue(inst, attr, m_value, ctx); //>>>>>>>>>>>>>>>>
        }
    }
    else {

        //attribute name must match
        const bool useInverse = false;
        auto N = engiGetEntityNoAttributesEx (ent, true, useInverse);
        for (SdaiInteger i = 0; i < N; i++) {
            auto attr = engiGetEntityAttributeByIndex(ent, i, true, useInverse);
            const char* name = 0;
            engiGetAttributeTraits(attr, &name, NULL, NULL, NULL, NULL, NULL, NULL);
            if (m_name.Match(name, true, ctx)) {
                auto matched = MatchAttributeValue(inst, attr, m_value, ctx);
                if (matched != Matched::NotFound) {
                    return matched; //>>>>>>>>>>>>>
                }
            }
        }
    }

    return Matched::NotFound;
}

/// <summary>
/// 
/// </summary>
Facet::Matched Facet::MatchAttributeValue(SdaiInstance inst, SdaiAttr attr, IdsValue& idsvalue, Context& ctx)
{
    if (engiAttrIsInverse(attr)) {
        return Matched::NotFound; //do not apply to imverse attribute
    }

    Matched match = Matched::NotFound;

    auto sdaiType = engiGetInstanceAttrType(inst, attr);
    switch (sdaiType) {
        case 0://$
            break;
        case sdaiBINARY: {
            const char* value = nullptr;
            if (sdaiGetAttr(inst, attr, sdaiBINARY, &value) && value && *value) {
                match = idsvalue.Match(value, false, ctx) ? Matched::Yes : Matched::No;
            }
            break;
        }
        case sdaiENUM: {
            const char* value = nullptr;
            if (sdaiGetAttr(inst, attr, sdaiENUM, &value) && value && *value) {
                match = idsvalue.Match(value, false, ctx) ? Matched::Yes : Matched::No;
            }
            break;
        }
        case sdaiSTRING: {
            const wchar_t* value = nullptr;
            if (sdaiGetAttr(inst, attr, sdaiUNICODE, &value) && value && *value) {
                match = idsvalue.Match(value, false, ctx) ? Matched::Yes : Matched::No;
            }
            break;
        }
        case sdaiINTEGER: {
            SdaiInteger value = 0;
            if (sdaiGetAttr(inst, attr, sdaiINTEGER, &value)) {
                match = idsvalue.Match(value, ctx) ? Matched::Yes : Matched::No;
            }
            break;
        }
        case sdaiREAL:
        case sdaiNUMBER: {
            double value = 0;
            if (sdaiGetAttr(inst, attr, sdaiREAL, &value)) {
                match = idsvalue.Match(value, ctx) ? Matched::Yes : Matched::No;
            }
            break;
        }
        case sdaiAGGR: {
            SdaiAggr aggr = 0;
            if (sdaiGetAttr(inst, attr, sdaiAGGR, &aggr)) {
                match = MatchAggrValue(aggr, idsvalue, ctx) ? Matched::Yes : Matched::No;
            }
            break;
        }
        case sdaiLOGICAL: {
            const char* value = nullptr;
            if (sdaiGetAttr(inst, attr, sdaiLOGICAL, &value)) {
                if (value){
                    switch (*value) {
                        case 'T':
                        case 't':
                            match = idsvalue.Match("TRUE", false, ctx) ? Matched::Yes : Matched::No;
                            break;
                        case 'F':
                        case 'f':
                            match = idsvalue.Match("FALSE", false, ctx) ? Matched::Yes : Matched::No;
                            break;
                        //U always fails
                    }
                }
            }
            break;
        }
        case sdaiBOOLEAN: {
            bool value = false;
            if (sdaiGetAttr(inst, attr, sdaiBOOLEAN, &value)) {
                match = idsvalue.Match(value ? "TRUE" : "FALSE", false, ctx) ? Matched::Yes : Matched::No;
            }
            break;
        }
        case sdaiINSTANCE: {
            SdaiInstance value = 0;
            if (sdaiGetAttr(inst, attr, sdaiINSTANCE, &value)) {
                match = MatchInstanceValue (idsvalue) ? Matched::Yes : Matched::No;
            }
            break;
        }
        case sdaiADB: {
            SdaiADB value = 0;
            if (sdaiGetAttr(inst, attr, sdaiADB, &value)) {
                match = MatchADBValue(value, idsvalue, ctx) ? Matched::Yes : Matched::No;
            }
            break;
        }
        default:
            ctx.LogMsg(MsgLevel::NotImplemented, "Match unknown attribute type");
            assert(0);
    }

    return match;
}

/// <summary>
/// 
/// </summary>
bool Facet::MatchAggrValue(SdaiAggr aggr, IdsValue& idsvalue, Context& ctx)
{
    SdaiPrimitiveType sdaiType;
    engiGetAggrType(aggr, &sdaiType);

    auto N = sdaiGetMemberCount(aggr);
    for (SdaiInteger i = 0; i < N; i++) {

        bool match = false;

        switch (sdaiType) {
            case 0://$
                assert(!"to test");
                break;
            case sdaiBINARY: {
                assert(!"to test");
                const char* value = nullptr;
                if (sdaiGetAggrByIndex(aggr, i, sdaiBINARY, &value) && value && *value) {
                    match = idsvalue.Match(value, false, ctx);
                }
                break;
            }
            case sdaiENUM: {
                assert(!"to test");
                const char* value = nullptr;
                if (sdaiGetAggrByIndex(aggr, i, sdaiENUM, &value) && value && *value) {
                    match = idsvalue.Match(value, false, ctx);
                }
                break;
            }
            case sdaiSTRING: {
                assert(!"to test");
                const wchar_t* value = nullptr;
                if (sdaiGetAggrByIndex(aggr, i, sdaiUNICODE, &value) && value && *value) {
                    match = idsvalue.Match(value, false, ctx);
                }
                break;
            }
            case sdaiINTEGER: {
                assert(!"to test");
                SdaiInteger value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiINTEGER, &value)) {
                    match = idsvalue.Match(value, ctx);
                }
                break;
            }
            case sdaiREAL:
            case sdaiNUMBER: {
                double value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiREAL, &value)) {
                    match = idsvalue.Match(value, ctx);
                }
                break;
            }
            case sdaiAGGR: {
                assert(!"to test");
                SdaiAggr value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiAGGR, &value)) {
                    match = MatchAggrValue(value, idsvalue, ctx);
                }
                break;
            }
            case sdaiLOGICAL: {
                assert(!"to test");
                const char* value = nullptr;
                if (sdaiGetAggrByIndex(aggr, i, sdaiLOGICAL, &value)) {
                    if (value) {
                        switch (*value) {
                            case 'T':
                            case 't':
                                match = idsvalue.Match("TRUE", false, ctx);
                                break;
                            case 'F':
                            case 'f':
                                match = idsvalue.Match("FALSE", false, ctx);
                                break;
                                //U always fails
                        }
                    }
                }
                break;
            }
            case sdaiBOOLEAN: {
                assert(!"to test");
                bool value = false;
                if (sdaiGetAggrByIndex(aggr, i, sdaiBOOLEAN, &value)) {
                    match = idsvalue.Match(value ? "TRUE" : "FALSE", false, ctx);
                }
                break;
            }
            case sdaiINSTANCE: {
                assert(!"to test");
                SdaiInstance value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiINSTANCE, &value)) {
                    match = MatchInstanceValue(idsvalue);
                }
                break;
            }
            case sdaiADB: {
                assert(!"to test");
                SdaiADB value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiADB, &value)) {
                    match = MatchADBValue(value, idsvalue, ctx);
                }
                break;
            }
            default:
                ctx.LogMsg(MsgLevel::NotImplemented, "Match unknown attribute type");
                assert(0);
        }

        if (match) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool Facet::MatchADBValue(SdaiADB adb, IdsValue& idsvalue, Context& ctx)
{
    bool match = false;

    SdaiPrimitiveType sdaiType = sdaiGetADBType(adb);

    switch (sdaiType) {
        case 0://$
            assert(!"to test");
            break;
        case sdaiBINARY: {
            assert(!"to test");
            const char* value = nullptr;
            if (sdaiGetADBValue(adb, sdaiBINARY, &value) && value && *value) {
                match = idsvalue.Match(value, false, ctx);
            }
            break;
        }
        case sdaiENUM: {
            assert(!"to test");
            const char* value = nullptr;
            if (sdaiGetADBValue(adb, sdaiENUM, &value) && value && *value) {
                match = idsvalue.Match(value, false, ctx);
            }
            break;
        }
        case sdaiSTRING: {
            assert(!"to test");
            const wchar_t* value = nullptr;
            if (sdaiGetADBValue(adb, sdaiUNICODE, &value) && value && *value) {
                match = idsvalue.Match(value, false, ctx);
            }
            break;
        }
        case sdaiINTEGER: {
            assert(!"to test");
            SdaiInteger value = 0;
            if (sdaiGetADBValue(adb, sdaiINTEGER, &value)) {
                match = idsvalue.Match(value, ctx);
            }
            break;
        }
        case sdaiREAL:
        case sdaiNUMBER: {
            double value = 0;
            if (sdaiGetADBValue(adb, sdaiREAL, &value)) {
                match = idsvalue.Match(value, ctx);
            }
            break;
        }
        case sdaiAGGR: {
            assert(!"to test");
            SdaiAggr value = 0;
            if (sdaiGetADBValue(adb, sdaiAGGR, &value)) {
                match = MatchAggrValue(value, idsvalue, ctx);
            }
            break;
        }
        case sdaiLOGICAL: {
            assert(!"to test");
            const char* value = nullptr;
            if (sdaiGetADBValue(adb, sdaiLOGICAL, &value)) {
                if (value) {
                    switch (*value) {
                        case 'T':
                        case 't':
                            match = idsvalue.Match("TRUE", false, ctx);
                            break;
                        case 'F':
                        case 'f':
                            match = idsvalue.Match("FALSE", false, ctx);
                            break;
                            //U always fails
                    }
                }
            }
            break;
        }
        case sdaiBOOLEAN: {
            assert(!"to test");
            bool value = false;
            if (sdaiGetADBValue(adb, sdaiBOOLEAN, &value)) {
                match = idsvalue.Match(value ? "TRUE" : "FALSE", false, ctx);
            }
            break;
        }
        case sdaiINSTANCE: {
            assert(!"to test");
            SdaiInstance value = 0;
            if (sdaiGetADBValue(adb, sdaiINSTANCE, &value)) {
                match = MatchInstanceValue(idsvalue);
            }
            break;
        }
        case sdaiADB: {
            assert(!"to test");
            SdaiADB value = 0;
            if (sdaiGetADBValue(adb, sdaiADB, &value)) {
                match = MatchADBValue(value, idsvalue, ctx);
            }
            break;
        }
        default:
            ctx.LogMsg(MsgLevel::NotImplemented, "Match unknown attribute type");
            assert(0);
    }

    return match;
}

/// <summary>
/// 
/// </summary>
Facet::Matched FacetProperty::MatchImpl(SdaiInstance inst, Context& ctx)
{
    std::set<std::wstring> testedProps;

    if (!TestProperties(inst, ctx, testedProps)) {
        return Matched::No;
    }

    return testedProps.size() > 0 ?  Matched::Yes : Matched::NotFound;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::TestProperties(SdaiInstance inst, Context& ctx, std::set<std::wstring>& testedProps)
{
    if (sdaiIsKindOf(inst, ctx._IfcTypeObject())) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(inst, ctx._IfcTypeObject_HasPropertySets(), sdaiAGGR, &aggr);
        if (!TestInSetOfPSDef(aggr, ctx, testedProps)) {
            return false;
        }
    }

    if (sdaiIsKindOf(inst, ctx._IfcObject())) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(inst, ctx._IfcObject_IsDefinedBy(), sdaiAGGR, &aggr);
        if (!TestInSetOfRel(aggr, ctx, testedProps)) {
            return false;
        }
    }

    if (sdaiIsKindOf(inst, ctx._IfcContext())) {
        SdaiAggr aggr = 0;
        if (ctx.GetIfcVersion() > Context::IfcVersion::Ifc2x3) {
            sdaiGetAttr(inst, ctx._IfcContext_IsDefinedBy(), sdaiAGGR, &aggr);
            if (!TestInSetOfRel(aggr, ctx, testedProps)) {
                return false;
            }
        }
    }

    if (auto type = GetTypeObject(inst, ctx)) {
        if (!TestProperties(type, ctx, testedProps)) {
            return false;
        }
    }

    return true;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::TestInSetOfPSDef(SdaiAggr aggr, Context& ctx, std::set<std::wstring>& testedProps)
{
    SdaiInstance pset = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &pset)) {
        if (!TestInPSDef(pset, ctx, testedProps)) {
            return false;
        }
    }
    return true;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::TestInSetOfRel(SdaiAggr aggr, Context& ctx, std::set<std::wstring>& testedProps)
{
    SdaiInstance rel = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &rel)) {
        SdaiInstance pset = 0;
        if (sdaiGetAttr(rel, ctx._IfcRelDefinesByProperties_RelatingPropertyDefinition(), sdaiINSTANCE, &pset)) {
            if (!TestInPSDef(pset, ctx, testedProps)) {
                return false;
            }
        }
        else {
            assert(!"not tested");
            SdaiAggr psdefset = 0;
            if (sdaiGetAttr(rel, ctx._IfcRelDefinesByProperties_RelatingPropertyDefinition(), sdaiAGGR, &psdefset)) {
                SdaiInteger j = 0;
                while (sdaiGetAggrByIndex(psdefset, j++, sdaiINSTANCE, &pset)) {
                    if (!TestInPSDef(pset, ctx, testedProps)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::TestInPSDef(SdaiInstance inst, Context& ctx, std::set<std::wstring>& testedProps)
{
    const wchar_t* psetName = 0;

    if (m_propertySet.IsSet()) {
        sdaiGetAttr(inst, ctx._IfcRoot_Name(), sdaiUNICODE, &psetName);
        if (!m_propertySet.Match(psetName, false, ctx)) {
            return true; //not to test
        }
    }

    auto type = sdaiGetInstanceType(inst);
    
    bool propNameMatched = false;

    if (type == ctx._IfcPropertySet()) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(inst, ctx._IfcPropertySet_HasProperties(), sdaiAGGR, &aggr);
        SdaiInstance prop = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &prop)) {
            if (!TestProperty(prop, ctx, psetName, testedProps, propNameMatched)) {
                return false;//>>>>>
            }
        }
    }
    else if (type == ctx._IfcElementQuantity()) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(inst, ctx._IfcElementQuantity_Quantities(), sdaiAGGR, &aggr);
        SdaiInstance quant = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &quant)) {
            if (!TestQuantity(quant, ctx, psetName, testedProps, propNameMatched)) {
                return false; //>>>>>>
            }
        }
    }
    else {
        auto N = engiGetEntityNoAttributesEx(type, false, false);
        for (SdaiInteger i = 0; i < N; i++) {
            auto attr = engiGetEntityAttributeByIndex(type, i, false, false);
            if (!TestPredefinedProperty(inst, attr, psetName, testedProps, propNameMatched, ctx)) {
                return false; //>>>>>>
            }
        }
    }

    return propNameMatched; //fail-all_matching_property_sets_must_satisfy_requirements_2_3.ids
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::TestProperty(SdaiInstance prop, Context& ctx, const wchar_t* pset, std::set<std::wstring>& testedProps, bool& propNameMatched)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(prop, ctx._IfcProperty_Name(), sdaiUNICODE, &name);
    if (!m_name.Match(name, false, ctx)) {
        return true;  //not to test
    }

    propNameMatched = true;
    
    std::wstring testedName(pset);
    testedName.append(L"/@");
    testedName.append(name);

    if (!testedProps.insert(testedName).second) {
        return true; //already tested, this type property was overriden in insatnce
    }

    auto entity = sdaiGetInstanceType(prop);

    if (entity == ctx._IfcComplexProperty()) {
        ctx.LogMsg(MsgLevel::NotImplemented, "IfcComplexProperty");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertyBoundedValue()) {
        return MatchPropertyBoundedValue(prop, ctx);
    }
    else if (entity == ctx._IfcPropertyEnumeratedValue()) {
        return MatchPropertyEnumeratedValue(prop, ctx);
    }
    else if (entity == ctx._IfcPropertyListValue()) {
        return MatchPropertyListValue(prop, ctx);
    }
    else if (entity == ctx._IfcPropertyReferenceValue()) {
        return false; //reference_properties_are_treated_as_objects_and_not_supported
    }
    else if (entity == ctx._IfcPropertySingleValue()) {
        return MatchPropertySingleValue(prop, ctx);
    }
    else if (entity == ctx._IfcPropertyTableValue()) {
        return MatchPropertyTableValue(prop, ctx);
    }
    else {
        ctx.LogMsg(MsgLevel::NotImplemented, "Unknown entity");
        assert(0);
        return false;
    }
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::TestQuantity(SdaiInstance qto, Context& ctx, const wchar_t* pset, std::set<std::wstring>& testedProps, bool& propNameMatched)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(qto, ctx._IfcPhysicalQuantity_Name(), sdaiUNICODE, &name);
    if (!m_name.Match(name, false, ctx)) {
        return true; //not to test
    }

    propNameMatched = true;

    std::wstring testedName(pset);
    testedName.append(L"/@");
    testedName.append(name);

    if (!testedProps.insert(testedName).second) {
        return true; //already tested, this type property was overriden in insatnce
    }


    auto entity = sdaiGetInstanceType(qto);

    if (entity == ctx._IfcPhysicalComplexQuantity()) {
        //complex_properties_are_not_supported
        return false;
    }

    SdaiInstance unit = 0;
    sdaiGetAttr(qto, ctx._IfcPhysicalSimpleQuantity_Unit(), sdaiINSTANCE, &unit);

    if (entity == ctx._IfcQuantityArea()) {
        assert(!"to test");
        double value = 0;
        if (sdaiGetAttr(qto, ctx._IfcQuantityArea_AreaValue(), sdaiREAL, &value))
            return MatchValue(value, unit, "IFCAREAMEASURE", ctx);
        else
            return false;
    }
    else if (entity == ctx._IfcQuantityCount()) {
        assert(!"not tested");
        SdaiInteger value = 0;
        if (sdaiGetAttr(qto, ctx._IfcQuantityCount_CountValue(), sdaiINTEGER, &value))
            return m_value.Match(value, ctx);
        else
            return false;
    }
    else if (entity == ctx._IfcQuantityLength()) {
        double value = 0;
        if (sdaiGetAttr(qto, ctx._IfcQuantityLength_LengthValue(), sdaiREAL, &value))
            return MatchValue(value, unit, "IFCLENGTHMEASURE", ctx);
        else
            return false;
    }
    else if (entity == ctx._IfcQuantityNumber()) {
        assert(!"not tested");
        double value = 0;
        if (sdaiGetAttr(qto, ctx._IfcQuantityNumber_NumberValue(), sdaiREAL, &value))
            return MatchValue(value, unit, nullptr, ctx);
        else
            return false;
    }
    else if (entity == ctx._IfcQuantityTime()) {
        assert(!"to test");
        double value = 0;
        if (sdaiGetAttr(qto, ctx._IfcQuantityTime_TimeValue(), sdaiREAL, &value))
            return MatchValue(value, unit, "IFCTIMEMEASURE", ctx);
        else
            return false;
    }
    if (entity == ctx._IfcQuantityVolume()) {
        assert(!"to test");
        double value = 0;
        if (sdaiGetAttr(qto, ctx._IfcQuantityVolume_VolumeValue(), sdaiREAL, &value))
            return MatchValue(value, unit, "IFCVOLUMEMEASURE", ctx);
        else
            return false;
    }
    else if (entity == ctx._IfcQuantityWeight()) {
        assert(!"to test");
        double value = 0;
        if (sdaiGetAttr(qto, ctx._IfcQuantityWeight_WeightValue(), sdaiREAL, &value))
            return MatchValue(value, unit, "IFCMASSMEASURE", ctx);
        else
            return false;
    }
    else {
        ctx.LogMsg(MsgLevel::NotImplemented, "Unknown entity");
        assert(0);
        return false;
    }
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::TestPredefinedProperty(SdaiInstance pset, SdaiAttr prop, const wchar_t* psetName, std::set<std::wstring>& testedProps, bool& propNameMatched, Context& ctx)
{
    const char* name = 0;
    engiGetAttributeTraits(prop, &name, NULL, NULL, nullptr, nullptr, NULL, NULL);
    if (!m_name.Match(name, false, ctx)) {
        return true; //not to test
    }

    propNameMatched = true;

    //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wname = converter.from_bytes(name);

    std::wstring testedName(psetName);
    testedName.append(L"/@");
    testedName.append(wname);

    if (!testedProps.insert(testedName).second) {
        return true; //already tested, this type property was overriden in insatnce
    }

    return MatchAttributeValue(pset, prop, m_value, ctx) == Matched::Yes;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchPropertyEnumeratedValue(SdaiInstance prop, Context& ctx)
{
    SdaiInstance unit = 0;
    SdaiInstance enumeration = 0;
    sdaiGetAttr(prop, ctx._IfcPropertyEnumeratedValue_EnumerationReference(), sdaiINSTANCE, &enumeration);
    if (enumeration) {
        sdaiGetAttr(enumeration, ctx._IfcPropertyEnumeration_Unit(), sdaiINSTANCE, &unit);
    }

    SdaiAggr values = 0;
    sdaiGetAttr(prop, ctx._IfcPropertyEnumeratedValue_EnumerationValues(), sdaiAGGR, &values);
    if (values) {
        SdaiADB value = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(values, i++, sdaiADB, &value)) {
            if (MatchValue(value, unit, ctx)) {
                return true;
            }
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchListOfValues(SdaiInstance prop, SdaiAttr attrUnit, SdaiAttr attrValues, Context& ctx)
{
    SdaiInstance unit = 0;
    sdaiGetAttr(prop, attrUnit, sdaiINSTANCE, &unit);

    SdaiAggr values = 0;
    sdaiGetAttr(prop, attrValues, sdaiAGGR, &values);
    if (values) {
        SdaiADB value = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(values, i++, sdaiADB, &value)) {
            if (MatchValue(value, unit, ctx)) {
                return true;
            }
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchPropertyListValue(SdaiInstance prop, Context& ctx)
{
    return MatchListOfValues(prop, ctx._IfcPropertyListValue_Unit(), ctx._IfcPropertyListValue_ListValues(), ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchPropertyTableValue(SdaiInstance prop, Context& ctx)
{
    if (MatchListOfValues(prop, ctx._IfcPropertyTableValue_DefiningUnit(), ctx._IfcPropertyTableValue_DefiningValues(), ctx)) {
        return true;
    }

    return MatchListOfValues(prop, ctx._IfcPropertyTableValue_DefinedUnit(), ctx._IfcPropertyTableValue_DefinedValues(), ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchPropertySingleValue(SdaiInstance prop, Context& ctx)
{
    SdaiADB nominalValue = 0;
    sdaiGetAttr(prop, ctx._IfcPropertySingleValue_NominalValue(), sdaiADB, &nominalValue);

    SdaiInstance            unit = 0;
    sdaiGetAttr(prop, ctx._IfcPropertySingleValue_Unit(), sdaiINSTANCE, &unit);

    return MatchValue(nominalValue, unit, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchPropertyBoundedValue(SdaiInstance prop, Context& ctx)
{
    SdaiInstance            unit = 0;
    sdaiGetAttr(prop, ctx._IfcPropertyBoundedValue_Unit(), sdaiINSTANCE, &unit);

    SdaiAttr rAttr[] = { ctx._IfcPropertyBoundedValue_UpperBoundValue(), ctx._IfcPropertyBoundedValue_LowerBoundValue(), ctx._IfcPropertyBoundedValue_SetPointValue()};
    for (auto attr : rAttr) {
        SdaiADB value = 0;
        sdaiGetAttr(prop, attr, sdaiADB, &value);
        if (MatchValue(value, unit, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchValue(SdaiADB adbValue, SdaiInstance unit, Context& ctx)
{
    if (!adbValue) {
        return false;
    }

    enum_express_attr_type  attrType = enum_express_attr_type::__NONE;

    auto ifcType = sdaiGetADBTypePath(adbValue, 0);

    if (!m_datatype.empty()) {
        if (StrICmp(m_datatype.c_str(), ifcType)) {
            return false;//datatype mismatch
        }
    }

    if (ifcType) {
        auto type = sdaiGetEntity(ctx.model, ifcType);
        attrType = engiGetDefinedType(type, nullptr, nullptr);
    }

    switch (attrType) {
        case enum_express_attr_type::__BINARY:
        case enum_express_attr_type::__BINARY_32:
        {
            assert(!"to test");
            const char* value = nullptr;
            if (sdaiGetADBValue(adbValue, sdaiBINARY, &value) && value && *value)
                return m_value.Match(value, false, ctx);
            else
                return false;
        }
        case enum_express_attr_type::__STRING:
        {
            const wchar_t* value = nullptr;
            if (sdaiGetADBValue(adbValue, sdaiUNICODE, &value) && value && *value)
                return m_value.Match(value, false, ctx);
            else
                return false;
        }
        case enum_express_attr_type::__ENUMERATION:
        {
            assert(!"to test");
            const char* value = nullptr;
            if (sdaiGetADBValue(adbValue, sdaiENUM, &value) && value && *value)
                return m_value.Match(value, false, ctx);
            else
                return false;
        }
        case enum_express_attr_type::__BOOLEAN:
        {
            bool match = false;
            bool value = false;
            if (sdaiGetADBValue(adbValue, sdaiBOOLEAN, &value)) {
                match = m_value.Match(value ? "TRUE" : "FALSE", false, ctx);
            }
            return match;
        }
        case enum_express_attr_type::__INTEGER:
        {
            SdaiInteger value = 0;
            if (sdaiGetADBValue(adbValue, sdaiINTEGER, &value))
                return m_value.Match(value, ctx);
            else
                return false;
        }
        case enum_express_attr_type::__LOGICAL:
        {
            assert(!"to test");
            bool match = false;
            const char* value = nullptr;
            if (sdaiGetADBValue(adbValue, sdaiLOGICAL, &value)) {
                if (value) {
                    switch (*value) {
                        case 'T':
                        case 't':
                            match = m_value.Match("TRUE", false, ctx);
                            break;
                        case 'F':
                        case 'f':
                            match = m_value.Match("FALSE", false, ctx);
                            break;
                            //U always fails
                    }
                }
            }
            return match;
        }
        case enum_express_attr_type::__NUMBER:
        case enum_express_attr_type::__REAL:
        {
            double value = 0;
            if (sdaiGetADBValue(adbValue, sdaiREAL, &value))
                return MatchValue(value, unit, ifcType, ctx);
            else
                return false;
        }
        default:
        {
            ctx.LogMsg(MsgLevel::NotImplemented, "type of IfcValue");
            assert(0);
            return false;
        }
    }
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchValue(double value, SdaiInstance unit, const char* ifcType, Context& ctx)
{
    if (!m_datatype.empty()) {
        if (StrICmp(m_datatype.c_str(), ifcType)) { //quantities_must_also_match_the_appropriate_measure
            return false;//datatype mismatch
        }
    }

    if (!m_value.IsSet()) {
        return true; //no need to calculate units
    }

    const char* unitKind = nullptr;
    auto it = s_ifcDataTypesUnits.find(ifcType);
    if (it != s_ifcDataTypesUnits.end()) {
        unitKind = it->second.c_str();
    }

    double scale = ctx.GetUnitScale(unit, unitKind);

    value *= scale;

    return m_value.Match(value, ctx);
}

/// <summary>
/// 
/// </summary>
Facet::Matched FacetMaterial::MatchImpl(SdaiInstance inst, Context& ctx)
{
    SdaiAggr aggrAssoc = 0;
    sdaiGetAttr(inst, ctx._IfcObjectDefinition_HasAssociations(), sdaiAGGR, &aggrAssoc);

    SdaiInstance relAssoc = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(aggrAssoc, i++, sdaiINSTANCE, &relAssoc)) {

        auto entityAssoc = sdaiGetInstanceType(relAssoc);
        if (entityAssoc == ctx._IfcRelAssociatesMaterial()) {

            SdaiInstance material = 0;
            sdaiGetAttr(relAssoc, ctx._IfcRelAssociatesMaterial_RelatingMaterial(), sdaiINSTANCE, &material);
            if (material) {

                return MatchMaterialSelect(material, ctx) ? Matched::Yes : Matched::No;
            }
        }
    }

    auto type = GetTypeObject(inst, ctx);
    if (type) {
        return MatchImpl(type, ctx);
    }

    return Matched::No;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialSelect(SdaiInstance material, Context& ctx)
{
    if (!material) {
        return false;
    }

    auto entity = sdaiGetInstanceType(material);

    if (entity == ctx._IfcMaterial()) {
        return MatchMaterialSimple(material, ctx);
    }
    else if (entity == ctx._IfcMaterialLayer()) {
        return MatchMaterialLayer(material, ctx);
    }
    else if (entity == ctx._IfcMaterialLayerSet()) {
        return MatchMaterialLayerSet(material, ctx);
    }
    else if (entity == ctx._IfcMaterialProfile()) {
        return MatchMaterialProfile(material, ctx);
    }
    else if (entity == ctx._IfcMaterialProfileSet()) {
        return MatchMaterialProfileSet(material, ctx);
    }
    else if (entity == ctx._IfcMaterialConstituent()) {
        return MatchMaterialConstituent(material, ctx);
    }
    else if (entity == ctx._IfcMaterialConstituentSet()) {
        return MatchMaterialConstituentSet(material, ctx);
    }
    else if (entity == ctx._IfcMaterialLayerSetUsage()) {
        return MatchMaterialLayerSetUsage(material, ctx);
    }
    else if (entity == ctx._IfcMaterialProfileSetUsage()) {
        return MatchMaterialProfileSetUsage(material, ctx);
    }
    else if (entity == ctx._IfcMaterialList()) {
        return MatchMaterialList(material, ctx);
    }
    else {
        ctx.LogMsg(MsgLevel::NotImplemented, "Material type");
        return false;
    }
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialSimple(SdaiInstance material, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(material, ctx._IfcMaterial_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    if (ctx.GetIfcVersion() > Context::IfcVersion::Ifc2x3) {
        sdaiGetAttr(material, ctx._IfcMaterial_Category(), sdaiUNICODE, &name);
        if (m_value.Match(name, false, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialLayer(SdaiInstance layer, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(layer, ctx._IfcMaterialLayer_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    sdaiGetAttr(layer, ctx._IfcMaterialLayer_Category(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiInstance material = 0;
    sdaiGetAttr(layer, ctx._IfcMaterialLayer_Material(), sdaiINSTANCE, &material);
    return MatchMaterialSimple(material, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialLayerSet(SdaiInstance material, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(material, ctx._IfcMaterialLayerSet_LayerSetName(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiAggr layers = 0;
    sdaiGetAttr(material, ctx._IfcMaterialLayerSet_MaterialLayers(), sdaiAGGR, &layers);

    SdaiInstance layer = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(layers, i++, sdaiINSTANCE, &layer)) {
        if (MatchMaterialLayer(layer, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialLayerSetUsage(SdaiInstance material, Context& ctx)
{
    SdaiInstance layerset = 0;
    sdaiGetAttr(material, ctx._IfcMaterialLayerSetUsage_ForLayerSet(), sdaiINSTANCE, &layerset);
    return MatchMaterialLayerSet(layerset, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialProfile(SdaiInstance profile, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(profile, ctx._IfcMaterialProfile_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    sdaiGetAttr(profile, ctx._IfcMaterialProfile_Category(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiInstance material = 0;
    sdaiGetAttr(profile, ctx._IfcMaterialProfile_Material(), sdaiINSTANCE, &material);
    return MatchMaterialSimple(material, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialProfileSet(SdaiInstance material, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(material, ctx._IfcMaterialProfileSet_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiAggr profiles = 0;
    sdaiGetAttr(material, ctx._IfcMaterialProfileSet_MaterialProfiles(), sdaiAGGR, &profiles);

    SdaiInstance profile = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(profiles, i++, sdaiINSTANCE, &profile)) {
        if (MatchMaterialProfile(profile, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialProfileSetUsage(SdaiInstance material, Context& ctx)
{
    SdaiInstance profileset = 0;
    sdaiGetAttr(material, ctx._IfcMaterialProfileSetUsage_ForProfileSet(), sdaiINSTANCE, &profileset);
    return MatchMaterialLayerSet(profileset, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialConstituent(SdaiInstance constit, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(constit, ctx._IfcMaterialConstituent_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    sdaiGetAttr(constit, ctx._IfcMaterialConstituent_Category(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiInstance material = 0;
    sdaiGetAttr(constit, ctx._IfcMaterialConstituent_Material(), sdaiINSTANCE, &material);
    return MatchMaterialSimple(material, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialConstituentSet(SdaiInstance material, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(material, ctx._IfcMaterialConstituentSet_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiAggr parts = 0;
    sdaiGetAttr(material, ctx._IfcMaterialConstituentSet_MaterialConstituents(), sdaiAGGR, &parts);

    if (parts) {
        SdaiInstance part = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(parts, i++, sdaiINSTANCE, &part)) {
            if (MatchMaterialConstituent(part, ctx)) {
                return true;
            }
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialList(SdaiInstance material, Context& ctx)
{
    SdaiAggr parts = 0;
    sdaiGetAttr(material, ctx._IfcMaterialList_Materials(), sdaiAGGR, &parts);

    SdaiInstance part = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(parts, i++, sdaiINSTANCE, &part)) {
        if (MatchMaterialSimple(part, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
void MultiTypeValueCache::Get(double* pval) 
{ 
    if (!m_dSet) {
        m_dSet = true;

        char* end = 0;
        
        m_dVal = strtod(m_strVal.c_str(), &end);

        if (!end || *end) {
            m_dVal = FLT_MAX;
        }
    }
    
    *pval = m_dVal; 
}

/// <summary>
/// 
/// </summary>
void MultiTypeValueCache::Get(SdaiInteger* pval) 
{ 
    if (!m_iSet) { 
        m_iSet = true; 
    
        char* end = 0;

        m_iVal = strtol(m_strVal.c_str(), &end, 10);

        if (!end || *end) {
            m_iVal = MAXLONG;
        }
    }
    
    *pval = m_iVal; 
}

/// <summary>
/// 
/// </summary>
struct ComparerStr
{
    ComparerStr(bool compareNoCase) : m_compareNoCase(compareNoCase) {}

    int compare(const char* v1, const char* v2)
    {
        if (m_compareNoCase) {
            return StrICmp(v1, v2);
        }
        else {
            return strcmp(v1, v2);
        }
    }

    const char* GetString(const char* v)
    {
        return v;
    }

    bool m_compareNoCase;
};

/// <summary>
/// 
/// </summary>
struct ComparerFloat
{
    ComparerFloat(double precision) : m_precision(precision) {}

    int compare(double v1, double v2)
    {
        if (v1 <= v2 * (1 - m_precision) - m_precision)
            return -1;
        else if (v1 >= v2 * (1 + m_precision) + m_precision)
            return 1;
        else
            return 0;
    }

    const char* GetString(double)
    {
        //assert(0); //not expected to use
        //snprintf(m_buff, 79, "%g", v);
        //return m_buff;
        return nullptr;
    }

    double m_precision;
    //char   m_buff[80];
};

#if 0
/// <summary>
/// 
/// </summary>
struct ComparerInt
{
    int compare(SdaiInteger v1, SdaiInteger v2)
    {
        if (v1 < v2)
            return -1;
        else if (v1 > v2)
            return 1;
        else
            return 0;
    }
};
#endif

/// <summary>
/// 
/// </summary>
template <typename T, class Comparer>
bool IdsValue::MatchValue(T value, Comparer& cmp)
{
    //
    if (!m_simpleValue.empty()) {
        T v;
        m_simpleVal.Get(&v);
        if (0!=cmp.compare(value, v)) {
            return false;
        }
    }


    //
    for (auto& rest : m_restrictions) {
        if (!rest->Fit(value, cmp)) {
            return false;
        }
    }

    return true;
}

/// <summary>
/// 
/// </summary>
bool IdsValue::Match(const wchar_t* value, bool compareNoCase, Context& ctx)
{
    if (value) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string utf8 = converter.to_bytes(value);
        return Match(utf8.c_str(), compareNoCase, ctx);
    }
    else {
        return Match ((const char*) nullptr, compareNoCase, ctx);
    }
}

/// <summary>
/// 
/// </summary>
bool IdsValue::Match(const char* value, bool compareNoCase, Context&)
{
    if (!m_isSet) {
        return true;
    }
    if (!value || !*value) {
        return false;
    }

    ComparerStr cmp(compareNoCase);
    
    return MatchValue(value, cmp);
}

/// <summary>
/// 
/// </summary>
bool IdsValue::Match(SdaiInteger value, Context& ctx)
{
#if 1
    //pass-integers_follow_the_same_rules_as_numbers_2_2
    return Match((double)value, ctx);
#else
    if (!m_isSet) {
        return true;
    }

    ComparerInt cmp;
 
    return MatchValue(value, cmp);
#endif
}

/// <summary>
/// 
/// </summary>
bool IdsValue::Match(double value, Context& ctx)
{
    if (!m_isSet) {
        return true;
    }

    ComparerFloat cmp (ctx.Precision());

    return MatchValue(value, cmp);
}

/// <summary>
/// 
/// </summary>
template <typename T, class Comparer> bool Restriction::Fit(T value, Comparer& cmp)
{
    //
    if (!m_enumeration.empty()) {
        bool match = false;
        for (auto& val : m_enumeration) {
            T v = 0;
            val->Get(&v);
            if (0 == cmp.compare(value, v)) {
                match = true;
                break;
            }
        }
        if (!match) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& patt : m_pattern) {
        const char* r = nullptr;
        patt->Get(&r);
        auto str = cmp.GetString(value);
        if (!str) {
            return false; //>>>>>>
        }
        bool match = std::regex_match(str, std::regex(r));
        if (!match) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_minInclusive) {
        T v = 0;
        r->Get(&v);
        auto c = cmp.compare(value, v);
        if (c == -1) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_maxInclusive) {
        T v = 0;
        r->Get(&v);
        auto c = cmp.compare(value, v);
        if (c == 1) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_minExclusive) {
        T v = 0;
        r->Get(&v);
        auto c = cmp.compare(value, v);
        if (c != 1) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_maxExclusive) {
        T v = 0;
        r->Get(&v);
        auto c = cmp.compare(value, v);
        if (c != -1) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_length) {
        SdaiInteger len = 0;
        r->Get(&len);
        auto str = cmp.GetString(value);
        if (!str) {
            return false; //>>>>>>
        }
        if ((SdaiInteger)strlen(str) != len) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_minLength) {
        SdaiInteger len = 0;
        r->Get(&len);
        auto str = cmp.GetString(value);
        if (!str) {
            return false; //>>>>>>
        }
        if ((SdaiInteger)strlen(str) < len) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_maxLength) {
        SdaiInteger len = 0;
        r->Get(&len);
        auto str = cmp.GetString(value);
        if (!str) {
            return false; //>>>>>>
        }
        if ((SdaiInteger)strlen(str) > len) {
            return false; //>>>>>>
        }
    }

    return true;
}

