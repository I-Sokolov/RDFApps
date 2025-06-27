#pragma once


class ImportPLY
{
public:
    ImportPLY(OwlModel model, char* errorBuff);

    OwlInstance Import(const char* filePath);

private:
    void LogError(const char* msg, ...);

private:
    bool SetVerticies(const aiMesh* mesh, OwlInstance brep);
    bool SetFaces(const aiMesh* mesh, OwlInstance brep);
    bool SetCoordinates(const char* propName, aiVector3D* rpt, size_t npt, int_t dim, OwlInstance brep);

private:
    bool SetPointSet(const aiMesh* mesh, OwlInstance pointSet);
    bool SetFacetSurface(const aiMesh* mesh, OwlInstance pointSet, OwlInstance facetSurface);

private:
    bool SetMaterial(const char* plyFilePath, const aiScene* scene, const aiMesh* mesh, OwlInstance brep);

private:
    void DumpStatistic(const aiMesh* mesh);

private:
    OwlModel    m_model;
    char*       m_errorBuff;
};

