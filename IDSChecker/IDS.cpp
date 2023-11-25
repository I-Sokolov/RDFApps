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
    log.out("<");

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
        log.out("ids='");
        log.out(xpath.c_str());
        log.out("'");
    }

    //
    log.out(">\n    ");

    char msg[512];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, 511, format, args);
    va_end(args);

    log.out(msg);
    log.out("\n");

    //
    log.out("</");
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
    Dump(elem);

    for (auto child : elem.children()) {
        if (child) {
            auto& name = child->getName();
            if (name == "info") {
                ReadInfo(*child, ctx);
            }
            else if (name == "specifications") {
                ReadSpecifications(*child, ctx);
            }
        }
    }
}

/// <summary>
/// 
/// </summary>
void File::ReadInfo(_xml::_element& elem, Context& ctx)
{
    Dump(elem);
}

/// <summary>
/// 
/// </summary>
void File::ReadSpecifications(_xml::_element& elem, Context& ctx)
{
    Dump(elem);
}

/// <summary>
/// 
/// </summary>
bool Specification::Read(_xml::_element& xml, Context& ctx)
{
    return true;
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