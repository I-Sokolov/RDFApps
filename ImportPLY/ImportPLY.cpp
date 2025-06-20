#include "pch.h"
#include "ImportPLY.h"

#define ERR_BUFF_SIZE 512

#define CLS_BREP        "BoundaryRepresentation"
#define CLS_COLLECTION  "Collection"
#define CLS_MATERIAL    "Material"
#define CLS_TEXTURE     "Texture"

#define PROP_POINTS     "vertices"
#define PROP_TEX_COORD  "textureCoordinates"
#define PROP_IDXS       "indices"
#define PROP_OBJECTS    "objects"
#define PROP_MATERIAL   "material"
#define PROP_TEXTURES   "textures"
#define PROP_NAME       "name"

#define GET_PROPERTY(prop, name)    auto prop = GetPropertyByName(m_model, name); if (!prop) { LogError("Missed property %s", name); return 0;}
#define GET_CLASS(cls, name)        auto cls = GetClassByName(m_model, name); if (!cls) { LogError("Missed class %s", name); return 0;}

#define CREATE_INSTANCE(instance, cls, retOnFail)                               \
            auto instance = CreateInstance(cls);                                \
            if (!instance) {                                                    \
                LogError("Failed to create instance %s", GetNameOfClass(cls));  \
                return retOnFail;                                               \
            }                                                                   \

#define SET_PROPERTY_VALUE(inst, prop, kind, val, card, retOnFail)                          \
            {                                                                               \
                auto res = Set##kind##Property(inst, prop, val, card);                      \
                if (res) {                                                                  \
                    LogError("Failed to set property %s for %s",                            \
                        GetNameOfProperty(prop), GetNameOfClass(GetInstanceClass(inst)));   \
                    return retOnFail;                                                       \
                }                                                                           \
            }                                                                               \

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

    GET_CLASS(clsBrep, CLS_BREP);

    std::vector<OwlInstance> breps;

    for (unsigned int iMesh = 0; iMesh < scene->mNumMeshes; iMesh++) {
        const aiMesh* mesh = scene->mMeshes[iMesh];
        if (mesh) {
            if (auto brep = CreateInstance(clsBrep)) {
                if (SetVerticies(mesh, brep)) {
                    if (SetFaces(mesh, brep)) {
                        SetMaterial(scene, mesh, brep);
                        breps.push_back(brep);
                        brep = 0;
                    }
                }
                if (brep) {
                    RemoveInstance(brep);
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
        GET_CLASS(clsCollection, CLS_COLLECTION);
        GET_PROPERTY(prop, PROP_OBJECTS);

        CREATE_INSTANCE(collection, clsCollection, breps[0]);

        SET_PROPERTY_VALUE(collection, prop, Object, breps.data(), breps.size(), breps[0]);

        return collection;
    }
}

/// <summary>
/// 
/// </summary>
bool ImportPLY::SetCoordinates(const char* propName, aiVector3D* rpt, size_t npt, short dim, OwlInstance brep)
{
    GET_PROPERTY(prop, propName);

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
    GET_PROPERTY(prop, PROP_IDXS);

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
bool ImportPLY::SetMaterial(const aiScene* scene, const aiMesh* mesh, OwlInstance brep)
{
    if (scene->HasMaterials()) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString texPath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {

            auto texPath_ = texPath.C_Str();

            GET_CLASS(clsTexture, CLS_TEXTURE);
            CREATE_INSTANCE(texture, clsTexture, false);

            GET_PROPERTY(propName, PROP_NAME);
            SET_PROPERTY_VALUE(texture, propName, Datatype, &texPath_, 1, false);

            GET_CLASS(clsMaterial, CLS_MATERIAL);
            CREATE_INSTANCE(material, clsMaterial, false);

            GET_PROPERTY(propTextures, PROP_TEXTURES);
            SET_PROPERTY_VALUE(material, propTextures, Object, &texture, 1, false);

            GET_PROPERTY(propMaterial, PROP_MATERIAL);

            SET_PROPERTY_VALUE(brep, propMaterial, Object, &material, 1, false);
        }
    }

    return true;
}
