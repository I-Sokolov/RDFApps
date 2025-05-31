#pragma once

#include "PointCloudShell.h"

/// <summary>
/// 
/// </summary>
class MarchingCubesHoppe : public IAlgorithm
{
public:
    static bool CreateClass(OwlModel model);

protected:
    virtual pcl::PolygonMesh::Ptr ReconstructMesh(OwlInstance inst, pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals) override;
};
