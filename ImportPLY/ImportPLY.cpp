#include "pch.h"
#include "ImportPLY.h"

//#define LEGACY_BREP

#define ERR_BUFF_SIZE 512

#define PROP_DIM        "dim"
#define PROP_COORD      "coordinates"
#define PROP_NORMALS    "normalsCoordinates"
#define PROP_TANGENTS   "tangentCoordinates"

#define PROP_TEX_DIM    "textureDim"
#define PROP_TEX_COORD  "textureCoordinates"



#define CLS_BREP        "BoundaryRepresentation"
#define CLS_POINTSET    "PointSet"
#define CLS_FACETSURFACE "FacetSurface"
#define CLS_COLLECTION  "Collection"
#define CLS_MATERIAL    "Material"
#define CLS_TEXTURE     "Texture"

#define PROP_POINTS     "vertices"
#define PROP_IDXS       "indices"
#define PROP_OBJECT     "object"
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
ImportPLY::ImportPLY(OwlModel model, char* errorBuff)
    : m_model(model)
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
    const aiScene* scene = reader.ReadFile(filePath, /*aiProcess_Triangulate |
    aiProcess_GenNormals | */ aiProcess_JoinIdenticalVertices);
    if (!scene || !scene->HasMeshes()) {
        LogError("Failed to read or no meshes");
        return NULL;
    }

#ifdef    LEGACY_BREP
    GET_CLASS(clsBrep, CLS_BREP);
#else
    GET_CLASS(clsBrep, CLS_FACETSURFACE)
    GET_CLASS(clsPointSet, CLS_POINTSET);
#endif

    std::vector<OwlInstance> breps;

    for (unsigned int iMesh = 0; iMesh < scene->mNumMeshes; iMesh++) {
        const aiMesh* mesh = scene->mMeshes[iMesh];

        //DumpStatistic(mesh);

        if (mesh) {
            if (auto brep = CreateInstance(clsBrep)) {
#ifdef LEGACY_BREP
                if (SetVerticies(mesh, brep)) {
                    if (SetFaces(mesh, brep)) {
#else
                if (auto pointSet = CreateInstance(clsPointSet)) {
                    if (SetPointSet(mesh, pointSet) && SetFacetSurface(mesh, pointSet, brep)) {
#endif
                        SetMaterial(filePath, scene, mesh, brep);
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
bool ImportPLY::SetCoordinates(const char* propName, aiVector3D* rpt, size_t npt, int_t dim, OwlInstance brep)
{
    if (!rpt) {
        return true;
    }

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

    SetCoordinates(PROP_TEX_COORD, mesh->mTextureCoords[0], mesh->mNumVertices, 2, brep);

    return true;
}

/// <summary>
/// 
/// </summary>
bool ImportPLY::SetPointSet(const aiMesh* mesh, OwlInstance pointSet)
{
    if (!mesh->mNumVertices) {
        LogError("Empty mesh (no verticies)");
        return false;
    }

    int_t dim = 3;
    GET_PROPERTY(propDim, "dim");
    SET_PROPERTY_VALUE(pointSet, propDim, Datatype, &dim, 1, false);

    if (!SetCoordinates(PROP_COORD, mesh->mVertices, mesh->mNumVertices, dim, pointSet)) {
        return false;
    }
    
    SetCoordinates(PROP_NORMALS, mesh->mNormals, mesh->mNumVertices, dim, pointSet);

    SetCoordinates(PROP_TANGENTS, mesh->mTangents, mesh->mNumVertices, dim, pointSet);

    for (int channel = 0; channel < AI_MAX_NUMBER_OF_TEXTURECOORDS; channel++) {
        if (mesh->mTextureCoords[channel]) {
            
            int_t texDim = mesh->mNumUVComponents[channel];
            if (texDim < 1 || texDim > 3)
                texDim = 2;

            GET_PROPERTY(propTexDim, PROP_TEX_DIM);
            SET_PROPERTY_VALUE(pointSet, propTexDim, Datatype, &texDim, 1, false);

            SetCoordinates(PROP_TEX_COORD, mesh->mTextureCoords[channel], mesh->mNumVertices, texDim, pointSet);

            break; //use first found channel
        }
    }

    return true;
}

/// <summary>
/// 
/// </summary>
bool ImportPLY::SetFacetSurface(const aiMesh* mesh, OwlInstance pointSet, OwlInstance facetSurface)
{
    GET_PROPERTY(propIdxs, PROP_IDXS);
    GET_PROPERTY(propObject, PROP_OBJECT);

    //((aiMesh*)mesh)->mNumFaces = 1;

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

    auto err = SetDatatypeProperty(facetSurface, propIdxs, idxs, N_ind);

    delete[]idxs;

    if (err) {
        LogError("Failed to set property value for %s", PROP_IDXS);
        return false;
    }

    err = SetObjectProperty(facetSurface, propObject, pointSet);

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
bool ImportPLY::SetMaterial(const char* plyFilePath, const aiScene* scene, const aiMesh* mesh, OwlInstance brep)
{
    if (scene->HasMaterials()) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString texName;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texName) == AI_SUCCESS) {

            std::filesystem::path _plyFilePath(plyFilePath);

            std::filesystem::path texFullPath(_plyFilePath.parent_path());
            texFullPath /= texName.C_Str();
            
            const char* texPath_ = texFullPath.string().c_str();

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

/// <summary>
/// 
/// </summary>
static double EPS = 1e-7;

struct PointComparer
{
    bool operator()(aiVector3D const& v1, aiVector3D const& v2) const
    {
        for (int i = 0; i < m_dim; i++) {
            if (v1[i] > v2[i] + EPS)
                return true;
        }
        return false;
    }

    int m_dim = 3;
};

/// <summary>
/// 
/// </summary>
void ImportPLY::DumpStatistic(const aiMesh* mesh)
{
    printf("Number of verticied %d\n", mesh->mNumVertices);
    printf("Number of faces %d\n", mesh->mNumFaces);

    size_t N_ind = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        N_ind += face.mNumIndices + 1;
    }
    printf("Number of indecies %d\n", (int)N_ind);

    auto minEdge = DBL_MAX;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            int ind1 = face.mIndices[j];
            int ind2 = face.mIndices[(j + 1) % face.mNumIndices];
            
            aiVector3D& pt1 = mesh->mVertices[ind1];
            aiVector3D& pt2 = mesh->mVertices[ind2];

            aiVector3D v = pt1 - pt2;
            auto edge = v.Length();

            if (minEdge>edge) minEdge = edge;
        }
    }
    printf("Min edge length %g\n", minEdge);

    std::set<aiVector3d, PointComparer> diffPoints;
    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        diffPoints.insert(mesh->mVertices[i]);
    }
    printf("Number of different points %d (EPS %g)\n", (int)diffPoints.size(), EPS);
}

