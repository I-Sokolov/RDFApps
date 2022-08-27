#include "stdafx.h"
#include "IFCEngineInteract.h"
#include "material.h"
#include "headerInfo.h"
#include "unit.h"
#include "containsdecomposedby.h"
#include "DesignTree.h"

#include "ProgressWnd.h"

#include "IfcSiteTranslation.h"


#include <unordered_map>
#include <assert.h>


extern	STRUCT__SIUNIT	* units;

extern	__int64			encodingSetting,
						encodingMask;

int_t	ifcModelGlobalTmp = 0;
int_t	ifcSpace_TYPE = 0,
		ifcDistributionElement_TYPE = 0,
		ifcElectricalElement_TYPE = 0,
		ifcElementAssembly_TYPE = 0,
		ifcElementComponent_TYPE = 0,
		ifcEquipmentElement_TYPE = 0,
		ifcFeatureElement_TYPE = 0,
		ifcFeatureElementSubtraction_TYPE = 0,
		ifcFurnishingElement_TYPE = 0,
		ifcReinforcingElement_TYPE = 0,
		ifcTransportElement_TYPE = 0,
		ifcVirtualElement_TYPE = 0;

std::list<D3DMATERIAL9*> mtrls;
extern	STRUCT_MATERIAL	* listOfMaterials;
extern	std::list<D3DMATERIAL9*> mtrls;

wchar_t	ifcFileName[512], * ifcSchemaName_IFC2x3_TC1 = nullptr, * ifcSchemaName_IFC4x3 = nullptr;

STRUCT_TREE_ITEM			* topTreeItem = nullptr, * topModelTreeItem = nullptr, * topSpaceBoundaryTreeItem = nullptr, * topNonReferencedTreeItem = nullptr, * topGroupTreeItem = nullptr;
STRUCT__IFC__OBJECT			* ifcObjectsLinkedList = nullptr;

int_t	globalIfcModel = 0;
bool	firstItemWithGeometryPassed;

std::unordered_map<int_t, STRUCT__IFC__OBJECT *> allIfcObjects;

STRUCT__IFC__OBJECT	** queryIfcObjects(
							int_t				ifcModel,
							STRUCT__IFC__OBJECT	** firstFreeIfcObject,
							wchar_t				* entityName,
							bool				hide,
							int_t				segmentationParts
						)
{
	int_t	i, * ifcObjectInstances, noIfcObjectInstances;

	ifcObjectInstances = sdaiGetEntityExtentBN(ifcModel, (char*) entityName);

	noIfcObjectInstances = sdaiGetMemberCount(ifcObjectInstances);
	if	(noIfcObjectInstances) {
		int_t	ifcEntity = sdaiGetEntity(ifcModel, (char*) entityName);
		for (i = 0; i < noIfcObjectInstances; ++i) {
			int_t	ifcObjectInstance = 0;
			engiGetAggrElement(ifcObjectInstances, i, sdaiINSTANCE, &ifcObjectInstance);

			STRUCT__IFC__OBJECT	* ifcObject = CreateIfcObject(ifcEntity, ifcObjectInstance, entityName, hide, segmentationParts);
			(*firstFreeIfcObject) = ifcObject;
			firstFreeIfcObject = &ifcObject->next;

			allIfcObjects[ifcObjectInstance] = ifcObject;
		}
	}

	return	firstFreeIfcObject;
}

struct		PLANEf {
	float			a;
	float			b;
	float			c;
	float			d;
};

struct		VECTOR3f {
	float			x;
	float			y;
	float			z;
};

float	Vector3Dot(
				const VECTOR3f	* pV1,
				const VECTOR3f	* pV2
			)
{
	return	pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

float	Vector3Normalize(
				VECTOR3f	* pInOut
			)
{
	float	size, sqrtSize = 0,
			x = pInOut->x, y = pInOut->y, z = pInOut->z;

	size = x * x + y * y + z * z;

	if (size > 0.0000000000000001) {
		sqrtSize = sqrt(size);
		pInOut->x = x / sqrtSize;
		pInOut->y = y / sqrtSize;
		pInOut->z = z / sqrtSize;
	}
	else {
		if (pInOut->x) { pInOut->x = 0; }
		if (pInOut->y) { pInOut->y = 0; }
		if (pInOut->z) { pInOut->z = 0; }
	}

	return	sqrtSize;
}

void	Vector3Subtract(
				VECTOR3f		* pOut,
				const VECTOR3f	* pV1,
				const VECTOR3f	* pV2
			)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
}

bool	PlaneFromPoints(
				PLANEf			* pOut,
				const VECTOR3f	* pV1,
				const VECTOR3f	* pV2,
				const VECTOR3f	* pV3
			)
{
	VECTOR3f	dV1, dV2;
	Vector3Subtract(&dV1, pV1, pV2);
	Vector3Subtract(&dV2, pV2, pV3);

	pOut->a = dV1.y * dV2.z - dV1.z * dV2.y;
	pOut->b = dV1.z * dV2.x - dV1.x * dV2.z;
	pOut->c = dV1.x * dV2.y - dV1.y * dV2.x;
	double	size = Vector3Normalize((VECTOR3f*) pOut);
	if (size == 0) {
		assert((pOut->a > -0.00000001 && pOut->a < 0.00000001) && (pOut->b > -0.00000001 && pOut->b < 0.00000001) && (pOut->c > -0.00000001 && pOut->c < 0.00000001));
		pOut->a = 0;
		pOut->b = 0;
		pOut->c = 1;
		return	false;
	}
	pOut->d = -(pV3->x * pOut->a + pV3->y * pOut->b + pV3->z * pOut->c);
	return	true;
}

