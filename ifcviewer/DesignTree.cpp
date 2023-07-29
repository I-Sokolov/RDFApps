
// DesignTree.cpp : .
//

#include "stdafx.h"
#include "DesignTree.h"

#include "ifcengine\include\engine.h"



static int64_t flagbit0__64bit = 1;				// 2^^0    0000.0000..0000.0001
static int64_t flagbit1__64bit = 2;				// 2^^1    0000.0000..0000.0010
static int64_t flagbit2__64bit = 4;				// 2^^2    0000.0000..0000.0100
static int64_t flagbit3__64bit = 8;				// 2^^3    0000.0000..0000.1000

static int64_t flagbit4__64bit = 16;			// 2^^4    0000.0000..0001.0000
static int64_t flagbit5__64bit = 32;			// 2^^5    0000.0000..0010.0000
static int64_t flagbit6__64bit = 64;			// 2^^6    0000.0000..0100.0000
static int64_t flagbit7__64bit = 128;			// 2^^7    0000.0000..1000.0000

static int64_t flagbit8__64bit = 256;			// 2^^8    0000.0001..0000.0000
static int64_t flagbit9__64bit = 512;			// 2^^9    0000.0010..0000.0000
static int64_t flagbit10__64bit = 1024;			// 2^^10   0000.0100..0000.0000
static int64_t flagbit11__64bit = 2048;			// 2^^11   0000.1000..0000.0000

static int64_t flagbit12__64bit = 4096;			// 2^^12   0001.0000..0000.0000
static int64_t flagbit13__64bit = 8192;			// 2^^13   0010.0000..0000.0000
static int64_t flagbit14__64bit = 16384;		// 2^^14   0100.0000..0000.0000
static int64_t flagbit15__64bit = 32768;		// 2^^15   1000.0000..0000.0000



int64_t		CreateLine3Dn(
				int64_t		owlModel,
				double		x,
				double		y,
				double		z
			)
{
	double	points[6];

	points[0] = 0;
	points[1] = 0;
	points[2] = 0;
	points[3] = x;
	points[4] = y;
	points[5] = z;

	int64_t	owlInstanceLine3D = CreateInstance(GetClassByName(owlModel, "Line3Dn"), (char*)nullptr);

	SetDatatypeProperty(owlInstanceLine3D, GetPropertyByName(owlModel, "points"), points, 6);

	return	owlInstanceLine3D;
}

int64_t		CreateLine3Dn(
					int64_t		owlModel,
					double		minX,
					double		minY,
					double		minZ,
					double		maxX,
					double		maxY,
					double		maxZ
				)
{
	double	points[6];

	points[0] = minX;
	points[1] = minY;
	points[2] = minZ;
	points[3] = maxX;
	points[4] = maxY;
	points[5] = maxZ;

	int64_t	owlInstanceLine3D = CreateInstance(GetClassByName(owlModel, "Line3Dn"), (char*)nullptr);

	SetDatatypeProperty(owlInstanceLine3D, GetPropertyByName(owlModel, "points"), points, 6);

	return	owlInstanceLine3D;
}

