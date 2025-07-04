#include "pch.h"
#include "PointCloud.h"

// https://pcl.readthedocs.io/projects/tutorials/en/master/index.html

/// <summary>
/// 
/// </summary>
#define CLASS_NAME                  "PCL - Point Clound"

#define PROP_INPUT_FILE             "Point Cloud Data File"
#define PROP_INPUT_OBJECT           "object"
#define PROP_INPUT_OBJECTS          "objects"

#define PROP_CLOUD_WIDTH            "Cloud Width"
#define PROP_CLOUD_HEIGHT           "Cloud Height"
#define PROP_CLOUD_ISDENSE          "Cloud Is Dense"
#define PROP_CLOUD_HDR_SEQ          "Cloud Sequence Number"
#define PROP_CLOUD_HDR_STAMP        "Cloud Timestamp"
#define PROP_CLOUD_HDR_FRAME        "Cloud Frame_id"
#define PROP_INPUT_FILE_LOADED      "Data File Loaded (autogenerated)"
#define PROP_CLOUD_POINTS           "Point Cloud Coordinates (autogenerated)"

#define PARAM_RANGE_X               "X clip range"
#define PARAM_RANGE_Y               "Y clip range"
#define PARAM_RANGE_Z               "Z clip range"
#define PARAM_VOXEG_GRID_SIZE       "Voxel Grid Leaf Size (0 off, def 0.01)"
#define PARAM_OUTLIER_NEIGHBORS     "Neighbors To Remove Statistical Outliers (0 off, def 50)"
#define PROP_ENABLE_MLS             "Smooth by Moving Least Squares (F)"
#define PARAM_NORMALS_NEIGHBORS     "Neighbors to Estimate Mormals (20)"


#define PROP_POINT_CLOUD            "object"

/// <summary>
/// 
/// </summary>
static void TransformPoint(
    VECTOR3& out,
    const VECTOR3& in,
    const MATRIX& M
)
{
    out.x = in.x * M._11 + in.y * M._21 + in.z * M._31 + M._41;
    out.y = in.x * M._12 + in.y * M._22 + in.z * M._32 + M._42;
    out.z = in.x * M._13 + in.y * M._23 + in.z * M._33 + M._43;
}

/// <summary>
/// 
/// </summary>
extern	void	MatrixIdentity(
    MATRIX* pOut
)
{
    pOut->_12 = pOut->_13 =
        pOut->_21 = pOut->_23 =
        pOut->_31 = pOut->_32 =
        pOut->_41 = pOut->_42 = pOut->_43 = 0.;

    pOut->_11 = pOut->_22 = pOut->_33 = 1.;
}

/// <summary>
/// 
/// </summary>
void	TransformTransform(
    MATRIX* pOut,
    const MATRIX* pM1,
    const MATRIX* pM2
)
{
    assert(pOut && pM1 && pM2);

    MATRIX pTmp;

    if (pOut) {
        pTmp._11 = pM1->_11 * pM2->_11 + pM1->_12 * pM2->_21 + pM1->_13 * pM2->_31;
        pTmp._12 = pM1->_11 * pM2->_12 + pM1->_12 * pM2->_22 + pM1->_13 * pM2->_32;
        pTmp._13 = pM1->_11 * pM2->_13 + pM1->_12 * pM2->_23 + pM1->_13 * pM2->_33;

        pTmp._21 = pM1->_21 * pM2->_11 + pM1->_22 * pM2->_21 + pM1->_23 * pM2->_31;
        pTmp._22 = pM1->_21 * pM2->_12 + pM1->_22 * pM2->_22 + pM1->_23 * pM2->_32;
        pTmp._23 = pM1->_21 * pM2->_13 + pM1->_22 * pM2->_23 + pM1->_23 * pM2->_33;

        pTmp._31 = pM1->_31 * pM2->_11 + pM1->_32 * pM2->_21 + pM1->_33 * pM2->_31;
        pTmp._32 = pM1->_31 * pM2->_12 + pM1->_32 * pM2->_22 + pM1->_33 * pM2->_32;
        pTmp._33 = pM1->_31 * pM2->_13 + pM1->_32 * pM2->_23 + pM1->_33 * pM2->_33;

        pTmp._41 = pM1->_41 * pM2->_11 + pM1->_42 * pM2->_21 + pM1->_43 * pM2->_31 + pM2->_41;
        pTmp._42 = pM1->_41 * pM2->_12 + pM1->_42 * pM2->_22 + pM1->_43 * pM2->_32 + pM2->_42;
        pTmp._43 = pM1->_41 * pM2->_13 + pM1->_42 * pM2->_23 + pM1->_43 * pM2->_33 + pM2->_43;

        pOut->_11 = pTmp._11;
        pOut->_12 = pTmp._12;
        pOut->_13 = pTmp._13;

        pOut->_21 = pTmp._21;
        pOut->_22 = pTmp._22;
        pOut->_23 = pTmp._23;

        pOut->_31 = pTmp._31;
        pOut->_32 = pTmp._32;
        pOut->_33 = pTmp._33;

        pOut->_41 = pTmp._41;
        pOut->_42 = pTmp._42;
        pOut->_43 = pTmp._43;
    }
    else {
        assert(false);
    }
}


