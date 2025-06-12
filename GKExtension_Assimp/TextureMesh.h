#pragma once

class TextureMesh
{
public:
    static bool CreateClass(OwlModel model);

private:
    static bool LoadPLYFile(const char* filePath, SHELL* shell);
    static bool SetVerticies(const aiMesh* mesh, SHELL* shell);

private:
    static bool GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*);
    static void CreateShell(OwlInstance inst, void*);

};

