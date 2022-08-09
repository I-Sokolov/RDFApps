// LandXML2IFC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "CreateIFC.h"

#include "ParseLandXML.h"
#include "ifcengine.h"

#include "assert.h"



static	const	double		Pi = 3.14159265358979323846;


int_t	CreateIfcCartesianPointInstance(
				int_t			model,
				STRUCT_ELEMENT	* startElement
			)
{
	int_t	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, (char*) "IFCCARTESIANPOINT"),
			* aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, (char*) "Coordinates");

	if (startElement->elements && startElement->elements->element == nullptr) {
		double	x = atof(startElement->elements->value);
		sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &x);
		if (startElement->elements->next && startElement->elements->next->element == nullptr) {
			double	y = atof(startElement->elements->next->value);
			sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &y);
			if (startElement->elements->next->next && startElement->elements->next->next->element == nullptr) {
				double	z = atof(startElement->elements->next->next->value);
				assert(z == 0.);
//				sdaiAppend((int_t) aggrCoordinates, sdaiREAL, &z);
			}
			else {
				assert(false);
			}
		}
		else {
			assert(false);
		}
	}
	else {
		assert(false);
	}

	return	ifcCartesianPointInstance;
}

void	CreateIFC(
				char				* fileName,
				STRUCT_ELEMENT_LIST	* elements
			)
{
	const std::string model_text{
			"ISO-10303-21;\n"
			"HEADER;\n"
			"FILE_DESCRIPTION(('ViewDefinition [CoordinationView]'),'2;1'); \n"
			"FILE_NAME ($, $, (), (), $, $, $);\n"
			"FILE_SCHEMA (('IFC4x3_RC4'));\n"
			"ENDSEC;\n"
			"DATA;\n"
			"#1=IFCPROJECT('1FNFy8AJeHwwz7wDZHIYIu',#3,'IFC Rail Referencedata','Rail alignment sample',$,'Longname','Design',$,#9);\n"
			"#2=IFCAPPLICATION($,'2021','RDF Ltd.',$);\n"
			"#3=IFCOWNERHISTORY(#6,#2,$,$,$,$,$,$);\n"
			"#4=IFCPERSON('TS','RDF Ltd.',$,$,$,$,$,$);\n"
			"#5=IFCORGANIZATION('RDF','RDF',$,$,$);\n"
			"#6=IFCPERSONANDORGANIZATION(#4,#5,$);\n"
			"#7=IFCSIUNIT(*,.LENGTHUNIT.,$,.METRE.);\n"
			"#8=IFCSIUNIT(*,.PLANEANGLEUNIT.,$,.RADIAN.);\n"
			"#9=IFCUNITASSIGNMENT((#7,#8));\n"
			"#10=IFCCARTESIANPOINT((0.,0.,0.));\n"
			"#11=IFCDIRECTION((0.,0.,1.));\n"
			"#12=IFCDIRECTION((1.,0.,0.));\n"
			"#13=IFCAXIS2PLACEMENT3D(#10,#11,#12);\n"
			"#14=IFCLOCALPLACEMENT($,#13);\n"
			"#15=IFCRAILWAY('1FNFy9AJeHwuVmwDZHIYIu',#3,'optional Railway Name','optional Railway Description',$,#14,$,'Longname for Railway track geometry (alignment)',$);\n"
			"#16=IFCDIRECTION((0.,1.,0.));\n"
			"#17=IFCGEOMETRICREPRESENTATIONCONTEXT($,'MODEL',3,1.E-05,#13,#16);\n"
			"#18=IFCRELAGGREGATES('1FNFyAAJeHwu6TwDZHIYIu',#3,'Documentation Container','',#1,(#15));\n"
			"ENDSEC;\n"
			"END-ISO-10303-21;\n"
		};

	int_t	model = engiOpenModelByArray(1, (unsigned char*) model_text.c_str(), model_text.size(), "");

	int_t	ifcOwnerHistoryInstance = internalGetInstanceFromP21Line(model, 3),
			ifcProjectInstance = internalGetInstanceFromP21Line(model, 1),
			ifcRailwayInstance = internalGetInstanceFromP21Line(model, 15),
			ifcRelContainedInSpatialStructureInstance = sdaiCreateInstanceBN(model, (char*) "IFCRELCONTAINEDINSPATIALSTRUCTURE"),
			* aggrRelatedElements = sdaiCreateAggrBN(ifcRelContainedInSpatialStructureInstance, "RelatedElements");

	{
		STRUCT_ELEMENT	* projectElement = GetElement(elements, 0, (char*) "Project");
		if (projectElement) {
			sdaiPutAttrBN(ifcProjectInstance, "Name", sdaiSTRING, (char*) GetAttribute(projectElement, (char*) "name"));
			sdaiPutAttrBN(ifcProjectInstance, "Description", sdaiSTRING, (char*) GetAttribute(projectElement, (char*) "desc"));
		}
	}

	{
		sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "GlobalId", engiGLOBALID, (void*) 0);
		sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
		sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "Name", sdaiSTRING, (char*) "IfcProducts contained in myIfcRailway");
		sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "Description", sdaiSTRING, (char*) "Container for IfcProject, collects relevant products");
		sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "RelatingStructure", sdaiINSTANCE, (void*) ifcRailwayInstance);
	}

	int	metricCnt = GetElementCnt(elements, (char*) "Metric");
	if (metricCnt == 1) {
		STRUCT_ELEMENT	* metricElement = GetElement(elements, 0, (char*) "Metric");
		char	* linearUnit = GetAttribute(metricElement, (char*) "linearUnit"),
				* areaUnit = GetAttribute(metricElement, (char*) "areaUnit"),
				* volumeUnit = GetAttribute(metricElement, (char*) "volumeUnit");

		assert(equals(linearUnit, (char*) "meter"));
		assert(equals(areaUnit, (char*) "squareMeter"));
		assert(equals(volumeUnit, (char*) "cubicMeter"));
	}
	else {
		assert(false);
	}

	int	alignmentCnt = GetElementCnt(elements, (char*) "Alignment");
	for (int i = 0; i < alignmentCnt; i++) {
		STRUCT_ELEMENT	* alignmentElement = GetElement(elements, i, (char*) "Alignment"),
						* coordGeomElement = GetElement(alignmentElement->elements, 0, (char*) "CoordGeom"),
						* profileElement = GetElement(alignmentElement->elements, 0, (char*) "Profile"),
						* cantElement = GetElement(alignmentElement->elements, 0, (char*) "Cant");

		int_t	ifcAlignmentHorizontalInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTHORIZONTAL"),
				* aggrRelatedObjectsAlignment;

		double	startDistAlong = atof(GetAttribute(alignmentElement, (char*) "staStart"));

		{
			int_t	ifcAlignmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENT");
			sdaiAppend((int_t) aggrRelatedElements, sdaiINSTANCE, (void*) ifcAlignmentInstance);

			{
				int_t	ifcRelNestsAlignmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCRELNESTS");
				aggrRelatedObjectsAlignment = sdaiCreateAggrBN(ifcRelNestsAlignmentInstance, "RelatedObjects");
				sdaiPutAttrBN(ifcRelNestsAlignmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
				sdaiPutAttrBN(ifcRelNestsAlignmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
				sdaiPutAttrBN(ifcRelNestsAlignmentInstance, "Name", sdaiSTRING, (char*) "name");
				sdaiPutAttrBN(ifcRelNestsAlignmentInstance, "RelatingObject", sdaiINSTANCE, (void*) ifcAlignmentInstance);
			}

			sdaiPutAttrBN(ifcAlignmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
			sdaiPutAttrBN(ifcAlignmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
			sdaiPutAttrBN(ifcAlignmentInstance, "Name", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "name"));
			sdaiPutAttrBN(ifcAlignmentInstance, "Description", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "desc"));

			sdaiAppend((int_t) aggrRelatedObjectsAlignment, sdaiINSTANCE, (void*) ifcAlignmentHorizontalInstance);

			sdaiPutAttrBN(ifcAlignmentHorizontalInstance, "GlobalId", engiGLOBALID, (void*) 0);
			sdaiPutAttrBN(ifcAlignmentHorizontalInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
			sdaiPutAttrBN(ifcAlignmentHorizontalInstance, "Name", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "name"));
			sdaiPutAttrBN(ifcAlignmentHorizontalInstance, "Description", sdaiSTRING, (char*) GetAttribute(coordGeomElement, (char*) "desc"));


			//
			//	old structure, now added as a property to IFCREFERENT
			//
	//		double	startDistAlong = atof(GetAttribute(alignmentElement, (char*) "staStart"));
	//		sdaiPutAttrBN(ifcAlignmentHorizontalInstance, "StartDistAlong", sdaiREAL, (void*) &startDistAlong);

			{
				int_t	ifcReferentInstance = sdaiCreateInstanceBN(model, (char*) "IFCREFERENT");

				sdaiPutAttrBN(ifcReferentInstance, "GlobalId", engiGLOBALID, (void*) 0);
				sdaiPutAttrBN(ifcReferentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
				sdaiPutAttrBN(ifcReferentInstance, "PredefinedType", sdaiENUM, (void*) "REFERENCEMARKER");

				sdaiAppend((int_t) aggrRelatedObjectsAlignment, sdaiINSTANCE, (void*) ifcReferentInstance);

				//		#332 = IFCPROPERTYSET('2hc62djjPFfAvF62p1bpS7', #323, 'Pset_Stationing', $, (#333, #334));
				//		#333 = IFCPROPERTYSINGLEVALUE('IncomingStation', $, IFCLENGTHMEASURE(1001.800003), $);
				//		#334 = IFCPROPERTYSINGLEVALUE('Station', $, IFCLENGTHMEASURE(0.0), $);
				//		#335 = IFCRELDEFINESBYPROPERTIES('2xBbwPwX50mwmoIK$TntpY', #323, $, $, (#331), #332);

				int_t	ifcPropertySetInstance = sdaiCreateInstanceBN(model, (char*) "IFCPROPERTYSET");

				sdaiPutAttrBN(ifcPropertySetInstance, "GlobalId", engiGLOBALID, (void*) 0);
				sdaiPutAttrBN(ifcPropertySetInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
				sdaiPutAttrBN(ifcPropertySetInstance, "Name", sdaiSTRING, (char*) "Pset_Stationing");

				int_t	ifcPropertySingleValueInstance_I = sdaiCreateInstanceBN(model, (char*) "IFCPROPERTYSINGLEVALUE"),
						ifcPropertySingleValueInstance_II = sdaiCreateInstanceBN(model, (char*) "IFCPROPERTYSINGLEVALUE");

				{
					double	valueComponent = startDistAlong;
					sdaiPutAttrBN(ifcPropertySingleValueInstance_I, "Name", sdaiSTRING, (char*) "IncomingStation");
					int_t	* valueComponentADB = (int_t*) sdaiCreateADB(sdaiREAL, &valueComponent);
					sdaiPutADBTypePath(valueComponentADB, 1, "IFCLENGTHMEASURE"); 
					sdaiPutAttrBN(ifcPropertySingleValueInstance_I, "NominalValue", sdaiADB, (void*) valueComponentADB);
				}

				{
					double	valueComponent = 0.;
					sdaiPutAttrBN(ifcPropertySingleValueInstance_II, "Name", sdaiSTRING, (char*) "Station");
					int_t	* valueComponentADB = (int_t*) sdaiCreateADB(sdaiREAL, &valueComponent);
					sdaiPutADBTypePath(valueComponentADB, 1, "IFCLENGTHMEASURE"); 
					sdaiPutAttrBN(ifcPropertySingleValueInstance_II, "NominalValue", sdaiADB, (void*) valueComponentADB);
				}

				int_t	ifcRelDefinesByPropetiesInstance = sdaiCreateInstanceBN(model, (char*) "IFCRELDEFINESBYPROPERTIES");

				sdaiPutAttrBN(ifcRelDefinesByPropetiesInstance, "GlobalId", engiGLOBALID, (void*) 0);
				sdaiPutAttrBN(ifcRelDefinesByPropetiesInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
				int_t	* aggrRelatedObjects = sdaiCreateAggrBN(ifcRelDefinesByPropetiesInstance, "RelatedObjects");
				sdaiAppend((int_t) aggrRelatedObjects, sdaiINSTANCE, (void*) ifcReferentInstance);
				sdaiPutAttrBN(ifcRelDefinesByPropetiesInstance, "RelatingPropertyDefinition", sdaiINSTANCE, (void*) ifcPropertySetInstance);

				int_t	* aggrHasProperties = sdaiCreateAggrBN(ifcPropertySetInstance, "HasProperties");
				sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) ifcPropertySingleValueInstance_I);
				sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) ifcPropertySingleValueInstance_II);
			}
		}

		int_t	* aggrRelatedObjectsHorizontaAlignment;

		{
			int_t	ifcRelNestsHorizontaAlignmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCRELNESTS");
			aggrRelatedObjectsHorizontaAlignment = sdaiCreateAggrBN(ifcRelNestsHorizontaAlignmentInstance, "RelatedObjects");
			sdaiPutAttrBN(ifcRelNestsHorizontaAlignmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
			sdaiPutAttrBN(ifcRelNestsHorizontaAlignmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
			sdaiPutAttrBN(ifcRelNestsHorizontaAlignmentInstance, "Name", sdaiSTRING, (char*) "name");
			sdaiPutAttrBN(ifcRelNestsHorizontaAlignmentInstance, "RelatingObject", sdaiINSTANCE, (void*) ifcAlignmentHorizontalInstance);
		}

		double	endDirection = 0.;

		STRUCT_ELEMENT		* endElement = nullptr;
		STRUCT_ELEMENT_LIST	* coordGeomElements = coordGeomElement->elements;
		while (coordGeomElements) {
			endElement = nullptr;

			int_t	ifcAlignmentSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTSEGMENT");
	        sdaiAppend((int_t) aggrRelatedObjectsHorizontaAlignment, sdaiINSTANCE, (void*) ifcAlignmentSegmentInstance);

			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "Name", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "name"));
//			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "Description", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "desc"));

			int_t	ifcAlignmentHorizontalSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTHORIZONTALSEGMENT");

			double	startDirection = 0.,
					startRadiusOfCurvature = 0.,
					endRadiusOfCurvature = 0.,
					segmentLength = 0.;
			if (equals(coordGeomElements->element->tag->name, (char*) "Curve")) {
				char	* rot = GetAttribute(coordGeomElements->element, (char*) "rot");
				assert(equals(rot, (char*) "cw") || equals(rot, (char*) "ccw"));

				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, (void*) CreateIfcCartesianPointInstance(model, GetElement(coordGeomElements, 0, (char*) "Start")));
				startDirection = atof(GetAttribute(coordGeomElements->element, (char*) "dirStart"));
				startRadiusOfCurvature = atof(GetAttribute(coordGeomElements->element, (char*) "radius"));
				endRadiusOfCurvature = startRadiusOfCurvature;
				segmentLength = atof(GetAttribute(coordGeomElements->element, (char*) "length"));

				STRUCT_ELEMENT	* startElement = GetElement(coordGeomElements->element->elements, 0, (char*) "Start");
				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartTag", sdaiSTRING, (void*) GetAttribute(startElement, (char*) "code"));

				endElement = GetElement(coordGeomElements->element->elements, 0, (char*) "End");
				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndTag", sdaiSTRING, (void*) GetAttribute(endElement, (char*) "code"));

				if (equals(rot, (char*) "ccw")) {
					startRadiusOfCurvature = -startRadiusOfCurvature;
					endRadiusOfCurvature = -endRadiusOfCurvature;
				}

				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "PredefinedType", sdaiENUM, (void*) "CIRCULARARC");
				endDirection = atof(GetAttribute(coordGeomElements->element, (char*) "dirEnd"));
			}
			else if (equals(coordGeomElements->element->tag->name, (char*) "Line")) {
STRUCT_ELEMENT	* startElement__DEBUG = GetElement(coordGeomElements, 0, (char*) "Start");
				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, (void*) CreateIfcCartesianPointInstance(model, GetElement(coordGeomElements, 0, (char*) "Start")));
				startDirection = atof(GetAttribute(coordGeomElements->element, (char*) "dir"));
				segmentLength = atof(GetAttribute(coordGeomElements->element, (char*) "length"));

				STRUCT_ELEMENT	* startElement = GetElement(coordGeomElements->element->elements, 0, (char*) "Start");
				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartTag", sdaiSTRING, (void*) GetAttribute(startElement, (char*) "code"));

				endElement = GetElement(coordGeomElements->element->elements, 0, (char*) "End");
				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndTag", sdaiSTRING, (void*) GetAttribute(endElement, (char*) "code"));

				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "PredefinedType", sdaiENUM, (void*) "LINE");
				endDirection = startDirection;
			}
			else if (equals(coordGeomElements->element->tag->name, (char*) "Spiral")) {
				char	* rot = GetAttribute(coordGeomElements->element, (char*) "rot");
				assert(equals(rot, (char*) "cw") || equals(rot, (char*) "ccw"));

				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, (void*) CreateIfcCartesianPointInstance(model, GetElement(coordGeomElements, 0, (char*) "Start")));
				startDirection = atof(GetAttribute(coordGeomElements->element, (char*) "dirStart"));
				char	* radiusStart = GetAttribute(coordGeomElements->element, (char*) "radiusStart");
				startRadiusOfCurvature = equals(radiusStart, (char*) "INF") ? 0. : atof(radiusStart);
				char	* radiusEnd = GetAttribute(coordGeomElements->element, (char*) "radiusEnd");
				endRadiusOfCurvature = equals(radiusEnd, (char*) "INF") ? 0. : atof(radiusEnd);
				segmentLength = atof(GetAttribute(coordGeomElements->element, (char*) "length"));

				STRUCT_ELEMENT	* startElement = GetElement(coordGeomElements->element->elements, 0, (char*) "Start");
				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartTag", sdaiSTRING, (void*) GetAttribute(startElement, (char*) "code"));

				endElement = GetElement(coordGeomElements->element->elements, 0, (char*) "End");
				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndTag", sdaiSTRING, (void*) GetAttribute(endElement, (char*) "code"));

				if (equals(rot, (char*) "ccw")) {
					startRadiusOfCurvature = -startRadiusOfCurvature;
					endRadiusOfCurvature = -endRadiusOfCurvature;
				}

				sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "PredefinedType", sdaiENUM, (void*) "CLOTHOID");
				endDirection = atof(GetAttribute(coordGeomElements->element, (char*) "dirEnd"));
			}
			else {
				assert(false);
			}

			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartDirection", sdaiREAL, (void*) &startDirection);
			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, (void*) &startRadiusOfCurvature);
			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, (void*) &endRadiusOfCurvature);
			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "SegmentLength", sdaiREAL, (void*) &segmentLength);

			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) ifcAlignmentHorizontalSegmentInstance);

			coordGeomElements = coordGeomElements->next;
		}

		assert(endElement);

		{
			//
			//	last zero-length segment
			//
			int_t	ifcAlignmentSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTSEGMENT");
	        sdaiAppend((int_t) aggrRelatedObjectsHorizontaAlignment, sdaiINSTANCE, (void*) ifcAlignmentSegmentInstance);

			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "Name", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "name"));
