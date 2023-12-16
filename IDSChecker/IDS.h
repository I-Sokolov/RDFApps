//
//  Author:  Peter Bonsma
//  $Date:  $
//  $Revision:  
//  Project: IFC Engine Library
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@rdf.bg
//

#ifndef __RDF_LTD__IDS_H
#define __RDF_LTD__IDS_H

// Implements buildingSMART Information Delivery Specification
// https://github.com/buildingSMART/IDS/tree/master/Documentation

#include "_xml.h"
#include "ifcengine.h"

namespace RDF
{
    namespace IDS
    {
        class Context;

        /// <summary>
        /// 
        /// </summary>
        template <typename T> class OwningPtrList : public std::list<T*>
        {
        public:
            ~OwningPtrList() { clear(); }

            void clear()
            {
                for (auto p : *this) {
                    delete p;
                }
                std::list<T*>::clear();
            }
        };

        /// <summary>
        /// 
        /// </summary>
        class MultiTypeValueCache
        {
        public:
            MultiTypeValueCache(std::string& strVal) : m_strVal (strVal){}

            void Get(const char** pval)     { *pval = m_strVal.c_str(); }
            void Get(double* pval);
            void Get(SdaiInteger* pval);

        private:
            std::string& m_strVal;

            double       m_dVal = 0;
            bool         m_dSet = false;

            SdaiInteger  m_iVal = 0;
            bool         m_iSet = false;
        };

        /// <summary>
        /// 
        /// </summary>
        class Value
        {
        public:
            Value(_xml::_element& elem, Context& ctx);

            template<typename T> void Get(T* pval) { m_val.Get(pval); }

        private:
            std::string m_value;
            std::string m_fixed;

            MultiTypeValueCache m_val;
        };

        /// <summary>
        /// 
        /// </summary>
        class Restriction
        {
        public:
            Restriction(_xml::_element& elem, Context& ctx);
         
            template <typename T, class Comparer> bool Fit(T value, Comparer& cmp);

        private:
            void Read_enumeration(_xml::_element& elem, Context& ctx) { m_enumeration.push_back(new Value(elem, ctx)); }
            void Read_pattern(_xml::_element& elem, Context& ctx) { m_pattern.push_back(new Value(elem, ctx)); }
            void Read_minInclusive(_xml::_element& elem, Context& ctx) { m_minInclusive.push_back(new Value(elem, ctx)); }
            void Read_maxInclusive(_xml::_element& elem, Context& ctx) { m_maxInclusive.push_back(new Value(elem, ctx)); }
            void Read_minExclusive(_xml::_element& elem, Context& ctx) { m_minExclusive.push_back(new Value(elem, ctx)); }
            void Read_maxExclusive(_xml::_element& elem, Context& ctx) { m_maxExclusive.push_back(new Value(elem, ctx)); }
            void Read_length(_xml::_element& elem, Context& ctx) { m_length.push_back(new Value(elem, ctx)); }
            void Read_minLength(_xml::_element& elem, Context& ctx) { m_minLength.push_back(new Value(elem, ctx)); }
            void Read_maxLength(_xml::_element& elem, Context& ctx) { m_maxLength.push_back(new Value(elem, ctx)); }

        private:
            std::string             m_base;
            OwningPtrList<Value>    m_enumeration;
            OwningPtrList<Value>    m_pattern;
            OwningPtrList<Value>    m_minInclusive;
            OwningPtrList<Value>    m_maxInclusive;
            OwningPtrList<Value>    m_minExclusive;
            OwningPtrList<Value>    m_maxExclusive;
            OwningPtrList<Value>    m_length;
            OwningPtrList<Value>    m_minLength;
            OwningPtrList<Value>    m_maxLength;
        };

        /// <summary>
        /// 
        /// </summary>
        class IdsValue
        {
        public:
            IdsValue() : m_simpleVal(m_simpleValue) {}

            void Read(_xml::_element& elem, Context& ctx);

            bool IsSet() { return m_isSet; }
            const char* GetSimpleValue() { if (m_isSet && m_restrictions.empty()) return m_simpleValue.c_str(); else return nullptr; }