void	GenerateWireFrameGeometry(
				int_t				ifcModel,
				STRUCT__IFC__OBJECT	* ifcObject,
				int64_t				vertexElementSize
			)
{
	ASSERT(vertexElementSize == 40);

	if	(ifcObject  &&  ifcObject->ifcInstance) {
		SetFormat(ifcModel, flagbit14, flagbit14);
		SetBehavior(ifcModel, flagbit12, flagbit12);

		CreateInstance(GetClassByName((int64_t) ifcModel, "Transformation"), nullptr);

		__int64	noVertices = 0, noIndices = 0;
		CalculateInstance((__int64) ifcObject->ifcInstance, &noVertices, &noIndices, nullptr);

		int64_t	owlModel = 0,
				owlInstance = 0;

		if (noVertices  &&  noIndices) {
			owlGetModel(
					ifcModel,
					&owlModel
				);

			owlGetInstance(
					ifcModel,
					ifcObject->ifcInstance,
					&owlInstance
				);
		}

		if (noVertices  &&  noIndices) {
			ifcObject->noVertices = (int_t) noVertices;
			float	* vertices = new float[(int)noVertices * ((int_t) vertexElementSize / sizeof(float))];
			int32_t	* indices = new int32_t[(int)noIndices];

			UpdateInstanceVertexBuffer(owlInstance, vertices);
			UpdateInstanceIndexBuffer(owlInstance, indices);

			if	(firstItemWithGeometryPassed == false) {
				double	transformationMatrix[12], minVector[3], maxVector[3];
//				SetBoundingBoxReference(owlInstance, transformationMatrix, minVector, maxVector);
				GetBoundingBox(owlInstance, nullptr, minVector, maxVector);

				transformationMatrix[9]  = (minVector[0] + maxVector[0]) / 2.;
				transformationMatrix[10] = (minVector[1] + maxVector[1]) / 2.;
				transformationMatrix[11] = (minVector[2] + maxVector[2]) / 2.;

				if	( (-1000 > transformationMatrix[9]   ||  transformationMatrix[9]  > 1000)  ||
				      (-1000 > transformationMatrix[10]  ||  transformationMatrix[10] > 1000)  ||
				      (-1000 > transformationMatrix[11]  ||  transformationMatrix[11] > 1000) ) {
					SetVertexBufferOffset(ifcModel, -transformationMatrix[9], -transformationMatrix[10], -transformationMatrix[11]);

					ClearedInstanceExternalBuffers(owlInstance);

					__int64	noVert = noVertices, noInd = noIndices;

					CalculateInstance((__int64)ifcObject->ifcInstance, &noVertices, &noIndices, nullptr);
					assert(noVert == noVertices &&
						   noInd == noIndices);
					UpdateInstanceVertexBuffer((__int64)ifcObject->ifcInstance, vertices);
					UpdateInstanceIndexBuffer((__int64)ifcObject->ifcInstance, indices);
				}
			}

			firstItemWithGeometryPassed = true;

			ifcObject->noPrimitivesForWireFrame = 0;
			ASSERT(ifcObject->indicesForLinesWireFrame == 0);
			int32_t	* indicesForLinesWireFrame = new int32_t[2 * (int) noIndices];

			ifcObject->noVertices = (int_t) noVertices;
			ASSERT(ifcObject->indicesForFaces == 0);
			int32_t	* indicesForFaces = new int32_t[(int) noIndices];
			int32_t	* indicesForLines = new int32_t[(int) noIndices];
			int32_t	* indicesForPoints = new int32_t[(int) noIndices];

//SaveModel(ifcModel, "c:\\s\\out.bin");

			int_t	faceCnt = getConceptualFaceCnt(ifcObject->ifcInstance);
			int_t	* maxIndex = new int_t[faceCnt],
					* primitivesForFaces = new int_t[faceCnt];

			for  (int_t j = 0; j < faceCnt; j++) {
				int_t	startIndexTriangles = 0, noIndicesTrangles = 0,
						startIndexLines = 0, noIndicesLines = 0,
						startIndexPoints = 0, noIndicesPoints = 0,
						startIndexConceptualFacesPolygons__ = 0, noIndicesConceptualFacesPolygons__ = 0;

				getConceptualFaceEx(
						ifcObject->ifcInstance, j,
						&startIndexTriangles, &noIndicesTrangles,
						&startIndexLines, &noIndicesLines,
						&startIndexPoints, &noIndicesPoints,
						0, 0,
						&startIndexConceptualFacesPolygons__, &noIndicesConceptualFacesPolygons__
					);
				
				if (j) {
					maxIndex[j] = maxIndex[j - 1];
				} else {
					maxIndex[j] = 0;
				}

				if (noIndicesTrangles  &&  maxIndex[j] < startIndexTriangles + noIndicesTrangles) { maxIndex[j] = startIndexTriangles + noIndicesTrangles; }
				if (noIndicesLines  &&  maxIndex[j] < startIndexLines + noIndicesLines) { maxIndex[j] = startIndexLines + noIndicesLines; }
				if (noIndicesPoints  &&  maxIndex[j] < startIndexPoints + noIndicesPoints) { maxIndex[j] = startIndexPoints + noIndicesPoints; }
				if (noIndicesConceptualFacesPolygons__ &&
					maxIndex[j] < startIndexConceptualFacesPolygons__ + noIndicesConceptualFacesPolygons__) {
					maxIndex[j] = startIndexConceptualFacesPolygons__ + noIndicesConceptualFacesPolygons__;
				}

				int_t	i = 0;
				while  (i < noIndicesTrangles) {
					indicesForFaces[ifcObject->noPrimitivesForFaces * 3 + i + 0] = indices[startIndexTriangles + i + 0];
					indicesForFaces[ifcObject->noPrimitivesForFaces * 3 + i + 1] = indices[startIndexTriangles + i + 1];
					indicesForFaces[ifcObject->noPrimitivesForFaces * 3 + i + 2] = indices[startIndexTriangles + i + 2];
#ifdef _DEBUG
					int_t		indexI = indices[startIndexTriangles + i + 0],
								indexII = indices[startIndexTriangles + i + 1],
								indexIII = indices[startIndexTriangles + i + 2],
								VE = (int_t) vertexElementSize / sizeof(float);
					PLANEf		plane;
					VECTOR3f	* vecI = (VECTOR3f*) &vertices[VE * indexI + 0],
								* vecII = (VECTOR3f*) &vertices[VE * indexII + 0],
								* vecIII = (VECTOR3f*) &vertices[VE * indexIII + 0];
					if (PlaneFromPoints(&plane, vecI, vecII, vecIII)) {
//						VECTOR3f	* normal = (VECTOR3f*)&plane;

//						VECTOR3f	* normalI = (VECTOR3f*)&vertices[VE * indexI + 3],
//									* normalII = (VECTOR3f*)&vertices[VE * indexII + 3],
//									* normalIII = (VECTOR3f*)&vertices[VE * indexIII + 3];
//						float		dotI = Vector3Dot(normal, normalI),
//									dotII = Vector3Dot(normal, normalII),
//									dotIII = Vector3Dot(normal, normalIII);

//						assert(dotI >= -1.1 && dotI <= -0.7);
//						assert(dotII >= -1.1 && dotII <= -0.7);
//						assert(dotIII >= -1.1 && dotIII <= -0.7);
					}
#endif // _DEBUG

					i += 3;
				}
				assert(i == noIndicesTrangles);

				ifcObject->noPrimitivesForFaces += noIndicesTrangles / 3;
				primitivesForFaces[j] = noIndicesTrangles / 3;

				i = 0;
				while (i < noIndicesLines) {
					indicesForLines[ifcObject->noPrimitivesForLines * 2 + i] = indices[startIndexLines + i];
					i++;
				}
				ifcObject->noPrimitivesForLines += noIndicesLines / 2;

				i = 0;
				while (i < noIndicesPoints) {
					indicesForPoints[ifcObject->noPrimitivesForPoints * 1 + i] = indices[startIndexPoints + i];
					i++;
				}
				ifcObject->noPrimitivesForPoints += noIndicesPoints / 1;

				i = 0;
				int32_t	lastItem = -1;
				while (i < noIndicesConceptualFacesPolygons__) {
					indicesForLinesWireFrame[2 * ifcObject->noPrimitivesForWireFrame + 0] = indices[startIndexConceptualFacesPolygons__ + i + 0];
					indicesForLinesWireFrame[2 * ifcObject->noPrimitivesForWireFrame + 1] = indices[startIndexConceptualFacesPolygons__ + i + 1];
					ifcObject->noPrimitivesForWireFrame++;
					i += 2;

//					if (lastItem >= 0  &&  indices[startIndexConceptualFacesPolygons+i] >= 0) {
//						indicesForLinesWireFrame[2*ifcObject->noPrimitivesForWireFrame + 0] = lastItem;
//						indicesForLinesWireFrame[2*ifcObject->noPrimitivesForWireFrame + 1] = indices[startIndexConceptualFacesPolygons+i];
//						ifcObject->noPrimitivesForWireFrame++;
//					}
//					lastItem = indices[startIndexConceptualFacesPolygons+i];
//					i++;
				}
			}

			ifcObject->__vertices = new float[6 * ifcObject->noVertices];
			ifcObject->indicesForPoints = new int32_t[3 * ifcObject->noPrimitivesForPoints];
			ifcObject->indicesForLines = new int32_t[3 * ifcObject->noPrimitivesForLines];
			ifcObject->indicesForFaces = new int32_t[3 * ifcObject->noPrimitivesForFaces];
			ifcObject->indicesForLinesWireFrame = new int32_t[2 * ifcObject->noPrimitivesForWireFrame];
		
			for (int k = 0; k < ifcObject->noVertices; k++) {
				ifcObject->__vertices[6 * k + 0] = vertices[(vertexElementSize / sizeof(float)) * k + 0];		//	x
				ifcObject->__vertices[6 * k + 1] = vertices[(vertexElementSize / sizeof(float)) * k + 1];		//	y
				ifcObject->__vertices[6 * k + 2] = vertices[(vertexElementSize / sizeof(float)) * k + 2];		//	z
				ifcObject->__vertices[6 * k + 3] = vertices[(vertexElementSize / sizeof(float)) * k + 3];		//	nX
				ifcObject->__vertices[6 * k + 4] = vertices[(vertexElementSize / sizeof(float)) * k + 4];		//	nY
				ifcObject->__vertices[6 * k + 5] = vertices[(vertexElementSize / sizeof(float)) * k + 5];		//	nZ
			}

			memcpy(ifcObject->indicesForPoints, indicesForPoints, 1 * ifcObject->noPrimitivesForPoints * sizeof(int32_t));
			memcpy(ifcObject->indicesForLines, indicesForLines, 2 * ifcObject->noPrimitivesForLines * sizeof(int32_t));
			memcpy(ifcObject->indicesForFaces, indicesForFaces, 3 * ifcObject->noPrimitivesForFaces * sizeof(int32_t));
			memcpy(ifcObject->indicesForLinesWireFrame, indicesForLinesWireFrame, 2 * ifcObject->noPrimitivesForWireFrame * sizeof(int32_t));

			delete[]  indicesForLinesWireFrame;
			delete[]  indicesForFaces;
			delete[]  indicesForLines;
			delete[]  indicesForPoints;
			delete[]  indices;

			{
				if (ifcObject->noPrimitivesForFaces) {
					int32_t	index = ifcObject->indicesForFaces[0];
					
					uint32_t	ambient  = ((int32_t*) vertices)[index * (vertexElementSize / sizeof(float)) + 6],
								diffuse  = ((int32_t*) vertices)[index * (vertexElementSize / sizeof(float)) + 7],
								emissive = ((int32_t*) vertices)[index * (vertexElementSize / sizeof(float)) + 8],
								specular = ((int32_t*) vertices)[index * (vertexElementSize / sizeof(float)) + 9];

					STRUCT_MATERIALS	* materials = new_STRUCT_MATERIALS(
															newMaterial(
																	ambient,
																	diffuse,
																	emissive,
																	specular
																)
														);
					ifcObject->materials = materials;

					for (int k = 0; k < ifcObject->noPrimitivesForFaces; k++) {
						index = ifcObject->indicesForFaces[3 * k];
						uint32_t	currentAmbient  = ((int32_t*) vertices)[index * (vertexElementSize / sizeof(float)) + 6],
									currentDiffuse  = ((int32_t*) vertices)[index * (vertexElementSize / sizeof(float)) + 7],
									currentEmissive = ((int32_t*) vertices)[index * (vertexElementSize / sizeof(float)) + 8],
									currentSpecular = ((int32_t*) vertices)[index * (vertexElementSize / sizeof(float)) + 9];
						if ((ambient  == currentAmbient) &&
							(diffuse  == currentDiffuse) &&
							(emissive == currentEmissive) &&
							(specular == currentSpecular)) {
							materials->__noPrimitivesForFaces++;
						}
						else {
							ambient = currentAmbient;
							diffuse = currentDiffuse;
							emissive = currentEmissive;
							specular = currentSpecular;

							materials->next = new_STRUCT_MATERIALS(
													newMaterial(
															ambient,
															diffuse,
															emissive,
															specular
														)
												);

							materials->next->__indexArrayOffset = materials->__indexArrayOffset + 3 * materials->__noPrimitivesForFaces;
							materials = materials->next;

							materials->__noPrimitivesForFaces = 1;
						}
					}
				}
			}

			delete[]  vertices;

/*			if (ifcObject->materials) {
				STRUCT_MATERIALS	* materials = ifcObject->materials;
				if (materials->next) {
					__int64	indexBufferSize = 0, indexArrayOffset = 0, j = 0;
					while (materials) {
						ASSERT(materials->__indexBufferSize >= 0);
						ASSERT(materials->__noPrimitivesForFaces == 0);
						indexBufferSize += materials->__indexBufferSize;
						materials->__indexArrayOffset = indexArrayOffset;
						while (j < faceCnt  &&  maxIndex[j] <= indexBufferSize) {
							materials->__noPrimitivesForFaces += primitivesForFaces[j];
							indexArrayOffset += 3 * primitivesForFaces[j];
							j++;
						}
						materials = materials->next;
					}
					ASSERT(j == faceCnt  &&  indexBufferSize == noIndices);
				} else {
					ASSERT(materials->__indexBufferSize == -1);
					materials->__indexArrayOffset = 0;
					materials->__noPrimitivesForFaces = ifcObject->noPrimitivesForFaces;
				}
			} else {
				ASSERT(false);
			}	*/

			delete[]  primitivesForFaces;
			delete[]  maxIndex;
		} else {
			ASSERT(ifcObject->noVertices == 0 && ifcObject->noPrimitivesForPoints == 0 && ifcObject->noPrimitivesForLines == 0 && ifcObject->noPrimitivesForFaces == 0 && ifcObject->noPrimitivesForWireFrame == 0);

			ifcObject->selectState = TI_NONE;
			ASSERT(ifcObject->treeItemModel == nullptr);
			ASSERT(ifcObject->treeItemSpaceBoundary == nullptr);
		}
	}
}