//			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "Description", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "desc"));

			int_t	ifcAlignmentHorizontalSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTHORIZONTALSEGMENT");

			double	startRadiusOfCurvature = 0.,
					endRadiusOfCurvature = 0.,
					segmentLength = 0.;

			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, (void*) CreateIfcCartesianPointInstance(model, endElement));

			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "PredefinedType", sdaiENUM, (void*) "LINE");
			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartDirection", sdaiREAL, (void*) &endDirection);
			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, (void*) &startRadiusOfCurvature);
			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, (void*) &endRadiusOfCurvature);
			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "SegmentLength", sdaiREAL, (void*) &segmentLength);
			sdaiPutAttrBN(ifcAlignmentHorizontalSegmentInstance, "PredefinedType", sdaiENUM, (void*) "LINE");

			sdaiPutAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) ifcAlignmentHorizontalSegmentInstance);
		}

		if (profileElement) {
			STRUCT_ELEMENT_LIST	* profAlignElements = profileElement->elements;
			if (profAlignElements && profAlignElements->element) {
				STRUCT_ELEMENT_LIST	* pviElements = profAlignElements->element->elements;

				int_t	ifcAlignmentVerticalInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTVERTICAL"),
						* aggrRelatedObjectsVerticalAlignment;

				{
					sdaiAppend((int_t) aggrRelatedObjectsAlignment, sdaiINSTANCE, (void*) ifcAlignmentVerticalInstance);

					sdaiPutAttrBN(ifcAlignmentVerticalInstance, "GlobalId", engiGLOBALID, (void*) 0);
					sdaiPutAttrBN(ifcAlignmentVerticalInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
					sdaiPutAttrBN(ifcAlignmentVerticalInstance, "Name", sdaiSTRING, (char*) GetAttribute(profAlignElements->element, (char*) "name"));
//					sdaiPutAttrBN(ifcAlignmentVerticalInstance, "Description", sdaiSTRING, (char*) GetAttribute(coordGeomElement, (char*) "desc"));

					int_t	ifcRelNestsVerticalAlignmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCRELNESTS");
					aggrRelatedObjectsVerticalAlignment = sdaiCreateAggrBN(ifcRelNestsVerticalAlignmentInstance, "RelatedObjects");
					sdaiPutAttrBN(ifcRelNestsVerticalAlignmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
					sdaiPutAttrBN(ifcRelNestsVerticalAlignmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
					sdaiPutAttrBN(ifcRelNestsVerticalAlignmentInstance, "Name", sdaiSTRING, (char*) "name");
					sdaiPutAttrBN(ifcRelNestsVerticalAlignmentInstance, "RelatingObject", sdaiINSTANCE, (void*) ifcAlignmentVerticalInstance);
				}

				double	removeXValue = 0.;
				bool	foundPvi = false;
				__VECTOR2	vecPrevious;
				while (pviElements) {
					if (pviElements->element && equals(pviElements->element->tag->name, (char*) "PVI")) {
						STRUCT_ELEMENT	* pviElement = pviElements->element;

						if (pviElement->elements &&
							pviElement->elements->next &&
							pviElement->elements->next->next == nullptr) {
							__VECTOR2	vecCurrent = {
												atof(pviElement->elements->value),
												atof(pviElement->elements->next->value)
											};
							if (foundPvi) {
								//
								//	Create Vertical alignment segment
								//
								int_t	ifcAlignmentSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTSEGMENT");
								sdaiAppend((int_t) aggrRelatedObjectsVerticalAlignment, sdaiINSTANCE, (void*) ifcAlignmentSegmentInstance);

								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "Name", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "name"));

								int_t	ifcAlignmentVerticalSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTVERTICALSEGMENT");

								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) ifcAlignmentVerticalSegmentInstance);

								{
									double	__startDistAlong = vecPrevious.x - startDistAlong,
											__horizontalLength = vecCurrent.x - vecPrevious.x,
											__startHeight = vecPrevious.y,
											__startGradient = (vecCurrent.y - vecPrevious.y) / __horizontalLength,
											__endGradient = __startGradient;

									assert(__horizontalLength > 0.);

									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &__startDistAlong);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &__horizontalLength);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, (void*) &__startHeight);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, (void*) &__startGradient);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "EndGradient", sdaiREAL, (void*) &__endGradient);

									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "PredefinedType", sdaiENUM, (void*) "CONSTANTGRADIENT");
								}
							}
							else {
								foundPvi = true;
							}
							vecPrevious = vecCurrent;
						}
						else {
							assert(false);
						}
					}
					else if (equals(pviElements->element->tag->name, (char*) "CircCurve")) {
						assert(foundPvi);

						STRUCT_ELEMENT	* pviElement = pviElements->element;
						if (pviElement->elements &&
							pviElement->elements->next &&
							pviElement->elements->next->next == nullptr) {
							__VECTOR2	vecCenter = {
												atof(pviElement->elements->value),
												atof(pviElement->elements->next->value)
											};
							double	radius = atof(GetAttribute(pviElement, (char*) "radius")),
									length = atof(GetAttribute(pviElement, (char*) "length"));

							double	circleLength = 2. * Pi * std::fabs(radius);
							double	angle = 2 * Pi * length / circleLength;

							double	distance = Vec2Distance(&vecCenter, &vecPrevious);

							double	a = std::fabs(radius) * sin(angle / 2.);

							//
							//	two segments, i.e. line and circle
							//

							double	stGr = 0.;

							{
								//
								//	Create Vertical alignment segment
								//
								int_t	ifcAlignmentSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTSEGMENT");
								sdaiAppend((int_t) aggrRelatedObjectsVerticalAlignment, sdaiINSTANCE, (void*) ifcAlignmentSegmentInstance);

								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "Name", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "name"));

								int_t	ifcAlignmentVerticalSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTVERTICALSEGMENT");

								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) ifcAlignmentVerticalSegmentInstance);

								{
									double	__startDistAlong = vecPrevious.x - startDistAlong,
											__horizontalLength = vecCenter.x - vecPrevious.x - a,
											__startHeight = vecPrevious.y,
											__startGradient = (vecCenter.y - vecPrevious.y) / (__horizontalLength + a),
											__endGradient = __startGradient;

									assert(__horizontalLength > 0.);

									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &__startDistAlong);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &__horizontalLength);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, (void*) &__startHeight);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, (void*) &__startGradient);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "EndGradient", sdaiREAL, (void*) &__endGradient);

									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "PredefinedType", sdaiENUM, (void*) "CONSTANTGRADIENT");

									vecPrevious.x += __horizontalLength;
									vecPrevious.y += __startGradient * __horizontalLength;

									stGr = __startGradient;
								}
							}

							{
								//
								//	Create Vertical alignment segment
								//
								int_t	ifcAlignmentSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTSEGMENT");
								sdaiAppend((int_t) aggrRelatedObjectsVerticalAlignment, sdaiINSTANCE, (void*) ifcAlignmentSegmentInstance);

								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "Name", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "name"));

								int_t	ifcAlignmentVerticalSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTVERTICALSEGMENT");

								sdaiPutAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) ifcAlignmentVerticalSegmentInstance);

								{
									double	__startDistAlong = vecPrevious.x - startDistAlong,
											__horizontalLength = a + a,
											__startHeight = vecPrevious.y,
											__startGradient = sin(angle / 2.),
											__endGradient = -__startGradient,
											__radiusOfCurvature = radius;

									if (radius < 0.) {
										assert(stGr / std::fabs(stGr) != __startGradient / std::fabs(__startGradient));
										__startGradient = -__startGradient;
										__endGradient = -__startGradient;
									}
									else {
										if (stGr / std::fabs(stGr) == __startGradient / std::fabs(__startGradient)) {
											int u = 0;
										}
										else {
									//		__startGradient = -__startGradient;
									//		__endGradient = -__startGradient;
										}
									}

									assert(__horizontalLength > 0.);

									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &__startDistAlong);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &__horizontalLength);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, (void*) &__startHeight);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, (void*) &__startGradient);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "EndGradient", sdaiREAL, (void*) &__endGradient);
									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "RadiusOfCurvature", sdaiREAL, (void*) &__radiusOfCurvature);

									sdaiPutAttrBN(ifcAlignmentVerticalSegmentInstance, "PredefinedType", sdaiENUM, (void*) "CIRCULARARC");

									vecPrevious.x += __horizontalLength;
								}
							}
						}
						else {
							assert(false);
						}
					}
					else {
						assert(equals(pviElements->element->tag->name, (char*) "Feature"));
					}
					pviElements = pviElements->next;
				}
			}
			else {
				assert(false);
			}
		}
		else {
			assert(false);
		}

		if (cantElement) {
			bool	foundStation = false;
			double	stationPrevious = 0.;

			int_t	ifcAlignmentCantInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTCANT"),
					* aggrRelatedObjectsCantAlignment;

			{
				sdaiAppend((int_t) aggrRelatedObjectsAlignment, sdaiINSTANCE, (void*) ifcAlignmentCantInstance);

				sdaiPutAttrBN(ifcAlignmentCantInstance, "GlobalId", engiGLOBALID, (void*) 0);
				sdaiPutAttrBN(ifcAlignmentCantInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
				sdaiPutAttrBN(ifcAlignmentCantInstance, "Name", sdaiSTRING, (char*) GetAttribute(cantElement, (char*) "name"));
//				sdaiPutAttrBN(ifcAlignmentCantInstance, "Description", sdaiSTRING, (char*) GetAttribute(coordGeomElement, (char*) "desc"));

				int_t	ifcRelNestsCantAlignmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCRELNESTS");
				aggrRelatedObjectsCantAlignment = sdaiCreateAggrBN(ifcRelNestsCantAlignmentInstance, "RelatedObjects");
				sdaiPutAttrBN(ifcRelNestsCantAlignmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
				sdaiPutAttrBN(ifcRelNestsCantAlignmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
				sdaiPutAttrBN(ifcRelNestsCantAlignmentInstance, "Name", sdaiSTRING, (char*) "name");
				sdaiPutAttrBN(ifcRelNestsCantAlignmentInstance, "RelatingObject", sdaiINSTANCE, (void*) ifcAlignmentCantInstance);
			}

			STRUCT_ELEMENT_LIST	* cantStationElements = cantElement->elements;
			while (cantStationElements) {
				assert(equals(cantStationElements->element->tag->name, (char*) "CantStation"));
				double	stationCurrent = atof(GetAttribute(cantStationElements->element, (char*) "station"));
				if (foundStation) {
					//
					//	Create Cant alignment segment
					//
					int_t	ifcAlignmentSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTSEGMENT");
					sdaiAppend((int_t) aggrRelatedObjectsCantAlignment, sdaiINSTANCE, (void*) ifcAlignmentSegmentInstance);

					sdaiPutAttrBN(ifcAlignmentSegmentInstance, "GlobalId", engiGLOBALID, (void*) 0);
					sdaiPutAttrBN(ifcAlignmentSegmentInstance, "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
					sdaiPutAttrBN(ifcAlignmentSegmentInstance, "Name", sdaiSTRING, (char*) GetAttribute(alignmentElement, (char*) "name"));

					int_t	ifcAlignmentCantSegmentInstance = sdaiCreateInstanceBN(model, (char*) "IFCALIGNMENTCANTSEGMENT");

					sdaiPutAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, (void*) ifcAlignmentCantSegmentInstance);

					{
						//		StartDistAlong
						//		HorizontalLength
						//		StartCantLeft
						//		EndCantLeft
						//		StartCantRight
						//		EndCantRight
						//		PredefinedType	LINEARTRANSITION == clothoid

						double	__startDistAlong = stationPrevious - startDistAlong,
								__horizontalLength = stationCurrent - stationPrevious,
								__startCantLeft = 0.,
								__endCantLeft = 0.,
								__startCantRight = 0.,
								__endCantRight = 0.;

						sdaiPutAttrBN(ifcAlignmentCantSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &__startDistAlong);
						sdaiPutAttrBN(ifcAlignmentCantSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &__horizontalLength);
						sdaiPutAttrBN(ifcAlignmentCantSegmentInstance, "StartCantLeft", sdaiREAL, (void*) &__startCantLeft);
						sdaiPutAttrBN(ifcAlignmentCantSegmentInstance, "EndCantLeft", sdaiREAL, (void*) &__endCantLeft);
						sdaiPutAttrBN(ifcAlignmentCantSegmentInstance, "StartCantRight", sdaiREAL, (void*) &__startCantRight);
						sdaiPutAttrBN(ifcAlignmentCantSegmentInstance, "EndCantRight", sdaiREAL, (void*) &__endCantRight);

						if (equals(GetAttribute(cantStationElements->element, (char*) "station"), (char*) "clothoid")) {
							sdaiPutAttrBN(ifcAlignmentCantSegmentInstance, "PredefinedType", sdaiENUM, (void*) "LINEARTRANSITION");
						}
						else {
							sdaiPutAttrBN(ifcAlignmentCantSegmentInstance, "PredefinedType", sdaiENUM, (void*) "CONSTANTCANT");
						}
					}
				}
				else {
					foundStation = true;
				}
				stationPrevious = stationCurrent;
				cantStationElements = cantStationElements->next;
			}
		}
		else {
			assert(false);
		}
	}

	sdaiSaveModelBN(model, fileName);
	sdaiCloseModel(model);
}