/// <summary>
/// 
/// </summary>
bool PointCloud::CreateClass(OwlModel model)
{
    OwlClass clsPointCloud = GetClassByName(model, CLASS_NAME);
    if (!clsPointCloud)
        clsPointCloud = ::CreateClass(model, CLASS_NAME);
    REQUIRED(clsPointCloud, "Failed to create class\n");

    OwlClass clsGeometricItem = GetClassByName(model, "GeometricItem");
    REQUIRED(clsGeometricItem, "Failed GetClassByName (GeometricItem)\n");
    if (!IsClassAncestor(clsPointCloud, clsGeometricItem))
        REQUIRED(SetClassParent(clsPointCloud, clsGeometricItem), "Fail to set parent");

    AddClassProperty(clsPointCloud, PROP_INPUT_FILE, DATATYPEPROPERTY_TYPE_STRING);
    AddClassProperty(clsPointCloud, PROP_INPUT_OBJECT, OBJECTPROPERTY_TYPE);
    AddClassProperty(clsPointCloud, PROP_INPUT_OBJECTS, OBJECTPROPERTY_TYPE, 0, NULL, -1);
    AddClassProperty(clsPointCloud, PROP_ENABLE_MLS, DATATYPEPROPERTY_TYPE_BOOLEAN, 1);

    AddClassProperty(clsPointCloud, PARAM_RANGE_X, DATATYPEPROPERTY_TYPE_DOUBLE, 0, NULL, 2);
    AddClassProperty(clsPointCloud, PARAM_RANGE_Y, DATATYPEPROPERTY_TYPE_DOUBLE, 0, NULL, 2);
    AddClassProperty(clsPointCloud, PARAM_RANGE_Z, DATATYPEPROPERTY_TYPE_DOUBLE, 0, NULL, 2);
    AddClassProperty(clsPointCloud, PARAM_VOXEG_GRID_SIZE, DATATYPEPROPERTY_TYPE_DOUBLE);
    AddClassProperty(clsPointCloud, PARAM_OUTLIER_NEIGHBORS, DATATYPEPROPERTY_TYPE_INTEGER);
    AddClassProperty(clsPointCloud, PARAM_NORMALS_NEIGHBORS, DATATYPEPROPERTY_TYPE_INTEGER);
    
    AddClassProperty(clsPointCloud, PROP_CLOUD_WIDTH, DATATYPEPROPERTY_TYPE_INTEGER);
    AddClassProperty(clsPointCloud, PROP_CLOUD_HEIGHT, DATATYPEPROPERTY_TYPE_INTEGER);
    AddClassProperty(clsPointCloud, PROP_CLOUD_ISDENSE, DATATYPEPROPERTY_TYPE_BOOLEAN);
    AddClassProperty(clsPointCloud, PROP_CLOUD_HDR_SEQ, DATATYPEPROPERTY_TYPE_INTEGER);
    AddClassProperty(clsPointCloud, PROP_CLOUD_HDR_STAMP, DATATYPEPROPERTY_TYPE_INTEGER);
    AddClassProperty(clsPointCloud, PROP_CLOUD_HDR_FRAME, DATATYPEPROPERTY_TYPE_STRING);

    AddClassProperty(clsPointCloud, PROP_INPUT_FILE_LOADED, DATATYPEPROPERTY_TYPE_STRING); 
    AddClassProperty(clsPointCloud, PROP_CLOUD_POINTS, DATATYPEPROPERTY_TYPE_DOUBLE, 0, NULL, -1);

    rdfgeom_SetClassGeometry(clsPointCloud, PointCloudGeometry::CreateShell,  PointCloudGeometry::GetBoundingBox, NULL);

    return true;
}


