#pragma once


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
    IfcVersion GetIfcVersion();

public:
    Console& console;
    MsgLevel    msgLevel;
    bool        stopAtFirstError;

    SdaiModel       model = 0;
    SdaiInstance    currentInstane = 0;    Specification* currentSpecification = nullptr;

private:
    IfcVersion m_ifcVersion = IfcVersion::NotItitialized;

    ENTITY(IfcRoot);
    ENTITY(IfcObjectDefinition);
    ENTITY(IfcRelAssociatesClassification);
    ENTITY(IfcClassificationReference);
    ENTITY(IfcExternalReference);
    ENTITY(IfcClassification);
    ENTITY(IfcClassificationNotation);
    ENTITY(IfcRelDecomposes);
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

    ENTITY(IfcProperty);
    ENTITY(IfcComplexProperty);
    ENTITY(IfcPropertyBoundedValue);
    ENTITY(IfcPropertyEnumeratedValue);
    ENTITY(IfcPropertyListValue);
    ENTITY(IfcPropertyReferenceValue);
    ENTITY(IfcPropertySingleValue);
    ENTITY(IfcPropertyTableValue);

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

    ATTR(IfcRoot, Name, IfcRoot_Name);
    ATTR(IfcObjectDefinition, IsDecomposedBy, IfcObjectDefinition_IsDecomposedBy);
    ATTR(IfcObjectDefinition, HasAssignments, IfcObjectDefinition_HasAssignments);
    ATTR(IfcRelAssociatesClassification, RelatingClassification, IfcRelAssociatesClassification_RelatingClassification);
    ATTR(IfcRelDecomposes, RelatingObject, IfcRelDecomposes_RelatingObject);
    ATTR(IfcRelAssignsToGroup, RelatedGroup, IfcRelAssignsToGroup_RelatedGroup);
    ATTR(IfcFeatureElementSubtraction, VoidsElements, IfcFeatureElementSubtraction_VoidsElements);
    ATTR(IfcRelVoidsElement, RelatingBuildingElement, IfcRelVoidsElement_RelatingBuildingElement);
    ATTR(IfcElement, FillsVoids, IfcElement_FillsVoids);
    ATTR(IfcRelFillsElement, RelatingOpeningElement, IfcRelFillsElement_RelatingOpeningElement);
    ATTR(IfcRelContainedInSpatialStructure, RelatingStructure, IfcRelContainedInSpatialStructure_RelatingStructure);
    ATTR(IfcRelContainedInSpatialStructure, RelatedElements, IfcRelContainedInSpatialStructure_RelatedElements);
    ATTR(IfcExternalReference, Identification, IfcExternalReference_Identification);
    ATTR(IfcExternalReference, ItemReference, IfcExternalReference_ItemReference);
    ATTR(IfcExternalReference, Location, IfcExternalReference_Location);
    ATTR(IfcClassificationReference, ReferencedSource, IfcClassificationReference_ReferencedSource);
    ATTR(IfcClassification, Name, IfcClassification_Name);
    ATTR(IfcClassification, Location, IfcClassification_Location);
    ATTR(IfcClassification, Specification, IfcClassification_Specification);
    ATTR(IfcClassification, Source, IfcClassification_Source);
    ATTR(IfcObject, IsDefinedBy, IfcObject_IsDefinedBy);
    ATTR(IfcTypeObject, HasPropertySets, IfcTypeObject_HasPropertySets);
    ATTR(IfcContext, IsDefinedBy, IfcContext_IsDefinedBy);
    ATTR(IfcRelDefinesByProperties, RelatingPropertyDefinition, IfcRelDefinesByProperties_RelatingPropertyDefinition);
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
    ATTR(IfcQuantittyWeight, WeightValue, IfcQuantityWeight_WeightValue);
    ATTR(IfcPropertySingleValue, NominalValue, IfcPropertySingleValue_NominalValue);
    ATTR(IfcPropertySingleValue, Unit, IfcPropertySingleValue_Unit);
};

/// <summary>
/// 
/// </summary>
struct IfcDataType
{
    int_t sdaiType;
    std::string unitType;
    
    IfcDataType(int_t sdaiType_, const char* unitType_) : sdaiType(sdaiType_), unitType(unitType_) {}
};

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