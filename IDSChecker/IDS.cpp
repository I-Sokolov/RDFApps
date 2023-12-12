//
//  Author:  Peter Bonsma
//  $Date: 1999-12-31 23:59:59 +0000 (Wed, 31 Jan 1999) $
//  $Revision: 3999 $
//  Project: Engine Library
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@rdf.bg
//

#include "pch.h"

#include "IDS.h"
using namespace RDF::IDS;


/// <summary>
/// 
/// </summary>
#define GET_ATTR(name)                      \
    for (auto attr : elem.attributes()) {   \
    if (attr) {                             \
        auto attrName = attr->getName();    \
        if (attrName == #name) {            \
            m_##name = attr->getValue();    \
        }                                   

#define NEXT_ATTR(name)                     \
            else if (attrName == #name) {   \
            m_##name = attr->getValue();    \
        }

#define END_ATTR        \
            else { ctx.LogMsg(MsgLevel::Warning, "Unknown attribute '%s'", attrName.c_str()); } } }


/// <summary>
/// 
/// </summary>
#define GET_CHILD(name)                     \
    for (auto child : elem.children()) {    \
        if (child) {                        \
            auto&  tag= child->getName();   \
            if (tag == #name) {             \
                Read_##name(*child, ctx);   \
            }

#define NEXT_CHILD(name)                    \
            else if (tag == #name) {        \
                Read_##name(*child, ctx);   \
            }

#define GET_CHILD_MEMBER(name)              \
    for (auto child : elem.children()) {    \
        if (child) {                        \
            auto&  tag= child->getName();   \
            if (tag == #name) {             \
                m_##name.Read(*child, ctx); \
            }

#define NEXT_CHILD_MEMBER(name)             \
            else if (tag == #name) {        \
                m_##name.Read(*child, ctx); \
            }

#define END_CHILDREN \
            else { ctx.LogMsg(MsgLevel::Warning, "Unknown child element <%s>", tag.c_str()); } } }


/// <summary>
/// 
/// </summary>
#define ENTITY(EntityName)                      \
    private:                                    \
        SdaiEntity m_entity##EntityName = 0;    \
    public:                                     \
        SdaiEntity _##EntityName()              \
        {                                       \
            if (!m_entity##EntityName) {        \
                m_entity##EntityName = sdaiGetEntity(model, #EntityName);  \
                assert(m_entity##EntityName);   \
            }                                   \
            return m_entity##EntityName;        \
        }                                       

/// <summary>
/// 
/// </summary>
#define ATTR(Ent,Attr,EntAttr)                                          \
    private:                                                            \
        SdaiAttr    m_attribute_##EntAttr = 0;                          \
    public:                                                             \
        SdaiAttr    _##EntAttr ()                                       \
        {                                                               \
            if (!m_attribute_##EntAttr){                                \
                auto ent = _##Ent();                                    \
                m_attribute_##EntAttr = sdaiGetAttrDefinition(ent, #Attr);\
                assert(m_attribute_##EntAttr);                          \
            }                                                           \
            return m_attribute_##EntAttr;                               \
        }

/// <summary>
/// 
/// </summary>
static int StrICmp(const char* s1, const char* s2)
{
    return _stricmp(s1, s2);
}

/// <summary>
/// 
/// </summary>
static void ToUpper(std::string& str)
{
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = (char)toupper (str[i]);
    }
}


/// <summary>
/// 
/// </summary>
enum class RqType { Required, Optional, Prohibited };
static RqType GetRqType(MultiTypeValueCache& minOccur, MultiTypeValueCache& maxOccur, RqType def)
{
    const char* min__ = nullptr;
    minOccur.Get(&min__);

    if (!min__ || !*min__) {
        return def;
    }

    SdaiInteger min_ = 0;
    minOccur.Get(&min_);
    if (min_ > 0) {
        return RqType::Required;
    }

    SdaiInteger max_ = MAXINT;
    const char* max__ = nullptr;
    maxOccur.Get(&max__);
    if (*max__) {
        maxOccur.Get(&max_);
    }

    if (max_ == 0)
        return RqType::Prohibited;

    return RqType::Optional;
}

#include "Context.h"

/// <summary>
/// 
/// </summary>
Context::IfcVersion Context::GetIfcVersion(const char** pstr)
{
    if (m_ifcVersion == IfcVersion::NotItitialized) {
        assert(model);
        
        const char* schemaName = nullptr;
        GetSPFFHeaderItem(model, 9, 0, sdaiSTRING, &schemaName);
        assert(schemaName);

        m_ifcVersionStr = schemaName;

        if (strstr(schemaName, "IFC4x3")) {
            m_ifcVersion = IfcVersion::Ifc4x3;
        }
        else if (strstr(schemaName, "IFC4")) {
            m_ifcVersion = IfcVersion::Ifc4;
        }
        else if (strstr(schemaName, "IFC2x3")) {
            m_ifcVersion = IfcVersion::Ifc2x3;
        }
        else {
            m_ifcVersion = IfcVersion::Unsupported;
            assert(0);
            LogMsg(MsgLevel::NotImplemented, "Unknown IFC version %s", schemaName);
        }
    }

    if (pstr) {
        *pstr = m_ifcVersionStr.c_str();
    }

    return m_ifcVersion;
}

/// <summary>
/// 
/// </summary>
static SdaiInstance GetTypeObject(SdaiInstance inst, Context& ctx)
{
    SdaiInstance relType = 0;
    if (sdaiGetAttr(inst, ctx._IfcObject_IsTypedBy(), sdaiINSTANCE, &relType)) {
        SdaiInstance type = 0;
        if (sdaiGetAttr(relType, ctx._IfcRelDefinesByType_RelatingType(), sdaiINSTANCE, &type)) {
            return type;
        }
    }
    return NULL;
}

/// <summary>
/// 
/// </summary>
class DefaultConsole : public Console
{
    virtual void out(const char* msg) override { printf("%s", msg); }
};


/// <summary>
/// 
/// </summary>
void Context::LogMsg (MsgLevel type, const char* format, ...)
{
    if (type < msgLevel) {
        return; //>>>>>>>>>>>>>
    }

    auto& log = console;

    //
    log.out("\t<");

    const char* msgType;
    switch (type) {
        case MsgLevel::Info:     msgType = "info"; break;
        case MsgLevel::Warning:  msgType = "Warning"; break;
        default:                 msgType = "ERROR"; break;
    }

    log.out(msgType);

    if (currentInstane > 0) {
        log.out(" stepId='#");
        auto id = internalGetP21Line(currentInstane);
        char str[64];
        sprintf(str, "%lld", id);
        log.out(str);
        log.out("'");
    }

    if (currentSpecification) {
        log.out(" specification='");
        auto name = currentSpecification->DisplayName();
        log.out(name.c_str());
        log.out("'");
    }

    //
    log.out(">\n\t\t");

    char msg[512];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, 511, format, args);
    va_end(args);

    log.out(msg);
    log.out("\n");

    //
    log.out("\t</");
    log.out(msgType);
    log.out(">\n");
}

/// <summary>
/// 
/// </summary>
bool File::Read(const char* idsFilePath)
{
    bool ok = false;

    DefaultConsole con;
    Context ctx(con, MsgLevel::All, false);

    try {
        _xml::_document doc(nullptr);
        doc.load(idsFilePath);

        if (auto root = doc.getRoot()) {
            Read(*root, ctx);
            ok = true;
        }
    }
    catch (exception& ex) {
        ctx.LogMsg(MsgLevel::Error, "Failed read IDS file: '%s', error: %s", idsFilePath, ex.what());
    }

    return ok;
}


/// <summary>
/// 
/// </summary>
void File::Read(_xml::_element& elem , Context& ctx)
{
    GET_CHILD(info)
    NEXT_CHILD(specifications)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
void File::Read_info(_xml::_element& elem, Context& ctx)
{
    GET_CHILD(title)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
void File::Read_specifications(_xml::_element& elem, Context& ctx)
{
    GET_CHILD(specification)
    END_CHILDREN
}


/// <summary>
/// 
/// </summary>
Specification::Specification(_xml::_element& elem, Context& ctx)
    : m_minOccursVal (m_minOccurs)
    , m_maxOccursVal (m_maxOccurs)
{
    GET_ATTR(name)
    NEXT_ATTR(minOccurs)
    NEXT_ATTR(maxOccurs)
    NEXT_ATTR(ifcVersion)
    NEXT_ATTR(identifier)
    NEXT_ATTR(description)
    NEXT_ATTR(instructions)
    END_ATTR

    GET_CHILD_MEMBER(applicability)
    NEXT_CHILD_MEMBER(requirements)
    END_CHILDREN
}


/// <summary>
/// 
/// </summary>
void Requirements::Read(_xml::_element& elem, Context& ctx)
{
    GET_ATTR(description)
    END_ATTR

    Facets::Read (elem, ctx);
}

/// <summary>
/// 
/// </summary>
void Facets::Read(_xml::_element& elem, Context& ctx)
{
    GET_CHILD(entity)
    NEXT_CHILD(partOf)
    NEXT_CHILD(classification)
    NEXT_CHILD(attribute)
    NEXT_CHILD(property)
    NEXT_CHILD(material)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
void Facet::Read(_xml::_element& elem, Context& ctx)
{
    GET_ATTR(minOccurs)
    NEXT_ATTR(maxOccurs)
    NEXT_ATTR(datatype)
    NEXT_ATTR(relation)
    END_ATTR
}

/// <summary>
/// 
/// </summary>
void Facet::ResetCache()
{
    ResetCacheImpl();
}

/// <summary>
/// 
/// </summary>
bool Facet::Match(SdaiInstance inst, Context& ctx)
{
    bool match = MatchImpl(inst, ctx);

    auto rq = GetRqType(m_minOccursVal, m_maxOccursVal, RqType::Required);

    switch (rq) {
        case RqType::Prohibited:
            match = !match;
            break;

        case RqType::Optional:
            ctx.LogMsg(MsgLevel::Info, "Optional requirement%s match", match ? "" : " does not");
            match = true;
            break;
    }

    return match;
}

/// <summary>
/// 
/// </summary>
void FacetEntity::Read(_xml::_element& elem, Context& ctx)
{
    Facet::Read(elem, ctx);

    GET_CHILD_MEMBER(name)
    NEXT_CHILD_MEMBER(predefinedType)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetPartOf::FacetPartOf(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(entity)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetClassification::FacetClassification(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(value)
    NEXT_CHILD_MEMBER(system)
    NEXT_CHILD_MEMBER(URI)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetAttribute::FacetAttribute(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(name)
    NEXT_CHILD_MEMBER(value)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetProperty::FacetProperty(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(propertySet)
    NEXT_CHILD_MEMBER(name)
    NEXT_CHILD_MEMBER(value)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
FacetMaterial::FacetMaterial(_xml::_element& elem, Context& ctx)
    : Facet(elem, ctx)
{
    GET_CHILD_MEMBER(value)
    END_CHILDREN
}


/// <summary>
/// 
/// </summary>
void IdsValue::Read(_xml::_element& elem, Context& ctx)
{
    m_isSet = true;

    GET_CHILD(simpleValue)
    NEXT_CHILD(restriction)
    END_CHILDREN
}

/// <summary>
/// 
/// </summary>
Value::Value(_xml::_element& elem, Context& ctx)
    : m_val (m_value)
{
    GET_ATTR(value)
    NEXT_ATTR(fixed)
    END_ATTR
}

/// <summary>
/// 
/// </summary>
Restriction::Restriction(_xml::_element& elem, Context& ctx)
{
    GET_ATTR(base)
    END_ATTR

    GET_CHILD(enumeration)
    NEXT_CHILD(pattern)
    NEXT_CHILD(minInclusive)
    NEXT_CHILD(maxInclusive)
    NEXT_CHILD(minExclusive)
    NEXT_CHILD(maxExclusive)
    NEXT_CHILD(length)
    NEXT_CHILD(minLength)
    NEXT_CHILD(maxLength)
    END_CHILDREN
}


/// <summary>
/// 
/// </summary>
bool File::Check(const char* ifcFilePath, bool stopAtFirstError, MsgLevel msgLevel, Console* output)
{
    for (auto spec : m_specifications) {
        if (spec) {
            spec->ResetCache();
        }
    }
     
    bool ok = false;

    DefaultConsole con;
    if (!output) {
        output = &con;
    }

    Context ctx(*output, msgLevel, stopAtFirstError);

    ctx.model = sdaiOpenModelBN((SdaiRep)0, ifcFilePath, "");
    if (ctx.model) {       
        ok = CheckInstances(ctx);

        if (ok || !ctx.stopAtFirstError) {
            if (!CheckSpecificationsUsed(ctx)) {
                ok = false;
            }
        }

        sdaiCloseModel(ctx.model);
        ctx.model = 0;
    }
    else {
        ctx.LogMsg(MsgLevel::Error, "Failed to read IFC file '%s'", ifcFilePath);
        ok = false;
    }

    return ok;
}

/// <summary>
/// 
/// </summary>
bool File::CheckInstances(Context& ctx)
{
    bool ok = true;

    auto aggr = xxxxGetAllInstances(ctx.model);
    int i = 0;
    SdaiInstance inst = 0;
    while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &inst)) {

        ctx.currentInstane = inst;

        for (auto spec : m_specifications) {
            if (spec) {

                ctx.currentSpecification = spec;

                if (!spec->Check(inst, ctx)) {
                    ok = false;
                }

                ctx.currentSpecification = nullptr;
                if (!ok && ctx.stopAtFirstError) {
                    return false; //>>>>>>>>>>>>>>>>>>>>>
                }
            }
        }
        ctx.currentInstane = 0;
    }

    return ok;
}

/// <summary>
/// 
/// </summary>
bool File::CheckSpecificationsUsed(Context& ctx)
{
    bool ok = true;

    for (auto spec : m_specifications) {
        if (spec) {

            ctx.currentSpecification = spec;

            if (!spec->CheckUsed(ctx)) {
                ok = false;
            }

            ctx.currentSpecification = nullptr;
            if (!ok && ctx.stopAtFirstError) {
                return false; //>>>>>>>>>>>>>>>>>>>>>
            }
        }
    }

    return ok;
}

/// <summary>
/// 
/// </summary>
std::string Specification::DisplayName()
{
    std::string ret;

    if (!m_identifier.empty()) {
        ret = m_identifier;
    }

    if (!m_name.empty()) {
        if (ret.empty()) {
            ret = m_name;
        }
        else {
            ret += "(" + m_name + ")";
        }
    }

    if (ret.empty()) {
        ret = m_description;
    }

    return ret;
}


/// <summary>
/// 
/// </summary>
void Specification::ResetCache() 
{ 
    m_nOccurs = 0;
    m_ifcVersionChecked = false;

    m_applicability.ResetCache();
    m_requirements.ResetCache();
};

/// <summary>
/// 
/// </summary>
bool Specification::Check(SdaiInstance inst, Context& ctx)
{
    bool ok = true;

    if (SuitableIfcVersion(ctx)) {
        if (m_applicability.Match(inst, ctx)) {
            
            auto rq = GetRqType(m_minOccursVal, m_maxOccursVal, RqType::Optional);

            ok = m_requirements.Match(inst, ctx);

            if (rq == RqType::Prohibited) {
                ok = !ok;
            }

            if (ok) {
                ctx.LogMsg(MsgLevel::Info, "Checked ok");
                m_nOccurs++;
            }
            else {
                ctx.LogMsg(MsgLevel::Error, "Instance does not match specification");
            }
        }
    }

    return ok;
}


/// <summary>
/// 
/// </summary>
bool Specification::CheckUsed(Context& ctx)
{
    bool ok = true;

    if (SuitableIfcVersion(ctx)) {
        
        auto rq = GetRqType(m_minOccursVal, m_maxOccursVal, RqType::Optional);
        
        switch (rq) {
            case RqType::Required:
                if (m_nOccurs == 0) {
                    ok = false;
                    ctx.LogMsg(MsgLevel::Error, "Required specification never match");
                }
                break;
        }
    }

    ctx.LogMsg(MsgLevel::Info, "Specification was applied to %d instances", m_nOccurs);

    return ok;
}


/// <summary>
/// Function to split a string by whitespaces 
/// </summary>
static void splitString(const std::string& input, std::set<std::string>& result)
{
    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
        result.insert(token);
    }
}

/// <summary>
/// 
/// </summary>
bool Specification::SuitableIfcVersion(Context& ctx)
{
    //Each Specification may specify the IFC schema(s) that it applies to. This is purely for information purposes and a user may choose to audit the model with the specification and get a pass or fail result

    if (!m_ifcVersionChecked) {
        m_ifcVersionChecked = true;
        
        if (!m_ifcVersion.empty()) {

            std::set<std::string> all;
            splitString(m_ifcVersion, all);

            bool match = false;

            const char* ifcVersion = nullptr;
            switch (ctx.GetIfcVersion(&ifcVersion)) {
                case Context::IfcVersion::Ifc2x3:
                    match = all.find("IFC2x3") != all.end();
                    break;
                case Context::IfcVersion::Ifc4:
                    match = all.find("IFC4") != all.end();
                    break;
                case Context::IfcVersion::Ifc4x3:
                    match = all.find("IFC4x3") != all.end();
                    break;
                default:
                    assert(0);
                    ctx.LogMsg(MsgLevel::NotImplemented, "Model has unknown IFC version: %s", ifcVersion);
            }

            if (!match) {
                ctx.LogMsg(MsgLevel::Warning, "Specification is intended for %s but model has version %s", m_ifcVersion.c_str(), ifcVersion);
            }
        }
    }

    return true;
}


/// <summary>
/// 
/// </summary>
bool Facets::Match(SdaiInstance inst, Context& ctx)
{
    for (auto facet : m_facets) {
        if (!facet->Match(inst, ctx)) {
            return false;
        }
    }
    return true;
}

/// <summary>
/// 
/// </summary>
void FacetEntity::ResetCacheImpl()
{
    m_sdaiEntity = 0;
}

/// <summary>
/// 
/// </summary>
bool FacetEntity::MatchImpl(SdaiInstance inst, Context& ctx)
{
    // check entity name
    //
    bool entityNameMatch = false;

    auto instType = sdaiGetInstanceType(inst);

    //doc says: The IFC Class must match exactly
    if (auto name = m_name.GetSimpleValue()) {
        if (!m_sdaiEntity) {
            //entities_must_be_specified_as_uppercase_strings
            std::string uppercase = name;
            ToUpper(uppercase);
            if (strcmp(name, uppercase.c_str())) {
                m_sdaiEntity = -1;
            }
            else {
                m_sdaiEntity = sdaiGetEntity(ctx.model, name);
                if (!m_sdaiEntity) {
                    m_sdaiEntity = -1;
                }
            }
        }

        entityNameMatch = (instType == m_sdaiEntity);
    }
    else {
        std::string instTypeName = engiGetEntityName(instType, sdaiSTRING);
        ToUpper (instTypeName);
        entityNameMatch = m_name.Match(instTypeName.c_str(), false, ctx);
    }


    if (!entityNameMatch) {
        return false; //>>>>>>>>>>>>>>>>>>>>>>>
    }
    
    //
    // check predefined type
    //
    if (m_predefinedType.IsSet()) {
        return MatchPredefinedType(inst, ctx);
    }

    //
    return true;
}

/// <summary>
/// 
/// </summary>
bool FacetEntity::MatchPredefinedType(SdaiInstance inst, Context& ctx)
{
    const char* predType = nullptr;
    sdaiGetAttrBN(inst, "PredefinedType", sdaiENUM, &predType);

    if (predType){
        if (strcmp(predType, "USERDEFINED")) {
            return m_predefinedType.Match(predType, false, ctx); //>>>>>>>>>>
        }
        else {
            const wchar_t* objType = nullptr;
            sdaiGetAttrBN(inst, "ObjectType", sdaiUNICODE, &objType);
            if (!objType) {
                sdaiGetAttrBN(inst, "ElementType", sdaiUNICODE, &objType);
            }
            if (!objType) {
                sdaiGetAttrBN(inst, "ProcessType", sdaiUNICODE, &objType);
            }
            return m_predefinedType.Match(objType, false, ctx); //>>>>>>>>>
        }
    }
    else if (auto type = GetTypeObject (inst, ctx)){
        return MatchPredefinedType(type, ctx); //>>>>>>>>>>
    }

    return false;
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::ResetCacheImpl()
{
    m_entity.ResetCache();
    m_navigations.clear();
}

/// <summary>
/// 
/// </summary>
bool FacetPartOf::MatchImpl(SdaiInstance inst, Context& ctx)
{
    if (m_navigations.empty()) {
        FillParentsNavigators(ctx);
    }

    std::list<SdaiInstance> toCheckParents;
    toCheckParents.push_back(inst);

    while (!toCheckParents.empty()) {
        
        inst = toCheckParents.front();
        toCheckParents.pop_front();

        for (auto& nav : m_navigations) {
            std::list<SdaiInstance> follow;
            nav->Follow(inst, follow, ctx);
            for (auto parent : follow) {
                if (m_entity.Match(parent, ctx)) {
                    return true; //>>>>>>>>>>>>>>>>>>>>>>>>>
                }
                toCheckParents.push_back(parent);
            }
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::FillParentsNavigators(Context& ctx)
{
    if (m_relation.empty()) {
        CreateNavigatorByAttributes(ctx._IfcObjectDefinition_IsDecomposedBy(), sdaiAGGR, NULL, ctx._IfcRelDecomposes_RelatingObject(), ctx);
        CreateNavigatorByAttributes(ctx._IfcObjectDefinition_HasAssignments(), sdaiAGGR, ctx._IfcRelAssignsToGroup(), ctx._IfcRelAssignsToGroup_RelatedGroup(), ctx);
        CreateNavigatorByAttributes(ctx._IfcFeatureElementSubtraction_VoidsElements(), sdaiINSTANCE, NULL, ctx._IfcRelVoidsElement_RelatingBuildingElement(), ctx);
        CreateNavigatorByAttributes(ctx._IfcElement_FillsVoids(), sdaiAGGR, NULL, ctx._IfcRelFillsElement_RelatingOpeningElement(), ctx);

        CreateNavigatorByRelation(ctx._IfcRelContainedInSpatialStructure(),
                                  ctx._IfcRelContainedInSpatialStructure_RelatingStructure(),
                                  ctx._IfcRelContainedInSpatialStructure_RelatedElements(), ctx);
    }
    else if (m_relation == "IFCRELAGGREGATES") {
        CreateNavigatorByAttributes(ctx._IfcObjectDefinition_IsDecomposedBy(), sdaiAGGR, ctx._IfcRelAggregates(), ctx._IfcRelDecomposes_RelatingObject(), ctx);
    }
    else if (m_relation == "IFCRELNESTS") {
        CreateNavigatorByAttributes(ctx._IfcObjectDefinition_IsDecomposedBy(), sdaiAGGR, ctx._IfcRelNests(), ctx._IfcRelDecomposes_RelatingObject(), ctx);
    }
    else if (m_relation == "IFCRELASSIGNSTOGROUP") {
        CreateNavigatorByAttributes(ctx._IfcObjectDefinition_HasAssignments(), sdaiAGGR, ctx._IfcRelAssignsToGroup(), ctx._IfcRelAssignsToGroup_RelatedGroup(), ctx);
    }
    else if (m_relation == "IFCRELCONTAINEDINSPATIALSTRUCTURE") {
        CreateNavigatorByRelation(ctx._IfcRelContainedInSpatialStructure(),
                                  ctx._IfcRelContainedInSpatialStructure_RelatingStructure(),
                                  ctx._IfcRelContainedInSpatialStructure_RelatedElements(), ctx);
    }
    else if (m_relation == "IFCRELVOIDSELEMENT") {
        CreateNavigatorByAttributes(ctx._IfcFeatureElementSubtraction_VoidsElements(), sdaiINSTANCE, NULL, ctx._IfcRelVoidsElement_RelatingBuildingElement(), ctx);
    }
    else if (m_relation == "IFCRELFILLSELEMENT") {
        CreateNavigatorByAttributes(ctx._IfcElement_FillsVoids(), sdaiAGGR, NULL, ctx._IfcRelFillsElement_RelatingOpeningElement(), ctx);
    }
    else {
        ctx.LogMsg(MsgLevel::NotImplemented, "Unsupported relationship %s", m_relation.c_str());
        assert(0);
    }
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::CreateNavigatorByAttributes(SdaiAttr attrRelation, SdaiPrimitiveType sdaiType, SdaiEntity relClass, SdaiAttr attrParent, Context&)
{
    auto nav = new NavigateByAttributes ();
    m_navigations.push_back(nav);

    nav->attrRelation = attrRelation;
    nav->sdaiType = sdaiType;
    nav->relClass = relClass;
    nav->attrInstance = attrParent;
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::CreateNavigatorByRelation(SdaiEntity relClass, SdaiAttr attrParent, SdaiAttr attrChildren, Context&)
{
    auto rel = new NavigateByRelation();
    m_navigations.push_back(rel);

    rel->relClass = relClass;
    rel->attrParent = attrParent;
    rel->attrChildren = attrChildren;
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::NavigateByAttributes::Follow(SdaiInstance inst, std::list<SdaiInstance>& follow, Context&)
{
    if (sdaiType == sdaiAGGR) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(inst, attrRelation, sdaiAGGR, &aggr);
        if (aggr) {
            int i = 0;
            SdaiInstance rel = 0;
            while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &rel)) {
                FollowRel(rel, follow);
            }
        }
    }
    else {
        SdaiInstance rel = 0;
        sdaiGetAttr(inst, attrRelation, sdaiINSTANCE, &rel);
        FollowRel(rel, follow);
    }
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::NavigateByAttributes::FollowRel(SdaiInstance rel, std::list<SdaiInstance>& follow)
{
    if (!rel) {
        return;
    }
    
    if (relClass) {
        if (!sdaiIsInstanceOf(rel, relClass)) {
            return;
        }
    }

    SdaiInstance inst;
    sdaiGetAttr(rel, attrInstance, sdaiINSTANCE, &inst);
    
    if (inst) {
        follow.push_back(inst);
    }
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::NavigateByRelation::Follow(SdaiInstance inst, std::list<SdaiInstance>& follow, Context& ctx)
{
    auto ext = xxxxGetEntityAndSubTypesExtent(ctx.model, relClass);

    SdaiInstance rel = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(ext, i++, sdaiINSTANCE, &rel)) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(rel, attrChildren, sdaiAGGR, &aggr);
        if (aggr) {
            SdaiInstance ai;
            SdaiInteger j = 0;
            while (sdaiGetAggrByIndex(aggr, j++, sdaiINSTANCE, &ai)) {
                if (ai == inst) {
                    SdaiInstance f = 0;
                    sdaiGetAttr(rel, attrParent, sdaiINSTANCE, &f);
                    if (f) {
                        follow.push_back(f);
                    }
                    break;
                }
            }
        }
    }
}

/// <summary>
/// 
/// </summary>
void FacetClassification::ResetCacheImpl()
{
}

/// <summary>
/// 
/// </summary>
bool FacetClassification::MatchImpl(SdaiInstance inst, Context& ctx)
{
    References references;

    CollectIfcRelAssociatesClassification(inst, ctx, references);

    CollectIfcExternalReferenceRelationship(inst, ctx, references);

    if (auto type = GetTypeObject(inst, ctx)) {
        CollectIfcRelAssociatesClassification(type, ctx, references);
    }

    return Match (references, ctx);
}

/// <summary>
/// 
/// </summary>
void FacetClassification::CollectIfcRelAssociatesClassification(SdaiInstance inst, Context& ctx, References& references)
{
    //when inst is IfcPropertyDefinition or IfcObjectDefinition
    SdaiAggr aggrAssoc = 0;
    sdaiGetAttrBN(inst, "HasAssociations", sdaiAGGR, &aggrAssoc); 

    if (aggrAssoc) {
        SdaiInstance relAssoc = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(aggrAssoc, i++, sdaiINSTANCE, &relAssoc)) {

            auto entityAssoc = sdaiGetInstanceType(relAssoc);
            if (entityAssoc == ctx._IfcRelAssociatesClassification()) {

                SdaiInstance clsf = 0;
                sdaiGetAttr(relAssoc, ctx._IfcRelAssociatesClassification_RelatingClassification(), sdaiINSTANCE, &clsf);
                if (clsf) {

                    std::wstring system;
                    Reference reference;

                    HandleClassificationSelect(clsf, ctx, system, reference);
                    
                    references.insert(References::value_type(system, reference));
                }
            }
        }
    }
}

/// <summary>
/// 
/// </summary>
void FacetClassification::CollectIfcExternalReferenceRelationship(SdaiInstance inst, Context& ctx, References& references)
{
    //when inst is IfcResourceObjectSelect
    SdaiAggr aggrExternalRefs = 0;
    sdaiGetAttrBN(inst, "HasExternalReferences", sdaiAGGR, &aggrExternalRefs);

    if (aggrExternalRefs) {
        SdaiInstance relExternalRefRel = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(aggrExternalRefs, i++, sdaiINSTANCE, &relExternalRefRel)) {

            SdaiInstance externalRef = 0;
            sdaiGetAttr(relExternalRefRel, ctx._IfcExternalReferenceRelationship_RelatingReference(), sdaiINSTANCE, &externalRef);

            auto entityRef = sdaiGetInstanceType(externalRef);
            if (entityRef == ctx._IfcClassificationReference()) {

                std::wstring system;
                Reference reference;

                HandleClassificationSelect(externalRef, ctx, system, reference);

                references.insert (References::value_type (system, reference));
            }
        }
    }
}


/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassificationSelect(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference)
{
    auto clsfType = sdaiGetInstanceType(clsf);
    if (clsfType == ctx._IfcClassificationReference()) {
        HandleClassificationReference(clsf, ctx, system, reference);
        return;
    }

    if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
        if (clsfType == ctx._IfcClassificationNotation()) {
            HandleClassificationNotation(clsf, ctx, system, reference);
            return;
        }
    }
    else {
        if (clsfType == ctx._IfcClassification()) {
            HandleClassification(clsf, ctx, system, reference);
            return;
        }
    }

    auto clsfTypeName = engiGetEntityName(clsfType, sdaiSTRING);
    assert(clsfTypeName);
    ctx.LogMsg(MsgLevel::NotImplemented, "Not implemented IfcClassificationSelect type %s", clsfTypeName ? clsfTypeName : NULL);
    assert(0);
}

/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassificationReference(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference)
{
    //
    const wchar_t* item = nullptr;

    if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
        sdaiGetAttr(clsf, ctx._IfcExternalReference_ItemReference(), sdaiUNICODE, &item);
    }
    else {
        sdaiGetAttr(clsf, ctx._IfcExternalReference_Identification(), sdaiUNICODE, &item);
    }

    if (item) {
        reference.items.push_back(item);
    }

    //
    const wchar_t* uri = nullptr;
    sdaiGetAttr(clsf, ctx._IfcExternalReference_Location(), sdaiUNICODE, &uri);

    if (uri) {
        reference.URI.push_back(uri);
    }

    //
    SdaiInstance source = 0;
    sdaiGetAttr(clsf, ctx._IfcClassificationReference_ReferencedSource(), sdaiINSTANCE, &source);
    HandleClassificationSelect(source, ctx, system, reference);
}

/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassification(SdaiInstance clsf, Context& ctx, std::wstring& system, Reference& reference)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(clsf, ctx._IfcClassification_Name(), sdaiUNICODE, &name);
    if (name) {
        assert(system.empty());
        system = name;
    }

    if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc4) {
        const wchar_t* source = nullptr;
        sdaiGetAttr(clsf, ctx._IfcClassification_Location(), sdaiUNICODE, &source);
        if (source) {
            reference.URI.push_back(source);
        }
    }

    if (ctx.GetIfcVersion() > Context::IfcVersion::Ifc4) {
        const wchar_t* source = nullptr;
        sdaiGetAttr(clsf, ctx._IfcClassification_Specification(), sdaiUNICODE, &source);
        if (source) {
            reference.URI.push_back(source);
        }
    }

    const wchar_t* source = nullptr;
    sdaiGetAttr(clsf, ctx._IfcClassification_Source(), sdaiUNICODE, &source);
    if (source) {
        reference.URI.push_back(source);
    }
}

/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassificationNotation(SdaiInstance /*clsf*/, Context& ctx, std::wstring& /*system*/, Reference& /*reference*/)
{
    ctx.LogMsg(MsgLevel::NotImplemented, "IfcClassificationNotation in facet classification");
    assert(!"TODO... if somebody uses");
}

/// <summary>
/// 
/// </summary>
bool FacetClassification::Match(References& references, Context& ctx)
{
    for (auto& ref : references) {
        if (m_system.Match(ref.first.c_str(), true, ctx)) {

            bool itemMatch = !m_value.IsSet();
            for (auto& item : ref.second.items) {
                itemMatch = m_value.Match(item.c_str(), true, ctx);
                if (itemMatch) {
                    break;
                }
            }

            if (itemMatch) {

                bool uriMatch = !m_URI.IsSet();
                for (auto& uri : ref.second.URI) {
                    uriMatch = m_URI.Match(uri.c_str(), true, ctx);
                }

                if (uriMatch) {
                    return true;
                }
            }
        }
    }

    return false;
}


/// <summary>
/// 
/// </summary>
bool FacetAttribute::MatchImpl(SdaiInstance inst, Context& ctx)
{
    SdaiEntity ent = sdaiGetInstanceType(inst);

    if (auto attrName = m_name.GetSimpleValue()) {

        //explicitly specified attribute
        auto attr = sdaiGetAttrDefinition(ent, attrName);
        if (attr) {
            return MatchAttribute(inst, attr, ctx); //>>>>>>>>>>>>>>>>
        }
    }
    else {

        //attribute name must match
        const bool useInverse = false;
        auto N = engiGetEntityNoAttributesEx (ent, true, useInverse);
        for (SdaiInteger i = 0; i < N; i++) {
            auto attr = engiGetEntityAttributeByIndex(ent, i, true, useInverse);
            const char* name = 0;
            engiGetAttributeTraits(attr, &name, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            if (m_name.Match(name, true, ctx)) {
                if (MatchAttribute(inst, attr, ctx)) {
                    return true; //>>>>>>>>>>>>>
                }
            }
        }
    }
    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetAttribute::MatchAttribute(SdaiInstance inst, SdaiAttr attr, Context& ctx)
{
    if (engiAttrIsInverse(attr)) {
        return false; //do not apply to imverse attribute
    }

    bool match = false;

    auto sdaiType = engiGetInstanceAttrType(inst, attr);
    switch (sdaiType) {
        case 0://$
            break;
        case sdaiBINARY: {
            const char* value = nullptr;
            if (sdaiGetAttr(inst, attr, sdaiBINARY, &value) && value && *value) {
                match = m_value.Match(value, false, ctx);
            }
            break;
        }
        case sdaiENUM: {
            const char* value = nullptr;
            if (sdaiGetAttr(inst, attr, sdaiENUM, &value) && value && *value) {
                match = m_value.Match(value, false, ctx);
            }
            break;
        }
        case sdaiSTRING: {
            const wchar_t* value = nullptr;
            if (sdaiGetAttr(inst, attr, sdaiUNICODE, &value) && value && *value) {
                match = m_value.Match(value, false, ctx);
            }
            break;
        }
        case sdaiINTEGER: {
            SdaiInteger value = 0;
            if (sdaiGetAttr(inst, attr, sdaiINTEGER, &value)) {
                match = m_value.Match(value, ctx);
            }
            break;
        }
        case sdaiREAL:
        case sdaiNUMBER: {
            double value = 0;
            if (sdaiGetAttr(inst, attr, sdaiREAL, &value)) {
                match = m_value.Match(value, ctx);
            }
            break;
        }
        case sdaiAGGR: {
            SdaiAggr aggr = 0;
            if (sdaiGetAttr(inst, attr, sdaiAGGR, &aggr)) {
                match = MatchAggr(aggr, ctx);
            }
            break;
        }
        case sdaiLOGICAL: {
            const char* value = nullptr;
            if (sdaiGetAttr(inst, attr, sdaiLOGICAL, &value)) {
                if (value){
                    switch (*value) {
                        case 'T':
                        case 't':
                            match = m_value.Match("TRUE", false, ctx);
                            break;
                        case 'F':
                        case 'f':
                            match = m_value.Match("FALSE", false, ctx);
                            break;
                        //U always fails
                    }
                }
            }
            break;
        }
        case sdaiBOOLEAN: {
            bool value = false;
            if (sdaiGetAttr(inst, attr, sdaiBOOLEAN, &value)) {
                match = m_value.Match(value ? "TRUE" : "FALSE", false, ctx);
            }
            break;
        }
        case sdaiINSTANCE: {
            SdaiInstance value = 0;
            if (sdaiGetAttr(inst, attr, sdaiINSTANCE, &value)) {
                match = MatchInstance();
            }
            break;
        }
        case sdaiADB: {
            SdaiADB value = 0;
            if (sdaiGetAttr(inst, attr, sdaiADB, &value)) {
                match = MatchADB(value, ctx);
            }
            break;
        }
        default:
            ctx.LogMsg(MsgLevel::NotImplemented, "Match unknown attribute type");
            assert(0);
    }
    return match;
}

/// <summary>
/// 
/// </summary>
bool FacetAttribute::MatchAggr(SdaiAggr aggr, Context& ctx)
{
    SdaiPrimitiveType sdaiType;
    engiGetAggrType(aggr, &sdaiType);

    auto N = sdaiGetMemberCount(aggr);
    for (SdaiInteger i = 0; i < N; i++) {

        bool match = false;

        switch (sdaiType) {
            case 0://$
                break;
            case sdaiBINARY: {
                const char* value = nullptr;
                if (sdaiGetAggrByIndex(aggr, i, sdaiBINARY, &value) && value && *value) {
                    match = m_value.Match(value, false, ctx);
                }
                break;
            }
            case sdaiENUM: {
                const char* value = nullptr;
                if (sdaiGetAggrByIndex(aggr, i, sdaiENUM, &value) && value && *value) {
                    match = m_value.Match(value, false, ctx);
                }
                break;
            }
            case sdaiSTRING: {
                const wchar_t* value = nullptr;
                if (sdaiGetAggrByIndex(aggr, i, sdaiUNICODE, &value) && value && *value) {
                    match = m_value.Match(value, false, ctx);
                }
                break;
            }
            case sdaiINTEGER: {
                SdaiInteger value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiINTEGER, &value)) {
                    match = m_value.Match(value, ctx);
                }
                break;
            }
            case sdaiREAL:
            case sdaiNUMBER: {
                double value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiREAL, &value)) {
                    match = m_value.Match(value, ctx);
                }
                break;
            }
            case sdaiAGGR: {
                SdaiAggr value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiAGGR, &value)) {
                    match = MatchAggr(value, ctx);
                }
                break;
            }
            case sdaiLOGICAL: {
                const char* value = nullptr;
                if (sdaiGetAggrByIndex(aggr, i, sdaiLOGICAL, &value)) {
                    if (value) {
                        switch (*value) {
                            case 'T':
                            case 't':
                                match = m_value.Match("TRUE", false, ctx);
                                break;
                            case 'F':
                            case 'f':
                                match = m_value.Match("FALSE", false, ctx);
                                break;
                                //U always fails
                        }
                    }
                }
                break;
            }
            case sdaiBOOLEAN: {
                bool value = false;
                if (sdaiGetAggrByIndex(aggr, i, sdaiBOOLEAN, &value)) {
                    match = m_value.Match(value ? "TRUE" : "FALSE", false, ctx);
                }
                break;
            }
            case sdaiINSTANCE: {
                SdaiInstance value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiINSTANCE, &value)) {
                    match = MatchInstance();
                }
                break;
            }
            case sdaiADB: {
                SdaiADB value = 0;
                if (sdaiGetAggrByIndex(aggr, i, sdaiADB, &value)) {
                    match = MatchADB(value, ctx);
                }
                break;
            }
            default:
                ctx.LogMsg(MsgLevel::NotImplemented, "Match unknown attribute type");
                assert(0);
        }

        if (match) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetAttribute::MatchADB(SdaiADB adb, Context& ctx)
{
    bool match = false;

    SdaiPrimitiveType sdaiType = sdaiGetADBType(adb);

    switch (sdaiType) {
        case 0://$
            break;
        case sdaiBINARY: {
            const char* value = nullptr;
            if (sdaiGetADBValue(adb, sdaiBINARY, &value) && value && *value) {
                match = m_value.Match(value, false, ctx);
            }
            break;
        }
        case sdaiENUM: {
            const char* value = nullptr;
            if (sdaiGetADBValue(adb, sdaiENUM, &value) && value && *value) {
                match = m_value.Match(value, false, ctx);
            }
            break;
        }
        case sdaiSTRING: {
            const wchar_t* value = nullptr;
            if (sdaiGetADBValue(adb, sdaiUNICODE, &value) && value && *value) {
                match = m_value.Match(value, false, ctx);
            }
            break;
        }
        case sdaiINTEGER: {
            SdaiInteger value = 0;
            if (sdaiGetADBValue(adb, sdaiINTEGER, &value)) {
                match = m_value.Match(value, ctx);
            }
            break;
        }
        case sdaiREAL:
        case sdaiNUMBER: {
            double value = 0;
            if (sdaiGetADBValue(adb, sdaiREAL, &value)) {
                match = m_value.Match(value, ctx);
            }
            break;
        }
        case sdaiAGGR: {
            SdaiAggr value = 0;
            if (sdaiGetADBValue(adb, sdaiAGGR, &value)) {
                match = MatchAggr(value, ctx);
            }
            break;
        }
        case sdaiLOGICAL: {
            const char* value = nullptr;
            if (sdaiGetADBValue(adb, sdaiLOGICAL, &value)) {
                if (value) {
                    switch (*value) {
                        case 'T':
                        case 't':
                            match = m_value.Match("TRUE", false, ctx);
                            break;
                        case 'F':
                        case 'f':
                            match = m_value.Match("FALSE", false, ctx);
                            break;
                            //U always fails
                    }
                }
            }
            break;
        }
        case sdaiBOOLEAN: {
            bool value = false;
            if (sdaiGetADBValue(adb, sdaiBOOLEAN, &value)) {
                match = m_value.Match(value ? "TRUE" : "FALSE", false, ctx);
            }
            break;
        }
        case sdaiINSTANCE: {
            SdaiInstance value = 0;
            if (sdaiGetADBValue(adb, sdaiINSTANCE, &value)) {
                match = MatchInstance();
            }
            break;
        }
        case sdaiADB: {
            SdaiADB value = 0;
            if (sdaiGetADBValue(adb, sdaiADB, &value)) {
                match = MatchADB(value, ctx);
            }
            break;
        }
        default:
            ctx.LogMsg(MsgLevel::NotImplemented, "Match unknown attribute type");
            assert(0);
    }

    return match;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchImpl(SdaiInstance inst, Context& ctx)
{
    SdaiAggr aggr = 0;
    sdaiGetAttr(inst, ctx._IfcTypeObject_HasPropertySets(), sdaiAGGR, &aggr);
    if (MatchInSetOfPSDef(aggr, ctx)) {
        return true;
    }

    sdaiGetAttr(inst, ctx._IfcObject_IsDefinedBy(), sdaiAGGR, &aggr);
    if (MatchInSetOfRel(aggr, ctx)) {
        return true;
    }

    if (ctx.GetIfcVersion() > Context::IfcVersion::Ifc2x3) {
        sdaiGetAttr(inst, ctx._IfcContext_IsDefinedBy(), sdaiAGGR, &aggr);
        if (MatchInSetOfRel(aggr, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchInSetOfPSDef(SdaiAggr aggr, Context& ctx)
{
    SdaiInstance pset = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &pset)) {
        if (MatchInPSDef(pset, ctx)) {
            return true;
        }
    }
    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchInSetOfRel(SdaiAggr aggr, Context& ctx)
{
    SdaiInstance rel = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &rel)) {
        SdaiInstance pset = 0;
        if (sdaiGetAttr(rel, ctx._IfcRelDefinesByProperties_RelatingPropertyDefinition(), sdaiINSTANCE, &pset)) {
            if (MatchInPSDef(pset, ctx)) {
                return true;
            }
        }
        else {
            SdaiAggr psdefset = 0;
            if (sdaiGetAttr(rel, ctx._IfcRelDefinesByProperties_RelatingPropertyDefinition(), sdaiAGGR, &psdefset)) {
                SdaiInteger j = 0;
                while (sdaiGetAggrByIndex(psdefset, j++, sdaiINSTANCE, &pset)) {
                    if (MatchInPSDef(pset, ctx)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchInPSDef(SdaiInstance inst, Context& ctx)
{
    if (m_propertySet.IsSet()) {
        const wchar_t* name = 0;
        sdaiGetAttr(inst, ctx._IfcRoot_Name(), sdaiUNICODE, &name);
        if (!m_name.Match(name, false, ctx)) {
            return false;
        }
    }

    auto type = sdaiGetInstanceType(inst);

    if (type == ctx._IfcPropertySet()) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(inst, ctx._IfcPropertySet_HasProperties(), sdaiAGGR, &aggr);
        SdaiInstance prop = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &prop)) {
            if (MatchProperty(prop, ctx)) {
                return true;
            }
        }
    }
    else if (type == ctx._IfcElementQuantity()) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(inst, ctx._IfcElementQuantity_Quantities(), sdaiAGGR, &aggr);
        SdaiInstance quant = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &quant)) {
            if (MatchQuantity(quant, ctx)) {
                return true;
            }
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchProperty(SdaiInstance prop, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(prop, ctx._IfcProperty_Name(), sdaiUNICODE, &name);
    if (!m_name.Match(name, false, ctx)) {
        return false;
    }

    if (!m_value.IsSet()) {
        return true;
    }

    auto entity = sdaiGetInstanceType(prop);

    if (entity == ctx._IfcComplexProperty()) {
        ctx.LogMsg(MsgLevel::NotImplemented, "IfcComplexProperty");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertyBoundedValue()) {
        ctx.LogMsg(MsgLevel::NotImplemented, "_IfcPropertyBoundedValue");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertyEnumeratedValue()) {
        ctx.LogMsg(MsgLevel::NotImplemented, "_IfcPropertyEnumeratedValue");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertyListValue()) {
        ctx.LogMsg(MsgLevel::NotImplemented, "_IfcPropertyListValue");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertyReferenceValue()) {
        ctx.LogMsg(MsgLevel::NotImplemented, "_IfcPropertyReferenceValue");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertySingleValue()) {
        return MatchPropertySingleValue(prop, ctx);
    }
    else if (entity == ctx._IfcPropertyTableValue()) {
        ctx.LogMsg(MsgLevel::NotImplemented, "_IfcPropertyTableValue");
        assert(0);
        return false;
    }
    else {
        ctx.LogMsg(MsgLevel::NotImplemented, "Unknown entity");
        assert(0);
        return false;
    }
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchQuantity(SdaiInstance qto, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(qto, ctx._IfcPhysicalQuantity_Name(), sdaiUNICODE, &name);
    if (!m_name.Match(name, false, ctx)) {
        return false;
    }

    if (!m_value.IsSet()) {
        return true;
    }

    auto entity = sdaiGetInstanceType(qto);

    if (entity == ctx._IfcPhysicalComplexQuantity()) {
        ctx.LogMsg(MsgLevel::NotImplemented, "_IfcPhysicalComplexQuantity");
        assert(0);
        return false;
    }

    SdaiInstance unit = 0;
    sdaiGetAttr(qto, ctx._IfcPhysicalSimpleQuantity_Unit(), sdaiINSTANCE, &unit);

    if (entity == ctx._IfcQuantityArea()) {
        double value = 0;
        sdaiGetAttr(qto, ctx._IfcQuantityArea_AreaValue(), sdaiREAL, &value);
        return MatchValue(value, unit, "AREAUNIT", ctx);
    }
    else if (entity == ctx._IfcQuantityCount()) {
        SdaiInteger value = 0;
        sdaiGetAttr(qto, ctx._IfcQuantityCount_CountValue(), sdaiINTEGER, &value);
        return m_value.Match(value, ctx);
    }
    else if (entity == ctx._IfcQuantityLength()) {
        double value = 0;
        sdaiGetAttr(qto, ctx._IfcQuantityLength_LengthValue(), sdaiREAL, &value);
        return MatchValue(value, unit, "LENGTHUNIT", ctx);
    }
    else if (entity == ctx._IfcQuantityNumber()) {
        double value = 0;
        sdaiGetAttr(qto, ctx._IfcQuantityNumber_NumberValue(), sdaiREAL, &value);
        return MatchValue(value, unit, nullptr, ctx);
    }
    else if (entity == ctx._IfcQuantityTime()) {
        double value = 0;
        sdaiGetAttr(qto, ctx._IfcQuantityTime_TimeValue(), sdaiREAL, &value);
        return MatchValue(value, unit, "TIMEUNIT", ctx);
    }
    if (entity == ctx._IfcQuantityVolume()) {
        double value = 0;
        sdaiGetAttr(qto, ctx._IfcQuantityVolume_VolumeValue(), sdaiREAL, &value);
        return MatchValue(value, unit, "VOLUMEUNIT", ctx);
    }
    else if (entity == ctx._IfcQuantityWeight()) {
        double value = 0;
        sdaiGetAttr(qto, ctx._IfcQuantityWeight_WeightValue(), sdaiREAL, &value);
        return MatchValue(value, unit, "MASSUNIT", ctx);
    }
    else {
        ctx.LogMsg(MsgLevel::NotImplemented, "Unknown entity");
        assert(0);
        return false;
    }
}


/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchPropertySingleValue(SdaiInstance prop, Context& ctx)
{
    SdaiADB nominalValue = 0;
    sdaiGetAttr(prop, ctx._IfcPropertySingleValue_NominalValue(), sdaiADB, &nominalValue);

    enum_express_attr_type  attrType = enum_express_attr_type::__NONE;
    auto ifcType = sdaiGetADBTypePath(nominalValue, 0);
    if (ifcType) {
        auto type = sdaiGetEntity(ctx.model, ifcType);
        attrType = engiGetDefinedType(type, nullptr, nullptr);
    }

    switch (attrType) {
        case enum_express_attr_type::__BINARY:
        case enum_express_attr_type::__BINARY_32:
        {
            const char* value = nullptr;
            sdaiGetADBValue(nominalValue, sdaiBINARY, &value);
            return m_value.Match(value, false, ctx);
        }
        case enum_express_attr_type::__STRING:
        {
            const wchar_t* value = nullptr;
            sdaiGetADBValue(nominalValue, sdaiUNICODE, &value);
            return m_value.Match(value, false, ctx);
        }
        case enum_express_attr_type::__ENUMERATION:
        {
            const char* value = nullptr;
            sdaiGetADBValue(nominalValue, sdaiENUM, &value);
            return m_value.Match(value, false, ctx);
        }
        case enum_express_attr_type::__BOOLEAN:
        {
            bool value = 0;
            sdaiGetADBValue(nominalValue, sdaiBOOLEAN, &value);
            return m_value.Match(value, ctx);
        }
        case enum_express_attr_type::__INTEGER:
        {
            SdaiInteger value = 0;
            sdaiGetADBValue(nominalValue, sdaiINTEGER, &value);
            return m_value.Match(value, ctx);
        }
        case enum_express_attr_type::__LOGICAL:
        {
            const char* value = 0;
            sdaiGetADBValue(nominalValue, sdaiLOGICAL, &value);
            return m_value.Match(value, false, ctx);
        }
        case enum_express_attr_type::__NUMBER:
        case enum_express_attr_type::__REAL:
        {
            SdaiInstance            unit = 0;
            sdaiGetAttr(prop, ctx._IfcPropertySingleValue_Unit(), sdaiINSTANCE, &unit);

            const char* unitKind = nullptr;
            auto it = s_ifcDataTypesUnits.find(ifcType);
            if (it != s_ifcDataTypesUnits.end()) {
                unitKind = it->second.c_str();
            }

            double value = 0;
            sdaiGetADBValue(nominalValue, sdaiREAL, &value);
            return MatchValue(value, unit, unitKind, ctx);
        }
        default:
        {
            ctx.LogMsg(MsgLevel::NotImplemented, "type of IfcValue");
            assert(0);
            return false;
        }
    }
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchValue(double value, SdaiInstance unit, const char* unitKind, Context& ctx)
{
    double scale = ctx.GetUnitScale(unit, unitKind);

    value *= scale;

    return m_value.Match(value, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchImpl(SdaiInstance inst, Context& ctx)
{
    SdaiAggr aggrAssoc = 0;
    sdaiGetAttr(inst, ctx._IfcObjectDefinition_HasAssociations(), sdaiAGGR, &aggrAssoc);

    SdaiInstance relAssoc = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(aggrAssoc, i++, sdaiINSTANCE, &relAssoc)) {

        auto entityAssoc = sdaiGetInstanceType(relAssoc);
        if (entityAssoc == ctx._IfcRelAssociatesMaterial()) {

            SdaiInstance material = 0;
            sdaiGetAttr(relAssoc, ctx._IfcRelAssociatesMaterial_RelatingMaterial(), sdaiINSTANCE, &material);
            if (material) {

                if (MatchMaterialSelect(material, ctx)) {
                    return true;
                }
            }
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialSelect(SdaiInstance material, Context& ctx)
{
    if (!material) {
        return false;
    }

    auto entity = sdaiGetInstanceType(material);

    if (entity == ctx._IfcMaterial()) {
        return MatchMaterialSimple(material, ctx);
    }
    else if (entity == ctx._IfcMaterialLayer()) {
        return MatchMaterialLayer(material, ctx);
    }
    else if (entity == ctx._IfcMaterialLayerSet()) {
        return MatchMaterialLayerSet(material, ctx);
    }
    else if (entity == ctx._IfcMaterialProfile()) {
        return MatchMaterialProfile(material, ctx);
    }
    else if (entity == ctx._IfcMaterialProfileSet()) {
        return MatchMaterialProfileSet(material, ctx);
    }
    else if (entity == ctx._IfcMaterialConstituent()) {
        return MatchMaterialConstituent(material, ctx);
    }
    else if (entity == ctx._IfcMaterialConstituentSet()) {
        return MatchMaterialConstituentSet(material, ctx);
    }
    else if (entity == ctx._IfcMaterialLayerSetUsage()) {
        return MatchMaterialLayerSetUsage(material, ctx);
    }
    else if (entity == ctx._IfcMaterialProfileSetUsage()) {
        return MatchMaterialProfileSetUsage(material, ctx);
    }
    else if (entity == ctx._IfcMaterialList()) {
        return MatchMaterialList(material, ctx);
    }
    else {
        ctx.LogMsg(MsgLevel::NotImplemented, "Material type");
        return false;
    }
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialSimple(SdaiInstance material, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(material, ctx._IfcMaterial_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    if (ctx.GetIfcVersion() > Context::IfcVersion::Ifc2x3) {
        sdaiGetAttr(material, ctx._IfcMaterial_Category(), sdaiUNICODE, &name);
        if (m_value.Match(name, false, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialLayer(SdaiInstance layer, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(layer, ctx._IfcMaterialLayer_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    sdaiGetAttr(layer, ctx._IfcMaterialLayer_Category(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiInstance material = 0;
    sdaiGetAttr(layer, ctx._IfcMaterialLayer_Material(), sdaiINSTANCE, &material);
    return MatchMaterialSimple(material, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialLayerSet(SdaiInstance material, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(material, ctx._IfcMaterialLayerSet_LayerSetName(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiAggr layers = 0;
    sdaiGetAttr(material, ctx._IfcMaterialLayerSet_MaterialLayers(), sdaiAGGR, &layers);

    SdaiInstance layer = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(layers, i++, sdaiINSTANCE, &layer)) {
        if (MatchMaterialLayer(layer, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialLayerSetUsage(SdaiInstance material, Context& ctx)
{
    SdaiInstance layerset = 0;
    sdaiGetAttr(material, ctx._IfcMaterialLayerSetUsage_ForLayerSet(), sdaiINSTANCE, &layerset);
    return MatchMaterialLayerSet(layerset, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialProfile(SdaiInstance profile, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(profile, ctx._IfcMaterialProfile_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    sdaiGetAttr(profile, ctx._IfcMaterialProfile_Category(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiInstance material = 0;
    sdaiGetAttr(profile, ctx._IfcMaterialProfile_Material(), sdaiINSTANCE, &material);
    return MatchMaterialSimple(material, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialProfileSet(SdaiInstance material, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(material, ctx._IfcMaterialProfileSet_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiAggr profiles = 0;
    sdaiGetAttr(material, ctx._IfcMaterialProfileSet_MaterialProfiles(), sdaiAGGR, &profiles);

    SdaiInstance profile = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(profiles, i++, sdaiINSTANCE, &profile)) {
        if (MatchMaterialProfile(profile, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialProfileSetUsage(SdaiInstance material, Context& ctx)
{
    SdaiInstance profileset = 0;
    sdaiGetAttr(material, ctx._IfcMaterialProfileSetUsage_ForProfileSet(), sdaiINSTANCE, &profileset);
    return MatchMaterialLayerSet(profileset, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialConstituent(SdaiInstance constit, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(constit, ctx._IfcMaterialConstituent_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    sdaiGetAttr(constit, ctx._IfcMaterialConstituent_Category(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiInstance material = 0;
    sdaiGetAttr(constit, ctx._IfcMaterialConstituent_Material(), sdaiINSTANCE, &material);
    return MatchMaterialSimple(material, ctx);
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialConstituentSet(SdaiInstance material, Context& ctx)
{
    const wchar_t* name = nullptr;
    sdaiGetAttr(material, ctx._IfcMaterialConstituentSet_Name(), sdaiUNICODE, &name);
    if (m_value.Match(name, false, ctx)) {
        return true;
    }

    SdaiAggr parts = 0;
    sdaiGetAttr(material, ctx._IfcMaterialConstituentSet_MaterialConstituents(), sdaiAGGR, &parts);

    if (parts) {
        SdaiInstance part = 0;
        SdaiInteger i = 0;
        while (sdaiGetAggrByIndex(parts, i++, sdaiINSTANCE, &part)) {
            if (MatchMaterialConstituent(part, ctx)) {
                return true;
            }
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
bool FacetMaterial::MatchMaterialList(SdaiInstance material, Context& ctx)
{
    SdaiAggr parts = 0;
    sdaiGetAttr(material, ctx._IfcMaterialList_Materials(), sdaiAGGR, &parts);

    SdaiInstance part = 0;
    SdaiInteger i = 0;
    while (sdaiGetAggrByIndex(parts, i++, sdaiINSTANCE, &part)) {
        if (MatchMaterialSimple(part, ctx)) {
            return true;
        }
    }

    return false;
}

/// <summary>
/// 
/// </summary>
void MultiTypeValueCache::Get(double* pval) 
{ 
    if (!m_dSet) {
        m_dSet = true;

        char* end = 0;
        
        m_dVal = strtod(m_strVal.c_str(), &end);

        if (!end || *end) {
            m_dVal = FLT_MAX;
        }
    }
    
    *pval = m_dVal; 
}

/// <summary>
/// 
/// </summary>
void MultiTypeValueCache::Get(SdaiInteger* pval) 
{ 
    if (!m_iSet) { 
        m_iSet = true; 
    
        char* end = 0;

        m_iVal = strtol(m_strVal.c_str(), &end, 10);

        if (!end || *end) {
            m_iVal = MAXLONG;
        }
    }
    
    *pval = m_iVal; 
}

/// <summary>
/// 
/// </summary>
struct ComparerStr
{
    ComparerStr(bool compareNoCase) : m_compareNoCase(compareNoCase) {}

    int compare(const char* v1, const char* v2)
    {
        if (m_compareNoCase) {
            return StrICmp(v1, v2);
        }
        else {
            return strcmp(v1, v2);
        }
    }

    const char* ToString(const char* v)
    {
        return v;
    }

    bool m_compareNoCase;
};

/// <summary>
/// 
/// </summary>
struct ComparerFloat
{
    ComparerFloat(double precision) : m_precision(precision) {}

    int compare(double v1, double v2)
    {
        if (v1 <= v2 * (1 - m_precision) - m_precision)
            return -1;
        else if (v1 >= v2 * (1 + m_precision) + m_precision)
            return 1;
        else
            return 0;
    }

    const char* ToString(double v)
    {
        assert(0); //not expected to use
        snprintf(m_buff, 79, "%g", v);
        return m_buff;
    }

    double m_precision;
    char   m_buff[80];
};

#if 0
/// <summary>
/// 
/// </summary>
struct ComparerInt
{
    int compare(SdaiInteger v1, SdaiInteger v2)
    {
        if (v1 < v2)
            return -1;
        else if (v1 > v2)
            return 1;
        else
            return 0;
    }
};
#endif

/// <summary>
/// 
/// </summary>
template <typename T, class Comparer>
bool IdsValue::MatchValue(T value, Comparer& cmp)
{
    //
    if (!m_simpleValue.empty()) {
        T v;
        m_simpleVal.Get(&v);
        if (0!=cmp.compare(value, v)) {
            return false;
        }
    }


    //
    for (auto& rest : m_restrictions) {
        if (!rest->Fit(value, cmp)) {
            return false;
        }
    }

    return true;
}

/// <summary>
/// 
/// </summary>
bool IdsValue::Match(const wchar_t* value, bool compareNoCase, Context& ctx)
{
    if (value) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string utf8 = converter.to_bytes(value);
        return Match(utf8.c_str(), compareNoCase, ctx);
    }
    else {
        return !m_isSet;
    }
}

/// <summary>
/// 
/// </summary>
bool IdsValue::Match(const char* value, bool compareNoCase, Context&)
{
    if (!m_isSet) {
        return true;
    }
    if (!value) {
        return false;
    }

    ComparerStr cmp(compareNoCase);
    
    return MatchValue(value, cmp);
}

/// <summary>
/// 
/// </summary>
bool IdsValue::Match(SdaiInteger value, Context& ctx)
{
#if 1
    //pass-integers_follow_the_same_rules_as_numbers_2_2
    return Match((double)value, ctx);
#else
    if (!m_isSet) {
        return true;
    }

    ComparerInt cmp;
 
    return MatchValue(value, cmp);
#endif
}

/// <summary>
/// 
/// </summary>
bool IdsValue::Match(double value, Context& ctx)
{
    if (!m_isSet) {
        return true;
    }

    ComparerFloat cmp (ctx.Precision());

    return MatchValue(value, cmp);
}

/// <summary>
/// 
/// </summary>
template <typename T, class Comparer> bool Restriction::Fit(T value, Comparer& cmp)
{
    //
    if (!m_enumeration.empty()) {
        bool match = false;
        for (auto& val : m_enumeration) {
            T v = 0;
            val->Get(&v);
            if (0 == cmp.compare(value, v)) {
                match = true;
                break;
            }
        }
        if (!match) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& patt : m_pattern) {
        const char* r = nullptr;
        patt->Get(&r);
        auto str = cmp.ToString(value);
        bool match = std::regex_match(str, std::regex(r));
        if (!match) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_minInclusive) {
        T v = 0;
        r->Get(&v);
        auto c = cmp.compare(value, v);
        if (c == -1) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_maxInclusive) {
        T v = 0;
        r->Get(&v);
        auto c = cmp.compare(value, v);
        if (c == 1) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_minExclusive) {
        T v = 0;
        r->Get(&v);
        auto c = cmp.compare(value, v);
        if (c != 1) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_maxExclusive) {
        T v = 0;
        r->Get(&v);
        auto c = cmp.compare(value, v);
        if (c != -1) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_length) {
        SdaiInteger len = 0;
        r->Get(&len);
        auto str = cmp.ToString(value);
        if ((SdaiInteger)strlen(str) != len) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_minLength) {
        SdaiInteger len = 0;
        r->Get(&len);
        auto str = cmp.ToString(value);
        if ((SdaiInteger)strlen(str) < len) {
            return false; //>>>>>>
        }
    }

    //
    for (auto& r : m_maxLength) {
        SdaiInteger len = 0;
        r->Get(&len);
        auto str = cmp.ToString(value);
        if ((SdaiInteger)strlen(str) > len) {
            return false; //>>>>>>
        }
    }

    return true;
}