/// <summary>
/// 
/// </summary>
pcl::PointCloud<pcl::PointXYZ>::Ptr PointCloud::GetDataFilePointCloud(OwlInstance instPointCloud)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud <pcl::PointXYZ>());

    //load from point cloud data file
    auto filePath = GetFilePathIfNeedToRead(instPointCloud);
    if (!filePath.empty()) {
        ReadCloudFileAndSaveOnInstance(instPointCloud, filePath, cloud);
    }
    else {
        GetPointsSavedOnInstance(instPointCloud, cloud);
    }

    return cloud;
}

/// <summary>
/// 
/// </summary>
pcl::PointCloud<pcl::PointXYZ>::Ptr PointCloud::CombinePointCloud(OwlInstance instPointCloud)
{
    auto cloud = GetDataFilePointCloud(instPointCloud);

    //add from nested objects
    const char* propNames[] = { PROP_INPUT_OBJECT, PROP_INPUT_OBJECTS };
    for (auto name : propNames) {
        OwlInstance* nested = NULL;
        int_t card = GetObjectPropertyValue(instPointCloud, name, &nested);
        for (int_t i = 0; i < card; i++) {
            AddPointsFromNestedObject(nested[i], cloud);
        }
    }

    SetCloudAttributesFromInstance(instPointCloud, cloud);

    return cloud;
}

/// <summary>
/// 
/// </summary>
void PointCloud::AddPointsFromNestedObject(OwlInstance instNested, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    assert(cloud); if (!cloud) return;

    SHELL* shell = rdfgeom_GetBRep(instNested);
    if (shell) {
        auto cfaceP = rdfgeom_GetConceptualFaces(shell);
        if (cfaceP) {
            AddPointsFromCFaces(*cfaceP, cloud, NULL);
        }
    }
}


/// <summary>
/// 
/// </summary>
void PointCloud::AddPointsFromCFaces(CONCEPTUAL_FACE* cface, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, MATRIX* parentTransform)
{
    if (!cface) {
        return;
    }

    AddPointsFromCFaces(*rdfgeom_cface_GetNext(cface), cloud, parentTransform);

    MATRIX* transform = parentTransform;
    MATRIX buff;
    if (auto localT = rdfgeom_cface_GetLocalTranformation(cface)) {
        if (transform) {
            assert(!"not tested");
            TransformTransform(&buff, transform, localT); //or maybe other way
            transform = &buff;
        }
        else {
            transform = localT;
        }
    }

    AddPointsFromCFaces(*rdfgeom_cface_GetChildren(cface), cloud, transform);

    if (OwlInstance faceOwnerInst = rdfgeom_cface_GetInstance(cface)) {

        if (SHELL* faceOwnerShell = rdfgeom_GetBRep(faceOwnerInst)) {
            if (auto nonTransformedVertices = rdfgeom_GetPoints(faceOwnerShell)) {
                VECTOR3 transformed;
                for (auto vertexP = rdfgeom_cface_GetVertices(cface); *vertexP; vertexP = rdfgeom_vertex_GetNext(*vertexP)) {

                    auto ptind = rdfgeom_vertex_GetPointIndex (*vertexP);

                    if (ptind >= rdfgeom_GetNumOfPoints(faceOwnerShell)) {
                        assert(false);
                        continue;
                    }

                    auto pt = rdfgeom_GetPoints (faceOwnerShell) + ptind;
                    if (transform) {
                        TransformPoint(transformed, *pt, *transform);
                        pt = &transformed;
                    }

                    cloud->push_back(pcl::PointXYZ(pt->x, pt->y, pt->z));
                }
            }
        }
    }
}

