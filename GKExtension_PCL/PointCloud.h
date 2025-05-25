#pragma once

/// <summary>
/// 
/// </summary>
class PointCloudGeometry : public IEngineExtGeometry
{
public:
    virtual bool GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix) override;
    virtual void CreateShell(OwlInstance inst, SHELL* shell, IEngineMemory* memory) override;
};


/// <summary>
/// 
/// </summary>
struct PointCloud
{
public:
    static bool CreateClass(OwlModel model);

    static bool AddPointCloudProp(OwlClass cls);
    static OwlInstance GetPointCloudInstance(OwlInstance instHost);

    static pcl::PointCloud<pcl::PointXYZ>::Ptr GetPointCloud(OwlInstance instWithPoints);
    static pcl::PointCloud<pcl::PointNormal>::Ptr GetCloudWithNormals(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

    static bool GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix);
private:
    static std::string GetFilePathIfNeedToRead(OwlInstance instReleating);
    static void ReadCloudFileAndSaveOnInstance(OwlInstance inst, const std::string& filePath, pcl::PointCloud <pcl::PointXYZ>::Ptr cloud);
    static void SaveOnInstance(OwlInstance inst, pcl::PointCloud <pcl::PointXYZ>::Ptr cloud);
    static void GetPointsSavedOnInstance(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
    
    static pcl::PointCloud<pcl::PointNormal>::Ptr EstimateNormals(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
    static pcl::PointCloud<pcl::PointNormal>::Ptr SmoothMLS(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

    static void Dump(pcl::PointCloud<pcl::PointNormal>::Ptr cloud);

    static void AddPointsFromNestedObject(OwlInstance instNested, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

    static void SetCloudAttributes(OwlInstance inst, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

};

/// <summary>
/// 
/// </summary>
extern bool AddClassProperty(OwlClass cls, const char* name, RdfPropertyType type, int_t minCard = 0, OwlClass relatedClass = NULL, int_t maxCard = 1);

/// <summary>
/// 
/// </summary>
static int_t GetDataProperyValue(OwlInstance inst, const char* name, void** arrValues, RdfProperty* pprop = NULL)
{
    if (!inst) {
        return 0;
    }

    auto model = GetModel(inst);
    assert(model); if (!model) return 0;

    auto prop = GetPropertyByName(model, name);

    if (pprop) {
        *pprop = prop;
    }

    assert(prop); if (!prop) return 0;

    int_t card = 0;
    auto err = GetDatatypeProperty(inst, prop, (void**)arrValues, &card);
    if (err)
        card = 0;
    
    return card;
}

/// <summary>
/// 
/// </summary>
template<typename TVal>
static int_t GetDataProperyValue(OwlInstance inst, const char* name, TVal* arrValues, int_t arrSize, RdfProperty* pprop = NULL)
{
    TVal* val = NULL;
    auto card = GetDataProperyValue(inst, name, (void**)&val, pprop);

    auto N = std::min(arrSize, card);

    for (int_t i = 0; i < N; i++) {
        arrValues[i] = val[i];
    }

    return card;
}

/// <summary>
/// 
/// </summary>
template<typename TVal>
static TVal GetDataProperyValue(OwlInstance inst, const char* name, TVal defval, RdfProperty* prop = NULL)
{
    GetDataProperyValue<TVal>(inst, name, &defval, 1, prop);
    return defval;
}

/// <summary>
/// 
/// </summary>
extern int_t GetObjectPropertyValue(OwlInstance inst, const char* name, OwlInstance** objects, RdfProperty* pprop = NULL);
