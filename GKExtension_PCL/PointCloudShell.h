#pragma once

/// <summary>
/// 
/// </summary>
class PointCloudShell : public IEngineExtGeometry
{
public:
    virtual bool GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix) override;
    virtual void CreateShell(OwlInstance inst, SHELL* shell, IEngineMemory* memory) override;

protected:
    virtual pcl::PolygonMesh::Ptr ReconstructMesh(OwlInstance inst, pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals) = 0;
};