void	GenerateGeometryNestedCall(int_t ifcModel, CProgressWnd * progressWnd, int steps)
{
	int	objectCnt = 0, stepSize;
	STRUCT__IFC__OBJECT	* ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		objectCnt++;
		ifcObject = ifcObject->next;
	}

	stepSize = objectCnt / steps;
	objectCnt = 0;

	ifcObject = ifcObjectsLinkedList;
	while  (ifcObject) {
		if	(objectCnt > stepSize) {
			progressWnd->StepIt();
			progressWnd->PeekAndPump();

			objectCnt = 0;
		} else {
			objectCnt++;
		}
		//
		//	Get Geometry
		//
		int_t	setting = 0, mask = 0;
		mask += flagbit2;        //    PRECISION (32/64 bit)
		mask += flagbit3;        //	   INDEX ARRAY (32/64 bit)
		mask += flagbit5;        //    NORMALS
		mask += flagbit8;        //    TRIANGLES
		mask += flagbit9;        //    LINES
		mask += flagbit10;       //    POINTS
		mask += flagbit12;       //    WIREFRAME (ALL FACES)
		mask += flagbit13;       //    WIREFRAME (CONCEPTUAL FACES)
		mask += flagbit24;		 //	   AMBIENT
		mask += flagbit25;		 //	   DIFFUSE
		mask += flagbit26;		 //	   EMISSIVE
		mask += flagbit27;		 //	   SPECULAR

		setting += 0;		     //    SINGLE PRECISION (float)
		setting += 0;            //    32 BIT INDEX ARRAY (Int32)
		setting += flagbit5;     //    NORMALS ON
		setting += flagbit8;     //    TRIANGLES ON
		setting += flagbit9;     //    LINES ON
		setting += flagbit10;    //    POINTS ON
		setting += flagbit12;    //    WIREFRAME OFF (ALL FACES)
		setting += flagbit13;    //    WIREFRAME ON (CONCEPTUAL FACES)
		setting += flagbit24;	 //	   AMBIENT
		setting += flagbit25;	 //	   DIFFUSE
		setting += flagbit26;	 //	   EMISSIVE
		setting += flagbit27;	 //	   SPECULAR
		int64_t	vertexElementkSize = SetFormat(ifcModel, setting, mask);

		setFilter(ifcModel, flagbit0 + flagbit1, flagbit0 + flagbit1);

		circleSegments(ifcObject->segmentationParts, 5);

		GenerateWireFrameGeometry(ifcModel, ifcObject, vertexElementkSize);
		cleanMemory(ifcModel, 0);

		ifcObject = ifcObject->next;
	}
}

