
#ifndef __RDF_LTD__PSD_H
#define __RDF_LTD__PSD_H

namespace RDF
{
    namespace PSD
    {
        const double DBLTOL = 1e-5;

        /// <summary>
        /// 
        /// </summary>
        enum class ErrorLevel { OK = 0, Warning = 1, Error = 2, DebugTrace = -1 };

        /// <summary>
        /// implement it if you wnat redirect output 
        /// </summary>
        struct Console
        {
           //default formater calls writeString(msg)
           virtual void formatOutput(ErrorLevel level, const char* message, SdaiInstance inst1, SdaiInstance inst2, SdaiInstance inst3);

           //default NUL output
           virtual void writeString(const char* /*msg*/) {}
        };

        /// <summary>
        /// 
        /// </summary>
        enum IfcVersion { Ifc2x3 = 0, Ifc4 = 1, Ifc4x3 = 2 };

        /// <summary>
        /// 
        /// </summary>
        class PropertyDef;
        struct PropertyDefs : std::map<std::string, PropertyDef>
        {
            void LoadPropertyDef(_xml::_element& propertyDef);
        };

        /// <summary>
        /// 
        /// </summary>
        class PropertyDef
        {
        public:
            void LoadType(_xml::_element& propertyType);
            void LoadQtoType(_xml::_element& qtoType);

        public:
            enum class TypeProperty
            {
                Undef, SingleValue, EnumeratedValue, BoundedValue, TableValue, ReferenceValue, ListValue, ComplexProperty, Quantity
            };

            struct ListValue
            {
                struct dblless { bool operator()(double x, double y) const { return y - x > DBLTOL; } };

                std::string dataType;
                std::string unitType;
                std::set<std::string>      sitems;
                std::set<double, dblless>  ditems;
            };

        public:
            TypeProperty m_typeProperty = TypeProperty::Undef;

            //SingleValue
            std::string             m_dataType;
            std::string             m_unitType;

            //BoundedValue (adds to SingleValue)
            std::string             m_lowerBoundValue;
            std::string             m_upperBoundValue;

            //EnumeratedValue
            std::string             m_enumListName;
            std::set<std::string>   m_enumListItems;

            //ReferenceValue
            std::string             m_reftype;
            std::string             m_guid;
            std::string             m_URL;
            std::string             m_libraryname;
            std::string             m_sectionref;

            //TableValue
            std::string             m_Expression;
            ListValue               m_definingValue;
            ListValue               m_definedValue;

            //ListValue
            ListValue               m_listValue;

            //ComplexProperty
            std::string             m_complexName;
            PropertyDefs            m_complexParts;

            //Quantity
            std::string             m_qtoEntity;

        private:
            void LoadTypeSingleOrBoundedValue(_xml::_element& typeProperty);
            void LoadValueRangeDef(_xml::_element& range);
            void LoadTypeEnumeratedValue(_xml::_element& typeProperty);
            void LoadTypeReferenceValue(_xml::_element& typeProperty);
            void LoadTypeTableValue(_xml::_element& typeProperty);
            void LoadListValue(_xml::_element& elemValue, ListValue& xxxValue);
            void LoadTypeListValue(_xml::_element& typeProperty);
            void LoadTypeComplexProperty(_xml::_element& typeProperty);
            void GetAttrValue(std::string& val, _xml::_element& elem, const char* attrName, bool required = true);
        };


        /// <summary>
        /// 
        /// </summary>
        class PropertySetDef 
        {
        public:
            enum class LoadState { Not, Tried, OK };

        public:
            void Load(const char* psdFilePath, bool standard);// throw(std::exception);
            LoadState GetLoadState() { return m_loadState; }

            bool IsClassApplicable(SdaiInstance inst);
            PropertyDef* GetPropertyDef(const char* name);

        private:
            void Load(_xml::_element& root);
            void AddApplicableClass(_xml::_element& className);

        private:
            LoadState              m_loadState;

            std::list<std::string> m_applicableClassNames;
            PropertyDefs           m_defs;

            std::set<SdaiEntity>   m_applicableEntities;
            SdaiModel              m_model = NULL;
        };

        typedef std::map<std::string, PropertySetDef> PropertySetDefs;


        /// <summary>
        /// 
        /// </summary>
        class Checker
        {
        public:
            Checker(const char* psdFolder, Console* console = NULL);
            ~Checker();

            void Check(const char* ifcFilePath);
            void Check(SdaiModel model);

            ErrorLevel GetErrorLevel() { return m_level; }

        private:
            void InitEntities();
            IfcVersion GetIfcVersion(SdaiModel model);

            void AddLogMsg(ErrorLevel level, const char* message, SdaiInstance inst1=NULL, SdaiInstance inst2=NULL, SdaiInstance inst3=NULL);

            void CheckObjectsApplicability();
            void CheckApplicability(SdaiInstance obj, SdaiInstance pset);

