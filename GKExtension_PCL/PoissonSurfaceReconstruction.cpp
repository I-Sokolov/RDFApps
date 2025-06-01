
#include "pch.h"
#include "PointCloud.h"
#include "PoissonSurfaceReconstruction.h"

#define CLASS_NAME               "PCL - Poisson Surface Reconstruction"

#define PARAM_POISSON_DEPTH      "Param: Poisson Depth (8)"


static PoissonSurfaceReconstruction s_Algorithm;

/// <summary>
/// 
/// </summary>
bool PoissonSurfaceReconstruction::CreateClass(OwlModel model)
{
    OwlClass clsMesh = GetClassByName(model, CLASS_NAME);
    if (!clsMesh)
        clsMesh = ::CreateClass(model, CLASS_NAME);
    REQUIRED(clsMesh, "Failed to create class\n");

    OwlClass clsGeometricItem = GetClassByName(model, "GeometricItem");
    REQUIRED(clsGeometricItem, "Failed GetClassByName (GeometricItem)\n");
    if (!IsClassAncestor(clsMesh, clsGeometricItem))
        REQUIRED(SetClassParent(clsMesh, clsGeometricItem), "Fail to set parent");

    PointCloud::AddPointCloudProp(clsMesh);

    AddClassProperty(clsMesh, PARAM_POISSON_DEPTH, DATATYPEPROPERTY_TYPE_INTEGER);

    rdfgeom_SetClassGeometry(clsMesh, PointCloudMesh::CreateShell, PointCloudMesh::GetBoundingBox, &s_Algorithm);

    return true;
}

/// <summary>
/// 
/// </summary>
pcl::PolygonMesh::Ptr PoissonSurfaceReconstruction::ReconstructMesh(OwlInstance inst, pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals)
{
    int_t depth = GetDataProperyValue<int_t>(inst, PARAM_POISSON_DEPTH, 8);

    pcl::Poisson<pcl::PointNormal> poisson;
    poisson.setDepth(depth); // Detailing
    poisson.setInputCloud(cloud_with_normals);

    pcl::PolygonMesh::Ptr mesh(new pcl::PolygonMesh());
    poisson.reconstruct(*mesh);

    return mesh;
}
