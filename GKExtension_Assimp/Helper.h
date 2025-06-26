#pragma once


//#ifdef _DEBUG
//#define LOG_ERROR printf
//#else
#define LOG_ERROR printf
//#endif

#define ON_ERROR(condition, message) if (!(condition)){ LOG_ERROR(message); return false;}

#define CLS_GEOMETRICITEM "GeometricItem"
#define CLS_POINTSET      "PointSet"

#define GET_CLASS(cls, name)                                            \
    OwlClass cls = GetClassByName(model, name);                         \
    ON_ERROR(clsGeometricItem, "Failed GetClassByName (" name ")\n");   \


struct Helper
{
    static bool AddClassProperty(OwlClass cls, const char* name, RdfPropertyType type, int_t minCard = 0, OwlClass relatedClass = NULL, int_t maxCard = 1);

    template<typename TVal>
    static TVal GetDataProperyValue(OwlInstance inst, const char* name, TVal defval, RdfProperty* prop = NULL);
        
    template<typename TVal>
    static int_t GetDataProperyValue(OwlInstance inst, const char* name, TVal* arrValues, int_t arrSize, RdfProperty* pprop = NULL);

    static int_t GetDataProperyValue(OwlInstance inst, const char* name, void** arrValues, RdfProperty* pprop = NULL);

    static int_t GetObjectPropertyValue(OwlInstance inst, const char* name, OwlInstance** objects, RdfProperty* pprop = NULL);

    static OwlInstance GetObjectPropertyValue(OwlInstance inst, const char* name);

    template<class TPoint>
    static void ExpandRange(VECTOR3* startVector, VECTOR3* endVector, const TPoint& pt);

    static void	MatrixIdentity(MATRIX* pOut);
};

/// <summary>
/// 
/// </summary>
template<class TPoint>
void Helper::ExpandRange(VECTOR3* startVector, VECTOR3* endVector, const TPoint& pt)
{
    startVector->x = min(startVector->x, (double)pt.x);
    startVector->y = min(startVector->y, (double)pt.y);
    startVector->z = min(startVector->z, (double)pt.z);
    endVector->x = max(endVector->x, (double)pt.x);
    endVector->y = max(endVector->y, (double)pt.y);
    endVector->z = max(endVector->z, (double)pt.z);
}

/// <summary>
/// 
/// </summary>
template<typename TVal>
TVal Helper::GetDataProperyValue(OwlInstance inst, const char* name, TVal defval, RdfProperty* prop)
{
    GetDataProperyValue<TVal>(inst, name, &defval, 1, prop);
    return defval;
}

/// <summary>
/// 
/// </summary>
template<typename TVal>
int_t Helper::GetDataProperyValue(OwlInstance inst, const char* name, TVal* arrValues, int_t arrSize, RdfProperty* pprop)
{
    TVal* val = NULL;
    auto card = GetDataProperyValue(inst, name, (void**)&val, pprop);

    auto N = min(arrSize, card);

    for (int_t i = 0; i < N; i++) {
        arrValues[i] = val[i];
    }

    return card;
}

