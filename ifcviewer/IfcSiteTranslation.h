//
//  Author:  Peter Bonsma
//  $Date: 1999-12-31 23:59:59 +0000 (Wed, 31 Jan 1999) $
//  $Revision: 3999 $
//  Project: IFC Engine DLL
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@rdf.bg
//

#ifndef __RDF_LTD__IFCSITETRANSLATION_H
#define __RDF_LTD__IFCSITETRANSLATION_H



#include "ifcengine\include\ifcengine.h"



bool	GetSiteTranslation(
				int_t		ifcModel,
				double		* translation
			);

bool	SetSiteTranslation(
				int_t		ifcModel,
				double		* translation
			);


#endif
