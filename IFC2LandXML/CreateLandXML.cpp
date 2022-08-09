// LandXML2IFC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "CreateLandXML.h"

#include "ifcengine.h"

#include "assert.h"


static	inline	void    GetCartesianPointCoordinates(
                                int_t   ifcCartesianPointInstance,
                                double  * pX,
                                double  * pY,
                                double  * pZ
							)
{
    int_t	* aggrCoordinates = nullptr, noAggrCoordinates;
    sdaiGetAttrBN(ifcCartesianPointInstance, "Coordinates", sdaiAGGR, &aggrCoordinates);
    noAggrCoordinates = sdaiGetMemberCount(aggrCoordinates);

    if (noAggrCoordinates >= 1 && pX) {
		double	x = 0.;
        engiGetAggrElement(aggrCoordinates, 0, sdaiREAL, &x);
		(*pX) = x;
    }
    if (noAggrCoordinates >= 2 && pY) {
		double	y = 0.;
		engiGetAggrElement(aggrCoordinates, 1, sdaiREAL, &y);
		(*pY) = y;
	}
    if (noAggrCoordinates >= 3 && pZ) {
		double	z = 0.;
		engiGetAggrElement(aggrCoordinates, 2, sdaiREAL, &z);
		(*pZ) = z;
	}

    assert(noAggrCoordinates == 2);
}

static  inline  int_t   GetAlignmentSegments(
                                int_t   model,
                                int_t   ifcAlignmentXyzInstance,
                                int_t   * pAlignmentSegment
                            )
{
    int_t   alignmentSegmentCnt = 0;

    int_t   expressID = internalGetP21Line(ifcAlignmentXyzInstance);

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentXyzInstance, "IsNestedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT")) {
                    if (pAlignmentSegment) {
                        pAlignmentSegment[alignmentSegmentCnt] = ifcObjectDefinitionInstance;
                    }
                    alignmentSegmentCnt++;
                }
            }
        }
    }

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentXyzInstance, "IsDecomposedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT")) {
                    if (pAlignmentSegment) {
                        pAlignmentSegment[alignmentSegmentCnt] = ifcObjectDefinitionInstance;
                    }
                    alignmentSegmentCnt++;

                    assert(false);  //  should be a nested relations
                }
            }
        }
    }

    assert(alignmentSegmentCnt);
    return  alignmentSegmentCnt;
}

static  inline  int_t   GetReferent(
                               int_t   model,
                               int_t   ifcAlignmentInstance
                            )
{
	int_t	ifcReferent_TYPE = sdaiGetEntity(model, (char*) "IFCREFERENT"),
			* isNestedByInstances = nullptr;
	sdaiGetAttrBN(ifcAlignmentInstance, "IsNestedBy", sdaiAGGR, &isNestedByInstances);
	int_t	isNestedByInstancesCnt = sdaiGetMemberCount(isNestedByInstances);
	for (int_t i = 0; i < isNestedByInstancesCnt; i++) {
		int_t	isNestedByInstance = 0;
		engiGetAggrElement(isNestedByInstances, i, sdaiINSTANCE, &isNestedByInstance);

		int_t	* relatedObjectsInstances = nullptr;
		sdaiGetAttrBN(isNestedByInstance, "RelatedObjects", sdaiAGGR, &relatedObjectsInstances);
		int_t	relatedObjectsInstancesCnt = sdaiGetMemberCount(relatedObjectsInstances);
		for (int_t j = 0; j < relatedObjectsInstancesCnt; j++) {
			int_t	relatedObjectsInstance = 0;
			engiGetAggrElement(relatedObjectsInstances, j, sdaiINSTANCE, &relatedObjectsInstance);

			if (sdaiGetInstanceType(relatedObjectsInstance) == ifcReferent_TYPE) {
				return	relatedObjectsInstance;
			}
		}
	}
	return	0;
}

