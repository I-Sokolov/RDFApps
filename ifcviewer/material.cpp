#include "stdafx.h"
#include "material.h"

#include <list>

#include "ifcengine\include\ifcengine.h"
#include "ifcengine\include\engine.h"


STRUCT_MATERIAL		* listOfMaterials, * defaultMaterial;

STRUCT_MATERIALS	* new_STRUCT_MATERIALS(STRUCT_MATERIAL * material)
{
	STRUCT_MATERIALS	* materials = new STRUCT_MATERIALS;

	materials->material = material;
	materials->next = nullptr;

	materials->__indexOffsetForFaces = 0;
	materials->__indexArrayOffset = 0;

	materials->__noPrimitivesForFaces = 0;
	materials->__indexBufferSize = 0;

	return	materials;
}

void	deleteMaterial(
				STRUCT_MATERIAL		* material
			)
{
	if (material) {
		delete	material;
	}
}

STRUCT_MATERIAL		* newMaterial(
							uint32_t	ambientColor,
							uint32_t	diffuseColor,
							uint32_t	emissiveColor,
							uint32_t	specularColor
						)
{
	STRUCT_MATERIAL	* material = listOfMaterials;
	while (material) {
		if ((material->ambientColor == ambientColor) &&
			(material->diffuseColor == diffuseColor) &&
			(material->emissiveColor == emissiveColor) &&
			(material->specularColor == specularColor)) {
			return  material;
		}
		material = material->next;
	}

	material = new STRUCT_MATERIAL;

	material->ambientColor = ambientColor;
	material->diffuseColor = diffuseColor;
	material->emissiveColor = emissiveColor;
	material->specularColor = specularColor;

	material->next = listOfMaterials;

	material->MTRL = nullptr;

	listOfMaterials = material;

	return	material;
}

void	initializeMaterial(
				int_t	ifcModel
			)
{
	listOfMaterials = 0;

	int32_t	R = 10,
			G = 150,
			B = 10,
			A = 255;
	int32_t	defaultColor = 256 * 256 * 256 * R +
						   256 * 256 * G +
						   256 * B +
						   A;
	SetDefaultColor(ifcModel, defaultColor, defaultColor, defaultColor, defaultColor);

	defaultMaterial = newMaterial(0, 0, 0, 0);
}

void	deleteMaterialFinal(
				STRUCT_MATERIAL		* material
			)
{
	if (material) {
		if (material->MTRL) {
			delete	material->MTRL;
		}
		delete  material;
	}
}

void	deleteMaterials()
{
	while (listOfMaterials) {
		STRUCT_MATERIAL	* currentMaterial = listOfMaterials;
		listOfMaterials = listOfMaterials->next;

//		if (currentMaterial != defaultMaterial) {
//			materialToDelete = currentMaterial;
//		}

		deleteMaterialFinal(currentMaterial);
	}

//	deleteMaterialFinal(defaultMaterial);
}

#undef int_t
