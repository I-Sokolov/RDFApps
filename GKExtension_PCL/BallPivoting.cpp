#include "pch.h"
#include "PointCloud.h"
#include "BallPivoting.h"

#define CLASS_NAME              "PCL - Ball Pivoting Algorithm"

#define PARAM_BPA_RESOLUTION    "Param: BPA Radii (.01,.02,.03)"


static BallPivoting s_Geometry;

/// <summary>
/// 
/// </summary>
bool BallPivoting::CreateClass(OwlModel model)
{
    REQUIRED(!GetClassByName(model, CLASS_NAME), "class already exists\n");

    OwlClass clsMesh = ::CreateClass(model, CLASS_NAME);
    REQUIRED(clsMesh, "Failed to create class\n");

    OwlClass clsGeometricItem = GetClassByName(model, "GeometricItem");
    REQUIRED(clsGeometricItem, "Failed GetClassByName (GeometricItem)\n");

    REQUIRED(SetClassParent(clsMesh, clsGeometricItem), "Fail to set parent");

    PointCloud::AddPointCloudProp(clsMesh);

    AddClassProperty(clsMesh, PARAM_BPA_RESOLUTION, DATATYPEPROPERTY_TYPE_DOUBLE, 3, NULL, 3);

    engine_SetClassGeometryShellBasedSolid(clsMesh, &s_Geometry);

    return true;
}

/// <summary>
/// 
/// </summary>
pcl::PolygonMesh::Ptr BallPivoting::ReconstructMesh(OwlInstance inst, pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals)
{
    double radii[3] = { .01,.02,.03 };
    GetDataProperyValue(inst, PARAM_BPA_RESOLUTION, radii, 3);

    pcl::BallPivoting<pcl::PointNormal> bpa;
    bpa.setInputCloud(cloud_with_normals);
    bpa.setRadii(std::vector<float>{0.01f, 0.02f, 0.03f});

    pcl::PolygonMesh::Ptr mesh(new pcl::PolygonMesh());
    bpa.reconstruct(*mesh);

    return mesh;
}