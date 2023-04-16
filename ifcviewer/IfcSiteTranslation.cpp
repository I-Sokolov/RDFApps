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

#include	"stdafx.h"

#include	<stdio.h>
#include	<assert.h>
#include	<stdlib.h>
#include	<string.h>

#include	"IfcSiteTranslation.h"


int_t	GetUniqueIfcSiteInstance(
				int_t	ifcModel
			)
{
	int_t	* ifcSiteInstances = sdaiGetEntityExtentBN(ifcModel, "IFCSITE"),
			noIfcSiteInstances = sdaiGetMemberCount(ifcSiteInstances);

	if (noIfcSiteInstances == 1) {
		int_t	ifcSiteInstance = 0;
		engiGetAggrElement(ifcSiteInstances, 0, sdaiINSTANCE, &ifcSiteInstance);
		return	ifcSiteInstance;
	}

	return	0;
}

int_t	GetDeepestObjectPlacement(
				int_t	ifcModel,
				int_t	ifcLocalPlacementInstance
			)
{
	assert(ifcModel && ifcLocalPlacementInstance);

	int_t	ifcObjectPlacementInstance = 0;
	sdaiGetAttrBN(
			ifcLocalPlacementInstance,
			"PlacementRelTo",
			sdaiINSTANCE,
			&ifcObjectPlacementInstance
		);

	if (ifcObjectPlacementInstance) {
		return	GetDeepestObjectPlacement(
						ifcModel,
						ifcObjectPlacementInstance
					);
	}
	else {
		int_t	ifcAxis2PlacementInstance = 0;
		sdaiGetAttrBN(
				ifcLocalPlacementInstance,
				"RelativePlacement",
				sdaiINSTANCE,
				&ifcAxis2PlacementInstance
			);

		if (ifcAxis2PlacementInstance) {
			int_t	ifcCartesianPointInstance = 0;
			sdaiGetAttrBN(
					ifcAxis2PlacementInstance,
					"Location",
					sdaiINSTANCE,
					&ifcCartesianPointInstance
				);
			
			return	ifcCartesianPointInstance;
		}
		else {
			assert(false);
		}
	}

	return	0;
}

bool	GetSiteTranslation(
				int_t	ifcModel,
				double	* translation
			)
{
	translation[0] = 0;
	translation[1] = 0;
	translation[2] = 0;

	int_t	ifcSiteInstance = GetUniqueIfcSiteInstance(ifcModel);

	if (ifcSiteInstance) {
		int_t	ifcObjectPlacementInstance = 0;
		sdaiGetAttrBN(
				ifcSiteInstance,
				"ObjectPlacement",
				sdaiINSTANCE,
				&ifcObjectPlacementInstance
			);
		
		if (ifcObjectPlacementInstance) {
			int_t	ifcCartesianPointInstance = GetDeepestObjectPlacement(
														ifcModel,
														ifcObjectPlacementInstance
													);

			if (ifcCartesianPointInstance) {
				int_t	* coordinatesAGGR = 0;
				sdaiGetAttrBN(
						ifcCartesianPointInstance,
						"Coordinates",
						sdaiAGGR,
						&coordinatesAGGR
					);

				int_t	noCoordinatesAGGR = sdaiGetMemberCount(coordinatesAGGR);
				if (noCoordinatesAGGR > 0) {
					engiGetAggrElement(coordinatesAGGR, 0, sdaiREAL, &translation[0]);
				}
				if (noCoordinatesAGGR > 1) {
					engiGetAggrElement(coordinatesAGGR, 1, sdaiREAL, &translation[1]);
				}
				if (noCoordinatesAGGR > 2) {
					engiGetAggrElement(coordinatesAGGR, 2, sdaiREAL, &translation[2]);
				}

				return	true;
			}
		}
	}

	return	false;
}

bool	SetSiteTranslation(
				int_t	ifcModel,
				double	* translation
			)
{
	int_t	ifcSiteInstance = GetUniqueIfcSiteInstance(ifcModel);

	if (ifcSiteInstance) {
		int_t	ifcObjectPlacementInstance = 0;
		sdaiGetAttrBN(
				ifcSiteInstance,
				"ObjectPlacement",
				sdaiINSTANCE,
				&ifcObjectPlacementInstance
			);
		
		if (ifcObjectPlacementInstance) {
			int_t	ifcCartesianPointInstance = GetDeepestObjectPlacement(
														ifcModel,
														ifcObjectPlacementInstance
													);

			if (ifcCartesianPointInstance) {
				int_t	* coordinatesAGGR = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");
				sdaiAppend(coordinatesAGGR, sdaiREAL, (void*) &translation[0]);
				sdaiAppend(coordinatesAGGR, sdaiREAL, (void*) &translation[1]);
				sdaiAppend(coordinatesAGGR, sdaiREAL, (void*) &translation[2]);

				return	true;
			}
		}
	}

	return	false;
}
