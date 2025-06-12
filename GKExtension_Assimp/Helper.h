#pragma once


//#ifdef _DEBUG
//#define LOG_ERROR printf
//#else
#define LOG_ERROR printf
//#endif

#define ON_ERROR(condition, message) if (!(condition)){ LOG_ERROR(message); return false;}

struct Helper
{
    static bool AddClassProperty(OwlClass cls, const char* name, RdfPropertyType type, int_t minCard = 0, OwlClass relatedClass = NULL, int_t maxCard = 1);

    template<typename TVal>
    static TVal GetDataProperyValue(OwlInstance inst, const char* name, TVal defval, RdfProperty* prop = NULL);
        
    template<typename TVal>
    static int_t GetDataProperyValue(OwlInstance inst, const char* name, TVal* arrValues, int_t arrSize, RdfProperty* pprop = NULL);

    static int_t GetDataProperyValue(OwlInstance inst, const char* name, void** arrValues, RdfProperty* pprop = NULL);
};


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

