#pragma once


/// <summary>
/// 
/// </summary>
class TextureMesh 
{
public:
    static bool CreateClass(OwlModel model);

private:
    static bool GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*);
    static void CreateShell(OwlInstance inst, void*);

private:
    static std::string GetFilePathIfNeedToRead(OwlInstance inst);
    static pcl::TextureMeshPtr ReadPLYFile(const char* filePath);
};
