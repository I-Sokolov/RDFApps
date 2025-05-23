
#include "pch.h"
#include "PointCloud.h"
#include "GreedyProjectionTriangulation.h"

#define CLASS_NAME  "PCL - Greedy Projection Triangulation"

#define PARAM_GP_SEARCH_RADIUS      "Param: GP3 Search Radius (.05)"
#define PARAM_GP3_MU                "Param: GP3 Mu (2.5)"
#define PARAM_GP3_MAX_NEIGHBORS     "Param: GP3 Max Neighbors(100)"
#define PARAM_GP3_MAX_SURF_ANGLE    "Param: GP3 Max Surf.Angle(45)" 
#define PARAM_GP3_MIN_ANGLE         "Param: GP3 Min Angle(10)" 
#define PARAM_GP3_MAX_ANGLE         "Param: GP3 Max Angle(120)" 
#define PARAM_GP3_N_CONSIST         "Param: GP3 NormalConsistency(F)"


static GreedyProjectionTriangulation s_Geometry;

/// <summary>
/// 
/// </summary>
bool GreedyProjectionTriangulation::CreateClass(OwlModel model)
{
    REQUIRED(!GetClassByName(model, CLASS_NAME), "class already exists\n");

    OwlClass clsMesh = ::CreateClass(model, CLASS_NAME);
    REQUIRED(clsMesh, "Failed to create class\n");

    OwlClass clsGeometricItem = GetClassByName(model, "GeometricItem");
    REQUIRED(clsGeometricItem, "Failed GetClassByName (GeometricItem)\n");

    REQUIRED(SetClassParent(clsMesh, clsGeometricItem), "Fail to set parent");

    PointCloud::AddPointCloudProp(clsMesh);

    AddClassProperty(clsMesh, PARAM_GP_SEARCH_RADIUS, DATATYPEPROPERTY_TYPE_DOUBLE);
    AddClassProperty(clsMesh, PARAM_GP3_MU, DATATYPEPROPERTY_TYPE_DOUBLE);
    AddClassProperty(clsMesh, PARAM_GP3_MAX_NEIGHBORS, DATATYPEPROPERTY_TYPE_DOUBLE);
    AddClassProperty(clsMesh, PARAM_GP3_MAX_SURF_ANGLE, DATATYPEPROPERTY_TYPE_DOUBLE);
    AddClassProperty(clsMesh, PARAM_GP3_MIN_ANGLE, DATATYPEPROPERTY_TYPE_DOUBLE);
    AddClassProperty(clsMesh, PARAM_GP3_MAX_ANGLE, DATATYPEPROPERTY_TYPE_DOUBLE);
    AddClassProperty(clsMesh, PARAM_GP3_N_CONSIST, DATATYPEPROPERTY_TYPE_BOOLEAN);

    engine_SetClassGeometryHandler(clsMesh, &s_Geometry);

    return true;
}



pcl::PolygonMesh::Ptr GreedyProjectionTriangulation::ReconstructMesh(OwlInstance inst, pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals)
{
    pcl::PolygonMesh::Ptr triangles(new pcl::PolygonMesh());

    // search tree
    pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);
    tree2->setInputCloud(cloud_with_normals);

    // Setup Greedy Triangulation
    pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;

    double dSearchRadius = GetDataProperyValue(inst, PARAM_GP_SEARCH_RADIUS, 0.05);
    double dMu = GetDataProperyValue(inst, PARAM_GP3_MU, 2.5);
    double dMaxNeighbors = GetDataProperyValue(inst, PARAM_GP3_MAX_NEIGHBORS, 100.0);
    double dMaxSurfAngle = GetDataProperyValue(inst, PARAM_GP3_MAX_SURF_ANGLE, 45.0);
    double dMinAngle = GetDataProperyValue(inst, PARAM_GP3_MIN_ANGLE, 10.0);
    double dMaxAngle = GetDataProperyValue(inst, PARAM_GP3_MAX_ANGLE, 120.0);
    double dConsist = GetDataProperyValue(inst, PARAM_GP3_N_CONSIST, false);

    gp3.setSearchRadius(dSearchRadius); // search radius
    gp3.setMu(dMu);
    gp3.setMaximumNearestNeighbors(dMaxNeighbors);
    gp3.setMaximumSurfaceAngle(dMaxSurfAngle * M_PI / 180);
    gp3.setMinimumAngle(dMinAngle * M_PI / 180);
    gp3.setMaximumAngle(dMaxAngle * M_PI / 180);
    gp3.setNormalConsistency(dConsist);

    gp3.setInputCloud(cloud_with_normals);
    gp3.setSearchMethod(tree2);
    gp3.reconstruct(*triangles);

    auto npt = triangles->cloud.width * triangles->cloud.height;
    auto ntr = triangles->polygons.size();
    printf("%d points, %d triangles\n", (int)npt, (int)ntr);

    return triangles;
}

