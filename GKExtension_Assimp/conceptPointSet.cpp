#include "pch.h"
#include "conceptPointSet.h"
#include "Helper.h"

#define CLS_POINTSET "PointSet"

#define PROP_DIM        "dim"
#define PROP_COORD      "coordinates"
#define PROP_NORMALS    "normalsCoordinates"
#define PROP_TANGENTS   "tangentCoordinates"

#define PROP_TEX_DIM    "textureDim"
#define PROP_TEX_COORD  "textureCoordinates"

/// <summary>
/// 
/// </summary>
bool PointSet::CreateClass(OwlModel model)
{
    OwlClass clsPointCloud = GetClassByName(model, CLS_POINTSET);
    if (!clsPointCloud)
        clsPointCloud = ::CreateClass(model, CLS_POINTSET);
    ON_ERROR(clsPointCloud, "Failed to create class " CLS_POINTSET "\n");

    GET_CLASS(clsGeometricItem, CLS_GEOMETRICITEM);
    if (!IsClassAncestor(clsPointCloud, clsGeometricItem))
        ON_ERROR(SetClassParent(clsPointCloud, clsGeometricItem), "Fail to set parent");

    Helper::AddClassProperty(clsPointCloud, PROP_DIM, DATATYPEPROPERTY_TYPE_INTEGER);
    Helper::AddClassProperty(clsPointCloud, PROP_COORD, DATATYPEPROPERTY_TYPE_DOUBLE, 0, NULL, -1);
    Helper::AddClassProperty(clsPointCloud, PROP_NORMALS, DATATYPEPROPERTY_TYPE_DOUBLE, 0, NULL, -1);
    Helper::AddClassProperty(clsPointCloud, PROP_TANGENTS, DATATYPEPROPERTY_TYPE_DOUBLE, 0, NULL, -1);

    Helper::AddClassProperty(clsPointCloud, PROP_TEX_DIM,  DATATYPEPROPERTY_TYPE_INTEGER);
    Helper::AddClassProperty(clsPointCloud, PROP_TEX_COORD, DATATYPEPROPERTY_TYPE_DOUBLE, 0, NULL, -1);

    rdfgeom_SetClassGeometry(clsPointCloud, CreateShell, GetBoundingBox, NULL);

    return true;
}


/// <summary>
/// 
/// </summary>
bool PointSet::GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*)
{
    if (transformationMatrix)
        Helper::MatrixIdentity(transformationMatrix);

    double* xyzStart = &startVector->x;
    double* xyzEnd = &endVector->x;

    for (int_t j = 0; j < 3; j++) {
        xyzStart[j] = DBL_MAX;
        xyzEnd[j] = -DBL_MAX;
    }

    int_t dim = Helper::GetDataProperyValue<int_t>(inst, PROP_DIM, 3);

    double* coords = NULL;
    int_t ncoords = Helper::GetDataProperyValue(inst, PROP_COORD, (void**)&coords);

    for (int_t i = 0; i < ncoords / dim; i++) {
        for (int_t j = 0; j < min(dim, 3); j++) {
            xyzStart[j] = min(xyzStart[j], coords[i * dim + j]);
            xyzEnd[j] = max(xyzEnd[j], coords[i * dim + j]);
        }
    }

    if (startVector->x <= endVector->x) {
        for (int_t j = dim; j < 3; j++) {
            xyzStart[j] = 0;
            xyzEnd[j] = 0;
        }
    }

    return startVector->x <= endVector->x;
}

/// <summary>
/// 
/// </summary>
void PointSet::CopyCoordinates(double* dstValues, int_t dstDim, double* srcValues, int_t srcDim)
{
    int_t i = 0;
    for (; i < min(dstDim, srcDim); i++) {
        dstValues[i] = srcValues[i];
    }
    for (; i < dstDim; i++) {
        dstValues[i] = 0;
    }
}

/// <summary>
/// 
/// </summary>
template<typename TPoint> void PointSet::CopyPoints(TPoint* dstPoints, int_t numDstPt, int_t dstDim, double* srcCoords, int_t numSrcCoords, int_t srcDim)
{
    if (!dstPoints) {
        return;
    }

    int_t numSrcPt = numSrcCoords / srcDim;

    int_t i = 0;
    
    for (; i < min(numDstPt, numSrcPt); i++) {
        TPoint& dst = dstPoints[i];
        double* xyz = (double*) &dst;
        CopyCoordinates(xyz, dstDim, srcCoords + srcDim * i, srcDim);
    }

    for (; i < numDstPt; i++) {
        TPoint& dst = dstPoints[i];
        double* xyz = (double*) &dst;
        for (int_t j = 0; j < dstDim; j++) {
            xyz[j] = 0;
        }
    }
}

/// <summary>
/// 
/// </summary>
void PointSet::CreateShell(OwlInstance inst, void*)
{
    auto shell = rdfgeom_GetBRep(inst);
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

    // set vertices
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

    STRUCT_VERTEX** vertexP = rdfgeom_cface_GetVertices(*cfaceP);

    for (int_t i = 0; i < Npt; i++) {
        rdfgeom_vertex_Create(inst, vertexP, i, i == Npt - 1);
        vertexP = rdfgeom_vertex_GetNext(*vertexP);
    }
}