bool	contains(wchar_t * txtI, wchar_t * txtII)
{
	int_t	i = 0;
	while  (txtI[i]  &&  txtII[i]) {
		if	(txtI[i] != txtII[i]) {
			return	false;
		}
		i++;
	}
	if	(txtII[i]) {
		return	false;
	} else {
		return	true;
	}
}

bool	charContains(wchar_t * txtI, wchar_t * txtII)
{
	int_t	i = 0;
	while  (txtI[i]) {
		int_t	j = 0;
		while  (txtII[j]  &&  txtI[i+j] == txtII[j]) {
			j++;
		}
		if	(!txtII[j]) { return  true; }
		i++;
	}
	return	false;
}

wchar_t	* charFill(wchar_t * txtI, wchar_t * txtII, wchar_t * buffer)
{
	int_t	i = 0;
	while  (txtI[i]) {
		int_t	j = 0;
		while  (txtII[j]  &&  txtI[i+j] == txtII[j]) {
			j++;
		}
		if	(!txtII[j]) {
			i += j;
			ASSERT(txtI[i+0] == '=');
			ASSERT(txtI[i+1] == '"');
			i += 2;
			j = 0;
			while  (txtI[i + j] != '"') {
				buffer[j++] = txtI[i + j];
			}
			buffer[j] = 0;
			return	buffer;
		}
		i++;
	}
	return	false;
}