int64_t		UpdateDesignTree(
				int64_t		owlModel,
				int64_t		owlInstance
			)
{

	int64_t	owlClass = GetInstanceClass(owlInstance);

	if (owlClass == GetClassByName(owlModel, "Collection")) {
		//
		//	Walk through all objects
		//
		int64_t	* values = nullptr, card = 0;
		GetObjectProperty(owlInstance, GetPropertyByName(owlModel, "objects"), &values, &card);
		if (card) {
			int64_t * returnValues = new int64_t[(unsigned int)card];
			for (int64_t i = 0; i < card; i++) {
				returnValues[i] = UpdateDesignTree(owlModel, values[i]);
			}

			SetObjectProperty(owlInstance, GetPropertyByName(owlModel, "objects"), returnValues, card);
		}

		return	owlInstance;
	}
	else if (owlClass == GetClassByName(owlModel, "Transformation")) {
		//
		//	Walk through object
		//
		int64_t	* values = nullptr, card = 0;
		GetObjectProperty(owlInstance, GetPropertyByName(owlModel, "object"), &values, &card);
		if (card == 1) {
			int64_t	returnValue = UpdateDesignTree(owlModel, values[0]);

			SetObjectProperty(owlInstance, GetPropertyByName(owlModel, "object"), &returnValue, card);
		}

		return	owlInstance;
	}
	else if (owlClass == GetClassByName(owlModel, "BooleanOperation")) {
		//
		//	Walk through object
		//
		int64_t	* values = nullptr, card = 0;
		GetObjectProperty(owlInstance, GetPropertyByName(owlModel, "firstObject"), &values, &card);
		if (card == 1) {
			return	UpdateDesignTree(owlModel, values[0]);
		}

		return	owlInstance;
	}
	else if (owlClass == GetClassByName(owlModel, "ExtrudedPolygon")) {
		//
		//	Replace it with the polygonDirection * extrusionLength
		//
		double	* valuesPolygonDirection = nullptr, * valuesExtrusionLength = nullptr, x, y, z;
		int64_t	cardPolygonDirection = 0, cardExtrusionLength = 0;

		GetDatatypeProperty(owlInstance, GetPropertyByName(owlModel, "polygonDirection"), (void**) &valuesPolygonDirection, &cardPolygonDirection);
		if (cardPolygonDirection == 3) {
			x = valuesPolygonDirection[0];
			y = valuesPolygonDirection[1];
			z = valuesPolygonDirection[2];
		}
		else {
			ASSERT(cardPolygonDirection == 0);
			x = 0;
			y = 0;
			z =	1;
		}

		GetDatatypeProperty(owlInstance, GetPropertyByName(owlModel, "extrusionLength"), (void**)&valuesExtrusionLength, &cardExtrusionLength);
		if (cardExtrusionLength == 1) {
			x *= valuesExtrusionLength[0];
			y *= valuesExtrusionLength[0];
			z *= valuesExtrusionLength[0];
		}
		else {
			ASSERT(false);
		}

		return	CreateLine3Dn(owlModel, x, y, z);
	}
	else if (owlClass == GetClassByName(owlModel, "SweptAreaSolid")) {
		//
		//	Replace it with the path direction
		//
		int64_t	* values = nullptr, card = 0;
		GetObjectProperty(owlInstance, GetPropertyByName(owlModel, "direction"), &values, &card);
		if (card == 1) {
			return	values[0];
		}

		ASSERT(false);
		return	owlInstance;
	}
	else if (owlClass == GetClassByName(owlModel, "BoundaryRepresentation")) {
		int64_t	owlInstanceLine3Dn = 0;

		//
		//	Replace it with the path direction
		//
		int64_t	setting = 0, mask = 2;
		mask += flagbit2__64bit;			//    PRECISION (32/64 bit)
		mask += flagbit3__64bit;			//	  INDEX ARRAY (32/64 bit)
		mask += flagbit5__64bit;			//    NORMALS
		mask += flagbit8__64bit;			//    TRIANGLES
		mask += flagbit9__64bit;			//    LINES
		mask += flagbit10__64bit;			//    POINTS
		mask += flagbit12__64bit;			//    WIREFRAME

		setting += flagbit2__64bit;			//    DOUBLE PRECISION (double)
		setting += 0;						//    32 BIT INDEX ARRAY (Int32)
		setting += 0;						//    NORMALS OFF
		setting += flagbit8__64bit;			//    TRIANGLES ON
		setting += 0;						//    LINES ON
		setting += 0;						//    POINTS OFF
		setting += 0;						//    WIREFRAME OFF
		
		int64_t originalSetting = GetFormat(owlModel, mask);
		SetFormat(owlModel, setting, mask);

		int64_t	vertexBufferSize = 0, indexBufferSize = 0;
		CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, 0);
		if (vertexBufferSize && indexBufferSize) {
			double	* vertices = new double[3 * (int) vertexBufferSize];
			UpdateInstanceVertexBuffer(owlInstance, vertices);
			double	minX = vertices[0], maxX = minX,
					minY = vertices[1], maxY = minY,
					minZ = vertices[2], maxZ = minZ;
			for (int64_t i = 1; i < vertexBufferSize; i++) {
				if (minX > vertices[3 * i + 0]) { minX = vertices[3 * i + 0]; }
				if (maxX < vertices[3 * i + 0]) { maxX = vertices[3 * i + 0]; }
				if (minY > vertices[3 * i + 1]) { minY = vertices[3 * i + 1]; }
				if (maxY < vertices[3 * i + 1]) { maxY = vertices[3 * i + 1]; }
				if (minZ > vertices[3 * i + 2]) { minZ = vertices[3 * i + 2]; }
				if (maxZ < vertices[3 * i + 2]) { maxZ = vertices[3 * i + 2]; }
			}
//			if (minX >= -0.000001) {
				owlInstanceLine3Dn = CreateLine3Dn(owlModel, minX, 0, 0, maxX, 0, 0);
//			}
		}
		SetFormat(owlModel, originalSetting, mask);

		return owlInstanceLine3Dn;
	}

//	char * name = nullptr;
//	GetNameOfClass(owlClass, &name);
//	SaveModel(owlModel, "c:\\9\\out.txt");
	return	owlInstance;
}
