#pragma once

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
    Console&    console;
    MsgLevel    msgLevel;
    bool        stopAtFirstError;

    SdaiModel       model = 0;
    SdaiInstance    currentInstane = 0;    
    Specification*  currentSpecification = nullptr;

private:
#if 0
    double GetDerivedUnitScale(SdaiInstance unit);
    double GetContexDependentUnitScale(SdaiInstance unit);
    double GetConversionBasedUnitScale(SdaiInstance unit);
    double GetSIUnitScale(SdaiInstance unit);
#endif
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
double Context::GetUnitScale(SdaiInstance unit, const char* unitKind)
{
    if (unit) {
        return getIfcUnitConversionFactor(unit, nullptr, nullptr, nullptr);
    }
    else if (unitKind) {
        return getUnitConversionFactor(model, unitKind, nullptr, nullptr, nullptr);
    }
    else {
        return 1;
    }
#if 0
    auto entity = sdaiGetInstanceType(unit);

    if (entity == _IfcDerivedUnit()) {
        return GetDerivedUnitScale(unit);
    }
    else if (entity == _IfcMonetaryUnit()) {
        LogMsg(*this, MsgLevel::Warning, "Can not scale monetary units");
        return 1;
    }
    else if (entity == _IfcContextDependentUnit()) {
        return GetContexDependentUnitScale(unit);
    }
    else if (entity == _IfcConversionBasedUnit()) {
        return GetConversionBasedUnitScale(unit);
    }
    else if (entity == _IfcSIUnit()) {
        return GetSIUnitScale(unit);
    }
    else {
        LogMsg(*this, MsgLevel::NotImplemented, "Unknown unit class");
    }
#endif
}

#if 0
/// <summary>
/// 
/// </summary>
SdaiInstance Context::FindProjectUnit(const char* unitKind)
{
}

/// <summary>
/// 
/// </summary>
static double powInt(double val, int exp)
{
    if (fabs(val) < FLT_MIN) {
        return 0;
    }

    if (exp < 0) {
        val = 1 / val;
        exp = -exp;
    }
    
    double res = 1.0;
    for (; exp > 0; exp--) {
        res *= val;
    }

    return res;
}

/// <summary>
/// 
/// </summary>
double Context::GetDerivedUnitScale(SdaiInstance unit)
{
    double factor = 1;

    SdaiAggr elems = 0;
    sdaiGetAttr(unit, _IfcDerivedUnit_Elements(), sdaiAGGR, &elems);
    
    SdaiInstance elem = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(elems, i, sdaiINSTANCE, &elem)) {
        
        SdaiInstance elemUnit = 0;
        sdaiGetAttr(elem, _IfcDerivedUnitElement_Unit(), sdaiINSTANCE, &elemUnit);

        double elemScale = GetUnitScale(elemUnit);

        SdaiInteger exponent = 0;
        sdaiGetAttr(elem, _IfcDerivedUnitElement_Exponent(), sdaiINTEGER, &exponent);

        elemScale = powInt(elemScale, exponent);

        factor *= elemScale;
    }

    return factor;
}


/// <summary>
/// 
/// </summary>
double Context::GetContexDependentUnitScale(SdaiInstance unit)
{
}

/// <summary>
/// 
/// </summary>
double Context::GetConversionBasedUnitScale(SdaiInstance unit)
{
}

/// <summary>
/// 
/// </summary>
double Context::GetSIUnitScale(SdaiInstance unit)
{
}

#endif