bool	charFind(wchar_t * txtI, wchar_t * txtII)
{
	int_t	i = 0;
	while  (txtI[i]  &&  txtI[i] != '>') {
		int_t	j = 0;
		while  (txtII[j]  &&  txtI[i+j] == txtII[j]) {
			j++;
		}
		if	(!txtII[j]) {
			return	true;
		}
		i++;
	}
	return	false;
}

void	CleanupIfcFile()
{
	if (globalIfcModel) {
		cleanMemory(globalIfcModel, 4);
		sdaiCloseModel(globalIfcModel);
		globalIfcModel = 0;
	}

	if (ifcObjectsLinkedList || topTreeItem || topModelTreeItem || topSpaceBoundaryTreeItem || topNonReferencedTreeItem || topGroupTreeItem) {
		DeleteIfcObjects();
		ifcObjectsLinkedList = nullptr;

		DeleteTreeItems(topTreeItem);
		topTreeItem = nullptr;
		topModelTreeItem = nullptr;
		topSpaceBoundaryTreeItem = nullptr;
		topGroupTreeItem = nullptr;
	}

	if (units) {
		CleanUnits__MemoryStructure(units);
		units = nullptr;
	}

	mtrls.clear();
	deleteMaterials();
}

void	SetColor(
				D3DCOLORVALUE	* iColor,
				uint32_t		color
			)
{
	iColor->r = (float) (color & ((unsigned int) 255 * 256 * 256 * 256)) / (256 * 256 * 256);
	iColor->r /= 255.f;

	iColor->g = (float) (color & (255 * 256 * 256)) / (256 * 256);
	iColor->g /= 255.f;

	iColor->b = (float) (color & (255 * 256)) / 256;
	iColor->b /= 255.f;

	iColor->a = (float) (color & (255));
	iColor->a /= 255.f;
//	iColor->a = 1.f;
//	iColor->a = 1.f;
}

