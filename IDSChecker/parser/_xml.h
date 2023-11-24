#pragma once

#include "_reader.h"
#include "_log.h"
#include "_serialization.h"

#include <set>
#include <map>
#include <vector>
using namespace std;

// ************************************************************************************************
namespace _xml
{
	// ********************************************************************************************
	const char id_attr[] = "id";
	const char href_attr[] = "href";
	const char name_attr[] = "name";
	const char type_attr[] = "type";
	const char abstract_attr[] = "abstract";
	const char substitutionGroup_attr[] = "substitutionGroup";
	const char xmlns_attr[] = "xmlns";
	const char namespace_attr[] = "namespace";
	const char schemaLocation_attr[] = "schemaLocation";
	const char targetNamespace_attr[] = "targetNamespace";	

	/* https://www.w3schools.com/xml/el_schema.asp */
	// Default value: unqualified
	const char elementFormDefault_attr[] = "elementFormDefault";
	const char attributeFormDefault_attr[] = "elementFormDefault";
	const char unqualifiedFormDefault[] = "unqualified";

	const char base_attr[] = "base";
	const char itemType_attr[] = "itemType";
	const char anyType_attr[] = "anyType";
	const char anySimpleType_attr[] = "anySimpleType";

	const char simpleType_elem[] = "simpleType";
	const char complexType_elem[] = "complexType";
	const char element_elem[] = "element";
	const char simpleContent_elem[] = "simpleContent";
	const char complexContent_elem[] = "complexContent";
	const char list_elem[] = "list";
	const char union_elem[] = "union";
	const char annotation_elem[] = "annotation";
	const char extension_elem[] = "extension";
	const char restriction_elem[] = "restriction";
	const char sequence_elem[] = "sequence";
	const char choice_elem[] = "choice";	
	const char attribute_elem[] = "attribute";
	const char anyAttribute_elem[] = "anyAttribute";
	const char attributeGroup_elem[] = "attributeGroup";

	const char string_type[] = "xs:string";

	// ********************************************************************************************
	class _utils;
	class _element;
	class _document;
	class _schema;
	class _infosets;

	// ********************************************************************************************
	class _attribute
	{

	private: // Members

		_element* m_pParent;

		string m_strPrefix;
		string m_strName;
		string m_strUniqueName; // ([prefix:]name)
		string m_strValue;

	public: // Methods

		_attribute(_element* pParent, const string& strName, const string& strValue);

		// serialization
		virtual void serialize(_srln::_archive* pArchive);
		_attribute(_element* pParent, _srln::_archive* pArchive, const string& strTag);

		virtual ~_attribute();

		_element* getParent() const { return m_pParent; }
		const string& getPrefix() const { return m_strPrefix; }
		const string& getName() const { return m_strName; }
		const string& getUniqueName() const { return m_strUniqueName; }
		const string& getValue() const { return m_strValue; }
	};

	// ********************************************************************************************
	class _element_site
	{
		friend class _element;

	protected: // Methods

		virtual void onElementLoaded(_element* pElement) = 0;
		virtual string getNamespace(const string& strPrefix, const string& strName) const = 0;
	};

	// ********************************************************************************************
	class _element
	{
		friend class _utils;

	private: // Members

		_element* m_pParent;
		_element_site* m_pSite;

		string m_strPrefix;
		string m_strName;
		string m_strUniqueName; // ([prefix:]name)
		vector<_attribute*> m_vecAttributes;
		vector<_element*> m_vecChildren;

	protected: // Members

		// <?xml...?>
		// <!--...--> <.../> 
		bool m_bEmpty;
		string m_strContent;

	public: // Methods

		_element(_element* pParent, _element_site* pSite, const string& strTag);

		// serialization
		virtual void serialize(_srln::_archive* pArchive);
		_element(_element* pParent, _element_site* pSite, _srln::_archive* pArchive, const string& strTag);

		virtual ~_element();

		_element* getParent() const { return m_pParent; }
		const string& getPrefix() const { return m_strPrefix; }
		const string& getName() const { return m_strName; }
		const string& getUniqueName() const { return m_strUniqueName; }
		string getNamespace() const;
		string getNamespace(const string& strUniqueName) const;
		string getNamespace(const string& strPrefix, const string& strName) const;
		const vector<_attribute*>& attributes() { return m_vecAttributes; }
		bool hasAttribute(const string& strName) const;
		_attribute* getAttribute(const string& strName) const;
		string getAttributeValue(const string& strName) const;
		const vector<_element*>& children() { return m_vecChildren; }
		const string& getContent() const { return m_strContent; }

