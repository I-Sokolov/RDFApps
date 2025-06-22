#pragma once

class ImportPLY
{
public:
    ImportPLY(OwlModel model, const char* textureFolder, const char* textureBasePath, char* errorBuff);

    OwlInstance Import(const char* filePath);

private:
    void LogError(const char* msg, ...);

private:
    bool SetPointSet(const aiMesh* mesh, OwlInstance pointSet);


private:
    bool SetVerticies(const aiMesh* mesh, OwlInstance brep);
    bool SetFaces(const aiMesh* mesh, OwlInstance brep);
    bool SetMaterial(const aiScene* scene, const aiMesh* mesh, OwlInstance brep);
    bool SetCoordinates(const char* propName, aiVector3D* rpt, size_t npt, int_t dim, OwlInstance brep);

private:
    OwlModel    m_model;
    const char* m_textureFolder;
    const char* m_textureBasePath;
    char*       m_errorBuff;
};