void	finalizeMaterial()
{
	STRUCT_MATERIAL	* material = listOfMaterials;

	int	arraySize = 0;
	while (material) {
		arraySize++;
		material = material->next;
	}

	material = listOfMaterials;
	while (material) {
//		ASSERT(material->MTRL == 0);
		D3DMATERIAL9	* mtrl = new D3DMATERIAL9;

		SetColor(&mtrl->Ambient, material->ambientColor);
		SetColor(&mtrl->Diffuse, material->diffuseColor);
		SetColor(&mtrl->Emissive, material->emissiveColor);

		mtrl->Emissive.r = 0.5f * mtrl->Emissive.r;
		mtrl->Emissive.g = 0.5f * mtrl->Emissive.g;
		mtrl->Emissive.b = 0.5f * mtrl->Emissive.b;
		SetColor(&mtrl->Specular, material->specularColor);
//		mtrl->Emissive.r = 0.f;
//		mtrl->Emissive.g = 0.f;
//		mtrl->Emissive.b = 0.f;
//		mtrl->Emissive.a = 0.f;
		mtrl->Power = 0.2f;

		mtrls.push_back(mtrl);

		material->MTRL = (void*) mtrl;

		material = material->next;
	}
}

STRUCT__IFC__OBJECT	** GetChildrenRecursively(
							int_t				ifcModel,
							int_t				ifcParentEntity,
							STRUCT__IFC__OBJECT	** firstFreeIfcObject,
							bool				hide,
							int_t				segmentationParts
						)
{
	int_t	* ifcEntityExtend = sdaiGetEntityExtent(ifcModel, ifcParentEntity),
			cnt = sdaiGetMemberCount(ifcEntityExtend);

	if ((ifcParentEntity == ifcSpace_TYPE) ||
		(ifcParentEntity == ifcFeatureElementSubtraction_TYPE)) {
		hide = true;
	}

	if ((ifcParentEntity == ifcDistributionElement_TYPE) ||
		(ifcParentEntity == ifcElectricalElement_TYPE) ||
		(ifcParentEntity == ifcElementAssembly_TYPE) ||
		(ifcParentEntity == ifcElementComponent_TYPE) ||
		(ifcParentEntity == ifcEquipmentElement_TYPE) ||
		(ifcParentEntity == ifcFeatureElement_TYPE) ||
		(ifcParentEntity == ifcFurnishingElement_TYPE) ||
		(ifcParentEntity == ifcTransportElement_TYPE) ||
		(ifcParentEntity == ifcVirtualElement_TYPE)) {
		segmentationParts = 12;
	}

	if (ifcParentEntity == ifcReinforcingElement_TYPE) {
		segmentationParts = 6;// 12;
	}

	if (cnt) {
		wchar_t	* ifcParentEntityName = nullptr;
		engiGetEntityName(ifcParentEntity, sdaiUNICODE, (char**) &ifcParentEntityName);

		firstFreeIfcObject = queryIfcObjects(ifcModel, firstFreeIfcObject, ifcParentEntityName, hide, segmentationParts);
	}

	cnt = engiGetEntityCount(ifcModel);
	for (int_t i = 0; i < cnt; i++) {
		int_t ifcEntity = engiGetEntityElement(ifcModel, i);
		if (engiGetEntityParent(ifcEntity) == ifcParentEntity) {
			firstFreeIfcObject = GetChildrenRecursively(ifcModel, ifcEntity, firstFreeIfcObject, hide, segmentationParts);
		}
	}

	return	firstFreeIfcObject;
}