		virtual void load(_stream_reader* pReader);

	protected: // Methods

		virtual void parseAttributes(const string& strAttributes);
	};

	// ********************************************************************************************
	/// <summary>
	/// <?xml ... ?>
	/// </summary>
	class _version : public _element
	{

	public: // Methods

		_version(_document* pDocument, const string& strTag);
		virtual ~_version();
	};

	// ********************************************************************************************
	/// <summary>
	/// <?xml ... ?>
	/// </summary>
	class _comment : public _element
	{

	public: // Methods

		_comment(_document* pDocument, const string& strTag);
		virtual ~_comment();
	};

	// ********************************************************************************************
	class _document_site
		: public _log_client
	{
		friend class _document;

	protected: // Methods

		virtual void onElementLoaded(_element* pElement) = 0;
		virtual void loadNamespaceSchemas(const string& strNamespace) = 0;
		virtual _schema* loadSchema(const string& strNamespace, const string& strLocation) = 0;
		virtual void loadSchemas() = 0;
	};

	// ********************************************************************************************
	class _document
		: public _srln::_serializable
		, public _element_site
	{

	private: // Members

		_document_site* m_pSite;

		_stream_reader* m_pReader;

		_version* m_pVersion;
		vector<_element*> m_vecComments;
		map<string, map<string, _schema*>> m_mapSchemas; // namespace : (location : _schema*)

		_element* m_pRoot;
		bool m_bLoadRootOnly;

		map<string, string> m_mapPrefix2Namespace; // (prefix : namespace)

		map<string, string> m_mapMetaData; // user-defined attributes		

		_infosets* m_pInfosets;

	public: // Methods

		_document(_document_site* pSite);
		virtual ~_document();

		// _serializable
		virtual void serialize(_srln::_archive* pArchive) override;
		virtual void deserialize(_srln::_archive* pArchive) override;

	protected: // Methods

		// _element_site
		virtual void onElementLoaded(_element* pElement) override;
		virtual string getNamespace(const string& strPrefix, const string& strName) const override;

	public: // Methods
		
		void load(const char* szFile);
		void load(istream* pStream);

		virtual void loadSchemas();

		virtual void clean();
			
		_document_site* getSite() const { return m_pSite; }
		_element* getRoot() const { return m_pRoot; }
		string getDefaultNamespace(bool bRemoveProtocol) const;
		const map<string, map<string, _schema*>>& getSchemas() const { return m_mapSchemas; }
		_schema*  getSchema(const string& strNamespace, const string& strLocation, bool& bNew);
		const map<string, string>& getPrefix2Namespace() const { return m_mapPrefix2Namespace; }		
		map<string, string>& metaData() { return m_mapMetaData; }		

		_infosets* getInfosets() const { return m_pInfosets; }

		static string getRootName(const char* szFile);
		static string getRootName(istream* pStream);

	protected: // Methods			
		
		virtual void load();

		_schema* loadSchema(const string& strNamespace, const string& strLocation);
		void loadNamespaceSchemas(const string& strNamespace);
		void loadNamespaces();

	private: // Methods

		void loadInfosets();
	};	

	// ********************************************************************************************
	class _schema : public _document
	{

	private: // Members

		string m_strNamespace;
		string m_strLocation;

		// Memory owner - _document
		vector<_schema*> m_vecImports; 
		vector<_schema*> m_vecIncludes;

	public: // Methods

		_schema(_document_site* pSite, const string& strNamespace, const string& strLocation);
		virtual ~_schema();

		// _serializable
		virtual void serialize(_srln::_archive* /*pArchive*/) override;
		virtual void deserialize(_srln::_archive* /*pArchive*/) override;

		// _document
		virtual void loadSchemas() override;
		virtual void clean() override;

	protected: // Methods

		// _document
		virtual void onElementLoaded(_element* pElement) override;

	public: // Methods			

		string getNamespace(bool bRemoveProtocol) const;
		string getLocation(bool bRemoveProtocol) const;
		string getTargetNamespace();		
		string getElementFormDefault();
		string getAttributeFormDefault();
		const vector<_schema*>& getImports() const { return m_vecImports; }
		const vector<_schema*>& getIncludes() const { return m_vecIncludes; }
	};

	// ********************************************************************************************
	// XML Schema Definition (XSD)
	class _infoset
	{

	private: // Members

		string m_strNamespace;
		vector<_schema*> m_vecSchemas;

		// element name : _element*
		// e.g. <xs:element name="CityModel" type="CityModelType" substitutionGroup="gml:_FeatureCollection"/>
		map<string, _element*> m_mapElements;