            void CheckPropertyDefinitions();
            void CheckPropertyDefinition(SdaiInstance pdef);
            void CheckPropertySet(SdaiInstance pset, PropertySetDef& psd);
            void CheckElementQuantity(SdaiInstance qset, PropertySetDef& psd);

            void CheckTypes();

            void CheckProperty(SdaiInstance pset, SdaiInstance prop, PropertySetDef& psd);
            void CheckQuantity(SdaiInstance qset, SdaiInstance qto, PropertySetDef& psd);

            PropertySetDef* GetPSD(const char* psetName);
            void LoadPSD(const char* psetName, PropertySetDef& psd);

            void CheckPropertySingleValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef);
            void CheckPropertyEnumeratedValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef);
            void CheckPropertyBoundedValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef);
            void CheckPropertyTableValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef);
            void CheckPropertyReferenceValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef);
            void CheckPropertyListValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef);
            void CheckPropertyComplexValue(SdaiInstance pset, SdaiInstance prop, PropertyDef& pdef);

            void CheckListValue(SdaiInstance pset, SdaiInstance prop, SdaiAttr attr, PropertyDef::ListValue& def);            
            void CheckDataType(SdaiInstance pset, SdaiInstance prop, SdaiAttr attr, const std::string& dataType);
            void CheckUnitType(SdaiInstance pset, SdaiInstance prop, SdaiAttr attr, const std::string& unitType);
            void CheckBoundLimit(SdaiInstance pset, SdaiInstance prop, SdaiAttr attr, const std::string& bound, bool lower);

            std::string Format(const char* format, ...);

        private:
            std::string  m_psdFolder;
            Console&    m_console;    

            ErrorLevel   m_level = ErrorLevel::OK;

            SdaiModel    m_model = NULL;
            IfcVersion   m_ifcVersion = IfcVersion::Ifc2x3;

            SdaiEntity   m_entityRoot = NULL;
            SdaiEntity   m_entityPropertyDefinition = NULL;
            SdaiEntity   m_entityPropertySetDefinition = NULL;
            SdaiEntity   m_entityPropertySet = NULL;
            SdaiEntity   m_entityElementQuantity = NULL;
            SdaiEntity   m_entityPropertySetTemplate = NULL;
            SdaiEntity   m_entityRelDefinesByProperties = NULL;
            SdaiEntity   m_entityTypeObject = NULL;
            SdaiEntity   m_entityObject = NULL;
            SdaiEntity   m_entityProperty = NULL;
            SdaiEntity   m_entityPhysicalQuantity = NULL;
            SdaiEntity   m_entityPropertySingleValue = NULL;
            SdaiEntity   m_entityNamedUnit = NULL;
            SdaiEntity   m_entityDerivedUnit = NULL;
            SdaiEntity   m_entityMonetaryUnit = NULL;
            SdaiEntity   m_entityPropertyEnumeratedValue = NULL;
            SdaiEntity   m_entityPropertyTableValue = NULL;
            SdaiEntity   m_entityPropertyListValue = NULL;
            SdaiEntity   m_entityPropertyBoundedValue = NULL;

            SdaiAttr m_attrRoot_Name = NULL;
            SdaiAttr m_attrPropertySet_HasProperties = NULL;
            SdaiAttr m_attrElementQuantity_Quantities = NULL;
            SdaiAttr m_attrRelDefinesByProperties_RelatingPropertyDefinition = NULL;
            SdaiAttr m_attrRelDefinesByProperties_RelatedObjects = NULL;
            SdaiAttr m_attrTypeObject_HasPropertySets = NULL;
            SdaiAttr m_attrProperty_Name = NULL;
            SdaiAttr m_attrPhysicalQuantity_Name = NULL;
            SdaiAttr m_attrPropertySingleValue_NominalValue = NULL;
            SdaiAttr m_attrPropertySingleValue_Unit = NULL;
            SdaiAttr m_attrNamedUnit_UnitType = NULL;
            SdaiAttr m_attrDerivedUnit_UnitType = NULL;
            SdaiAttr m_attrPropertyEnumeratedValue_EnumerationValues = NULL;
            SdaiAttr m_attrPropertyTableValue_DefiningValues = NULL;
            SdaiAttr m_attrPropertyTableValue_DefinedValues = NULL;
            SdaiAttr m_attrPropertyTableValue_DefiningUnit = NULL;
            SdaiAttr m_attrPropertyTableValue_DefinedUnit = NULL;
            SdaiAttr m_attrPropertyListValue_ListValues = NULL;
            SdaiAttr m_attrPropertyListValue_Unit = NULL;
            SdaiAttr m_attrPropertyBoundedValue_UpperBoundValue = NULL;
            SdaiAttr m_attrPropertyBoundedValue_LowerBoundValue = NULL;
            SdaiAttr m_attrPropertyBoundedValue_Unit = NULL;

            PropertySetDefs  m_PSDs[3]; //by IfcVersion
        };
    }
}

#endif