            //unset value matches to everything including NULL string
            bool Match(const char* value, bool compareNoCase, Context& ctx);
            bool Match(const wchar_t* value, bool compareNoCase, Context& ctx);
            bool Match(SdaiInteger value, Context& ctx);
            bool Match(double value, Context& ctx);
            bool Match(bool value, Context& ctx) { return Match((SdaiInteger)value, ctx); }

        private:
            void Read_simpleValue(_xml::_element& elem, Context&) { m_simpleValue = elem.getContent(); }
            void Read_restriction(_xml::_element& elem, Context& ctx) { m_restrictions.push_back(new Restriction(elem, ctx)); }

        private:
            template <typename T, class Comparer> bool MatchValue(T value, Comparer& cmp);

        private:
            bool                       m_isSet = false;
            std::string                m_simpleValue;
            OwningPtrList<Restriction> m_restrictions;

            MultiTypeValueCache        m_simpleVal;
        };

        /// <summary>
        /// 
        /// </summary>
        class Facet
        {
        public:
            virtual ~Facet() {}

            void ResetCache();
            bool Match(SdaiInstance inst, Context& ctx);

        protected:
            Facet() : m_minOccursVal(m_minOccurs), m_maxOccursVal(m_maxOccurs) {}
            Facet(_xml::_element& elem, Context& ctx) : m_minOccursVal(m_minOccurs), m_maxOccursVal(m_maxOccurs) { Read(elem, ctx); }

            virtual void ResetCacheImpl() = 0;
            virtual bool MatchImpl(SdaiInstance inst, Context& ctx) = 0;

        protected:
            void Read(_xml::_element& elem, Context& ctx);

        protected:
            std::string m_minOccurs;
            std::string m_maxOccurs;
            std::string m_datatype;
            std::string m_relation; //for partOf