/// <summary>
/// 
/// </summary>
template<class TCloudPtr>
void PointCloud::SetCloudAttributesFromInstance(OwlInstance inst, TCloudPtr cloud)
{
    assert(cloud); if (!cloud) return;

    cloud->width = GetDataProperyValue<int_t>(inst, PROP_CLOUD_WIDTH, cloud->size());
    cloud->height = GetDataProperyValue<int_t>(inst, PROP_CLOUD_HEIGHT, 1);

    cloud->is_dense = GetDataProperyValue(inst, PROP_CLOUD_ISDENSE, false);
    cloud->header.seq = GetDataProperyValue<int_t>(inst, PROP_CLOUD_HDR_SEQ, 0);
    cloud->header.stamp = GetDataProperyValue<int_t>(inst, PROP_CLOUD_HDR_STAMP, 0);
    cloud->header.frame_id = GetDataProperyValue(inst, PROP_CLOUD_HDR_FRAME, "");

    if (cloud->width * cloud->height != cloud->size()){
        //it can happen when points from nested objects are added after PCD file or when cloud is filtered
        if (cloud->height == 1) {
            cloud->width = cloud->size();
        }
        else {
            //igor.sokolov: not sure how to handle it
            cloud->height = 1;
            cloud->width = cloud->size();
        }
    }
}



/// <summary>
/// 
/// </summary>
void PointCloud::ReadCloudFileAndSaveOnInstance(OwlInstance inst, const std::string& filePath, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    assert(cloud); if (!cloud) return;

    std::string ext;
    if (filePath.length() > 3)
        ext = filePath.substr(filePath.length() - 4);

    if (0 == _stricmp(ext.c_str(), ".pcd")){
        pcl::io::loadPCDFile<pcl::PointXYZ>(filePath, *cloud);
    }
    else if (0 == _stricmp(ext.c_str(), ".ply")) {
        pcl::PolygonMesh mesh;
        if (pcl::io::loadPLYFile(filePath, mesh)!=-1) {
            pcl::fromPCLPointCloud2(mesh.cloud, *cloud);
        }
    }

    if (!cloud->size()) {
        printf("Can't read %s \n", filePath.c_str());
    }

    SaveOnInstance(inst, cloud);
}

/// <summary>
/// 
/// </summary>
pcl::PointCloud<pcl::PointNormal>::Ptr PointCloud::PrepateCloudToBuildMesh(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    pcl::PointCloud<pcl::PointNormal>::Ptr cloudWithNormal = NULL;
    if (cloud && cloud->size() > 0) {

        cloud = PathThrough(inst, cloud);

        cloud = VoxelGridDowmsampling(inst, cloud);

        cloud = StatisticalOutlierRemoval(inst, cloud);

        bool enableMLS = GetDataProperyValue(inst, PROP_ENABLE_MLS, false);
        if (enableMLS) {
            cloudWithNormal = SmoothMLS(inst, cloud);
        }
        else {
            cloudWithNormal = EstimateNormals(inst, cloud);
        }
    }
    return cloudWithNormal;
}

