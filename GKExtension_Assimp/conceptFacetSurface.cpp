#include "pch.h"
#include "conceptFacetSurface.h"
#include "Helper.h"

#define CLS_FACETSURFACE    "FacetSurface"

#define PROP_OBJECT         "object"
#define PROP_IDXS           "indices"

/// <summary>
/// 
/// </summary>
bool FacetSurface::CreateClass(OwlModel model)
{
    OwlClass clsFacetSurface = GetClassByName(model, CLS_FACETSURFACE);
    if (!clsFacetSurface)
        clsFacetSurface = ::CreateClass(model, CLS_FACETSURFACE);
    ON_ERROR(clsFacetSurface, "Failed to create class" CLS_FACETSURFACE "\n");

    GET_CLASS(clsGeometricItem, CLS_GEOMETRICITEM);
    if (!IsClassAncestor(clsFacetSurface, clsGeometricItem))
        ON_ERROR(SetClassParent(clsFacetSurface, clsGeometricItem), "Fail to set parent");

    OwlClass clsPointSet = NULL; //range restriction will affect all concepts! GetClassByName(model, CLS_POINTSET);

    Helper::AddClassProperty(clsFacetSurface, PROP_OBJECT, OBJECTPROPERTY_TYPE, 1, clsPointSet, 1);
    Helper::AddClassProperty(clsFacetSurface, PROP_IDXS, DATATYPEPROPERTY_TYPE_INTEGER, 0, NULL, -1);

    rdfgeom_SetClassGeometry(clsFacetSurface, CreateShell, GetBoundingBox, NULL);

    return true;
}


/// <summary>
/// 
/// </summary>
bool FacetSurface::GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*)
{
    OwlInstance pointSet = Helper::GetObjectPropertyValue(inst, PROP_OBJECT);
    if (pointSet){
        return ::GetBoundingBox(pointSet, (double*)transformationMatrix, (double*)startVector, (double*)endVector);
    }
    return false;
}



/// <summary>
/// 
/// </summary>
void FacetSurface::CreateShell(OwlInstance inst, void*)
{
    // Collect data
    
    //
    OwlInstance pointSet = Helper::GetObjectPropertyValue(inst, PROP_OBJECT);
    if (!IsInstanceOfClass(pointSet, CLS_POINTSET)) {
        return;
    }

    //
    SHELL* pointShell = rdfgeom_GetBRep(pointSet);
    int_t numPoints = rdfgeom_GetNumOfPoints(pointShell);
    if (numPoints <= 0) {
        return;
    }

    //
    SHELL* shell = rdfgeom_GetBRep(inst);
    if (!shell) {
        return;
    }

    //
    int_t* indecies = NULL;
    int_t numInd = Helper::GetDataProperyValue(inst, PROP_IDXS, (void**) &indecies);

    // Fill the shell

    //TODO - Can we avoid data copy?
    VECTOR3* srcPoints = rdfgeom_GetPoints(pointShell);
    VECTOR2* srcUV = rdfgeom_GetTextureCoordinates(pointShell);
    
    rdfgeom_AllocatePoints(inst, shell, numPoints, false, srcUV != NULL);
    VECTOR3* dstPoints = rdfgeom_GetPoints(shell);
    if (dstPoints && srcPoints) {
        memcpy(dstPoints, srcPoints, numPoints * sizeof(VECTOR3));
    }

    VECTOR2* dstUV = rdfgeom_GetTextureCoordinates(shell);
    if (dstUV && srcUV) {
        memcpy(dstUV, srcUV, numPoints * sizeof(VECTOR2));
    }

    //
    auto cfaceP = rdfgeom_GetConceptualFaces(shell);
    rdfgeom_cface_Create(inst, cfaceP);

    STRUCT_FACE** faces = rdfgeom_cface_GetFaces(*cfaceP);
    
    int_t firstIndex = -1;
    STRUCT_VERTEX** vertex = NULL;

    for (int_t i = 0; i < numInd; i++) {

        int_t ind = indecies[i];
        if (ind < 0 || ind > numPoints - 1) {
            //close boundary
            if (firstIndex >= 0 && vertex) {
                rdfgeom_vertex_Create(inst, vertex, firstIndex, true);
                vertex = NULL;
                firstIndex = -1;
            }
            continue;
        }

        if (!vertex) {
            //start boundary
            rdfgeom_face_Create(inst, faces);
            vertex = rdfgeom_face_GetBoundary(*faces);
            faces = rdfgeom_face_GetNext(*faces);
            firstIndex = ind;
        }

        rdfgeom_vertex_Create(inst, vertex, ind, false);
        vertex = rdfgeom_vertex_GetNext(*vertex);
    }
}

