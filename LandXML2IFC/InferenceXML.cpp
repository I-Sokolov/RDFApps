// LandXML2IFC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "InferenceXML.h"

#include "assert.h"

#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

//std::string to_string(double x);

double	Pi = 3.141592653589793238462643;

struct	VECTOR2
{
	double		x, y;
};

struct	VECTOR3
{
	double		x, y, z;
};

double	Sqr_(double a) {
	return	a * a;
}

double	Vec3Normalize(
				VECTOR3			* pOut,
				const VECTOR3	* pV
			)
{
	double	size = Sqr_(pV->x) + Sqr_(pV->y) + Sqr_(pV->z);

	if (size > 0.0000000000000001) {
		double	sqrtSize = sqrt(size);

		pOut->x = pV->x / sqrtSize;
		pOut->y = pV->y / sqrtSize;
		pOut->z = pV->z / sqrtSize;

		return	sqrtSize;
	}
	else {
		pOut->x = 0.;
		pOut->y = 0.;
		pOut->z = 0.;

		return	0.;
	}
}

void	Vec3Subtract(
				VECTOR3			* pOut,
				const VECTOR3	* pV1,
				const VECTOR3	* pV2
			)
{
	assert(pOut != pV1);

	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
}

void	GetCoordinates(
				STRUCT_ELEMENT_LIST	* elements,
				char				* tag,
				VECTOR3				* vec
			)
{
	assert(GetElementCnt(elements, tag) == 1);
	STRUCT_ELEMENT		* elementCoordinates = GetElement(elements, 0, tag);
	STRUCT_ELEMENT_LIST	* elementsCoordinates = elementCoordinates->elements;

	if (elementsCoordinates && elementsCoordinates->element == nullptr) {
		vec->x = atof(elementsCoordinates->value);
		if (elementsCoordinates->next && elementsCoordinates->next->element == nullptr) {
			vec->y = atof(elementsCoordinates->next->value);
			if (elementsCoordinates->next->next && elementsCoordinates->next->next->element == nullptr) {
				vec->z = atof(elementsCoordinates->next->next->value);
				assert(elementsCoordinates->next->next->next == nullptr);
				return;
			}
		}
	}
	assert(false);
}

void	InferenceCurve(
				STRUCT_ELEMENT_LIST	* elements
			)
{
	int elementLineCnt = GetElementCnt(elements, (char*) "Curve");
	for (int i = 0; i < elementLineCnt; i++) {
		STRUCT_ELEMENT	* elementLine = GetElement(elements, i, (char*) "Curve");
		char	* dirStart = GetAttribute(elementLine, (char*) "dirStart"),
				* dirEnd = GetAttribute(elementLine, (char*) "dirEnd"),
				* rot = GetAttribute(elementLine, (char*) "rot");
		{
			VECTOR3	vecStart = { 0., 0., 0. },
					vecCenter = { 0., 0., 0. },
					vecEnd = { 0., 0., 0. };
			GetCoordinates(elementLine->elements, (char*) "Start", &vecStart);
			GetCoordinates(elementLine->elements, (char*) "Center", &vecCenter);
			GetCoordinates(elementLine->elements, (char*) "End", &vecEnd);
			assert(vecStart.z == 0. && vecCenter.z == 0. && vecEnd.z == 0.);
			
			{
				VECTOR3	vecDirection;
				Vec3Subtract(&vecDirection, &vecStart, &vecCenter);
				Vec3Normalize(&vecDirection, &vecDirection);
				VECTOR2	vec2Start = { vecDirection.y, -vecDirection.x };
				if (equals(rot, (char*) "cw")) { vec2Start.x = -vec2Start.x; vec2Start.y = -vec2Start.y; }
				double	angleStartDir = atan2(vec2Start.y, vec2Start.x);
				if (angleStartDir < 0.) { angleStartDir += 2 * Pi; }

				if (dirStart == nullptr) {
					STRUCT_ATTRIBUTE_LIST	* myAttributeList = new STRUCT_ATTRIBUTE_LIST();
					myAttributeList->attribute = newATTRIBUTE((char*) "dirStart");
					myAttributeList->attribute->value = new char[32];
					sprintf_s(myAttributeList->attribute->value, 32, "%.9f", angleStartDir);
					myAttributeList->next = elementLine->attributes;
					elementLine->attributes = myAttributeList;
				}
				else {
					double	value = atof(dirStart);
					assert(std::fabs(value - angleStartDir) < 0.0000001);
				}
			}

			//...
			//...
			//...

			{
				VECTOR3	vecDirection;
				Vec3Subtract(&vecDirection, &vecEnd, &vecCenter);
				Vec3Normalize(&vecDirection, &vecDirection);
				VECTOR2	vec2End = { vecDirection.y, -vecDirection.x };
				if (equals(rot, (char*) "cw")) { vec2End.x = -vec2End.x; vec2End.y = -vec2End.y; }
				double	angleEndDir = atan2(vec2End.y, vec2End.x);
				if (angleEndDir < 0.) { angleEndDir += 2 * Pi; }

				if (dirEnd == nullptr) {
					STRUCT_ATTRIBUTE_LIST	* myAttributeList = new STRUCT_ATTRIBUTE_LIST();
					myAttributeList->attribute = newATTRIBUTE((char*) "dirEnd");
					myAttributeList->attribute->value = new char[32];
					sprintf_s(myAttributeList->attribute->value, 32, "%.9f", angleEndDir);
					myAttributeList->next = elementLine->attributes;
					elementLine->attributes = myAttributeList;
				}
				else {
					double	value = atof(dirEnd);
					assert(std::fabs(value - angleEndDir) < 0.0000001);
				}
			}
		}
	}
}