/// <summary>
/// 
/// </summary>
void PointCloud::Dump(pcl::PointCloud<pcl::PointNormal>::Ptr cloud)
{
    printf("------------------------------------\n");
    printf("Dump cloud\n");
    if (cloud) {
        printf ("SEQ: %d, STAMP: %d, FRAME_ID: %s, is_dense: %d\n", (int)cloud->header.seq, (int)cloud->header.stamp, cloud->header.frame_id.c_str(), (int)cloud->is_dense);
        printf("Width %d, Height: %d, Size: %d\n", (int)cloud->width, (int)cloud->height, (int)cloud->size());

        for (int_t h = 0; h < cloud->height; h++) {
            if (h < 7) {
                for (int_t w = 0; w < cloud->width; w++) {
                    if (w < 7) {
                        int_t i = h * cloud->width + w;
                        auto& pt = cloud->at(i);
                        printf("    [%d,%d] : P(%g,%g,%g) N(%g,%g,%g)\n", (int)h, (int)w, pt.x, pt.y, pt.z, pt.normal_x, pt.normal_y, pt.normal_z);
                    }
                    else {
                        printf("   .....\n");
                        break;
                    }
                }
            }
            else {
                printf(".........\n");
                break;
            }
        }

    }
    printf("------------------------------------\n");
}

/// <summary>
/// 
/// </summary>
void PointCloud::SaveOnInstance(OwlInstance inst, pcl::PointCloud <pcl::PointXYZ>::Ptr cloud)
{
    int_t height = cloud->height;
    int_t width = cloud->width;
    bool is_dense = cloud->is_dense;
    int_t hdrSeq = cloud->header.seq;
    int_t hdrStm = cloud->header.stamp;
    const char* hdrFrm = cloud->header.frame_id.c_str();

    std::vector<double> coords;
    if (cloud->size()) {
        coords.resize(cloud->size() * 3);
        for (int_t i = 0; i < cloud->size(); i++) {
            auto& pt = cloud->at(i);

            coords[i * 3 + 0] = pt.x;
            coords[i * 3 + 1] = pt.y;
            coords[i * 3 + 2] = pt.z;
        }
    }

    auto model = GetModel(inst);
    auto propPoints = GetPropertyByName(model, PROP_CLOUD_POINTS);
    auto propWidth = GetPropertyByName(model, PROP_CLOUD_WIDTH);
    auto propHeight = GetPropertyByName(model, PROP_CLOUD_HEIGHT);
    auto propIsDens = GetPropertyByName(model, PROP_CLOUD_ISDENSE);
    auto propHdrSeq = GetPropertyByName(model, PROP_CLOUD_HDR_SEQ);
    auto propHdrStm = GetPropertyByName(model, PROP_CLOUD_HDR_STAMP);
    auto propHdrFrm = GetPropertyByName(model, PROP_CLOUD_HDR_FRAME);

    SetDataTypeProperty(inst, propPoints, coords.data(), coords.size());
    SetDataTypeProperty(inst, propWidth, &width, 1);
    SetDataTypeProperty(inst, propHeight, &height, 1);
    SetDataTypeProperty(inst, propIsDens, &is_dense, 1);
    SetDataTypeProperty(inst, propHdrSeq, &hdrSeq, 1);
    SetDataTypeProperty(inst, propHdrStm, &hdrStm, 1);
    SetDataTypeProperty(inst, propHdrFrm, &hdrFrm, 1);
}

/// <summary>
/// 
/// </summary>
void PointCloud::GetPointsSavedOnInstance(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    assert(cloud); if (!cloud) return;

    //load points
    double* coords = NULL;
    int_t ncoords = GetDataProperyValue(inst, PROP_CLOUD_POINTS, (void**)&coords);
    if (ncoords) {
        auto oldSize = cloud->size();
        cloud->resize(oldSize + ncoords / 3);

        for (int_t i = 0; i < ncoords / 3; i++) {

            auto& pt = cloud->at(oldSize + i);

            pt.x = coords[i * 3 + 0];
            pt.y = coords[i * 3 + 1];
            pt.z = coords[i * 3 + 2];
        }
    }
}


