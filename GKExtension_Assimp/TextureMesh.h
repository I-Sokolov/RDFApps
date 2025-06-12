#pragma once

class TextureMesh
{
public:
    static bool CreateClass(OwlModel model);

private:
    static std::string GetFilePathIfNeedToRead(OwlInstance inst);
    static bool LoadPLYFile(const char* filePath, OwlInstance inst, SHELL* shell);
    static bool SetVerticies(const aiMesh* mesh, OwlInstance inst, SHELL* shell);
    static bool SetFaces(const aiMesh* mesh, OwlInstance inst, SHELL* shell);

private:
    static bool GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*);
    static void CreateShell(OwlInstance inst, void*);

};

