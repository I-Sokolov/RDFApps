#include "pch.h"
#include "ImportPLY.h"

#define ERR_BUFF_SIZE 512

#define CLS_BREP        "BoundaryRepresentation"
#define CLS_COLLECTION  "Collection"

#define PROP_POINTS     "vertices"
#define PROP_TEX_COORD  "textureCoordinates"
#define PROP_IDXS       "indices"
#define PROP_OBJECTS    "objects"

/// <summary>
/// 
/// </summary>
ImportPLY::ImportPLY(OwlModel model, const char* textureFolder, const char* textureBasePath, char* errorBuff)
    : m_model(model)
    , m_textureFolder(textureFolder)
    , m_textureBasePath(textureBasePath)
    , m_errorBuff(errorBuff)
{
    *m_errorBuff = 0;
}

/// <summary>
/// 
/// </summary>
void ImportPLY::LogError(const char* msg, ...)
{
    auto len = strlen(m_errorBuff);
    auto remains = ERR_BUFF_SIZE - len;

    char* p = m_errorBuff + len;

    if (remains > 0) {
        *p = '\n';
        p++;
        *p = 0;
        remains--;
    }

    if (remains > 0) {
        char buff[ERR_BUFF_SIZE];
        va_list args;
        va_start(args, msg);
        vsnprintf(buff, ERR_BUFF_SIZE - 2, msg, args);
        va_end(args);

        if (remains > strlen(buff)) {
            strcpy(p, buff);
        }
    }
}

/// <summary>
/// 
/// </summary>
OwlInstance ImportPLY::Import(const char* filePath)
{
    if (!m_model) {
        LogError("model is NULL");
        return NULL;
    }
    Assimp::Importer reader;
    const aiScene* scene = reader.ReadFile(filePath, /*aiProcess_Triangulate | */ aiProcess_JoinIdenticalVertices);
    if (!scene || !scene->HasMeshes()) {
        LogError("Failed to read or no meshes");
        return NULL;
    }

    auto clsBrep = GetClassByName(m_model, CLS_BREP);
    if (!clsBrep) {
        LogError("Missed class " CLS_BREP);
        return NULL;
    }

    std::vector<OwlInstance> breps;

    for (unsigned int iMesh = 0; iMesh < scene->mNumMeshes; iMesh++) {
        const aiMesh* mesh = scene->mMeshes[iMesh];
        if (mesh) {
            if (auto brep = CreateInstance(clsBrep)) {
                if (SetVerticies(mesh, brep)) {
                    if (SetFaces(mesh, brep)) {
                        SetMaterial(mesh, brep);
                        breps.push_back(brep);
                    }
                }
            }
            else {
                LogError("Failed create instance " CLS_BREP);
            }
        }
        else {
            LogError("Empty mesh");
        }
    }

    if (breps.empty()) {
        return NULL;
    }
    else if (breps.size() == 1) {
        return breps[0];
    }
    else {
        auto prop = GetPropertyByName(m_model, PROP_OBJECTS);
        if (!prop) {
            LogError("Missed property " PROP_OBJECTS);
            return breps[0];
        }

        auto clsCollection = GetClassByName(m_model, CLS_COLLECTION);
        if (!clsCollection) {
            LogError("Missed class " CLS_COLLECTION);
            return breps[0];
        }

        auto collection = CreateInstance(clsCollection);
        if (!collection) {
            LogError("Failed to create instance " CLS_COLLECTION);
            return breps[0];
        }

        auto res = SetObjectProperty(collection, prop, breps.data(), breps.size());
        if (res) {
            LogError("Failed to set property %s for %s", PROP_OBJECTS, CLS_COLLECTION);
            return breps[0];
        }

        return collection;
    }
}

/// <summary>
/// 
/// </summary>
bool ImportPLY::SetCoordinates(const char* propName, aiVector3D* rpt, size_t npt, short dim, OwlInstance brep)
{
    auto prop = GetPropertyByName(m_model, propName);
    if (!prop) {
        LogError("Missed property %s", propName);
        return false;
    }

    double* coords = new double[npt * dim];
    if (!coords) {
        LogError("Low memory");
        return false;
    }

    for (unsigned int i = 0; i < npt; ++i) {
        const aiVector3D& v = rpt[i];
        for (int j = 0; j < dim; j++) {
            coords[dim * i + j] = v[j];
        }
    }

    auto err = SetDatatypeProperty(brep, prop, coords, npt * dim);

    delete[]coords;

    if (err) {
        LogError("Failed to set property value for %s", propName);
        return false;
    }

    return true;
}


/// <summary>
/// 
/// </summary>
bool ImportPLY::SetVerticies(const aiMesh* mesh, OwlInstance brep)
{
    if (!mesh->mNumVertices) {
        LogError("Empty mesh (no verticies)");
        return false;
    }

    if (!SetCoordinates(PROP_POINTS, mesh->mVertices, mesh->mNumVertices, 3, brep)) {
        return false;
    }

    SetCoordinates(PROP_TEX_COORD, mesh->mVertices, mesh->mNumVertices, 2, brep);

    return true;
}

/// <summary>
/// 
/// </summary>
bool ImportPLY::SetFaces(const aiMesh* mesh, OwlInstance brep)
{
    auto prop = GetPropertyByName(m_model, PROP_IDXS);
    if (!prop) {
        LogError("Missed property %s", PROP_IDXS);
        return false;
    }

    size_t N_ind = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        N_ind += face.mNumIndices + 1;
    }

    if (N_ind == 0) {
        LogError("No faces in mesh");
        return false;
    }

    int_t* idxs = new int_t[N_ind];
    if (!idxs) {
        LogError("Low memory");
        return false;
    }

    int k = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            idxs[k++] = face.mIndices[j];
        }
        idxs[k++] = -1;
    }

    auto err = SetDatatypeProperty(brep, prop, idxs, N_ind);

    delete[]idxs;

    if (err) {
        LogError("Failed to set property value for %s", PROP_IDXS);
        return false;
    }

    return true;
}

/// <summary>
/// 
/// </summary>
bool ImportPLY::SetMaterial(const aiMesh* mesh, OwlInstance brep)
{
    return true;
#if 0
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
#endif
}
