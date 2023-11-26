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
            else { LogMsg(ctx, MsgType::Warning, "Unknown attribute '%s'", attrName.c_str()); } } }


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
            else { LogMsg(ctx, MsgType::Warning, "Unknown child element <%s>", tag.c_str()); } } }



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
    typedef std::list<_xml::_element*> XmlStack;

public:
    Context(Console& con) : m_con(con) {}
    ~Context() { assert(m_xmlStack.empty()); }

    Console& GetConsole() { return m_con; }

    void StartXmlElement(_xml::_element* xml) { assert(xml);  m_xmlStack.push_back(xml); }
    void EndXmlElement(_xml::_element* xml) { assert(xml == m_xmlStack.back()); m_xmlStack.pop_back(); }
    const XmlStack& GetXmlStack() { return m_xmlStack; }

private:
    Console&    m_con;
    XmlStack    m_xmlStack;
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
enum class MsgType
{
    Info, Warning, Error
};

/// <summary>
/// 
/// </summary>
static void LogMsg (Context& ctx, MsgType type, const char* format, ...)
{
    auto& log = ctx.GetConsole();

    //
    log.out("\t<");

    const char* msgType;
    switch (type) {
        case MsgType::Info:     msgType = "info"; break;
        case MsgType::Warning:  msgType = "Warning"; break;
        default:                msgType = "ERROR"; break;
    }

    log.out(msgType);

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
File::File()
{

}

/// <summary>
/// 
/// </summary>
File::~File()
{
    for (auto s : m_specifications) {
        delete s;
    }
    m_specifications.clear();
}

/// <summary>
/// 
/// </summary>
bool File::Read(const char* idsFilePath, Console* output)
{
    bool ok = false;

    DefaultConsole con;
    if (!output) {
        output = &con;
    }

    Context ctx(*output);

    try {
        _xml::_document doc(nullptr);
        doc.load(idsFilePath);

        if (auto root = doc.getRoot()) {
            ctx.StartXmlElement(root);
            Read(*root, ctx);
            ctx.EndXmlElement(root);
        }
    }
    catch (exception& ex) {
        LogMsg(ctx, MsgType::Error, "Failed read IDS file: '%s', error: %s", idsFilePath, ex.what());
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

    m_facets.Read (elem, ctx);
}

/// <summary>
/// 
/// </summary>
Facets::~Facets()
{
    for (auto f : m_facets) {
        delete f;
    }
    m_facets.clear();
}

/// <summary>
/// 
/// </summary>
void IdsValue::Read(_xml::_element& elem, Context& ctx)
{
    GET_CHILD(simpleValue)
    END_CHILDREN
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


#if 0
/// <summary>
/// 
/// </summary>
bool IDS::Check(const char* ifcFilePath, CheckingLog* log, bool untilFirstError)
{
    bool ok = true;

    SdaiModel model = 0;

    for (auto& spec : m_specifications) {
        auto o = spec.Check(model, log, untilFirstError);        
        ok = ok && o;

        if (!ok && untilFirstError) {
            break; //>>>
        }
    }

    return ok;
}

/// <summary>
/// 
/// </summary>
bool Specification::Check(SdaiModel model, CheckingLog* log, bool untilFirstError)
{
    return false;
}
#endif