static  inline  double   GetStaStart(
                                int_t   model,
                                int_t   ifcAlignmentInstance
                            )
{
	int_t	ifcReferentInstance = GetReferent(model, ifcAlignmentInstance);
	if (ifcReferentInstance) {
		int_t	* isDefinedByInstances = nullptr;
		sdaiGetAttrBN(ifcReferentInstance, "IsDefinedBy", sdaiAGGR, &isDefinedByInstances);
		int_t	isDefinedByInstancesCnt = sdaiGetMemberCount(isDefinedByInstances);
		for (int_t i = 0; i < isDefinedByInstancesCnt; i++) {
			int_t	isDefinedByInstance = 0;
			engiGetAggrElement(isDefinedByInstances, i, sdaiINSTANCE, &isDefinedByInstance);

			int_t	ifcPropertySetInstance = 0;
			sdaiGetAttrBN(isDefinedByInstance, "RelatingPropertyDefinition", sdaiINSTANCE, &ifcPropertySetInstance);

			int_t	* hasPropertiesInstances = nullptr;
			sdaiGetAttrBN(ifcPropertySetInstance, "HasProperties", sdaiAGGR, &hasPropertiesInstances);
			int_t	hasPropertiesInstancesCnt = sdaiGetMemberCount(hasPropertiesInstances);
			if (hasPropertiesInstancesCnt == 2) {
				int_t	isPropertySingleValueInstance = 0;
				engiGetAggrElement(hasPropertiesInstances, 0, sdaiINSTANCE, &isPropertySingleValueInstance);

				assert(sdaiGetInstanceType(isPropertySingleValueInstance) == sdaiGetEntity(model, (char*) "IFCPROPERTYSINGLEVALUE"));
				double	nominalValue = 0.;

				int_t	* nominalValueADB = nullptr;
				sdaiGetAttrBN(isPropertySingleValueInstance, "NominalValue", sdaiADB, &nominalValueADB);
				sdaiGetADBValue(nominalValueADB, sdaiREAL, (void*) &nominalValue);

				return	nominalValue;
			}
			else {
				assert(false);
			}
		}
	}
	return	0.;
}

static  inline  int_t   GetAlignmentHorizontal(
                                int_t   model,
                                int_t   ifcAlignmentInstance
                            )
{
    int_t   ifcAlignmentHorizontalInstance = 0;

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsNestedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTHORIZONTAL")) {
                    assert(ifcAlignmentHorizontalInstance == 0);
                    ifcAlignmentHorizontalInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    if (ifcAlignmentHorizontalInstance == 0) {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsDecomposedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTHORIZONTAL")) {
                    assert(ifcAlignmentHorizontalInstance == 0);
                    ifcAlignmentHorizontalInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    assert(ifcAlignmentHorizontalInstance);
    return  ifcAlignmentHorizontalInstance;
}

static  inline  int_t   GetAlignmentVertical(
                                int_t   model,
                                int_t   ifcAlignmentInstance
                            )
{
    int_t   ifcAlignmentHorizontalInstance = 0;

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsNestedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICAL")) {
                    assert(ifcAlignmentHorizontalInstance == 0);
                    ifcAlignmentHorizontalInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    if (ifcAlignmentHorizontalInstance == 0) {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsDecomposedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTVERTICAL")) {
                    assert(ifcAlignmentHorizontalInstance == 0);
                    ifcAlignmentHorizontalInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    assert(ifcAlignmentHorizontalInstance);
    return  ifcAlignmentHorizontalInstance;
}

static  inline  int_t   GetAlignmentCant(
                                int_t   model,
                                int_t   ifcAlignmentInstance
                            )
{
    int_t   ifcAlignmentHorizontalInstance = 0;

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsNestedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTCANT")) {
                    assert(ifcAlignmentHorizontalInstance == 0);
                    ifcAlignmentHorizontalInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    if (ifcAlignmentHorizontalInstance == 0) {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentInstance, "IsDecomposedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            engiGetAggrElement(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                engiGetAggrElement(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTCANT")) {
                    assert(ifcAlignmentHorizontalInstance == 0);
                    ifcAlignmentHorizontalInstance = ifcObjectDefinitionInstance;
                }
            }
        }
    }

    assert(ifcAlignmentHorizontalInstance);
    return  ifcAlignmentHorizontalInstance;
}