        private:
            MultiTypeValueCache m_minOccursVal;
            MultiTypeValueCache m_maxOccursVal;
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetEntity : public Facet
        {
        public:
            FacetEntity() {}
            FacetEntity(_xml::_element& elem, Context& ctx) { Read(elem, ctx); }

            void Read(_xml::_element& elem, Context& ctx);

        protected:
            virtual void ResetCacheImpl() override;
            virtual bool MatchImpl(SdaiInstance inst, Context& ctx) override;

        private:
            bool MatchPredefinedType(SdaiInstance inst, Context& ctx);

        private:
            IdsValue m_name;
            IdsValue m_predefinedType;

            SdaiEntity  m_sdaiEntity = 0;
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetPartOf : public Facet
        {
        public:
            FacetPartOf(_xml::_element& elem, Context& ctx);

        protected:
            virtual void ResetCacheImpl() override;
            virtual bool MatchImpl(SdaiInstance inst, Context& ctx) override;

        private:
            struct Navigator
            {
                void Follow(SdaiInstance inst, std::list<SdaiInstance>& follow, Context& ctx);

                SdaiAttr            attrRelation = 0;   //one of this
                const char*         attrRelationBN = 0;

                SdaiPrimitiveType   sdaiType = 0;

                SdaiEntity          relClass = 0;

                SdaiAttr            attrParent = 0;    //one of this
                const char*         attrParentBN = 0;

                bool                canMatch = true;

            private:
                void FollowRel(SdaiInstance rel, std::list<SdaiInstance>& follow);
            };

            typedef OwningPtrList<Navigator> Navigators;

        private:
            void FillParentsNavigators(Context& ctx);
            void CreateNavigator(SdaiAttr attrRelation, SdaiPrimitiveType sdaiType, SdaiEntity relClass, SdaiAttr attrParent, Context& ctx, bool canMatch = true);
            void CreateNavigatorBN(const char* attrRelation, SdaiPrimitiveType sdaiType, SdaiEntity relClass, SdaiAttr attrParent, Context& ctx, bool canMatch = true);
            void CreateNavigatorBN2(SdaiAttr attrRelation, SdaiPrimitiveType sdaiType, SdaiEntity relClass, const char* attrParent, Context& ctx, bool canMatch = true);

        private:
            FacetEntity         m_entity;
            Navigators          m_navigations;
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetClassification : public Facet
        {
        public:
            FacetClassification(_xml::_element& elem, Context& ctx);

        protected:
            virtual void ResetCacheImpl() override;
            virtual bool MatchImpl(SdaiInstance inst, Context& ctx) override;

        private:
            struct Reference //One classification reference
            {
                std::wstring            system;
                std::list<std::wstring> items;
                std::list<std::wstring> URI;
            };

            typedef std::map<std::wstring, Reference> References; //all references sorted by system

        private:
            void CollectIfcRelAssociatesClassification(SdaiInstance inst, Context& ctx, References& references);
            void CollectIfcExternalReferenceRelationship(SdaiInstance inst, Context& ctx, References& references);

            void HandleClassificationSelect(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference);
            void HandleClassificationReference(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference);
            void HandleClassification(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference);
            void HandleClassificationNotation(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference);

            bool Match(References& references, Context& ctx);

        private:
            IdsValue m_value;
            IdsValue m_system;
            IdsValue m_URI;
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetAttribute : public Facet
        {
        public:
            FacetAttribute(_xml::_element& elem, Context& ctx);

        protected:
            virtual void ResetCacheImpl() override {};
            virtual bool MatchImpl(SdaiInstance inst, Context& ctx) override;

        private:
            bool MatchAttribute(SdaiInstance inst, SdaiAttr attr, Context& ctx);
            bool MatchAggr(SdaiAggr aggr, Context& ctx);
            bool MatchInstance() { return !m_value.IsSet(); }
            bool MatchADB(SdaiADB adb, Context& ctx);

        private:
            IdsValue   m_name;
            IdsValue   m_value;
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetProperty : public Facet
        {
        public:
            FacetProperty(_xml::_element& elem, Context& ctx);

        protected:
            virtual void ResetCacheImpl() override {}
            virtual bool MatchImpl(SdaiInstance inst, Context& ctx) override;

        private:
            bool TestProperties(SdaiInstance inst, Context& ctx, std::set<std::wstring>& testedProps);

            bool TestInSetOfPSDef(SdaiAggr aggr, Context& ctx, std::set<std::wstring>& testedProps);
            bool TestInSetOfRel(SdaiAggr aggr, Context& ctx, std::set<std::wstring>& testedProps);
            bool TestInPSDef(SdaiInstance inst, Context& ctx, std::set<std::wstring>& testedProps);

            bool TestProperty(SdaiInstance prop, Context& ctx, const wchar_t* pset, std::set<std::wstring>& testedProps, bool& propNameMatched);
            bool MatchPropertySingleValue(SdaiInstance prop, Context& ctx);
            bool MatchPropertyEnumeratedValue(SdaiInstance prop, Context& ctx);
            bool MatchPropertyBoundedValue(SdaiInstance prop, Context& ctx);
            bool MatchPropertyListValue(SdaiInstance prop, Context& ctx);

            bool TestQuantity(SdaiInstance qto, Context& ctx, const wchar_t* pset, std::set<std::wstring>& testedProps, bool& propNameMatched);
            
            bool MatchValue(SdaiADB adbValue, SdaiInstance unit, Context& ctx);
            bool MatchValue(double value, SdaiInstance unit, const char* ifcType, Context& ctx);
            
        private:
            IdsValue m_propertySet;
            IdsValue m_name;
            IdsValue m_value;
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetMaterial : public Facet
        {
        public:
            FacetMaterial(_xml::_element& elem, Context& ctx);

        protected:
            virtual void ResetCacheImpl() override {}
            virtual bool MatchImpl(SdaiInstance inst, Context& ctx) override;

        private:
            bool MatchMaterialSelect(SdaiInstance material, Context& ctx);
            bool MatchMaterialSimple(SdaiInstance material, Context& ctx);
            bool MatchMaterialLayer(SdaiInstance material, Context& ctx);
            bool MatchMaterialLayerSet(SdaiInstance material, Context& ctx);
            bool MatchMaterialLayerSetUsage(SdaiInstance material, Context& ctx);
            bool MatchMaterialProfile(SdaiInstance material, Context& ctx);
            bool MatchMaterialProfileSet(SdaiInstance material, Context& ctx);
            bool MatchMaterialProfileSetUsage(SdaiInstance material, Context& ctx);
            bool MatchMaterialConstituent(SdaiInstance material, Context& ctx);
            bool MatchMaterialConstituentSet(SdaiInstance material, Context& ctx);
            bool MatchMaterialList(SdaiInstance material, Context& ctx);

        private:
            IdsValue m_value;
        };

        /// <summary>
        /// 
        /// </summary>
        class Facets
        {
        public:
            void Read(_xml::_element& elem, Context& ctx);
            void ResetCache() { for (auto f : m_facets) { if (f) f->ResetCache(); } }
            bool Match(SdaiInstance inst, Context& ctx);

        private:
            void Read_entity(_xml::_element& elem, Context& ctx) { m_facets.push_back(new FacetEntity(elem, ctx)); }
            void Read_partOf(_xml::_element& elem, Context& ctx) { m_facets.push_back(new FacetPartOf(elem, ctx)); }
            void Read_classification(_xml::_element& elem, Context& ctx) { m_facets.push_back(new FacetClassification(elem, ctx)); }
            void Read_attribute(_xml::_element& elem, Context& ctx) { m_facets.push_back(new FacetAttribute(elem, ctx)); }
            void Read_property(_xml::_element& elem, Context& ctx) { m_facets.push_back(new FacetProperty(elem, ctx)); }
            void Read_material(_xml::_element& elem, Context& ctx) { m_facets.push_back(new FacetMaterial(elem, ctx)); }

        private:
            OwningPtrList<Facet> m_facets;
        };

        /// <summary>
        /// 
        /// </summary>
        class Applicability : public Facets
        {
        };

        /// <summary>
        /// 
        /// </summary>
        class Requirements : public Facets
        {
        public:
            void Read(_xml::_element& elem, Context& ctx);

        private:
            std::string m_description;
        };

        /// <summary>
        /// 
        /// </summary>
        class Specification
        {
        public:  
            Specification(_xml::_element& elem, Context& ctx);

        public:
            void ResetCache();
            bool Check(SdaiInstance inst, Context& ctx);
            bool CheckUsed(Context& ctx);

            std::string DisplayName();

        private:
            bool SuitableIfcVersion(Context& ctx);

        private:
            std::string m_name;
            std::string m_minOccurs;
            std::string m_maxOccurs;
            std::string m_ifcVersion;
            std::string m_identifier;
            std::string m_description;
            std::string m_instructions;

            Applicability m_applicability;
            Requirements  m_requirements;
            
            SdaiInteger   m_nOccurs = 0;
            bool          m_ifcVersionChecked = false;

            MultiTypeValueCache m_minOccursVal;
            MultiTypeValueCache m_maxOccursVal;
        };

        /// <summary>
        /// implement it if you wnat redirect output 
        /// </summary>
        class Console
        {
        public:
            virtual void out(const char* sz) = NULL;
        };

        /// <summary>
        /// 
        /// </summary>
        enum class MsgLevel { All, Info, Warning, Error, NotImplemented };

        /// <summary>
        /// 
        /// </summary>
        class File
        {
        public:
            bool Read(const char* idsFilePath);
            bool Check(const char* ifcFilePath, bool stopAtFirstError, MsgLevel msgLevel, Console* output = nullptr);

        private:
            bool CheckInstances(Context& ctx);
            bool CheckSpecificationsUsed (Context& ctx);

        private:
            void Read(_xml::_element& elem, Context& ctx);
            void Read_info(_xml::_element& elem, Context& ctx);
            void Read_title(_xml::_element& elem, Context&) { m_title = elem.getContent(); }
            void Read_specifications(_xml::_element& elem, Context& ctx);
            void Read_specification(_xml::_element& elem, Context& ctx) { m_specifications.push_back(new Specification(elem, ctx)); }

        private:
            std::string                    m_title;
            OwningPtrList<Specification>   m_specifications;
        };
    }
}

#endif
