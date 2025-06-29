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
    if (transformationMatrix)
        Helper::MatrixIdentity(transformationMatrix);

    startVector->x = startVector->y = startVector->z = DBL_MAX;
    endVector->x = endVector->y = endVector->z = -DBL_MAX;

    auto geom = rdfgeom_GetBRep(inst);
    if (geom) {
        auto npt = rdfgeom_GetNumOfPoints(geom);
        auto rpt = rdfgeom_GetPoints(geom);

        for (int_t i = 0; i < npt && rpt; i++) {
            Helper::ExpandRange(startVector, endVector, rpt[i]);
        }
    }

    return startVector->x <= endVector->x;
}

/// <summary>
/// 
/// </summary>
void TextureMesh::CreateShell(OwlInstance inst, void*)
{
    auto shell = rdfgeom_GetBRep(inst);
    if (!shell)
        return;

    auto filePath = GetFilePathIfNeedToRead(inst);

    if (!filePath.empty()) {
        LoadPLYFile(filePath.c_str(), inst, shell);
    }
}

/// <summary>
/// 
/// </summary>
std::string TextureMesh::GetFilePathIfNeedToRead(OwlInstance inst)
{
    auto filePath = Helper::GetDataProperyValue(inst, PROP_INPUT_FILE, "");
    if (!filePath || !*filePath)
        return "";

    if (*filePath == '"')
        filePath++;

    std::string dataFilePath(filePath);

    if (dataFilePath.back() == '"') {
        dataFilePath.pop_back();
    }

    return dataFilePath;
}

/// <summary>
/// 
/// </summary>
bool TextureMesh::LoadPLYFile(const char* filePath, OwlInstance inst, SHELL* shell)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, /*aiProcess_Triangulate | */ aiProcess_JoinIdenticalVertices);
    ON_ERROR(scene && scene->HasMeshes(), "Failed to read");

    const aiMesh* mesh = scene->mMeshes[0];
    
    SetVerticies(mesh, inst, shell);
    SetFaces(mesh, inst, shell);

    // Texture file
#if 1
    if (scene->HasMaterials()) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString texPath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
            printf("Texture: %s\n", texPath.C_Str());
        }
        else {
            printf("No texture.\n");
        }
    }
    else {
        printf("No material.\n");
    }
#endif
    return true;
}

/// <summary>
/// 
/// </summary>
bool TextureMesh::SetVerticies(const aiMesh* mesh, OwlInstance inst, SHELL* shell)
{
    rdfgeom_AllocatePoints(inst, shell, mesh->mNumVertices, mesh->HasNormals(), mesh->HasTextureCoords(0));

    auto points = rdfgeom_GetPoints(shell);
    if (points) {
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            const aiVector3D& v = mesh->mVertices[i];
            points[i].x = v.x;
            points[i].y = v.y;
            points[i].z = v.z;
        }
    }

    auto normals = rdfgeom_GetNormals(shell);
    if (normals) {
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            const aiVector3D& n = mesh->mNormals[i];
            normals[i].x = n.x;
            normals[i].y = n.y;
            normals[i].z = n.z;
        }
    }

    auto texs = rdfgeom_GetTextureCoordinates(shell);
    if (texs) {
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            const aiVector3D& uv = mesh->mTextureCoords[0][i];
            texs[i].u = uv.x;
            texs[i].v = uv.y;
        }
    }

    return true;
}

/// <summary>
/// 
/// </summary>
static STRUCT_VERTEX** AddLoopEdge(int_t indPoint, STRUCT_VERTEX** ppNextEdge, OwlInstance inst, bool lastPoint = false)
{
    assert(ppNextEdge && !*ppNextEdge);
    if (!ppNextEdge)
        return NULL;

    rdfgeom_vertex_Create(inst, ppNextEdge, indPoint, lastPoint);

    return rdfgeom_vertex_GetNext(*ppNextEdge);
}

/// <summary>
/// 
/// </summary>
bool TextureMesh::SetFaces(const aiMesh* mesh, OwlInstance inst, SHELL* shell)
{
    CONCEPTUAL_FACE** ppCFace = rdfgeom_GetConceptualFaces(shell);
    rdfgeom_cface_Create(inst, ppCFace);

    STRUCT_FACE** ppFace = rdfgeom_cface_GetFaces(*ppCFace);

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];

        STRUCT_VERTEX** loop = NULL;
        int first = -1;

        for (unsigned int j = 0; j < face.mNumIndices; j++) {

            auto vertex = face.mIndices[j];

            if (!loop) {

                rdfgeom_face_Create(inst, ppFace);

                loop = rdfgeom_face_GetBoundary(*ppFace);

                ppFace = rdfgeom_face_GetNext(*ppFace);

                first = vertex;
            }

            loop = AddLoopEdge(vertex, loop, inst);
        }

        if (loop && first >= 0) {
            AddLoopEdge(first, loop, inst, true);
        }
    }

    return true;
}