		// e.g. 
		// <element name = "_CoordinateOperation" type = "gml:AbstractCoordinateOperationType" abstract = "true" substitutionGroup = "gml:Definition" / >
		// <element name="_SingleOperation" type="gml:AbstractCoordinateOperationType" abstract="true" substitutionGroup="gml:_CoordinateOperation">
		map<string, vector<_element*>> m_mapType2Elements;

		// type name (complexType/simpleType) : _element*
		// e.g. <xs:complexType name="CityModelType">
		map<string, _element*> m_mapTypes;

		// attribute name : _element*
		// e.g. <attribute name="uom" type="anyURI">
		map<string, _element*> m_mapAttributes;
		
	public: // Methods

		_infoset(const string& strNamespace);
		~_infoset();

		void loadSchema(_schema* pSchema);

		const vector<_schema*>& getSchemas() const { return m_vecSchemas; }
		const map<string, _element*>& elements() { return m_mapElements; }
		const map<string, vector<_element*>>& type2Elements() { return m_mapType2Elements; }
		const map<string, _element*>& types() { return m_mapTypes; }
		const map<string, _element*>& attributes() { return m_mapAttributes; }

	private: // Methods

		void loadElement(_element* pXSDElement);
		void loadType(_element* pXSDElement);		
		void loadAttribute(_element* pXSDElement);
	};

	// ********************************************************************************************
	class _infosets
	{

	private: // Members

		_document* m_pDocument;

		// namespace : _infoset*
		map<string, _infoset*> m_mapInfosets;

	public: // Methods

		_infosets(_document* pDocument);
		~_infosets();

		void load();

		_element* getXSDElement(_element* pXMLElement) const;		
		_element* getXSDType(_element* pXMLElement) const;
		_element* getXSDType(_element* pXMLElement, const string& strTypeUniqueName) const;		
		_element* getXSDAttribute(_element* pXMLElement, _xml::_attribute* pAttribute) const;
		_element* getXSDTypeBaseElement(_element* pXMLElement, _element* pXSDType, _infoset*& pInfoset) const;
		_element* getXSDElementType(_infoset* pInfoset, _element* pXSDElement) const;
		_element* getXSDElementSubstitutionElement(_infoset* pInfoset, _element* pXSDElement) const;

		const map<string, _infoset*>& infosets() const { return m_mapInfosets; }

	private: // Methods

		_element* searchXSDTypeForXSDElement(_element* pXSDType, const string& strElementName) const;
		_element* searchXSDTypeForXSDElementRecursive(_element* pXSDTypeChild, const string& strElementName) const;

	};

	// ********************************************************************************************
	enum class enumXSDType
	{
		simple = 0,
		complex,
	};

	enum class enumXSDContentType
	{		
		simple = 0,
		complex,
		na,
	};

	// Wrapper
	class _xsd_type
	{

	private: // Members

		_element* m_pXSDType;

	public: // Methods

		_xsd_type(_element* pXSDType);
		virtual ~_xsd_type();

		string getName() const { return getName(m_pXSDType); }
		static string getName(_element* pXSDType);
		enumXSDType getType() const { return getType(m_pXSDType); }
		static enumXSDType getType(_element* pXSDType);
		static bool isGlobalType(_element* pXSDType);
		static bool isNestedType(_element* pXSDType);
		enumXSDContentType getContentType() const { return getContentType(m_pXSDType);  }
		static enumXSDContentType getContentType(_element* pXSDType);
	};

	// ********************************************************************************************
	class _utils
	{

	public: // Methods

		// General		
		static void readTag(_stream_reader* pReader, string& strTag);
		static void readSpecialTag(_stream_reader* pReader, const string& strTagStart, const string& strTagEnd, string& strTag);
		static void readTagName(const string& strTag, string& strName);
		static void readTagAttributes(const string& strTag, string& strAttributes);
		static void getName(const string& strUniqueName, string& strName);
		static void getPrefixAndName(const string& strUniqueName, string& strPrefix, string& strName);

		// XPath
		static _element* getChild(_element* pElement, const string& strName);
		static _element* getChildByAttribute(
			_element* pElement, 
			const string& strName, 
			const string& strAttributeName, 
			const string& strAttributeValue);
		static _element* getChild(_element* pElement, const vector<string>& vecPath);
		static bool hasChild(_element* pElement, const vector<string>& vecChildren);		
		static string getChildAttribute(_element* pElement, const vector<string>& vecPath, const string& strAttribute);
		static void getChildren(_element* pElement, const string& strName, vector<_element*>& vecChildren);
	};
};