bool	ParseIfcFile(
				CWnd	* pParent
			)
{
	CProgressWnd wndProgress(pParent);

	wndProgress.SetRange(0,100);
	wndProgress.SetText(L"Loading IFC file...\n\nParsing file from disk ...");

	wndProgress.StepIt();
	wndProgress.PeekAndPump();

	firstItemWithGeometryPassed = false;

	CleanupIfcFile();

	setStringUnicode(1);
	int_t	ifcModel = sdaiOpenModelBNUnicode(0, (wchar_t*) ifcFileName, (wchar_t*) L"");
	ifcModelGlobalTmp = ifcModel;

	for (int m = 0; m < 2; m++) {
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
	}

	if (ifcModel) {
		//
		//	Check if this file is an IFC2x3 file or IFC4
		//

			//
			//	consistencyCheck
			//		bit0  (1)		merge elements in the vertex array are duplicated (epsilon used as distance)
			//		bit1  (2)		remove elements in the vertex array that are not referenced by elements in the index array (interpreted as SET if flags are defined)
			//		bit2  (4)		merge polygons placed in the same plane and sharing at least one edge
			//		bit3  (8)		merge polygons advanced (check of polygons have the opposite direction and are overlapping, but don't share points)
			//		bit4  (16)		check if faces are wrongly turned opposite from each other
			//		bit5  (32)		check if faces are inside-out
			//		bit6  (64)		check if faces result in solid, if not generate both sided faces
			//		bit7  (128)		invert direction of the faces / normals
			//		bit8  (256)		export all faces as one conceptual face
			//		bit9  (512)		remove irrelevant intermediate points on lines
			//		bit10 (1024)	check and repair faces that are not defined in a perfect plane
			//


		setBRepProperties(
				ifcModel,
				flagbit5 + flagbit0 + flagbit1 + flagbit2,
//BREP_PROPERTIES_CHECK_IF_FACES_ARE_INSIDE_OUT | BREP_MERGE_VERTICES | BREP_REMOVE_UNREFERENCED_VERTICES | BREP_MERGE_POLYGONS_IN_SAME_PLANE
//				1 + 2 + 4 + 32,	// + 64,
				0.92,
				0.000001,
				600
			);

		for (int m = 0; m < 10; m++) {
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
		}

		if (!ifcModel) { return  false; }

		setFilter(ifcModel, (int_t) encodingSetting, (int_t) encodingMask);

//setFilter(ifcModel, 1024, 1024);

		wndProgress.SetText(L"Loading IFC file...\n\nFind objects in Database ...");

		//
		//	Find all children from IfcProduct
		//
		STRUCT__IFC__OBJECT	** firstFreeIfcObject = &ifcObjectsLinkedList;
		ASSERT((*firstFreeIfcObject) == nullptr);

		ifcSpace_TYPE						= sdaiGetEntity(ifcModel, (char*) L"IFCSPACE");
		ifcDistributionElement_TYPE			= sdaiGetEntity(ifcModel, (char*) L"IFCDISTRIBUTIONELEMENT");
		ifcElectricalElement_TYPE			= sdaiGetEntity(ifcModel, (char*) L"IFCELECTRICALELEMENT");
		ifcElementAssembly_TYPE				= sdaiGetEntity(ifcModel, (char*) L"IFCELEMENTASSEMBLY");
		ifcElementComponent_TYPE			= sdaiGetEntity(ifcModel, (char*) L"IFCELEMENTCOMPONENT");
		ifcEquipmentElement_TYPE			= sdaiGetEntity(ifcModel, (char*) L"IFCEQUIPMENTELEMENT");
		ifcFeatureElement_TYPE				= sdaiGetEntity(ifcModel, (char*) L"IFCFEATUREELEMENT");
		ifcFeatureElementSubtraction_TYPE	= sdaiGetEntity(ifcModel, (char*) L"IFCFEATUREELEMENTSUBTRACTION");
		ifcFurnishingElement_TYPE			= sdaiGetEntity(ifcModel, (char*) L"IFCFURNISHINGELEMENT");
		ifcReinforcingElement_TYPE			= sdaiGetEntity(ifcModel, (char*) L"IFCREINFORCINGELEMENT");
		ifcTransportElement_TYPE			= sdaiGetEntity(ifcModel, (char*) L"IFCTRANSPORTELEMENT");
		ifcVirtualElement_TYPE				= sdaiGetEntity(ifcModel, (char*) L"IFCVIRTUALELEMENT");

		bool	hide = false;
		int		segmentationParts = 36;
		firstFreeIfcObject = GetChildrenRecursively(
									ifcModel,
									sdaiGetEntity(ifcModel, (char*)L"IFCPRODUCT"),
									firstFreeIfcObject,
									hide,
									segmentationParts
								);

		firstFreeIfcObject = queryIfcObjects(ifcModel, firstFreeIfcObject, L"IFCRELSPACEBOUNDARY", true, segmentationParts);

		for  (int m = 0; m < 3; m++) {
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
		}

		{
			//
			//	Get boundingBox
			//

			int_t	ifcAlignment_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCALIGNMENT");
//			int_t	ifcAlignment_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCBRIDGE");

			int_t	* ifcGeographicElementInstances = sdaiGetEntityExtentBN(ifcModel, (char*) L"IFCGEOGRAPHICELEMENT"),
					ifcGeographicElementInstancesCnt = sdaiGetMemberCount(ifcGeographicElementInstances);

			int_t	* ifcAlignmentInstances = sdaiGetEntityExtentBN(ifcModel, (char*) L"IFCALIGNMENT"),
//			int_t	* ifcAlignmentInstances = sdaiGetEntityExtentBN(ifcModel, (char*) L"IFCBRIDGE"),
					ifcAlignmentInstancesCnt = sdaiGetMemberCount(ifcAlignmentInstances);
			if (ifcAlignmentInstancesCnt || ifcGeographicElementInstancesCnt) {
				setFilter(ifcModel, flagbit0 + flagbit1, flagbit0 + flagbit1);

				int64_t	* owlInstanceArray = nullptr;
				
				if (ifcAlignmentInstancesCnt) {
					owlInstanceArray = new int64_t[ifcAlignmentInstancesCnt];
					for (int_t i = 0; i < ifcAlignmentInstancesCnt; i++) {
						int_t	ifcAlignmentInstance = 0;
						engiGetAggrElement(ifcAlignmentInstances, i, sdaiINSTANCE, &ifcAlignmentInstance);
						owlBuildInstance(ifcModel, ifcAlignmentInstance, &owlInstanceArray[i]);
					}
				}
				else {
					owlInstanceArray = new int64_t[ifcGeographicElementInstancesCnt];
					for (int_t i = 0; i < ifcGeographicElementInstancesCnt; i++) {
						int_t	ifcGeographicElementInstance = 0;
						engiGetAggrElement(ifcGeographicElementInstances, i, sdaiINSTANCE, &ifcGeographicElementInstance);
						owlBuildInstance(ifcModel, ifcGeographicElementInstance, &owlInstanceArray[i]);
					}
				}

				int64_t	myCollection = CreateInstance(GetClassByName(ifcModel, "Collection"), nullptr);
				SetObjectProperty(myCollection, GetPropertyByName(ifcModel, "objects"), owlInstanceArray, ifcAlignmentInstancesCnt ? ifcAlignmentInstancesCnt : ifcGeographicElementInstancesCnt);

				int64_t	vertexBufferSize = 0, indexBufferSize = 0;
				CalculateInstance(myCollection, &vertexBufferSize, &indexBufferSize, nullptr);

				VECTOR3	startVec, endVec;
				GetBoundingBox(myCollection, nullptr, (double*) &startVec, (double*) &endVec);

				if (vertexBufferSize && indexBufferSize) {
					float	* vertices = new float[6 * vertexBufferSize];
					UpdateInstanceVertexBuffer(myCollection, vertices);

					startVec.x = endVec.x = vertices[6 * 0 + 0];
					startVec.y = endVec.y = vertices[6 * 0 + 1];
					startVec.z = endVec.z = vertices[6 * 0 + 2];
					for (int i = 1; i < vertexBufferSize; i++) {
						if (startVec.x > vertices[6 * i + 0]) { startVec.x = vertices[6 * i + 0]; }
						if (startVec.y > vertices[6 * i + 1]) { startVec.y = vertices[6 * i + 1]; }
						if (startVec.z > vertices[6 * i + 2]) { startVec.z = vertices[6 * i + 2]; }
						if (endVec.x < vertices[6 * i + 0]) { endVec.x = vertices[6 * i + 0]; }
						if (endVec.y < vertices[6 * i + 1]) { endVec.y = vertices[6 * i + 1]; }
						if (endVec.z < vertices[6 * i + 2]) { endVec.z = vertices[6 * i + 2]; }
					}


					int u = 0;
				}

				//	480481  4553291  118
//SaveInstanceTree(myCollection, "c:\\4\\nn.bin");

				if (endVec.x - startVec.x + endVec.y - startVec.y + endVec.z - startVec.z > 10.) {
		/*			VECTOR3	center = {
									(endVec.x + startVec.x) / 2.,
									(endVec.y + startVec.y) / 2.,
									(endVec.z + startVec.z) / 2.
								};
					double	scale = endVec.x - startVec.x;
					if (scale < endVec.y - startVec.y) { scale = endVec.y - startVec.y; }
					if (scale < endVec.z - startVec.z) { scale = endVec.z - startVec.z; }
					double	matrix[12] = {
									1. / scale, 0., 0.,
									0., 1. / scale, 0.,
									0., 0., 1. / scale,
									-center.x / scale, -center.y / scale, -center.z / scale
								};	//	*/
	/*				double	scale = 100.,
							matrix[12] = {
									1. / scale, 0., 0.,
									0., 1. / scale, 0.,
									0., 0., 1. / scale,
									-479666. / scale, -4554675. / scale, -120. / scale
								};
					SetVertexBufferTransformation(ifcModel, matrix);

					firstItemWithGeometryPassed = true;	//	*/
				}
			}
		}

		if	(ifcObjectsLinkedList) {
			initializeMaterial(ifcModel);

			wndProgress.SetText(L"Loading IFC file...\n\nCreate geometry ...");

			GenerateGeometryNestedCall(ifcModel, &wndProgress, 80);

			wndProgress.SetText(L"Loading IFC file...\n\nAttach material ...");

			finalizeMaterial();

			wndProgress.SetText(L"Loading IFC file...\n\nPrepare interface ...");

			ASSERT(topTreeItem == nullptr);
			topTreeItem = GetHeaderInfo(ifcModel);

			CreateTreeItem_ifcProject(ifcModel);

			CreateTreeItem_ifcSpaceBoundary(ifcModel);

			CreateTreeItem_nonReferencedIfcItems(ifcModel);

			CreateTreeItem_ifcGroup(ifcModel);

			bool	checked = false, unchecked = false;
			InitializeTreeItems((STRUCT_TREE_ITEM_SELECTABLE*) topTreeItem, &checked, &unchecked);
		}

//		CreateTreeItem_nonReferencedIfcItems();

		for  (int m = 0; m < 3; m++) {
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
		}

		globalIfcModel = ifcModel;

		return	true;
	} else {
		ASSERT(ifcObjectsLinkedList == nullptr  &&  topTreeItem == nullptr);
		return	false;
	}
}