bool	equals(char* txtI, char* txtII)
{
	if (txtI && txtII) {
		int i = 0;
		while (txtI[i] && txtI[i] == txtII[i]) { i++; }
		if (txtI[i] == 0 && txtII[i] == 0) { return true; }
	}
	return  false;
}

void	CreateLandXML(
				char	* fileNameIFC,
				char	* fileNameLandXML
			)
{
	FILE	* fp = nullptr;
	fopen_s(&fp, fileNameLandXML, "w");

	int_t	model = sdaiOpenModelBN(1, fileNameIFC, "");

	if (fp && model) {
		fprintf(fp, "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n");
		fprintf(fp, "<LandXML xmlns=\"http://www.landxml.org/schema/LandXML-1.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:trv=\"http://www.trafikverket.se/LandXML\" xsi:schemaLocation=\"http://www.landxml.org/schema/LandXML-1.1 http://www.landxml.org/schema/LandXML-1.1/LandXML-1.1.xsd\" version=\"1.1\" date=\"2010-08-20\" time=\"10:00:00\">\n");

		{
			//		fprintf(fp, "  <Project name=\"Sandviken-Kungsgården\" desc=\"TESTSWE\">\n");
			int_t	ifcProjectInstance = 0;
			engiGetAggrElement(sdaiGetEntityExtentBN(model, "IFCPROJECT"), 0, sdaiINSTANCE, &ifcProjectInstance);

			char	* name = nullptr, * description = nullptr;
			sdaiGetAttrBN(ifcProjectInstance, "Name", sdaiSTRING, &name);
			sdaiGetAttrBN(ifcProjectInstance, "Description", sdaiSTRING, &description);

			fprintf(fp, "  <Project name=\"");
			if (name) { fprintf(fp, name); }
			fprintf(fp, "\" desc=\"");
			if (description) { fprintf(fp, description); }
			fprintf(fp, "\">\n");
		}

		fprintf(fp, "    <Feature code=\"GridScaleFactor\">\n");
		fprintf(fp, "      <Property label=\"GridScaleFactor\" value=\"1.000000000000\" />\n");
		fprintf(fp, "    </Feature>\n");
		fprintf(fp, "  </Project>\n");
		fprintf(fp, "  <Units>\n");
//		fprintf(fp, "    <Metric areaUnit=\"squareMeter\" linearUnit=\"meter\" volumeUnit=\"cubicMeter\" temperatureUnit=\"celsius\" pressureUnit=\"HPA\" />\n");
		fprintf(fp, "    <Metric linearUnit=\"meter\"/>\n");
		fprintf(fp, "  </Units>\n");
//		fprintf(fp, "  <Application name=\"Xxx\" manufacturer=\"Yyy, Inc.\" version=\"01.00\" manufacturerURL=\"www.yyy.com\" />\n");
		fprintf(fp, "  <Application name=\"??\" manufacturer=\"??\" version=\"??\" manufacturerURL=\"??\" />\n");
//		fprintf(fp, "  <CoordinateSystem epsgCode=\"3010;5613\" name=\"SWEREF99 16 30;RH 2000\" />\n");
		fprintf(fp, "  <CoordinateSystem epsgCode=\"??\" name=\"??\" />\n");
		
		int_t	* ifcAlignmentInstances = sdaiGetEntityExtentBN(model, "IFCALIGNMENT"),
				noIfcAlignmentInstances = sdaiGetMemberCount(ifcAlignmentInstances);
		if (noIfcAlignmentInstances) {
			fprintf(fp, "  <Alignments>\n");
			for (int_t i = 0; i < noIfcAlignmentInstances; i++) {
				int_t	ifcAlignmentInstance = 0;
				engiGetAggrElement(ifcAlignmentInstances, i, sdaiINSTANCE, &ifcAlignmentInstance);

				double	offset = GetStaStart(model, ifcAlignmentInstance);
				int_t	ifcAlignmentHorizontalInstance = GetAlignmentHorizontal(model, ifcAlignmentInstance);

				int_t   noSegmentInstances = GetAlignmentSegments(model, ifcAlignmentHorizontalInstance, nullptr);
				if (noSegmentInstances) {
					int_t   * segmentInstances = new int_t[noSegmentInstances];
					GetAlignmentSegments(model, ifcAlignmentHorizontalInstance, segmentInstances);

					{
						double	totalLength = 0.;
						for (int_t i = 0; i < noSegmentInstances; i++) {
							int_t	ifcAlignmentHorizontalSegmentInstance = 0;
							sdaiGetAttrBN(segmentInstances[i], "DesignParameters", sdaiINSTANCE, &ifcAlignmentHorizontalSegmentInstance);
							double	segmentLength = 0.;
							sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "SegmentLength", sdaiREAL, &segmentLength);
							totalLength += segmentLength;
						}

						char	* name = nullptr, * description = nullptr;
						sdaiGetAttrBN(ifcAlignmentInstance, "Name", sdaiSTRING, &name);
						sdaiGetAttrBN(ifcAlignmentInstance, "Description", sdaiSTRING, &description);

						double	startDistAlong = 0.;
						sdaiGetAttrBN(ifcAlignmentInstance, "StartDistAlong", sdaiREAL, &startDistAlong);

						fprintf(fp, "    <Alignment oID=\"??\" name=\"");
						if (name) { fprintf(fp, name); }
						fprintf(fp, "\" length=\"%.9f\"", totalLength);
						fprintf(fp, " staStart=\"%.9f\"", startDistAlong + offset);
						fprintf(fp, " state=\"??\" desc=\"");
						if (description) { fprintf(fp, description); }
						fprintf(fp, "\">\n");
					}

					{
						char	* description = nullptr;
						sdaiGetAttrBN(ifcAlignmentHorizontalInstance, "Description", sdaiSTRING, &description);

						fprintf(fp, "      <CoordGeom desc=\"");
						if (description) { fprintf(fp, description); }
						fprintf(fp, "\" state=\"??\">\n");
					}

					double	staStart = 0.;
					for (int_t i = 0; i < noSegmentInstances - 1; i++) {
						int_t	ifcAlignmentHorizontalSegmentInstance = 0,
								NEXT_ifcAlignmentHorizontalSegmentInstance = 0;
						sdaiGetAttrBN(segmentInstances[i], "DesignParameters", sdaiINSTANCE, &ifcAlignmentHorizontalSegmentInstance);
						sdaiGetAttrBN(segmentInstances[i + 1], "DesignParameters", sdaiINSTANCE, &NEXT_ifcAlignmentHorizontalSegmentInstance);

						double	dirStart = 0.,
								dirEnd = 0.,
								startRadiusOfCurvature = 0.,
								endRadiusOfCurvature = 0.,
								length = 0.;
						sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartDirection", sdaiREAL, &dirStart);
						sdaiGetAttrBN(NEXT_ifcAlignmentHorizontalSegmentInstance, "StartDirection", sdaiREAL, &dirEnd);
						sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartRadiusOfCurvature", sdaiREAL, &startRadiusOfCurvature);
						sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndRadiusOfCurvature", sdaiREAL, &endRadiusOfCurvature);
						sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "SegmentLength", sdaiREAL, &length);
						assert(length > 0.);

						char	* predefinedType = nullptr;	
						sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "PredefinedType", sdaiENUM, &predefinedType);
						if (equals(predefinedType, (char*) "CIRCULARARC")) {
							assert(startRadiusOfCurvature == endRadiusOfCurvature);
//							fprintf(fp, "        <Curve rot=\"ccw\" crvType=\"arc\" length=\"40.6135696\" chord=\"40.6127964\" dirStart=\"4.586640426\" dirEnd=\"4.565264863\" staStart=\"27581.1408441\" radius=\"1900.0000000\">\n");
							fprintf(fp, "        <Curve rot=\"");
							if (startRadiusOfCurvature < 0.) {
								fprintf(fp, "ccw");
							}
							else {
								fprintf(fp, "cw");
							}
							fprintf(fp, "\"");
							fprintf(fp, " crvType=\"arc\"");
							fprintf(fp, " length=\"%.7f\"", length);
							fprintf(fp, " chord=\"??.???\"");
							fprintf(fp, " dirStart=\"%.7f\"", dirStart);
							fprintf(fp, " dirEnd=\"%.7f\"", dirEnd);
							fprintf(fp, " staStart=\"%.7f\"", staStart + offset);
							fprintf(fp, " radius=\"%.7f\">\n", std::fabs(startRadiusOfCurvature));
						}
						else if (equals(predefinedType, (char*) "CLOTHOID")) {
//							fprintf(fp, "        <Spiral length=\"92.0000000\" radiusEnd=\"1900.0000000\" radiusStart=\"INF\" rot=\"ccw\" spiType=\"clothoid\" dirStart=\"4.610850952\" dirEnd=\"4.586640426\" staStart=\"27489.1408441\" constant=\"418.0908992\" theta=\"0.024210526\" totalY=\"0.7424250\" totalX=\"91.9946076\" tanLong=\"61.3352199\" tanShort=\"30.6683754\">\n");
							fprintf(fp, "        <Spiral");
							fprintf(fp, " length=\"%.7f\"", length);
							if (endRadiusOfCurvature) {
								fprintf(fp, " radiusEnd=\"%.7f\"", std::fabs(endRadiusOfCurvature));
							}
							else {
								fprintf(fp, " radiusEnd=\"INF\"");
							}
							if (startRadiusOfCurvature) {
								fprintf(fp, " radiusStart=\"%.7f\"", std::fabs(startRadiusOfCurvature));
							}
							else {
								fprintf(fp, " radiusStart=\"INF\"");
							}
							if (startRadiusOfCurvature + endRadiusOfCurvature < 0.) {
								fprintf(fp, " rot=\"ccw\"");
							}
							else {
								fprintf(fp, " rot=\"cw\"");
							}
							fprintf(fp, " spiType=\"clothoid\"");
							fprintf(fp, " dirStart=\"%.7f\"", dirStart);
							fprintf(fp, " dirEnd=\"%.7f\"", dirEnd);
							fprintf(fp, " staStart=\"%.7f\"", staStart + offset);
							fprintf(fp, " constant=\"??.???\"");
							fprintf(fp, " theta=\"??.???\"");
							fprintf(fp, " totalY=\"??.???\"");
							fprintf(fp, " totalX=\"??.???\"");
							fprintf(fp, " tanLong=\"??.???9\"");
							fprintf(fp, " tanShort=\"??.???\">\n");
						}
						else if (equals(predefinedType, (char*) "LINE")) {
							assert(startRadiusOfCurvature == 0.);
							assert(endRadiusOfCurvature == 0.);
							assert(std::fabs(dirStart - dirEnd) < 0.00001);
//							fprintf(fp, "        <Line length=\"1508.3410981\" dir=\"4.610850952\" staStart=\"25980.7997460\">\n");
							fprintf(fp, "        <Line");
							fprintf(fp, " length=\"%.7f\"", length);
							fprintf(fp, " dir=\"%.7f\"", dirStart);
							fprintf(fp, " staStart=\"%.7f\">\n", staStart + offset);
						}
						else {
							assert(false);
						}

						{
							//          <Start code="TS">
							//              6721786.180648 161072.376573 0.000000
							//          </Start>
							fprintf(fp, "          <Start code=\"");
							char	* startTag = nullptr;
							sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartTag", sdaiSTRING, &startTag);
							if (startTag) { fprintf(fp, startTag); }
							fprintf(fp, "\">\n");
							int_t	ifcCartesianPointInstance = 0;
							sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, &ifcCartesianPointInstance);
							double	x = 0., y = 0., z = 0.;
							GetCartesianPointCoordinates(ifcCartesianPointInstance, &x, &y, &z);
							fprintf(fp, "              %.7f %.7f %.7f\n", x, y, z);
							fprintf(fp, "          </Start>\n");
						}

						{
							//          <End code="SC">
							//              6721776.117138 160980.931044 0.000000
							//          </End>
							fprintf(fp, "          <End code=\"");
							char	* endTag = nullptr;
							sdaiGetAttrBN(ifcAlignmentHorizontalSegmentInstance, "EndTag", sdaiSTRING, &endTag);
							if (endTag) { fprintf(fp, endTag); }
							fprintf(fp, "\">\n");
							int_t	ifcCartesianPointInstance = 0;
							sdaiGetAttrBN(NEXT_ifcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, &ifcCartesianPointInstance);
							double	x = 0., y = 0., z = 0.;
							GetCartesianPointCoordinates(ifcCartesianPointInstance, &x, &y, &z);
							fprintf(fp, "              %.7f %.7f %.7f\n", x, y, z);
							fprintf(fp, "          </End>\n");
						}

						if (equals(predefinedType, (char*) "CIRCULARARC")) {
							fprintf(fp, "        </Curve>\n");
						}
						else if (equals(predefinedType, (char*) "CLOTHOID")) {
							fprintf(fp, "        </Spiral>\n");
						}
						else if (equals(predefinedType, (char*) "LINE")) {
							fprintf(fp, "        </Line>\n");
						}
						else {
							assert(false);
						}
						staStart += length;
					}
					fprintf(fp, "      </CoordGeom>\n");

					delete[] segmentInstances;
				}

				int_t	ifcAlignmentVerticalInstance = GetAlignmentVertical(model, ifcAlignmentInstance);
				{
					int_t   noSegmentInstances = GetAlignmentSegments(model, ifcAlignmentVerticalInstance, nullptr);
					if (noSegmentInstances) {
						fprintf(fp, "      <Profile>\n");
						fprintf(fp, "        <ProfAlign name=\"??\">\n");

						int_t   * segmentInstances = new int_t[noSegmentInstances];
						GetAlignmentSegments(model, ifcAlignmentVerticalInstance, segmentInstances);

						{
							int_t i = 0;

							int_t	alignmentVerticalSegmentInstance = 0;
							sdaiGetAttrBN(segmentInstances[i], "DesignParameters", sdaiINSTANCE, &alignmentVerticalSegmentInstance);

							double	__startDistAlong = 0.;
							sdaiGetAttrBN(alignmentVerticalSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &__startDistAlong);

							double	__startHeight = 0.;
							sdaiGetAttrBN(alignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, (void*) &__startHeight);

							fprintf(fp, "          <PVI>\n");
							fprintf(fp, "              %.6f %.6f\n", __startDistAlong + offset, __startHeight);
							fprintf(fp, "          </PVI>\n");

							i++;
							while (i < noSegmentInstances) {
								int_t	alignmentVerticalSegmentInstance = 0;
								sdaiGetAttrBN(segmentInstances[i], "DesignParameters", sdaiINSTANCE, &alignmentVerticalSegmentInstance);

								char	* predefinedType = nullptr;
								sdaiGetAttrBN(alignmentVerticalSegmentInstance, "PredefinedType", sdaiENUM, (void*) &predefinedType);
								if (equals(predefinedType, (char*) "CIRCULARARC") && i + 1 < noSegmentInstances) {
									int_t	P__alignmentVerticalSegmentInstance = 0;
									sdaiGetAttrBN(segmentInstances[i - 1], "DesignParameters", sdaiINSTANCE, &P__alignmentVerticalSegmentInstance);

									double	P__startDistAlong = 0.;
									sdaiGetAttrBN(P__alignmentVerticalSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &P__startDistAlong);

									double	P__startHeight = 0.;
									sdaiGetAttrBN(P__alignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, (void*) &P__startHeight);

									double	P__horizontalLength = 0.;
									sdaiGetAttrBN(P__alignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &P__horizontalLength);

									double	P__startGradient = 0.;
									sdaiGetAttrBN(P__alignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, (void*) &P__startGradient);


									double	__startHeight = 0.;
									sdaiGetAttrBN(alignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, (void*) &__startHeight);

									double	__radiusOfCurvature = 0.;
									sdaiGetAttrBN(alignmentVerticalSegmentInstance, "RadiusOfCurvature", sdaiREAL, (void*) &__radiusOfCurvature);

									double	__horizontalLength = 0.;
									sdaiGetAttrBN(alignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &__horizontalLength);

									double	angle = 2. * asin((__horizontalLength / 2.) / std::fabs(__radiusOfCurvature));
//									double	length = (std::fabs(__radiusOfCurvature) * (2. * Pi)) * angle / (2. * Pi);
									double	length = (std::fabs(__radiusOfCurvature)) * angle;

									fprintf(fp, "          <CircCurve radius=\"%.7f\" length=\"%.6f\">\n", __radiusOfCurvature, length);
									fprintf(fp, "              %.6f %.6f\n", P__startDistAlong + P__horizontalLength + __horizontalLength / 2. + offset, __startHeight + (__horizontalLength / 2.) * P__startGradient);
									fprintf(fp, "          </CircCurve>\n");
									

									i += 2;
								}
								else {
									assert(false);
									i++;
								}
							}

							i = noSegmentInstances - 1;

							{
								int_t	alignmentVerticalSegmentInstance = 0;
								sdaiGetAttrBN(segmentInstances[i], "DesignParameters", sdaiINSTANCE, &alignmentVerticalSegmentInstance);

								double	__startDistAlong = 0.;
								sdaiGetAttrBN(alignmentVerticalSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &__startDistAlong);

								double	__startHeight = 0.;
								sdaiGetAttrBN(alignmentVerticalSegmentInstance, "StartHeight", sdaiREAL, (void*) &__startHeight);

								double	__horizontalLength = 0.;
								sdaiGetAttrBN(alignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &__horizontalLength);

								double	__startGradient = 0.;
								sdaiGetAttrBN(alignmentVerticalSegmentInstance, "StartGradient", sdaiREAL, (void*) &__startGradient);

								fprintf(fp, "          <PVI>\n");
								fprintf(fp, "              %.6f %.6f\n", __startDistAlong + __horizontalLength + offset, __startHeight + __startGradient * __horizontalLength);
								fprintf(fp, "          </PVI>\n");
							}
						}

						fprintf(fp, "        </ProfAlign>\n");
						fprintf(fp, "      </Profile>\n");
					}
				}

				int_t	ifcAlignmentCantInstance = GetAlignmentCant(model, ifcAlignmentInstance);
				{
					int_t   noSegmentInstances = GetAlignmentSegments(model, ifcAlignmentCantInstance, nullptr);
					if (noSegmentInstances) {
						fprintf(fp, "      <Cant name=\"??\" desc=\"??\" gauge=\"??.??\" rotationPoint=\"??\">\n");

						int_t   * segmentInstances = new int_t[noSegmentInstances];
						GetAlignmentSegments(model, ifcAlignmentCantInstance, segmentInstances);

						{
							double	__horizontalLength = 0.;
							for (int_t i = 0; i < noSegmentInstances; i++) {
								int_t	alignmentCantSegmentInstance = 0;
								sdaiGetAttrBN(segmentInstances[i], "DesignParameters", sdaiINSTANCE, &alignmentCantSegmentInstance);

								double	__startDistAlong = 0.;
								sdaiGetAttrBN(alignmentCantSegmentInstance, "StartDistAlong", sdaiREAL, (void*) &__startDistAlong);

								sdaiGetAttrBN(alignmentCantSegmentInstance, "HorizontalLength", sdaiREAL, (void*) &__horizontalLength);
								__horizontalLength += __startDistAlong;

								fprintf(fp, "        <CantStation station=\"%.9f\"", __startDistAlong + offset);

								char	* predefinedType = nullptr;
								sdaiGetAttrBN(alignmentCantSegmentInstance, "PredefinedType", sdaiENUM, (void*) &predefinedType);
								if (equals(predefinedType, (char*) "LINEARTRANSITION")) {
									fprintf(fp, " transitionType = \"clothoid\"");
								}

								fprintf(fp, "/>\n");
							}

							fprintf(fp, "        <CantStation station=\"%.9f\"/>\n", __horizontalLength + offset);
						}

						fprintf(fp, "      </Cant>\n");
					}
				}

				fprintf(fp, "    </Alignment>\n");
			}
			fprintf(fp, "  </Alignments>\n");
		}
		fprintf(fp, "</LandXML>\n");
	
		sdaiCloseModel(model);
		fclose(fp);
	}
	else {
		assert(false);

		if (model) {
			sdaiCloseModel(model);
		}
		
		if (fp) {
			fclose(fp);
		}
	}
}