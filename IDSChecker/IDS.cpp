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
static void Dump(_xml::_element& elem)
{
    for (auto attr : elem.attributes()) {
        auto& n = attr->getName();
        auto& v = attr->getValue();
        printf(" Attr %s='%s'\n", n.c_str(), v.c_str());
    }

    for (auto child : elem.children()) {
        if (child) {
            auto& name = child->getName();
            printf("  child %s\n", name.c_str());
        }
    }

    auto cont = elem.getContent();
    printf("   content: %s\n", cont.c_str());
}

/// <summary>
/// 
/// </summary>
class RDF::IDS::Context
{
public:
    Context(Console& con_, MsgLevel msgLevel_, bool stopAtFirstError_)
        : console(con_), msgLevel(msgLevel_), stopAtFirstError(stopAtFirstError_)
    {}
    ~Context() {}

public:
    Console&    console;
    MsgLevel    msgLevel;
    bool        stopAtFirstError;

    SdaiInstance    currentInstane = 0;
    Specification*  currentSpecification = nullptr;
};

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
            spec->Reset();
        }
    }
     
    bool ok = false;

    DefaultConsole con;
    if (!output) {
        output = &con;
    }

    Context ctx(*output, msgLevel, stopAtFirstError);

    SdaiModel model = sdaiOpenModelBN((SdaiRep)0, ifcFilePath, "");
    if (model) {

        ok = CheckInstances(model, ctx);

        if (ok || !ctx.stopAtFirstError) {
            if (!CheckSpecificationsUsed(model, ctx)) {
                ok = false;
            }
        }

        sdaiCloseModel(model);
        model = 0;
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
bool File::CheckInstances(SdaiModel model, Context& ctx)
{
    bool ok = true;

    auto aggr = xxxxGetAllInstances(model);
    int i = 0;
    SdaiInstance inst = 0;
    while (sdaiGetAggrByIndex(aggr, i++, sdaiINSTANCE, &inst)) {

        ctx.currentInstane = inst;

        for (auto spec : m_specifications) {
            if (spec) {

                ctx.currentSpecification = spec;

                if (!spec->Check(model, inst, ctx)) {
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
bool File::CheckSpecificationsUsed(SdaiModel model, Context& ctx)
{
    bool ok = true;

    for (auto spec : m_specifications) {
        if (spec) {

            ctx.currentSpecification = spec;

            if (!spec->CheckUsed(model, ctx)) {
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
bool Specification::Check(SdaiModel model, SdaiInstance inst, Context& ctx)
{
    bool ok = true;

    if (SuitableIfcVersion(model)) {
        if (m_applicability.Match(inst)) {
            m_wasMatch = true;
            
            ok = m_requirements.Match(inst);

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
bool Specification::CheckUsed(SdaiModel model, Context& ctx)
{
    bool ok = true;

    if (SuitableIfcVersion(model) && IsRequired()) {
        
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
bool Facets::Match(SdaiInstance inst)
{
    for (auto facet : m_facets) {
        if (!facet->Match(inst)) {
            return false;
        }
    }
    return true;
}
