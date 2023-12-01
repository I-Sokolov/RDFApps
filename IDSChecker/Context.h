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
    ATTR(IfcQuantityWeight, WeightValue, IfcQuantityWeight_WeightValue);
};

