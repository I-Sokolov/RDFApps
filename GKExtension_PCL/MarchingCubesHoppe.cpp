#include "pch.h"
#include "PointCloud.h"
#include "MarchingCubesHoppe.h"

#define CLASS_NAME              "PCL - Marching Cubes Hoppe"

#define PARAM_MCH_RESOLUTION    "Param: M.C.H. Grid Resolution (50,50,50)"
#define PARAM_MCH_ISO_LEVEL     "Param: M.C.H. IsoLevel(0.)"
#define PARAM_MCH_EXTENT_GRID   "Param: M.C.H. Extent Grid (0.0%)"


static MarchingCubesHoppe s_Geometry;

/// <summary>
/// 
/// </summary>
bool MarchingCubesHoppe::CreateClass(OwlModel model)
{
    REQUIRED(!GetClassByName(model, CLASS_NAME), "class already exists\n");

    OwlClass clsMesh = ::CreateClass(model, CLASS_NAME);
    REQUIRED(clsMesh, "Failed to create class\n");

    OwlClass clsGeometricItem = GetClassByName(model, "GeometricItem");
    REQUIRED(clsGeometricItem, "Failed GetClassByName (GeometricItem)\n");

    REQUIRED(SetClassParent(clsMesh, clsGeometricItem), "Fail to set parent");

    PointCloud::AddPointCloudProp(clsMesh);

    AddClassProperty(clsMesh, PARAM_MCH_RESOLUTION, DATATYPEPROPERTY_TYPE_INTEGER, 3, NULL, 3);
    AddClassProperty(clsMesh, PARAM_MCH_ISO_LEVEL, DATATYPEPROPERTY_TYPE_DOUBLE);
    AddClassProperty(clsMesh, PARAM_MCH_EXTENT_GRID, DATATYPEPROPERTY_TYPE_DOUBLE);

    engine_SetClassGeometryHandler(clsMesh, &s_Geometry);

    return true;
}

/// <summary>
/// 
/// </summary>
pcl::PolygonMesh::Ptr MarchingCubesHoppe::ReconstructMesh(OwlInstance inst, pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals)
{
    int_t resolution[3] = { 50,50,50 };
    GetDataProperyValue(inst, PARAM_MCH_RESOLUTION, resolution, 3);
    double isoLevel = GetDataProperyValue(inst, PARAM_MCH_ISO_LEVEL, 0.);
    double extentGrid = GetDataProperyValue(inst, PARAM_MCH_EXTENT_GRID, 0.);

    pcl::MarchingCubesHoppe<pcl::PointNormal> mc;
    mc.setInputCloud(cloud_with_normals);
    mc.setGridResolution(resolution[0], resolution[1], resolution[2]);
    mc.setIsoLevel(isoLevel);
    mc.setPercentageExtendGrid(extentGrid);

    pcl::PolygonMesh::Ptr mesh(new pcl::PolygonMesh());
    mc.reconstruct(*mesh);

    return mesh;
}