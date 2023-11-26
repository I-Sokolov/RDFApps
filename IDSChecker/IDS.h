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
        class IdsValue
        {
        public:
            void Read(_xml::_element& elem, Context& ctx);
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
            void Read_name(_xml::_element& elem, Context& ctx) { m_name.Read(elem, ctx); }
            void Read_predefinedType(_xml::_element& elem, Context& ctx) { m_predefinedType.Read(elem, ctx); }

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
            void Read_entity(_xml::_element& elem, Context& ctx) { m_entity.Read(elem, ctx); }

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
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetAttribute : public Facet
        {
        public:
            FacetAttribute(_xml::_element& elem, Context& ctx);

        private:
            void Read_name(_xml::_element& elem, Context& ctx) { m_name.Read(elem, ctx); }
            void Read_value(_xml::_element& elem, Context& ctx) { m_value.Read(elem, ctx); }

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
        };

        /// <summary>
        /// 
        /// </summary>
        class FacetMaterial : public Facet
        {
        public:
            FacetMaterial(_xml::_element& elem, Context& ctx);
        };

        /// <summary>
        /// 
        /// </summary>
        class Facets
        {
        public:
            ~Facets();

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
            std::list<Facet*> m_facets;
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
            void Read_applicability(_xml::_element& elem, Context& ctx) { m_applicability.Read(elem, ctx); }
            void Read_requirements(_xml::_element& elem, Context& ctx) { m_requirements.Read(elem, ctx); }

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
            File();
            ~File();

        public:
            bool Read(const char* idsFilePath, Console* output = nullptr);

        private:
            void Read(_xml::_element& elem, Context& ctx);
            void Read_info(_xml::_element& elem, Context& ctx);
            void Read_title(_xml::_element& elem, Context& ctx);
            void Read_specifications(_xml::_element& elem, Context& ctx);
            void Read_specification(_xml::_element& elem, Context& ctx) { m_specifications.push_back(new Specification(elem, ctx)); }

        private:
            std::string                 m_title;
            std::list<Specification*>   m_specifications;
        };
    }
}

#endif
