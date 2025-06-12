#include "pch.h"
#include "Helper.h"

/// <summary>
/// 
/// </summary>
bool Helper::AddClassProperty(OwlClass cls, const char* name, RdfPropertyType type, int_t minCard, OwlClass relatedClass, int_t maxCard)
{
    auto model = GetModel(cls);

    RdfProperty prop = GetPropertyByName(model, name);
    if (prop) {
        ON_ERROR(GetPropertyType(prop) == type, "Property type mismatch");
    }
    else {
        prop = CreateProperty(model, type, name);
        ON_ERROR(prop, "Faile to create of property");
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
int_t Helper::GetDataProperyValue(OwlInstance inst, const char* name, void** arrValues, RdfProperty* pprop)
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
void	Helper::MatrixIdentity(MATRIX* pOut)
{
    pOut->_12 = pOut->_13 =
        pOut->_21 = pOut->_23 =
        pOut->_31 = pOut->_32 =
        pOut->_41 = pOut->_42 = pOut->_43 = 0.;

    pOut->_11 = pOut->_22 = pOut->_33 = 1.;
}