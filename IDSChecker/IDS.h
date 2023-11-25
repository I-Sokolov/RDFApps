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
            bool Read(_xml::_element& xml, Context& ctx);

        };

        //
        //
        class Specification
        {
            bool Read(_xml::_element& xml, Context& ctx);
        };
    }
}

#endif
