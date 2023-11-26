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
        //IDS structure
        class File;
        class Specification;

        //
        //implement to redirect output
        class Console
        {
        public:
            virtual void out(const char* sz) = NULL;
        };

        //internal
        class Context;

        //
        //
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
            void Read_specification(_xml::_element& elem, Context& ctx);

        private:
            std::string                 m_title;
            std::list<Specification>    m_specifications;
        };

        class Facet
        {
        public:
            virtual ~Facet() {}
        };

        class FacetEntity : public Facet
        {
        public:
            FacetEntity(_xml::_element& elem, Context& ctx);
        };

        class FacetPartOf : public Facet
        {
        public:
            FacetPartOf(_xml::_element& elem, Context& ctx);
        };

        class Facets
        {
        public:
            ~Facets();

        public:
            void Read(_xml::_element& elem, Context& ctx);

        private:
            void Read_entity(_xml::_element& elem, Context& ctx) { m_facets.push_back(new FacetEntity(elem, ctx)); }
            void Read_partOf(_xml::_element& elem, Context& ctx) { m_facets.push_back(new FacetPartOf(elem, ctx)); }

        private:
            std::list<Facet*> m_facets;
        };

        class Applicability
        {
        public:
            void Read(_xml::_element& elem, Context& ctx) { m_facets.Read(elem, ctx); };

        private:
            Facets  m_facets;
        };

        class Requirements
        {
        public:
            void Read(_xml::_element& elem, Context& ctx);

        private:
            std::string m_description;
            Facets      m_facets;
        };

        //
        //
        class Specification
        {
        public:  
            void Read(_xml::_element& elem, Context& ctx);

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
    }
}

#endif
