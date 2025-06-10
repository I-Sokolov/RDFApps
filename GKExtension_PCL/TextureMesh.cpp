#include "pch.h"
#include "PointCloud.h"
#include "TextureMesh.h"

#define CLASS_NAME              "PCL - Texture Mesh"

#define PROP_INPUT_FILE         "Data File (*.ply)"

/// <summary>
/// 
/// </summary>
bool TextureMesh::CreateClass(OwlModel model)
{
    OwlClass clsMesh = GetClassByName(model, CLASS_NAME);
    if (!clsMesh)
        clsMesh = ::CreateClass(model, CLASS_NAME);
    REQUIRED(clsMesh, "Failed to create class\n");

    OwlClass clsGeometricItem = GetClassByName(model, "GeometricItem");
    REQUIRED(clsGeometricItem, "Failed GetClassByName (GeometricItem)\n");
    if (!IsClassAncestor(clsMesh, clsGeometricItem))
        REQUIRED(SetClassParent(clsMesh, clsGeometricItem), "Fail to set parent");

    AddClassProperty(clsMesh, PROP_INPUT_FILE, DATATYPEPROPERTY_TYPE_STRING);

    rdfgeom_SetClassGeometry(clsMesh, CreateShell, GetBoundingBox, NULL);

    return true;
}

/// <summary>
/// 
/// </summary>
bool TextureMesh::GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*)
{
    if (transformationMatrix)
        MatrixIdentity(transformationMatrix);

    startVector->x = startVector->y = startVector->z = DBL_MAX;
    endVector->x = endVector->y = endVector->z = -DBL_MAX;

    return false;
}

/// <summary>
/// 
/// </summary>
void TextureMesh::CreateShell(OwlInstance inst, void*)
{
    auto filePath = GetFilePathIfNeedToRead(inst);
    if (filePath.empty()) {
        return;
    }
    
    auto textureMesh = ReadPLYFile(filePath.c_str());
    if (!textureMesh){
        return;
    }
}

std::string TextureMesh::GetFilePathIfNeedToRead(OwlInstance inst)
{
    const char* filePath = GetDataProperyValue<const char*>(inst, PROP_INPUT_FILE, "");
    if (!filePath || !*filePath)
        return "";

    //TODO check out-of-date

    return filePath;
}

pcl::TextureMeshPtr TextureMesh::ReadPLYFile(const char* filePath)
{
#if 0
    pcl::PolygonMeshPtr mesh(new pcl::PolygonMesh());

    if (pcl::io::loadPLYFile(filePath, *mesh) < 0) {
        return NULL;
    }
#endif
    assert(!"TODO");
    return NULL;
}