/// <summary>
/// 
/// </summary>
std::string PointCloud::GetFilePathIfNeedToRead(OwlInstance inst)
{
    OwlModel model = GetModel(inst);
    if (!model)
        return "";

    //
    // get file path
    //
    const char* filePath = GetDataProperyValue<const char*>(inst, PROP_INPUT_FILE, "");
    if (!filePath || !*filePath) 
        return "";

    if (*filePath == '"')
        filePath++;
    
    std::string dataFilePath (filePath);
  
    if (dataFilePath.back() == '"') {
        dataFilePath.pop_back();
    }

    //
    // loading mark
    //
    struct stat fstat;
    if (0!=stat(dataFilePath.c_str(), &fstat))
    {
        //file does not exist or not accessible, do not read it
        return "";
    }

    char timestamp[80];
    sprintf_s(timestamp, "%lld,", fstat.st_mtime);

    std::string loadingMark(timestamp);
    loadingMark.append(dataFilePath);

    RdfProperty propMark = 0;
    const char* existingMark = GetDataProperyValue(inst, PROP_INPUT_FILE_LOADED, "", &propMark);

    if (0 == strcmp(existingMark, loadingMark.c_str())) {
        return "";
    }
    
    existingMark = loadingMark.c_str();
    SetDataTypeProperty(inst, propMark, &existingMark, 1);

    return dataFilePath;
}

/// <summary>
/// 
/// </summary>
pcl::PointCloud<pcl::PointNormal>::Ptr PointCloud::SmoothMLS(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_smoothed(new pcl::PointCloud<pcl::PointNormal>());

    pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointNormal> mls;
    mls.setInputCloud(cloud);
    mls.setSearchRadius(0.05); 
    mls.setPolynomialOrder(2);
    mls.setComputeNormals(true);
    mls.setUpsamplingMethod(pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointNormal>::NONE);

    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
    mls.setSearchMethod(tree);

    mls.process(*cloud_smoothed);

    return cloud_smoothed;
}

/// <summary>
/// 
/// </summary>
pcl::PointCloud<pcl::PointNormal>::Ptr PointCloud::EstimateNormals(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    // Normals
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
    pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>());
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
    tree->setInputCloud(cloud);
    n.setInputCloud(cloud);
    n.setSearchMethod(tree);
    n.setKSearch(GetDataProperyValue<int_t>(inst, PARAM_NORMALS_NEIGHBORS, 20)); //
    n.compute(*normals);

    // join points with normals
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>());

    pcl::concatenateFields(*cloud, *normals, *cloud_with_normals);

    return cloud_with_normals;
}

/// <summary>
/// 
/// </summary>
bool PointCloud::AddPointCloudProp(OwlClass cls)
{
    auto model = GetModel(cls);
    auto relcls = NULL; //range restriction will affect all concepts! GetClassByName(model, CLASS_NAME);
    AddClassProperty(cls, PROP_POINT_CLOUD, OBJECTPROPERTY_TYPE, true, relcls);
    return true;
}

extern int_t GetObjectPropertyValue(OwlInstance inst, const char* name, OwlInstance** objects, RdfProperty* pprop)
{
    if (!inst) {
        return 0;
    }

    auto model = GetModel(inst);
    assert(model); if (!model) return 0;

    auto prop = GetPropertyByName(model, name);
    assert(prop); if (!prop) return 0;

    if (pprop) {
        *pprop = prop;
    }

    int_t card = 0;
    GetObjectProperty(inst, prop, objects, &card);

    return card;
}

/// <summary>
/// 
/// </summary>
OwlInstance PointCloud::GetPointCloudInstance(OwlInstance instHost)
{
    OwlInstance* inst = NULL;
    if (GetObjectPropertyValue(instHost, PROP_POINT_CLOUD, &inst)) {
        return inst[0];
    }
    else {
        return NULL;
    }
}


