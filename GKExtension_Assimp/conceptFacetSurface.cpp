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

    OwlClass clsPointSet = GetClassByName(model, "PointSet");

    Helper::AddClassProperty(clsFacetSurface, PROP_OBJECT, OBJECTPROPERTY_TYPE, 1, clsPointSet, 1);
    Helper::AddClassProperty(clsFacetSurface, PROP_IDXS, DATATYPEPROPERTY_TYPE_DOUBLE, 0, NULL, -1);

    rdfgeom_SetClassGeometry(clsFacetSurface, CreateShell, GetBoundingBox, NULL);

    return true;
}


/// <summary>
/// 
/// </summary>
bool FacetSurface::GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*)
{
}



/// <summary>
/// 
/// </summary>
void FacetSurface::CreateShell(OwlInstance inst, void*)
{
    auto shell = rdfgeom_GetInstanceRepresentation(inst);
    if (!shell)
        return;

    // get properties
    //
    int_t dim = Helper::GetDataProperyValue<int_t>(inst, PROP_DIM, 3);

    double* coords = NULL;
    int_t ncoords = Helper::GetDataProperyValue(inst, PROP_COORD, (void**)&coords);

    if (ncoords < dim) {
        return;
    }

    double* normalCoords = NULL;
    int_t nnormCoords = Helper::GetDataProperyValue(inst, PROP_NORMALS, (void**)&normalCoords);

    //TODO
    //double* tangentCoords = NULL;
    //int_t ntangetCoords = Helper::GetDataProperyValue(inst, PROP_TANGENTS, (void**)&tangentCoords);

    int_t textureDim = Helper::GetDataProperyValue(inst, PROP_TEX_DIM, 2);
    double* texCoord = NULL;
    int_t ntexCoord = Helper::GetDataProperyValue(inst, PROP_TEX_COORD, (void**)&texCoord);

    int_t Npt = ncoords / dim;

    // set verticies
    //
    rdfgeom_AllocatePoints(inst, shell, ncoords/dim, nnormCoords >= dim, ntexCoord >= textureDim);
    
    VECTOR3* points = rdfgeom_GetPoints(shell);
    CopyPoints(points, Npt, 3, coords, ncoords, dim);

    VECTOR3* normals = rdfgeom_GetNormals(shell);
    CopyPoints(normals, Npt, 3, normalCoords, nnormCoords, dim);

    VECTOR2* texUV = rdfgeom_GetTextureCoordinates(shell);
    CopyPoints(texUV, Npt, 2, texCoord, ntexCoord, textureDim);

    //set to show
    //
    auto cfaceP = rdfgeom_GetConceptualFaces(shell);
    rdfgeom_cface_Create(inst, cfaceP);

    STRUCT_VERTEX** vertexP = rdfgeom_cface_GetVerticies(*cfaceP);

    for (int_t i = 0; i < Npt; i++) {
        rdfgeom_vertex_Create(inst, vertexP, i, i == Npt - 1);
        vertexP = rdfgeom_vertex_GetNext(*vertexP);
    }
}