void	InferenceLine(
				STRUCT_ELEMENT_LIST	* elements
			)
{
	int elementLineCnt = GetElementCnt(elements, (char*) "Line");
	for (int i = 0; i < elementLineCnt; i++) {
		STRUCT_ELEMENT	* elementLine = GetElement(elements, i, (char*) "Line");
		char	* dir = GetAttribute(elementLine, (char*) "dir");
		{
			VECTOR3	vecStart = { 0., 0., 0. },
					vecEnd = { 0., 0., 0. };
			GetCoordinates(elementLine->elements, (char*) "Start", &vecStart);
			GetCoordinates(elementLine->elements, (char*) "End", &vecEnd);
			assert(vecStart.z == 0. && vecEnd.z == 0.);
			VECTOR3	vecDirection;
			Vec3Subtract(&vecDirection, &vecEnd, &vecStart);
			Vec3Normalize(&vecDirection, &vecDirection);
			double	angleDirection = atan2(vecDirection.y, vecDirection.x);
			if (angleDirection < 0.) { angleDirection += 2 * Pi; }

			if (dir == nullptr) {
				STRUCT_ATTRIBUTE_LIST	* myAttributeList = new STRUCT_ATTRIBUTE_LIST();
				myAttributeList->attribute = newATTRIBUTE((char*) "dir");
				myAttributeList->attribute->value = new char[32];
				sprintf_s(myAttributeList->attribute->value, 32, "%.9f", angleDirection);
				myAttributeList->next = elementLine->attributes;
				elementLine->attributes = myAttributeList;
			}
			else {
				double	value = atof(dir);
				assert(std::fabs(value - angleDirection) < 0.0000001);
			}
		}
	}
}

void	AddAttribute(
				STRUCT_ELEMENT	* element,
				char			* tag,
				char			* value
			)
{
	STRUCT_ATTRIBUTE_LIST	* myAttributeList = new STRUCT_ATTRIBUTE_LIST();
	myAttributeList->attribute = newATTRIBUTE(tag);
	myAttributeList->attribute->value = value;
	myAttributeList->next = element->attributes;
	element->attributes = myAttributeList;
}

void	InferenceDirections(
				STRUCT_ELEMENT_LIST	* elements
			)
{
	int	alignmentCnt = GetElementCnt(elements, (char*) "Alignment");
	for (int i = 0; i < alignmentCnt; i++) {
		STRUCT_ELEMENT	* alignmentElement = GetElement(elements, i, (char*) "Alignment"),
						* coordGeomElement = GetElement(alignmentElement->elements, 0, (char*) "CoordGeom");

		STRUCT_ELEMENT_LIST	* PREV_coordGeomElements = nullptr,
							* coordGeomElements = coordGeomElement->elements;
		char	* prevDirEnd = nullptr;
		while (coordGeomElements) {
			if (equals(coordGeomElements->element->tag->name, (char*) "Curve")) {
				char	* dirStart = GetAttribute(coordGeomElements->element, (char*) "dirStart"),
						* dirEnd = GetAttribute(coordGeomElements->element, (char*) "dirEnd");

				assert(dirStart && dirEnd);
				prevDirEnd = dirEnd;
			}
			else if (equals(coordGeomElements->element->tag->name, (char*) "Line")) {
				char	* dir = GetAttribute(coordGeomElements->element, (char*) "dir");

				assert(dir);
				prevDirEnd = dir;
			}
			else if (equals(coordGeomElements->element->tag->name, (char*) "Spiral")) {
				char	* dirStart = GetAttribute(coordGeomElements->element, (char*) "dirStart"),
						* dirEnd = GetAttribute(coordGeomElements->element, (char*) "dirEnd");

				assert(coordGeomElements->next && PREV_coordGeomElements);

				if (dirStart == nullptr) {
					assert(prevDirEnd);
					AddAttribute(coordGeomElements->element, (char*) "dirStart", prevDirEnd);
				}

				if (dirEnd == nullptr) {
					char	* myDir = nullptr;
					STRUCT_ELEMENT_LIST	* myCoordGeomElements = coordGeomElements->next;
					if (myCoordGeomElements) {
						if (equals(myCoordGeomElements->element->tag->name, (char*) "Curve")) {
							myDir = GetAttribute(myCoordGeomElements->element, (char*) "dirStart");
						}
						else if (equals(myCoordGeomElements->element->tag->name, (char*) "Line")) {
							myDir = GetAttribute(myCoordGeomElements->element, (char*) "dir");
						}
						else {
							assert(false);
						}
					}
					assert(myDir);
					AddAttribute(coordGeomElements->element, (char*) "dirEnd", myDir);
				}

				prevDirEnd = nullptr;
			}
			else {
				assert(false);
			}

			PREV_coordGeomElements = coordGeomElements;
			coordGeomElements = coordGeomElements->next;
		}
	}
}

void	InferenceXML(
				STRUCT_ELEMENT_LIST	* elements
			)
{
	InferenceCurve(elements);
	InferenceLine(elements);

	InferenceDirections(elements);
}
