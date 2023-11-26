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

#ifndef __RDF_LTD__IDS_H
#define __RDF_LTD__IDS_H

// Implements buildingSMART Information Delivery Specification
// https://github.com/buildingSMART/IDS/tree/master/Documentation

#include "_xml.h"

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
            ~OwningPtrList()
            {
                for (auto p : *this) {
                    delete p;
                }
            }
        };

        /// <summary>
        /// 
        /// </summary>
        class Value
        {
        public:
            Value(_xml::_element& elem, Context& ctx);

        private:
            std::string m_value;
            std::string m_fixed;
        };

        /// <summary>
        /// 
        /// </summary>
        class Restriction
        {
        public:
            Restriction(_xml::_element& elem, Context& ctx);

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
            void Read(_xml::_element& elem, Context& ctx);

        private:
            void Read_simpleValue(_xml::_element& elem, Context&) { m_simpleValue = elem.getContent(); }
            void Read_restriction(_xml::_element& elem, Context& ctx) { m_restrictions.push_back(new Restriction(elem, ctx)); }

        private:
            std::string                m_simpleValue;
            OwningPtrList<Restriction> m_restrictions;
        };

        /// <summary>
        /// 
        /// </summary>
        class Facet
        {
        public:
            virtual ~Facet() {}

        protected:
            Facet() {}
            Facet(_xml::_element& elem, Context& ctx) { Read(elem, ctx); }

        protected:
            void Read(_xml::_element& elem, Context& ctx);

        private:
            std::string m_minOccurs;
            std::string m_maxOccurs;
            std::string m_datatype;
            std::string m_relation;
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

        private:
            IdsValue m_name;
            IdsValue m_predefinedType;
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetPartOf : public Facet
        {
        public:
            FacetPartOf(_xml::_element& elem, Context& ctx);

        private:
            FacetEntity  m_entity;
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetClassification : public Facet
        {
        public:
            FacetClassification(_xml::_element& elem, Context& ctx);

        private:
            IdsValue m_value;
            IdsValue m_system;
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetAttribute : public Facet
        {
        public:
            FacetAttribute(_xml::_element& elem, Context& ctx);

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
        class Applicability
        {
        public:
            void Read(_xml::_element& elem, Context& ctx) { m_facets.Read(elem, ctx); };

        private:
            Facets  m_facets;
        };

        /// <summary>
        /// 
        /// </summary>
        class Requirements
        {
        public:
            void Read(_xml::_element& elem, Context& ctx);

        private:
            std::string m_description;
            Facets      m_facets;
        };

        /// <summary>
        /// 
        /// </summary>
        class Specification
        {
        public:  
            Specification(_xml::_element& elem, Context& ctx);

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
        class File
        {
        public:
            bool Read(const char* idsFilePath, Console* output = nullptr);

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
