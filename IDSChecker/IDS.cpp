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
            else { LogMsg(ctx, MsgLevel::Warning, "Unknown attribute '%s'", attrName.c_str()); } } }


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
            else { LogMsg(ctx, MsgLevel::Warning, "Unknown child element <%s>", tag.c_str()); } } }


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


#include "Context.h"

/// <summary>
/// 
/// </summary>
Context::IfcVersion Context::GetIfcVersion()
{
    if (m_ifcVersion == IfcVersion::NotItitialized) {
        assert(model);
        
        const char* schemaName = nullptr;
        GetSPFFHeaderItem(model, 9, 0, sdaiSTRING, &schemaName);
        assert(schemaName);

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
        }
    }
    return m_ifcVersion;
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
static void LogMsg (Context& ctx, MsgLevel type, const char* format, ...)
{
    if (type < ctx.msgLevel) {
        return; //>>>>>>>>>>>>>
    }

    auto& log = ctx.console;

    //
    log.out("\t<");

    const char* msgType;
    switch (type) {
        case MsgLevel::Info:     msgType = "info"; break;
        case MsgLevel::Warning:  msgType = "Warning"; break;
        default:                 msgType = "ERROR"; break;
    }

    log.out(msgType);

#if 0
    //
    std::string xpath;
    for (auto xml : ctx.GetXmlStack()) {
        if (xml) {
            auto name = xml->getName();
            if (!name.empty()) {
                if (!xpath.empty()) {
                    xpath.append("/");
                }
                xpath.append(name);
            }
        }
    }

    if (!xpath.empty()) {
        log.out(" ids='");
        log.out(xpath.c_str());
        log.out("'");
    }
#endif

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
        LogMsg(ctx, MsgLevel::Error, "Failed read IDS file: '%s', error: %s", idsFilePath, ex.what());
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
    return MatchImpl(inst, ctx);
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
        LogMsg(ctx, MsgLevel::Error, "Failed to read IFC file '%s'", ifcFilePath);
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
void Specification::ResetCache() 
{ 
    m_wasMatch = false; 
    m_suitableIfcVersion = -1; 

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
            m_wasMatch = true;
            
            ok = m_requirements.Match(inst, ctx);

            if (ok) {
                LogMsg(ctx, MsgLevel::Info, "Checked ok");
            }
            else {
                LogMsg(ctx, MsgLevel::Error, "Instance does not match specification");
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

    if (SuitableIfcVersion(ctx) && IsRequired()) {
        
        ok = m_wasMatch;
        
        if (ok) {
            LogMsg(ctx, MsgLevel::Info, "OK, required specification matched some instances");
        }
        else {
            LogMsg(ctx, MsgLevel::Error, "ERROR, required specification never match");
        }
    }

    return ok;
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
    m_attrPredefinedType = 0;
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

    if (auto name = m_name.GetSimpleValue()) {
        if (!m_sdaiEntity) {
            m_sdaiEntity = sdaiGetEntity(ctx.model, name);
        }
        entityNameMatch = sdaiIsInstanceOf (instType, m_sdaiEntity);
    }
    else {
        assert(!"to test, do we need polymorphic match");
        auto instTypeName = engiGetEntityName(instType, sdaiSTRING);
        entityNameMatch = m_name.Match(instTypeName, true);
    }

    if (!entityNameMatch) {
        return false; //>>>>>>>>>>>>>>>>>>>>>>>
    }
    
    // check predefined type
    //
    if (m_predefinedType.IsSet()) {
        const char* predTypeValue = nullptr;
        void* getResult = nullptr;

        if (m_sdaiEntity) {
            if (!m_attrPredefinedType) {
                m_attrPredefinedType = sdaiGetAttrDefinition(m_sdaiEntity, "PredefinedType");
            }
            getResult = sdaiGetAttr(inst, m_attrPredefinedType, sdaiSTRING, &predTypeValue);
        }
        else {
            getResult = sdaiGetAttrBN(inst, "PredefinedType", sdaiSTRING, &predTypeValue);
        }

        bool predTypeMatch = getResult && predTypeValue && m_predefinedType.Match(predTypeValue, false);

        if (!predTypeMatch) {
            return false; //>>>>>>>>>>>>>>>>>>>>>>>
        }
    }

    //
    return true;
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
        LogMsg(ctx, MsgLevel::NotImplemented, "Unsupported relationship %s", m_relation.c_str());
        assert(0);
    }
}

/// <summary>
/// 
/// </summary>
void FacetPartOf::CreateNavigatorByAttributes(SdaiAttr attrRelation, int_t sdaiType, SdaiEntity relClass, SdaiAttr attrParent, Context&)
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
    int_t i = 0;
    while (sdaiGetAggrByIndex(ext, i++, sdaiINSTANCE, &rel)) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(rel, attrChildren, sdaiAGGR, &aggr);
        if (aggr) {
            SdaiInstance ai;
            int_t j = 0;
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
    //inst IfcPropertyDefinition or IfcObjectDefinition
    SdaiAggr aggrAssoc = 0;
    sdaiGetAttrBN(inst, "HasAssociations", sdaiAGGR, &aggrAssoc); 

    SdaiInstance relAssoc = 0;
    int_t i = 0;
    while (sdaiGetAggrByIndex(aggrAssoc, i++, sdaiINSTANCE, &relAssoc)) {

        auto entityAssoc = sdaiGetInstanceType(relAssoc);
        if (entityAssoc == ctx._IfcRelAssociatesClassification()) {

            SdaiInstance clsf = 0;
            sdaiGetAttr(relAssoc, ctx._IfcRelAssociatesClassification_RelatingClassification(), sdaiINSTANCE, &clsf);
            if (clsf) {

                m_valueMatch = false;
                m_systemMatch = false;
                m_uriMatch = false;

                HandleClassificationSelect(clsf, ctx);

                if (m_valueMatch || !m_value.IsSet()) {
                    if (m_systemMatch || !m_system.IsSet()) {
                        if (m_uriMatch || !m_URI.IsSet()) {
                            return true; //>>>>>>>>>>>>>
                        }
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
void FacetClassification::HandleClassificationSelect(SdaiInstance clsf, Context& ctx)
{
    auto clsfType = sdaiGetInstanceType(clsf);
    if (clsfType == ctx._IfcClassificationReference()) {
        HandleClassificationReference(clsf, ctx);
        return;
    }

    if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
        if (clsfType == ctx._IfcClassificationNotation()) {
            HandleClassificationNotation(clsf, ctx);
            return;
        }
    }
    else {
        if (clsfType == ctx._IfcClassification()) {
            HandleClassification(clsf, ctx);
            return;
        }
    }

    auto clsfTypeName = engiGetEntityName(clsfType, sdaiSTRING);
    assert(clsfTypeName);
    LogMsg(ctx, MsgLevel::NotImplemented, "Not implemented IfcClassificationSelect type %s", clsfTypeName ? clsfTypeName : NULL);
    assert(0);
}

/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassificationReference(SdaiInstance clsf, Context& ctx)
{
    if (!m_valueMatch && m_value.IsSet()) {
        const char* value = nullptr;

        if (ctx.GetIfcVersion() == Context::IfcVersion::Ifc2x3) {
            sdaiGetAttr(clsf, ctx._IfcExternalReference_ItemReference(), sdaiSTRING, &value);
        }
        else {
            sdaiGetAttr(clsf, ctx._IfcExternalReference_Identification(), sdaiSTRING, &value);
        }

        m_valueMatch = m_value.Match(value, false);
    }

    if (!m_uriMatch && m_URI.IsSet()) {
        const char* uri = nullptr;
        sdaiGetAttr(clsf, ctx._IfcExternalReference_Location(), sdaiSTRING, &uri);
        m_uriMatch = m_URI.Match(uri, true);
    }

    SdaiInstance source = 0;
    sdaiGetAttr(clsf, ctx._IfcClassificationReference_ReferencedSource(), sdaiINSTANCE, &source);
    HandleClassificationSelect(source, ctx);
}

/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassification(SdaiInstance clsf, Context& ctx)
{
    if (!m_systemMatch && m_system.IsSet()) {
        const char* name = nullptr;
        sdaiGetAttr(clsf, ctx._IfcClassification_Name(), sdaiSTRING, &name);
        m_systemMatch = m_system.Match(name, true);
    }

    if (!m_uriMatch && m_URI.IsSet() && ctx.GetIfcVersion() == Context::IfcVersion::Ifc4) {
        const char* source = nullptr;
        sdaiGetAttr(clsf, ctx._IfcClassification_Location(), sdaiSTRING, &source);
        m_uriMatch = m_URI.Match(source, true);
    }

    if (!m_uriMatch && m_URI.IsSet() && ctx.GetIfcVersion() > Context::IfcVersion::Ifc4) {
        const char* source = nullptr;
        sdaiGetAttr(clsf, ctx._IfcClassification_Specification(), sdaiSTRING, &source);
        m_uriMatch = m_URI.Match(source, true);
    }

    if (!m_uriMatch && m_URI.IsSet()) {
        const char* source = nullptr;
        sdaiGetAttr(clsf, ctx._IfcClassification_Source(), sdaiSTRING, &source);
        m_uriMatch = m_URI.Match(source, true);
    }
}

/// <summary>
/// 
/// </summary>
void FacetClassification::HandleClassificationNotation(SdaiInstance /*clsf*/, Context& ctx)
{
    LogMsg(ctx, MsgLevel::NotImplemented, "IfcClassificationNotation in facet classification");
    assert(!"TODO... if somebody uses");
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
        auto N = engiGetEntityNoAttributes(ent);
        for (int_t i = 0; i < N; i++) {
            auto attr = engiGetEntityAttributeByIndex(ent, i, true, true);
            const char* name = 0;
            engiGetAttributeTraits(attr, &name, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            if (m_name.Match(name, true)) {
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
    bool match = false;

    auto sdaiType = engiGetInstanceAttrType(inst, attr);
    switch (sdaiType) {

        case sdaiBINARY:
        case sdaiENUM:
        case sdaiSTRING: {
            const char* value = nullptr;
            if (sdaiGetAttr(inst, attr, sdaiSTRING, &value)) {
                match = m_value.Match(value, false);
            }
            break;
        }
        case sdaiINTEGER: {
            SdaiInteger value = 0;
            if (sdaiGetAttr(inst, attr, sdaiINTEGER, &value)) {
                match = m_value.Match(value);
            }
            break;
        }
        case sdaiREAL:
        case sdaiNUMBER: {
            double value = 0;
            if (sdaiGetAttr(inst, attr, sdaiREAL, &value)) {
                match = m_value.Match(value);
            }
            break;
        }
        case sdaiBOOLEAN:
        case sdaiINSTANCE:
        case sdaiLOGICAL:
        case sdaiADB:
        case sdaiAGGR:
            LogMsg(ctx, MsgLevel::NotImplemented, "Match this type of attributes");
            assert(0);
            break;
        default:
            LogMsg(ctx, MsgLevel::NotImplemented, "Match unknown attribute type");
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
    int_t i = 0;
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
    int_t i = 0;
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
                int_t j = 0;
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
        const char* name = 0;
        sdaiGetAttr(inst, ctx._IfcRoot_Name(), sdaiSTRING, &name);
        if (!m_name.Match(name, false)) {
            return false;
        }
    }

    auto type = sdaiGetInstanceType(inst);

    if (type == ctx._IfcPropertySet()) {
        SdaiAggr aggr = 0;
        sdaiGetAttr(inst, ctx._IfcPropertySet_HasProperties(), sdaiAGGR, &aggr);
        SdaiInstance prop = 0;
        int_t i = 0;
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
        int_t i = 0;
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
    const char* name = nullptr;
    sdaiGetAttr(prop, ctx._IfcProperty_Name(), sdaiSTRING, &name);
    if (!m_name.Match(name, false)) {
        return false;
    }

    if (!m_value.IsSet()) {
        return true;
    }

    auto entity = sdaiGetInstanceType(prop);

    if (entity == ctx._IfcComplexProperty()) {
        LogMsg(ctx, MsgLevel::NotImplemented, "IfcComplexProperty");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertyBoundedValue()) {
        LogMsg(ctx, MsgLevel::NotImplemented, "_IfcPropertyBoundedValue");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertyEnumeratedValue()) {
        LogMsg(ctx, MsgLevel::NotImplemented, "_IfcPropertyEnumeratedValue");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertyListValue()) {
        LogMsg(ctx, MsgLevel::NotImplemented, "_IfcPropertyListValue");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertyReferenceValue()) {
        LogMsg(ctx, MsgLevel::NotImplemented, "_IfcPropertyReferenceValue");
        assert(0);
        return false;
    }
    else if (entity == ctx._IfcPropertySingleValue()) {
        return MatchPropertySingleValue(prop, ctx);
    }
    else if (entity == ctx._IfcPropertyTableValue()) {
        LogMsg(ctx, MsgLevel::NotImplemented, "_IfcPropertyTableValue");
        assert(0);
        return false;
    }
    else {
        LogMsg(ctx, MsgLevel::NotImplemented, "Unknown entity");
        assert(0);
        return false;
    }
}

/// <summary>
/// 
/// </summary>
bool FacetProperty::MatchQuantity(SdaiInstance qto, Context& ctx)
{
    const char* name = nullptr;
    sdaiGetAttr(qto, ctx._IfcPhysicalQuantity_Name(), sdaiSTRING, &name);
    if (!m_name.Match(name, false)) {
        return false;
    }

    if (!m_value.IsSet()) {
        return true;
    }

    auto entity = sdaiGetInstanceType(qto);

    if (entity == ctx._IfcPhysicalComplexQuantity()) {
        LogMsg(ctx, MsgLevel::NotImplemented, "_IfcPhysicalComplexQuantity");
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
        return m_value.Match(value);
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
        LogMsg(ctx, MsgLevel::NotImplemented, "Unknown entity");
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
            return m_value.Match(value, false);
        }
        case enum_express_attr_type::__STRING:
        {
            const char* value = nullptr;
            sdaiGetADBValue(nominalValue, sdaiSTRING, &value);
            return m_value.Match(value, false);
        }
        case enum_express_attr_type::__ENUMERATION:
        {
            const char* value = nullptr;
            sdaiGetADBValue(nominalValue, sdaiENUM, &value);
            return m_value.Match(value, false);
        }
        case enum_express_attr_type::__BOOLEAN:
        {
            bool value = 0;
            sdaiGetADBValue(nominalValue, sdaiBOOLEAN, &value);
            return m_value.Match(value);
        }
        case enum_express_attr_type::__INTEGER:
        {
            SdaiInteger value = 0;
            sdaiGetADBValue(nominalValue, sdaiINTEGER, &value);
            return m_value.Match(value);
        }
        case enum_express_attr_type::__LOGICAL:
        {
            const char* value = 0;
            sdaiGetADBValue(nominalValue, sdaiLOGICAL, &value);
            return m_value.Match(value, false);
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
            LogMsg(ctx, MsgLevel::NotImplemented, "type of IfcValue");
            assert(0);
            return false;
        }
    }

}