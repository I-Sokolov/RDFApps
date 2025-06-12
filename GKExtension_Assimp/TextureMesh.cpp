#include "pch.h"
#include "Helper.h"
#include "TextureMesh.h"

#define CLASS_NAME                  "Assimp - Textured Mesh"
#define PROP_INPUT_FILE             "PLY Data File"


/// <summary>
/// 
/// </summary>
bool TextureMesh::CreateClass(OwlModel model)
{
    OwlClass clsPointCloud = GetClassByName(model, CLASS_NAME);
    if (!clsPointCloud)
        clsPointCloud = ::CreateClass(model, CLASS_NAME);
    ON_ERROR(clsPointCloud, "Failed to create class\n");

    OwlClass clsGeometricItem = GetClassByName(model, "GeometricItem");
    ON_ERROR(clsGeometricItem, "Failed GetClassByName (GeometricItem)\n");
    if (!IsClassAncestor(clsPointCloud, clsGeometricItem))
        ON_ERROR(SetClassParent(clsPointCloud, clsGeometricItem), "Fail to set parent");

    Helper::AddClassProperty(clsPointCloud, PROP_INPUT_FILE, DATATYPEPROPERTY_TYPE_STRING);

    rdfgeom_SetClassGeometry(clsPointCloud, CreateShell, GetBoundingBox, NULL);

    return true;

}

/// <summary>
/// 
/// </summary>
bool TextureMesh::GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*)
{
    return false;
}

/// <summary>
/// 
/// </summary>
void TextureMesh::CreateShell(OwlInstance inst, void*)
{
    auto shell = rdfgeom_GetInstanceRepresentation(inst);
    if (!shell)
        return;

}


/// <summary>
/// 
/// </summary>
bool TextureMesh::LoadPLYFile(const char* filePath, SHELL* shell)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, /*aiProcess_Triangulate | */ aiProcess_JoinIdenticalVertices);
    ON_ERROR(scene && scene->HasMeshes(), "Failed to read");

    const aiMesh* mesh = scene->mMeshes[0];
    
    SetVerticies(mesh, shell);
}

/// <summary>
/// 
/// </summary>
bool TextureMesh::SetVerticies(const aiMesh* mesh, SHELL* shell)
{

}