/// <summary>
/// 
/// </summary>
extern bool AddClassProperty(OwlClass cls, const char* name, RdfPropertyType type, int_t minCard, OwlClass relatedClass, int_t maxCard)
{
    auto model = GetModel(cls);

    RdfProperty prop = GetPropertyByName(model, name);
    if (prop) {
        REQUIRED(GetPropertyType(prop) == type, "Property type mismatch");
    }
    else {
        prop = CreateProperty(model, type, name);
        REQUIRED(prop, "Failed to create of property");
    }

    SetClassPropertyCardinalityRestriction(cls, prop, minCard, maxCard);

    if (type == OBJECTPROPERTY_TYPE && relatedClass) {
        SetPropertyRangeRestriction(prop, relatedClass, TRUE);
    }

    return true;
}

/// <summary>
/// 
/// </summary>
template<class TPoint>
static void ExpandRange(VECTOR3* startVector, VECTOR3* endVector, const TPoint& pt)
{
    startVector->x = std::min(startVector->x, (double)pt.x);
    startVector->y = std::min(startVector->y, (double)pt.y);
    startVector->z = std::min(startVector->z, (double)pt.z);
    endVector->x = std::max(endVector->x, (double)pt.x);
    endVector->y = std::max(endVector->y, (double)pt.y);
    endVector->z = std::max(endVector->z, (double)pt.z);
}

/// <summary>
/// 
/// </summary>
bool PointCloudGeometry::GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*)
{
    if (transformationMatrix)
        MatrixIdentity(transformationMatrix);

    startVector->x = startVector->y = startVector->z = DBL_MAX;
    endVector->x = endVector->y = endVector->z = -DBL_MAX;

    auto geom = rdfgeom_GetBRep(inst);
    if (geom) {
        auto npt = rdfgeom_GetNumOfPoints(geom);
        auto rpt = rdfgeom_GetPoints(geom);

        for (int_t i = 0; i < npt && rpt; i++) {
            ExpandRange(startVector, endVector, rpt[i]);
        }
    }
    else {
        auto cloud = PointCloud::GetDataFilePointCloud(inst);
        if (cloud) {
            for (auto& pt : *cloud) {
                ExpandRange(startVector, endVector, pt);
            }
        }

        const char* propNames[] = { PROP_INPUT_OBJECT, PROP_INPUT_OBJECTS };
        for (auto name : propNames) {
            OwlInstance* nested = NULL;
            int_t card = GetObjectPropertyValue(inst, name, &nested);
            for (int_t i = 0; i < card; i++) {
                VECTOR3 box[2];
                MATRIX T;
                if (::GetBoundingBox(nested[i], (double*)&T, (double*)box, (double*)(box + 1))) {
                    VECTOR3 pt;
                    for (int j = 0; j < 1; j++) {
                        TransformPoint(pt, box[j], T);
                        ExpandRange(startVector, endVector, pt);
                    }
                }
            }
        }
    }

    return startVector->x <= endVector->x;
}

/// <summary>
/// 
/// </summary>
void PointCloudGeometry::CreateShell(OwlInstance inst, void*)
{
    auto shell = rdfgeom_GetBRep(inst);
    if (!shell)
        return;

    auto cloud0 = PointCloud::CombinePointCloud(inst);
    auto cloud = PointCloud::PrepateCloudToBuildMesh(inst, cloud0);

    if (!cloud || !cloud->size()) {
        return;
    }

    rdfgeom_AllocatePoints(inst, shell, cloud->size(), true, false);
    auto points = rdfgeom_GetPoints(shell);
    auto normals = rdfgeom_GetNormals(shell);

    auto cfaceP = rdfgeom_GetConceptualFaces(shell);
    rdfgeom_cface_Create(inst, cfaceP);

    STRUCT_VERTEX** vertexP = rdfgeom_cface_GetVertices(*cfaceP);

    for (size_t npt = 0; npt < cloud->size(); npt++) {
        auto& pt = cloud->at(npt);

        points[npt].x = pt.x;
        points[npt].y = pt.y;
        points[npt].z = pt.z;

        normals[npt].x = pt.normal_x;
        normals[npt].y = pt.normal_y;
        normals[npt].z = pt.normal_z;

        rdfgeom_vertex_Create(inst, vertexP, npt, npt == cloud->size() - 1);

        vertexP = rdfgeom_vertex_GetNext(*vertexP);
    }
}

