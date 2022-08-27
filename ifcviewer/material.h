#pragma once


#include "ifcengine/include/engdef.h"



struct STRUCT_MATERIAL {
	uint32_t						ambientColor;
	uint32_t						diffuseColor;
	uint32_t						emissiveColor;
	uint32_t						specularColor;

	void							* MTRL;

	STRUCT_MATERIAL					* next;
};

struct STRUCT_MATERIALS {
	int64_t							__indexOffsetForFaces;
	int64_t							__indexArrayOffset;	

	int64_t							__noPrimitivesForFaces;
	int64_t							__indexBufferSize;

	STRUCT_MATERIAL					* material;

	STRUCT_MATERIALS				* next;
};


void				initializeMaterial(int_t owlModel);
STRUCT_MATERIALS	* ifcObjectMaterial(int_t ifcModel, int_t ifcInstance);
void				deleteMaterials();

STRUCT_MATERIALS	* new_STRUCT_MATERIALS(STRUCT_MATERIAL * material);
STRUCT_MATERIAL		* newMaterial(
							uint32_t	ambientColor,
							uint32_t	diffuseColor,
							uint32_t	emissiveColor,
							uint32_t	specularColor
						);

