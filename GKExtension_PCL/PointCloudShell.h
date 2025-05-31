#pragma once

/// <summary>
/// 
/// </summary>
class PointCloudShell
{
public:
    static bool GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void* pvAlgorithm);
    static void CreateShell(OwlInstance inst, void* pvAlgorithm);
};

struct IAlgorithm
{
    virtual pcl::PolygonMesh::Ptr ReconstructMesh(OwlInstance inst, pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals) = 0;
};