pcl::PointCloud<pcl::PointNormal>::Ptr PointCloud::GetPointsWithNormals(OwlInstance instWithPoints)
{
    auto geom = rdfgeom_GetBRep(instWithPoints);
    if (!geom) {
        assert(false);
        return NULL;
    }

    auto npt = rdfgeom_GetNumOfPoints(geom);
    if (!npt) {
        assert(false);
        return NULL;
    }

    auto points = rdfgeom_GetPoints(geom);
    auto normals = rdfgeom_GetNormals(geom);
    if (!points || !normals) {
        assert(false);
        return NULL;
    }

    pcl::PointCloud<pcl::PointNormal>::Ptr cloud(new pcl::PointCloud <pcl::PointNormal>());

    cloud->resize(npt);
    for (int_t i = 0; i < npt; i++) {

        auto& pt = cloud->at(i);

        pt.x = points[i].x;
        pt.y = points[i].y;
        pt.z = points[i].z;

        pt.normal_x = normals[i].x;
        pt.normal_y = normals[i].y;
        pt.normal_z = normals[i].z;
    }

    SetCloudAttributesFromInstance(instWithPoints, cloud);

    return cloud;
}

pcl::PointCloud<pcl::PointXYZ>::Ptr PointCloud::StatisticalOutlierRemoval(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    if (cloud && cloud->size()) {

        auto K = GetDataProperyValue<int_t>(inst, PARAM_OUTLIER_NEIGHBORS, 0);

        if (K > 0) {
            pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

            pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
            sor.setInputCloud(cloud);
            sor.setMeanK(50);
            sor.setStddevMulThresh(1.0);
            sor.filter(*cloud_filtered);

            if (cloud_filtered && cloud_filtered->size())
                return cloud_filtered;
        }
    }

    return cloud;
}

pcl::PointCloud<pcl::PointXYZ>::Ptr PointCloud::VoxelGridDowmsampling(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    // https://pcl.readthedocs.io/projects/tutorials/en/master/voxel_grid.html#voxelgrid

    if (cloud && cloud->size()) {
        double leafSize = GetDataProperyValue<double>(inst, PARAM_VOXEG_GRID_SIZE, 0);
        if (leafSize > 1e-7) {
            pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
            
            pcl::VoxelGrid<pcl::PointXYZ> sor;
            sor.setInputCloud(cloud);
            sor.setLeafSize(leafSize, leafSize, leafSize);
            sor.filter(*cloud_filtered);

            if (cloud_filtered && cloud_filtered->size())
                return cloud_filtered;
        }
    }

    return cloud;

}

pcl::PointCloud<pcl::PointXYZ>::Ptr PointCloud::PathThrough(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    cloud = PathThrough(inst, cloud, PARAM_RANGE_X, "x");
    cloud = PathThrough(inst, cloud, PARAM_RANGE_Y, "y");
    cloud = PathThrough(inst, cloud, PARAM_RANGE_Z, "z");
    return cloud;
}

pcl::PointCloud<pcl::PointXYZ>::Ptr PointCloud::PathThrough(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, const char* param, const char* filter)
{
    if (cloud && cloud->size()) {
        double range[2];
        auto has = GetDataProperyValue(inst, param, range, 2);
        if (has == 2 && range[1] > range[0]) {
            pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

            // Create the filtering object
            pcl::PassThrough<pcl::PointXYZ> pass;
            
            pass.setInputCloud(cloud);
            pass.setFilterFieldName(filter);
            pass.setFilterLimits(range[0], range[1]);
            //pass.setNegative (true);

            pass.filter(*cloud_filtered);

            //if (cloud_filtered && cloud_filtered->size())
                return cloud_filtered;
        }
    }

    return cloud;

}
