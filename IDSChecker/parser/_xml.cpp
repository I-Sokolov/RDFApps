#include "pch.h"

#include "_xml.h"
#include "_net.h"
#include "_string.h"

#include <chrono>

// ------------------------------------------------------------------------------------------------
namespace _xml	
{
	// ********************************************************************************************
	const char LESS_THAN =			'<';
	const char GREATER_THAN =		'>';
	const char QUESTION_MARK =		'?';
	const char VERSION_START[] =	"?";
	const char VERSION_END[] =		"?";
	const char COMMENT_START[] =	"!--";
	const char COMMENT_END[] =		"--";

	// ********************************************************************************************
	const char import_tag[] = "import";
	const char include_tag[] = "include";

	// ********************************************************************************************
	static const char SCHEMA_TAG[] = "#schema";
	static const char DOCUMENT_TAG[] = "#document";

	// ********************************************************************************************
	_attribute::_attribute(_element* pParent, const string& strName, const string& strValue)
		: m_pParent(pParent)
		, m_strPrefix("")
		, m_strName("")
		, m_strUniqueName("")
		, m_strValue(strValue)
	{
		VERIFY_POINTER(m_pParent);

		if (strName.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		m_strUniqueName = strName;

		_utils::getPrefixAndName(m_strUniqueName, m_strPrefix, m_strName);
	}

	/*virtual*/ void _attribute::serialize(_srln::_archive* pArchive)
	{
		string strEscapedValue = m_strValue;
		_string::replace(strEscapedValue, "\\", "\\\\");

		vector<string> vecData
		{
			m_strPrefix,
			m_strName,
			m_strUniqueName,
			strEscapedValue,
		};

		pArchive->writeObject(this, vecData);
	}

	_attribute::_attribute(_element* pParent, _srln::_archive* pArchive, const string& strTag)
		: m_pParent(pParent)
	{
		VERIFY_POINTER(pArchive);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strTag);

		auto& vecData = pArchive->readData(strTag);
		if (vecData.size() != 4)
		{
			THROW_INTERNAL_ERROR();
		}

		m_strPrefix = vecData[0];
		m_strName = vecData[1];
		m_strUniqueName = vecData[2];
		m_strValue = vecData[3];
		_string::replace(m_strValue, "\\\\", "\\");
	}

	/*virtual*/ _attribute::~_attribute()
	{}

	// ********************************************************************************************
	_element::_element(_element* pParent, _element_site* pSite, const string& strTag)
		: m_pParent(pParent)
		, m_pSite(pSite)
		, m_strPrefix("")
		, m_strName("")
		, m_strUniqueName("")
		, m_vecAttributes()		
		, m_vecChildren()
		, m_bEmpty(false)
		, m_strContent("")
	{
		if (strTag.empty())
		{
			THROW_INTERNAL_ERROR();
		}

		_utils::readTagName(strTag, m_strUniqueName);
		_utils::getPrefixAndName(m_strUniqueName, m_strPrefix, m_strName);

		string strAttributes;
		_utils::readTagAttributes(strTag, strAttributes);
		_string::replace(strAttributes, CHR2STR(TAB), CHR2STR(SPACE));

		parseAttributes(strAttributes);

		m_bEmpty = strTag.back() == SLASH;
	}

	/*virtual*/ void _element::serialize(_srln::_archive* pArchive)
	{
		string strAttributes;
		for (auto pAttribute : m_vecAttributes)
		{
			pAttribute->serialize(pArchive);

			if (!strAttributes.empty())
			{
				strAttributes += ";";
			}

			strAttributes += pArchive->getObjectTag(pAttribute);
		}

		string strChildren;
		for (auto pChild : m_vecChildren)
		{
			pChild->serialize(pArchive);

			if (!strChildren.empty())
			{
				strChildren += ";";
			}

			strChildren += pArchive->getObjectTag(pChild);
		}

		string strEscapedContent = m_strContent;
		_string::replace(strEscapedContent, "\"", "\\\"");

		vector<string> vecData
		{
			m_strPrefix,
			m_strName,
			m_strUniqueName,
			strAttributes,
			strChildren,
			m_bEmpty ? "true" : "false",
			strEscapedContent,
		};		

		pArchive->writeObject(this, vecData);
	}

	_element::_element(_element* pParent, _element_site* pSite, _srln::_archive* pArchive, const string& strTag)
		: m_pParent(pParent)
		, m_pSite(pSite)
	{
		VERIFY_POINTER(pArchive);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strTag);

		auto& vecData = pArchive->readData(strTag);
		if (vecData.size() != 7)
		{
			THROW_INTERNAL_ERROR();
		}

		m_strPrefix = vecData[0];
		m_strName = vecData[1];
		m_strUniqueName = vecData[2];

		/* Attributes */
		vector<string> vecAttributes;
		_string::split(vecData[3], ";", vecAttributes);

		for (auto strAttributeTag : vecAttributes)
		{
			auto pAttribute = new _attribute(this, pArchive, strAttributeTag);
			m_vecAttributes.push_back(pAttribute);
		}

		/* Children */
		vector<string> vecChildren;
		_string::split(vecData[4], ";", vecChildren);

		for (auto strChildTag : vecChildren)
		{
			auto pChild = new _element(this, m_pSite, pArchive, strChildTag);
			m_vecChildren.push_back(pChild);
		}

		m_bEmpty = vecData[5] == "true";
		m_strContent = vecData[6];
		_string::replace(m_strContent, "\\\"", "\"");
	}

	/*virtual*/ _element::~_element()
	{
		for (auto pAttribute : m_vecAttributes)
		{
			delete pAttribute;
		}

		for (auto pChild : m_vecChildren)
		{
			delete pChild;
		}
	}

	string _element::getNamespace() const
	{
		return m_pSite->getNamespace(m_strPrefix, m_strName);
	}

	string _element::getNamespace(const string& strUniqueName) const
	{
		string strPrefix;
		string strName;
		_utils::getPrefixAndName(strUniqueName, strPrefix, strName);

		return m_pSite->getNamespace(strPrefix, strName);
	}

	string _element::getNamespace(const string& strPrefix, const string& strName) const
	{
		return m_pSite->getNamespace(strPrefix, strName);
	}

	bool _element::hasAttribute(const string& strName) const
	{
		return getAttribute(strName) != nullptr;
	}

	_attribute* _element::getAttribute(const string& strName) const
	{
		VERIFY_STLOBJ_IS_NOT_EMPTY(strName);

		auto itAttribute = find_if(m_vecAttributes.begin(), m_vecAttributes.end(),
			[&strName](_attribute* pAttribute) { return pAttribute->getName() == strName; });

		if (itAttribute != m_vecAttributes.end())
		{
			return m_vecAttributes[distance(m_vecAttributes.begin(), itAttribute)];
		}

		return nullptr;
	}

	string _element::getAttributeValue(const string& strName) const
	{
		auto pAttribute = getAttribute(strName);
		if (pAttribute != nullptr)
		{
			return pAttribute->getValue();
		}

		return "";
	}

	/*virtual*/ void _element::load(_stream_reader* pReader)
	{
		if (m_bEmpty)
		{
			m_pSite->onElementLoaded(this);

			return;
		}

		string END_TAG(1, SLASH);
		END_TAG += m_strUniqueName;

		char ch = pReader->getChar();
		while (ch != EOF)
		{
			if ((ch == TAB) || (ch == LF) || (ch == CR))
			{
				if (!m_strContent.empty() && (m_strContent.back() != SPACE))
				{
					m_strContent += SPACE;
				}				

				ch = pReader->getNextChar(false);

				continue;
			}

			if (ch == LESS_THAN)
			{
				string strTag;
				_utils::readTag(pReader, strTag);

				if (strTag.empty())
				{
					THROW_ERROR(_err::_format);
				}

				if (strTag == END_TAG)
				{
					_string::trim(m_strContent);

					m_pSite->onElementLoaded(this);

					return;
				}
				else if (_string::startsEndsWith(strTag, COMMENT_START, COMMENT_END, false))
				{
					// <!--...-->
					ch = pReader->getChar();

					continue;
				}
				else
				{
					if (_string::startsWith(strTag, CHR2STR(SLASH), false))
					{
						THROW_ERROR(_err::_format);
					}

					auto pElement = new _element(this, m_pSite, strTag);
					m_vecChildren.push_back(pElement);

					pElement->load(pReader);

					ch = pReader->getChar();

					continue;
				}
			} // if (ch == LESS_THAN)
			else
			{
				m_strContent += ch;
			}

			ch = pReader->getNextChar(false);
		} // while (ch != EOF)

		THROW_ERROR(_err::_format);
	}

	/*virtual*/ void _element::parseAttributes(const string& strAttributes)
	{
		if (strAttributes.empty())
		{
			return;
		}

		std::stringstream ssAttributes(strAttributes);

		_stream_reader readerAttributes;
		readerAttributes.init(&ssAttributes);

		char ch = readerAttributes.getChar();
		if (ch == EOF)
		{
			THROW_INTERNAL_ERROR();
		}

		string strName;
		bool bValueStart = false;
		char chValueEnd = DOULE_QUOT_MARK;

		string strBuffer;
		while (ch != EOF)
		{
			if ((ch == LF) || (ch == CR))
			{
				strBuffer += SPACE;
			}
			else if (ch == EQUAL)
			{
				if (strName.empty())
				{
					_string::trim(strBuffer);
					if (strBuffer.empty())
					{
						THROW_ERROR(_err::_format);
					}

					// [prefix:]name=...
					strName = strBuffer;
					strBuffer = "";

					ch = readerAttributes.getNextChar(false);

					continue;
				}
			}
			else if ((ch == SINGLE_QUOT_MARK) || (ch == DOULE_QUOT_MARK))
			{
				if (strName.empty())
				{
					THROW_ERROR(_err::_format);
				}

				if (!bValueStart)
				{
					bValueStart = true;
					chValueEnd = ch;

					ch = readerAttributes.getNextChar(false);

					continue;
				}
				else
				{
					if ((ch == chValueEnd) && 
						(strBuffer.empty() || (strBuffer.back() != BACK_SLASH)))
					{
						_string::trim(strBuffer);

						m_vecAttributes.push_back(new _attribute(this, strName, strBuffer));
						
						strBuffer = "";
						strName = "";
						bValueStart = false;
						chValueEnd = DOULE_QUOT_MARK;

						ch = readerAttributes.getNextChar(false);

						continue;
					}
				}
			}

			strBuffer += ch;

			ch = readerAttributes.getNextChar(false);
		} // while (ch != EOF)
	}	

	// ********************************************************************************************
	_version::_version(_document* pDocument, const string& strTag)
		: _element(nullptr, pDocument, strTag)
	{
		m_bEmpty = true;
	}

	/*virtual*/ _version::~_version()
	{}

	// ********************************************************************************************
	_comment::_comment(_document* pDocument, const string& strTag)
		: _element(nullptr, pDocument, "comment")
	{
		m_bEmpty = true;
		m_strContent = strTag;
	}

	/*virtual*/ _comment::~_comment()
	{}	

	// ********************************************************************************************
	_document::_document(_document_site* pSite)
		: m_pSite(pSite)
		, m_pReader(nullptr)
		, m_pVersion(nullptr)
		, m_vecComments()
		, m_mapSchemas()	
		, m_pRoot(nullptr)
		, m_bLoadRootOnly(false)
		, m_mapPrefix2Namespace()
		, m_mapMetaData()
		, m_pInfosets(nullptr)
	{}

	/*virtual*/ _document::~_document()
	{
		clean();
	}

	/*virtual*/ void _document::serialize(_srln::_archive* pArchive) /*override*/
	{
		VERIFY_POINTER(pArchive);
		VERIFY_POINTER(m_pRoot);

		m_pRoot->serialize(pArchive);

		string strPrefix2Namespace;
		for (auto itPrefix2Namespace : m_mapPrefix2Namespace)
		{
			if (!strPrefix2Namespace.empty())
			{
				strPrefix2Namespace += "^|^";
			}

			strPrefix2Namespace += itPrefix2Namespace.first;
			strPrefix2Namespace += "&|&";
			strPrefix2Namespace += itPrefix2Namespace.second;
		}

		vector<string> vecData
		{
			pArchive->getObjectTag(getRoot()),
			strPrefix2Namespace,
		};

		pArchive->writeData(DOCUMENT_TAG, vecData);
	}

	/*virtual*/ void _document::deserialize(_srln::_archive* pArchive) /*override*/
	{
		VERIFY_POINTER(pArchive);

		auto& vecData = pArchive->readData(DOCUMENT_TAG);
		if (vecData.size() != 2)
		{
			THROW_INTERNAL_ERROR();
		}

		vector<string> vecPrefix2Namespace;
		_string::split(vecData[1], "^|^", vecPrefix2Namespace, false);

		for (size_t iMapping = 0; iMapping < vecPrefix2Namespace.size(); iMapping++)
		{
			if (vecPrefix2Namespace[iMapping].empty())
			{
				continue;
			}

			vector<string> vecMapping;
			_string::split(vecPrefix2Namespace[iMapping], "&|&", vecMapping, false);

			if (vecMapping.size() != 2)
			{
				THROW_INTERNAL_ERROR();
			}

			m_mapPrefix2Namespace[vecMapping[0]] = vecMapping[1];
		}
		
		m_pRoot = new _element(nullptr, this, pArchive, vecData[0]);
	}

	/*virtual*/ void _document::onElementLoaded(_element* pElement) /*override*/
	{
		if (m_pSite != nullptr)
		{
			m_pSite->onElementLoaded(pElement);
		}
	}

	/*virtual*/ string _document::getNamespace(const string& strPrefix, const string& strName) const /*override*/
	{
		if (strName.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		if (!strPrefix.empty())
		{
			auto itPrefix2Namespace = m_mapPrefix2Namespace.find(strPrefix);
			if (itPrefix2Namespace == m_mapPrefix2Namespace.end())
			{
				THROW_INTERNAL_ERROR();
			}

			return itPrefix2Namespace->second;
		}

		return getDefaultNamespace(true);
	}	

	void _document::load(const char* szFile)
	{
		clean();

		if (szFile != nullptr)
		{
			// Load a file
			m_pReader = new _file_reader();
			if (!dynamic_cast<_file_reader*>(m_pReader)->open(szFile))
			{
				THROW_ERROR(_err::_file);
			}

			load();
		}
		else
		{
			// Create empty document
			m_pRoot = new _element(nullptr, this, "xml");
		}		
	}	

	void _document::load(istream* pStream)
	{
		clean();

		m_pReader = new _stream_reader();
		m_pReader->init(pStream);

		load();
	}	

	/*virtual*/ void _document::loadSchemas()
	{
		if (m_pRoot == nullptr)
		{
			assert(FALSE);

			return;
		}

		// schemaLocation
		if (m_pRoot->hasAttribute(schemaLocation_attr))
		{
			string strShemaLocationAttr = m_pRoot->getAttributeValue(schemaLocation_attr);

			vector<string> vecTokens;
			_string::split(strShemaLocationAttr, CHR2STR(SPACE), vecTokens);

			// namespace : location
			if ((vecTokens.size() % 2) == 0)
			{
				for (size_t iToken = 0; iToken < vecTokens.size();)
				{
					string strNamespace = vecTokens[iToken++];
					string strLocation = vecTokens[iToken++];

					loadSchema(strNamespace, strLocation);
				}
			}
			else
			{
				m_pSite->logErrf("Invalid format: '%s'.", schemaLocation_attr);
			}			
		} // if (m_pRoot->hasAttribute(xsi_schemaLocation_attr))
		
		// xmlns=namespace
		string strDefaultNamespace = getDefaultNamespace(false);
		if (!strDefaultNamespace.empty())
		{
			loadNamespaceSchemas(strDefaultNamespace);
		}		

		// xmlns:prefix=namespace
		for (auto itPrefix2Namespace : m_mapPrefix2Namespace)
		{
			loadNamespaceSchemas(itPrefix2Namespace.second);
		}

		for (auto itNamespace2Schemas : m_mapSchemas)
		{
			for (auto itLocation2Schema : itNamespace2Schemas.second)
			{
				itLocation2Schema.second->loadSchemas();
			}
		}

		// Load all schemas
		if (m_mapSchemas.empty())
		{
			m_pSite->loadSchemas();
		}
		
		loadInfosets();
	}

	/*virtual*/ void _document::clean()
	{
		delete m_pReader;
		m_pReader = nullptr;

		delete m_pVersion;
		m_pVersion = nullptr;

		for (auto pComment : m_vecComments)
		{
			delete pComment;
		}
		m_vecComments.clear();

		for (auto itNamespace2Schemas : m_mapSchemas)
		{
			for (auto itLocation2Schema : itNamespace2Schemas.second)
			{
				delete itLocation2Schema.second;
			}
		}
		m_mapSchemas.clear();

		m_mapMetaData.clear();

		delete m_pRoot;
		m_pRoot = nullptr; 

		m_mapPrefix2Namespace.clear();

		delete m_pInfosets;
		m_pInfosets = nullptr;
	}

	string _document::getDefaultNamespace(bool bRemoveProtocol) const
	{
		if (getRoot() == nullptr)
		{
			THROW_INTERNAL_ERROR();
		}

		string strNamespace;
		if (getRoot()->hasAttribute(xmlns_attr))
		{
			strNamespace = getRoot()->getAttributeValue(xmlns_attr);
			if (bRemoveProtocol)
			{
				return _net::_url::removeProtocol(strNamespace);
			}
		}

		return strNamespace;
	}

	_schema* _document::getSchema(const string& strNamespace, const string& strLocation, bool& bNew)
	{
		if (strNamespace.empty() || strLocation.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		bNew = false;

		string strNPNamespace = _net::_url::removeProtocol(strNamespace);
		string strNPLocation = _net::_url::removeProtocol(strLocation);

		auto itNamespace2Schemas = m_mapSchemas.find(strNPNamespace);
		if (itNamespace2Schemas != m_mapSchemas.end())
		{
			auto itLocation2Schema = itNamespace2Schemas->second.find(strNPLocation);
			if (itLocation2Schema != itNamespace2Schemas->second.end())
			{
				return itLocation2Schema->second;
			}
		}

		bNew = true;

		auto pSchema = new _schema(m_pSite, strNamespace, strLocation);	
		if (itNamespace2Schemas != m_mapSchemas.end())
		{
			itNamespace2Schemas->second[strNPLocation] = pSchema;
		}
		else
		{
			m_mapSchemas[strNPNamespace] = map<string, _schema*>{ { strNPLocation, pSchema } };
		}

		return pSchema;
	}

	/*static*/ string _document::getRootName(const char* szFile)
	{
		_document document(nullptr);
		document.m_bLoadRootOnly = true;

		document.load(szFile);

		document.m_bLoadRootOnly = false;

		if (document.m_pRoot == nullptr)
		{
			assert(FALSE);

			return "";
		}

		return document.m_pRoot->getName();
	}

	/*static*/ string _document::getRootName(istream* pStream)
	{
		_document document(nullptr);
		document.m_bLoadRootOnly = true;

		document.load(pStream);

		document.m_bLoadRootOnly = false;

		if (document.m_pRoot == nullptr)
		{
			assert(FALSE);

			return "";
		}

		return document.m_pRoot->getName();
	}

	/*virtual*/ void _document::load()
	{
		VERIFY_POINTER(m_pReader);

		char ch = m_pReader->getChar();		

		/* EOF */		
		if (ch == EOF)
		{
			THROW_ERROR(_err::_file);
		}		

		while (ch != EOF)
		{
			if (isblank(ch) || (ch == LF) || (ch == CR))
			{
				ch = m_pReader->getNextChar(false);

				continue;
			}

			if (ch == LESS_THAN)
			{
				string strTag;
				_utils::readTag(m_pReader, strTag);

				if (strTag.empty())
				{
					THROW_ERROR(_err::_format);
				}

				if (_string::startsEndsWith(strTag, VERSION_START, VERSION_END, false))
				{
					// <?xml...?>
					strTag = strTag.substr(1).substr(0, strTag.size() - 2);

					if (m_pVersion != nullptr)
					{
						THROW_ERROR(_err::_format);
					}

					m_pVersion = new _version(this, strTag);
					if (m_pVersion->getName() != "xml")
					{
						THROW_ERROR(_err::_format);
					}

					ch = m_pReader->getChar();

					continue;
				}
				else if (_string::startsEndsWith(strTag, COMMENT_START, COMMENT_END, false))
				{
					// <!--...-->
					auto pComment = new _comment(this, strTag);
					m_vecComments.push_back(pComment);

					ch = m_pReader->getChar();

					continue;
				}
				else
				{
					// Root
					if (m_pRoot != nullptr)
					{
						THROW_ERROR(_err::_format);
					}

					m_pRoot = new _element(nullptr, this, strTag);
					if (m_bLoadRootOnly)
					{
						return;
					}

					m_pRoot->load(m_pReader);

					ch = m_pReader->getChar();

					continue;
				}
			} // if (ch == LESS_THAN)

			ch = m_pReader->getNextChar(false);
		} // while (ch != EOF)

		loadNamespaces();
	}

	_schema* _document::loadSchema(const string& strNamespace, const string& strLocation)
	{
		VERIFY_POINTER(m_pSite);

		return m_pSite->loadSchema(strNamespace, strLocation);
	}

	void _document::loadNamespaceSchemas(const string& strNamespace)
	{
		VERIFY_POINTER(m_pSite);

		return m_pSite->loadNamespaceSchemas(strNamespace);
	}

	void _document::loadNamespaces()
	{
		if (m_pRoot == nullptr)
		{
			assert(FALSE);

			return;
		}

		// xmlns[:prefix]=namespace
		for (auto pAttribute : m_pRoot->attributes())
		{
			// xmlns=...
			if (pAttribute->getUniqueName() == xmlns_attr)
			{
				// default schema
				continue;
			}

			// xmlsn:*=...
			if (pAttribute->getPrefix() != xmlns_attr)
			{
				continue;
			}

			auto& strPrefix = pAttribute->getName();

			string strNPNamespace = _net::_url::removeProtocol(pAttribute->getValue());

			auto itPrefix2Namespace = m_mapPrefix2Namespace.find(strPrefix);
			if (itPrefix2Namespace != m_mapPrefix2Namespace.end())
			{
				if (itPrefix2Namespace->second == strNPNamespace)
				{
					continue;
				}
				else
				{
					THROW_INTERNAL_ERROR();
				}
			}
			else
			{
				m_mapPrefix2Namespace[strPrefix] = strNPNamespace;
			}
		} // for (auto pAttribute ...
	}

	void _document::loadInfosets()
	{
		assert(m_pInfosets == nullptr);

		m_pInfosets = new _infosets(this);
		m_pInfosets->load();
	}

	// ********************************************************************************************
	_schema::_schema(_document_site* pSite, const string& strNamespace, const string& strLocation)
		: _document(pSite)
		, m_strNamespace(strNamespace)
		, m_strLocation(strLocation)
		, m_vecImports()
		, m_vecIncludes()
	{
		VERIFY_POINTER(pSite);

		if (m_strNamespace.empty() || m_strLocation.empty())
		{
			THROW_ARGUMENT_ERROR();
		}
	}

	/*virtual*/ _schema::~_schema()
	{}

	/*virtual*/ void _schema::serialize(_srln::_archive* pArchive) /*override*/
	{
		VERIFY_POINTER(pArchive);

		_document::serialize(pArchive);

		string strImports;
		for (auto pImport : m_vecImports)
		{
			if (!strImports.empty())
			{
				strImports += "^|^";
			}

			strImports += pImport->getNamespace(false);
			strImports += "^|^";
			strImports += pImport->getLocation(false);
		}

		string strIncludes;
		for (auto pInclude : m_vecIncludes)
		{
			if (!strIncludes.empty())
			{
				strIncludes += "^|^";
			}

			strIncludes += pInclude->getNamespace(false);
			strIncludes += "^|^";
			strIncludes += pInclude->getLocation(false);
		}

		vector<string> vecData
		{
			m_strNamespace,
			m_strLocation,
			strImports,
			strIncludes,
		};

		pArchive->writeData(SCHEMA_TAG, vecData);
	}

	/*virtual*/ void _schema::deserialize(_srln::_archive* pArchive) /*override*/
	{
		VERIFY_POINTER(pArchive);

		_document::deserialize(pArchive);

		auto& vecData = pArchive->readData(SCHEMA_TAG);
		if (vecData.size() != 4)
		{
			THROW_INTERNAL_ERROR();
		}

		m_strNamespace = vecData[0];
		m_strLocation = vecData[1];

		vector<string> vecImports;
		_string::split(vecData[2], "^|^", vecImports, false);

		if (vecImports.size() >= 2)
		{
			for (size_t iImport = 0; iImport < vecImports.size(); iImport += 2)
			{
				auto pSchema = loadSchema(
					vecImports[iImport],
					vecImports[iImport + 1]);
				m_vecImports.push_back(pSchema);
			}
		}

		vector<string> vecIncludes;
		_string::split(vecData[3], "^|^", vecIncludes, false);

		if (vecIncludes.size() >= 2)
		{
			for (size_t iInclude = 0; iInclude < vecIncludes.size(); iInclude += 2)
			{
				auto pSchema = loadSchema(
					vecIncludes[iInclude],
					vecIncludes[iInclude + 1]);
				m_vecIncludes.push_back(pSchema);
			}
		}
	}

	/*virtual*/ void _schema::loadSchemas() /*override*/
	{
		if (getRoot() == nullptr)
		{
			return;
		}

		// xmlns=namespace
		string strDefaultNamespace = getDefaultNamespace(false);
		if (!strDefaultNamespace.empty())
		{
			loadNamespaceSchemas(strDefaultNamespace);
		}

		// xmlns:prefix=namespace
		for (auto itPrefix2Namespace : getPrefix2Namespace())
		{
			loadNamespaceSchemas(itPrefix2Namespace.second);
		}
	}

	/*virtual*/ void _schema::clean() /*override*/
	{
		_document::clean();

		m_vecImports.clear();
		m_vecIncludes.clear();
	}

	/*virtual*/ void _schema::onElementLoaded(_element* pElement) /*override*/
	{
		// <import
		if (pElement->getName() == import_tag)
		{
			if (!pElement->hasAttribute(namespace_attr) ||
				!pElement->hasAttribute(schemaLocation_attr))
			{
				THROW_INTERNAL_ERROR();
			}

			auto strNamespace = pElement->getAttributeValue(namespace_attr);
			auto strLocation = pElement->getAttributeValue(schemaLocation_attr);

			auto pSchema = loadSchema(
				strNamespace,
				strLocation);
			m_vecImports.push_back(pSchema);

			return;
		} // import_tag)

		// <include
		if (pElement->getName() == include_tag)
		{
			auto strLocation = pElement->getAttributeValue(schemaLocation_attr);

			auto pSchema = loadSchema(
				m_strNamespace,
				strLocation);
			m_vecIncludes.push_back(pSchema);

			return;
		} // include_tag
	}

	string _schema::getNamespace(bool bRemoveProtocol) const
	{ 
		if (bRemoveProtocol)
		{
			_net::_url::removeProtocol(m_strNamespace);
		}

		return m_strNamespace;
	}

	string _schema::getLocation(bool bRemoveProtocol) const
	{
		if (bRemoveProtocol)
		{
			_net::_url::removeProtocol(m_strLocation);
		}

		return m_strLocation;
	}

	string _schema::getTargetNamespace()
	{
		if (getRoot() == nullptr)
		{
			THROW_INTERNAL_ERROR();
		}

		if (!getRoot()->hasAttribute(targetNamespace_attr))
		{
			THROW_INTERNAL_ERROR();
		}

		return getRoot()->getAttributeValue(targetNamespace_attr);
	}

	string _schema::getElementFormDefault()
	{
		if (getRoot() == nullptr)
		{
			THROW_INTERNAL_ERROR();
		}

		if (!getRoot()->hasAttribute(elementFormDefault_attr))
		{
			return unqualifiedFormDefault;
		}

		return getRoot()->getAttributeValue(elementFormDefault_attr);
	}

	string _schema::getAttributeFormDefault()
	{
		if (getRoot() == nullptr)
		{
			THROW_INTERNAL_ERROR();
		}

		if (!getRoot()->hasAttribute(attributeFormDefault_attr))
		{
			return unqualifiedFormDefault;
		}

		return getRoot()->getAttributeValue(attributeFormDefault_attr);
	}

	// ********************************************************************************************
	_infoset::_infoset(const string& strNamespace)
		: m_strNamespace(strNamespace)
		, m_vecSchemas()
		, m_mapElements()
		, m_mapType2Elements()
		, m_mapTypes()		
		, m_mapAttributes()
	{}

	_infoset::~_infoset()
	{}

	void _infoset::loadSchema(_schema* pSchema)
	{
		VERIFY_POINTER(pSchema);

		for (auto pElement : pSchema->getRoot()->children())
		{
			if (pElement->getName() == element_elem)			
			{
				loadElement(pElement);		
			}
			else if ((pElement->getName() == simpleType_elem) ||
				(pElement->getName() == complexType_elem))
			{
				loadType(pElement);
			}
			else if (pElement->getName() == attribute_elem)
			{
				loadAttribute(pElement);
			}
		}

		m_vecSchemas.push_back(pSchema);
	}

	void _infoset::loadElement(_element* pXSDElement)
	{
		VERIFY_POINTER(pXSDElement);

		if (!pXSDElement->hasAttribute(name_attr))
		{
			assert(FALSE);

			return;
		}

		string strNameAttribute = pXSDElement->getAttributeValue(name_attr);		
		if (m_mapElements.find(strNameAttribute) != m_mapElements.end())
		{
			THROW_INTERNAL_ERROR();
		}

		m_mapElements[strNameAttribute] = pXSDElement;
		
		if (pXSDElement->hasAttribute(type_attr))
		{
			string strTypeAttribute = pXSDElement->getAttributeValue(type_attr);
			
			vector<string> vecTokens;
			_string::tokenize(strTypeAttribute, CHR2STR(COLON), vecTokens);

			if (vecTokens.empty() || (vecTokens.size() > 2))
			{
				THROW_INTERNAL_ERROR();
			}

			strTypeAttribute = vecTokens.size() == 1 ? vecTokens[0] : vecTokens[1];
			if (m_mapType2Elements.find(strTypeAttribute) == m_mapType2Elements.end())
			{
				m_mapType2Elements[strTypeAttribute] = vector<_element*>{ pXSDElement };
			}
			else
			{
				m_mapType2Elements.at(strTypeAttribute).push_back(pXSDElement);
			}			
		}		
	}

	void _infoset::loadType(_element* pXSDElement)
	{
		VERIFY_POINTER(pXSDElement);

		if (!pXSDElement->hasAttribute(name_attr))
		{
			THROW_INTERNAL_ERROR();
		}

		string strNameAttribute = pXSDElement->getAttributeValue(name_attr);
		if (m_mapTypes.find(strNameAttribute) != m_mapTypes.end())
		{
			THROW_INTERNAL_ERROR();
		}

		m_mapTypes[strNameAttribute] = pXSDElement;
	}	

	void _infoset::loadAttribute(_element* pXSDElement)
	{
		VERIFY_POINTER(pXSDElement);

		if (!pXSDElement->hasAttribute(name_attr))
		{
			assert(FALSE);

			return;
		}

		string strNameAttribute = pXSDElement->getAttributeValue(name_attr);
		if (m_mapAttributes.find(strNameAttribute) != m_mapAttributes.end())
		{
			THROW_INTERNAL_ERROR();
		}

		m_mapAttributes[strNameAttribute] = pXSDElement;
	}

	// ********************************************************************************************
	_infosets::_infosets(_document* pDocument)
		: m_pDocument(pDocument)
		, m_mapInfosets()
	{
		VERIFY_POINTER(m_pDocument);
	}

	_infosets::~_infosets()
	{
		for (auto infoset : m_mapInfosets)
		{
			delete infoset.second;
		}
	}

	void _infosets::load()
	{
		auto& mapSchemas = m_pDocument->getSchemas();

		for (auto itNamespace2Schemas : mapSchemas)
		{
			auto pInfoset = new _infoset(itNamespace2Schemas.first);			

			for (auto itLocation2Schema : itNamespace2Schemas.second)
			{
				auto pSchema = itLocation2Schema.second;

				if (pSchema->getRoot() != nullptr)
				{
					pInfoset->loadSchema(pSchema);
				}
			}

			m_mapInfosets[itNamespace2Schemas.first] = pInfoset;
		}
	}

	_element* _infosets::getXSDElement(_element* pXMLElement) const
	{
		VERIFY_POINTER(pXMLElement);

		string strNamespace = pXMLElement->getNamespace();
		if (strNamespace.empty())
		{
			THROW_INTERNAL_ERROR();
		}

		auto itInfoset = m_mapInfosets.find(strNamespace);
		if (itInfoset == m_mapInfosets.end())
		{
			m_pDocument->getSite()->logErrf("Unknown namespace: '%s'.", strNamespace.c_str());

			return nullptr;
		}

		auto itElement = itInfoset->second->elements().find(pXMLElement->getName());
		if (itElement != itInfoset->second->elements().end())
		{
			return itElement->second;
		}

		auto pXMLParent = pXMLElement->getParent();
		VERIFY_POINTER(pXMLParent);

		auto pParentXSDType = getXSDType(pXMLParent);
		VERIFY_POINTER(pParentXSDType);

		return searchXSDTypeForXSDElement(pParentXSDType, pXMLElement->getName());
	}

	_element* _infosets::getXSDType(_element* pXMLElement) const
	{
		VERIFY_POINTER(pXMLElement);

		auto pXSDElement = getXSDElement(pXMLElement);
		if (pXSDElement == nullptr)
		{
			m_pDocument->getSite()->logErrf("Unknown type: '%s'.", pXMLElement->getUniqueName().c_str());

			return nullptr;
		}

		if (pXSDElement->hasAttribute(type_attr))
		{
			string strTypeAttribute = pXSDElement->getAttributeValue(type_attr);

			return getXSDType(pXSDElement, strTypeAttribute);
		}

		// Check for Nested type
		auto pChild = _utils::getChild(pXSDElement, complexType_elem);
		if (pChild == nullptr)
		{
			pChild = _utils::getChild(pXSDElement, simpleType_elem);
		}

		if (pChild != nullptr)
		{
			return pChild;
		}

		THROW_INTERNAL_ERROR();
	}

	_element* _infosets::getXSDType(_element* pXMLElement, const string& strTypeUniqueName) const
	{
		VERIFY_POINTER(pXMLElement);

		if (strTypeUniqueName.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		string strTypePrefix;
		string strTypeName;
		_utils::getPrefixAndName(strTypeUniqueName, strTypePrefix, strTypeName);

		string strTypeNamespace = pXMLElement->getNamespace(strTypePrefix, strTypeName);

		auto itInfoset = m_mapInfosets.find(strTypeNamespace);
		if (itInfoset == m_mapInfosets.end())
		{			
			THROW_INTERNAL_ERROR();
		}

		auto itType = itInfoset->second->types().find(strTypeName);
		if (itType != itInfoset->second->types().end())
		{
			return itType->second;
		}

		return nullptr;
	}

	_element* _infosets::getXSDAttribute(_element* pXMLElement, _xml::_attribute* pAttribute) const
	{
		VERIFY_POINTER(pXMLElement);
		VERIFY_POINTER(pAttribute);

		string strAttributeNamespace = pXMLElement->getNamespace(pAttribute->getPrefix(), pAttribute->getName());

		auto itInfoset = m_mapInfosets.find(strAttributeNamespace);
		if (itInfoset == m_mapInfosets.end())
		{
			THROW_INTERNAL_ERROR();
		}

		auto itAttribute = itInfoset->second->attributes().find(pAttribute->getName());
		if (itAttribute != itInfoset->second->attributes().end())
		{
			return itAttribute->second;
		}

		return nullptr;
	}

	_element* _infosets::getXSDTypeBaseElement(_element* pXMLElement, _element* pXSDType, _infoset*& pInfoset) const
	{
		VERIFY_POINTER(pXMLElement);
		VERIFY_POINTER(pXSDType);

		pInfoset = nullptr;

		string strNamespace = pXMLElement->getNamespace();
		if (strNamespace.empty())
		{
			THROW_INTERNAL_ERROR();
		}

		auto itInfoset = m_mapInfosets.find(strNamespace);
		if (itInfoset == m_mapInfosets.end())
		{
			m_pDocument->getSite()->logErrf("Unknown namespace: '%s'.", strNamespace.c_str());

			return nullptr;
		}

		pInfoset = itInfoset->second;

		string strXSDType = _xml::_xsd_type::getName(pXSDType);

		vector<string> vecTokens;
		_string::tokenize(strXSDType, CHR2STR(COLON), vecTokens);

		if (vecTokens.empty() || (vecTokens.size() > 2))
		{
			THROW_INTERNAL_ERROR();
		}

		strXSDType = vecTokens.size() == 1 ? vecTokens[0] : vecTokens[1];

		// Schemas
		auto itType2Element = itInfoset->second->type2Elements().find(strXSDType);
		if (itType2Element != itInfoset->second->type2Elements().end())
		{
			if (itType2Element->second.size() > 1)
			{
				for (auto pElement : itType2Element->second)
				{
					if (pXMLElement->getName() == pElement->getAttributeValue(name_attr))
					{
						string strTypeAttribute = pElement->getAttributeValue(type_attr);
						if (!strTypeAttribute.empty())
						{
							string strPrefix;
							string strName;
							_utils::getPrefixAndName(strTypeAttribute, strPrefix, strName);

							if (strXSDType == strName)
							{
								return pElement;
							}
						}						
					}
				} // for (auto pElement ...
			} // if (itType2Element->second.size() > 1)
			else
			{
				return itType2Element->second[0];
			}
		} // if (itType2Element != ...

		// Imports & Includes
		for (auto pSchema : itInfoset->second->getSchemas())
		{
			// Imports
			for (auto pImport : pSchema->getImports())
			{
				itInfoset = m_mapInfosets.find(pImport->getNamespace(true));
				if (itInfoset == m_mapInfosets.end())
				{
					continue;
				}

				itType2Element = itInfoset->second->type2Elements().find(strXSDType);
				if (itType2Element != itInfoset->second->type2Elements().end())
				{
					if (itType2Element->second.size() > 1)
					{
						for (auto pElement : itType2Element->second)
						{
							if (pXMLElement->getName() == pElement->getAttributeValue(name_attr))
							{
								string strTypeAttribute = pElement->getAttributeValue(type_attr);
								if (!strTypeAttribute.empty())
								{
									string strPrefix;
									string strName;
									_utils::getPrefixAndName(strTypeAttribute, strPrefix, strName);

									if (strXSDType == strName)
									{
										return pElement;
									}
								}
							}
						} // for (auto pElement ...
					} // if (itType2Element->second.size() > 1)
					else
					{
						return itType2Element->second[0];
					}
				} // if (itType2Element != ...
			} // for (auto pImport ...

			// Includes
			for (auto pInclude : pSchema->getIncludes())
			{
				itInfoset = m_mapInfosets.find(pInclude->getNamespace(true));
				if (itInfoset == m_mapInfosets.end())
				{
					continue;
				}

				itType2Element = itInfoset->second->type2Elements().find(strXSDType);
				if (itType2Element != itInfoset->second->type2Elements().end())
				{
					if (itType2Element->second.size() > 1)
					{
						for (auto pElement : itType2Element->second)
						{
							if (pXMLElement->getName() == pElement->getAttributeValue(name_attr))
							{
								string strTypeAttribute = pElement->getAttributeValue(type_attr);
								if (!strTypeAttribute.empty())
								{
									string strPrefix;
									string strName;
									_utils::getPrefixAndName(strTypeAttribute, strPrefix, strName);

									if (strXSDType == strName)
									{
										return pElement;
									}
								}
							}
						} // for (auto pElement ...
					} // if (itType2Element->second.size() > 1)
					else
					{
						return itType2Element->second[0];
					}
				} // if (itType2Element != ...
			} // for (auto pInclude ...
		} // for (auto pSchema : 

		return nullptr;
	}

	_element* _infosets::getXSDElementType(_infoset* pInfoset, _element* pXSDElement) const
	{
		VERIFY_POINTER(pInfoset);
		VERIFY_POINTER(pXSDElement);

		string strTypeAttribute = pXSDElement->getAttributeValue(_xml::type_attr);
		if (strTypeAttribute.empty())
		{
			return nullptr;
		}

		string strXSDType = strTypeAttribute;

		vector<string> vecTokens;
		_string::tokenize(strXSDType, CHR2STR(COLON), vecTokens);

		if (vecTokens.empty() || (vecTokens.size() > 2))
		{
			THROW_INTERNAL_ERROR();
		}

		strXSDType = vecTokens.size() == 1 ? vecTokens[0] : vecTokens[1];

		// Schemas
		auto itType = pInfoset->types().find(strXSDType);
		if ((itType != pInfoset->types().end()))
		{
			return itType->second;
		}

		// Imports & Includes
		for (auto pSchema : pInfoset->getSchemas())
		{
			// Imports
			for (auto pImport : pSchema->getImports())
			{
				auto itInfoset = m_mapInfosets.find(pImport->getNamespace(true));
				if (itInfoset == m_mapInfosets.end())
				{
					continue;
				}

				itType = itInfoset->second->types().find(strXSDType);
				if ((itType != itInfoset->second->types().end()))
				{
					return itType->second;
				}
			} // for (auto pImport ...

			// Includes
			for (auto pInclude : pSchema->getIncludes())
			{
				auto itInfoset = m_mapInfosets.find(pInclude->getNamespace(true));
				if (itInfoset == m_mapInfosets.end())
				{
					continue;
				}

				itType = itInfoset->second->types().find(strXSDType);
				if ((itType != itInfoset->second->types().end()))
				{
					return itType->second;
				}
			} // for (auto pInclude ...
		} // for (auto pSchema : 

		return nullptr;
	}

	_element* _infosets::getXSDElementSubstitutionElement(_infoset* pInfoset, _element* pXSDElement) const
	{
		VERIFY_POINTER(pInfoset);
		VERIFY_POINTER(pXSDElement);

		string strSubstitutionGroup = pXSDElement->getAttributeValue(_xml::substitutionGroup_attr);
		if (strSubstitutionGroup.empty())
		{
			return nullptr;
		}

		string strXSDType = strSubstitutionGroup;

		vector<string> vecTokens;
		_string::tokenize(strXSDType, CHR2STR(COLON), vecTokens);

		if (vecTokens.empty() || (vecTokens.size() > 2))
		{
			THROW_INTERNAL_ERROR();
		}

		strXSDType = vecTokens.size() == 1 ? vecTokens[0] : vecTokens[1];

		auto itElement = pInfoset->elements().find(strXSDType);
		if ((itElement != pInfoset->elements().end()))
		{
			return itElement->second;
		}

		// Imports & Includes
		for (auto pSchema : pInfoset->getSchemas())
		{
			// Imports
			for (auto pImport : pSchema->getImports())
			{
				auto itInfoset = m_mapInfosets.find(pImport->getNamespace(true));
				if (itInfoset == m_mapInfosets.end())
				{
					continue;
				}

				itElement = itInfoset->second->elements().find(strXSDType);
				if ((itElement != itInfoset->second->elements().end()))
				{
					return itElement->second;
				}
			} // for (auto pImport ...

			// Includes
			for (auto pInclude : pSchema->getIncludes())
			{
				auto itInfoset = m_mapInfosets.find(pInclude->getNamespace(true));
				if (itInfoset == m_mapInfosets.end())
				{
					continue;
				}

				itElement = itInfoset->second->elements().find(strXSDType);
				if ((itElement != itInfoset->second->elements().end()))
				{
					return itElement->second;
				}
			} // for (auto pInclude ...
		} // for (auto pSchema : 

		return nullptr;
	}

	_element* _infosets::searchXSDTypeForXSDElement(_element* pXSDType, const string& strElementName) const
	{
		VERIFY_POINTER(pXSDType);

		if (strElementName.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		if (_xsd_type::isGlobalType(pXSDType))
		{
			// complexType/compexContent/extension/...
			auto pExtensionElement = _utils::getChild(pXSDType, vector<string>{ complexContent_elem, extension_elem });
			if (pExtensionElement != nullptr)
			{
				auto pXSDElement = searchXSDTypeForXSDElementRecursive(pExtensionElement, strElementName);
				if (pXSDElement == nullptr)
				{
					if (pExtensionElement->hasAttribute(base_attr))
					{
						string strBaseXSDType = pExtensionElement->getAttributeValue(base_attr);

						auto pBaseXSDType = getXSDType(pXSDType, strBaseXSDType);
						VERIFY_POINTER(pBaseXSDType);

						pXSDElement = searchXSDTypeForXSDElement(pBaseXSDType, strElementName);
					}
				}

				if (pXSDElement == nullptr)
				{
					THROW_INTERNAL_ERROR();
				}

				return pXSDElement;
			} // if (pExtensionElement != nullptr)
			
			// complexType/compexContent/restriction/...
			auto pRestrictionElement = _utils::getChild(pXSDType, vector<string>{ complexContent_elem, restriction_elem });
			if (pRestrictionElement != nullptr)
			{
				auto pXSDElement = searchXSDTypeForXSDElementRecursive(pRestrictionElement, strElementName);
				if (pXSDElement == nullptr)
				{
					if (pRestrictionElement->hasAttribute(base_attr))
					{
						string strBaseXSDType = pRestrictionElement->getAttributeValue(base_attr);

						auto pBaseXSDType = getXSDType(pXSDType, strBaseXSDType);
						VERIFY_POINTER(pBaseXSDType);

						pXSDElement = searchXSDTypeForXSDElement(pBaseXSDType, strElementName);
					}
				}

				if (pXSDElement == nullptr)
				{
					THROW_INTERNAL_ERROR();
				}

				return pXSDElement;
			}
			
			// complexType/...
			auto pXSDElement = searchXSDTypeForXSDElementRecursive(pXSDType, strElementName);
			if (pXSDElement == nullptr)
			{
				THROW_INTERNAL_ERROR();
			}

			return pXSDElement;
		} // if (_xsd_type::isGlobalType ...)
		else
		{
			return searchXSDTypeForXSDElementRecursive(pXSDType, strElementName);
		} // else if (_xsd_type::isGlobalType ...)
	}

	_element* _infosets::searchXSDTypeForXSDElementRecursive(_element* pXSDTypeChild, const string& strElementName) const
	{
		VERIFY_POINTER(pXSDTypeChild);

		if (strElementName.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		for (auto pChild : pXSDTypeChild->children())
		{
			if (pChild->getName() == element_elem)
			{
				if (pChild->getAttributeValue(name_attr) == strElementName)
				{
					return pChild;
				}
			}
			else if (pChild->getName() == sequence_elem)
			{
				auto pXSDElement = searchXSDTypeForXSDElementRecursive(pChild, strElementName);
				if (pXSDElement != nullptr)
				{
					return pXSDElement;
				}
			}
			else if (pChild->getName() == choice_elem)
			{
				auto pXSDElement = searchXSDTypeForXSDElementRecursive(pChild, strElementName);
				if (pXSDElement != nullptr)
				{
					return pXSDElement;
				}
			}
			else if (pChild->getName() == annotation_elem)
			{
				// NA
			}
			else if(pChild->getName() == attribute_elem)
			{
				// NA
			}
			else
			{
				assert(false);
			}
		} // for (auto pChild ...

		return nullptr;
	}

	// ********************************************************************************************
	_xsd_type::_xsd_type(_element* pXSDType)
		: m_pXSDType(pXSDType)
	{
		VERIFY_POINTER(m_pXSDType);
	}

	/*virtual*/ _xsd_type::~_xsd_type()
	{}

	/*static*/ string _xsd_type::getName(_element* pXSDType) 
	{ 
		VERIFY_POINTER(pXSDType);

		string strName = pXSDType->getAttributeValue(name_attr);
		if (!strName.empty())
		{
			return strName;
		}
		
		// Nested type
		return getName(pXSDType->getParent());
	}

	/*static*/ enumXSDType _xsd_type::getType(_element* pXSDType)
	{
		VERIFY_POINTER(pXSDType);

		if (pXSDType->getName() == simpleType_elem)
		{
			return enumXSDType::simple;
		}
		else if (pXSDType->getName() == complexType_elem)
		{
			return enumXSDType::complex;
		}

		THROW_INTERNAL_ERROR();
	}

	/*static*/ bool _xsd_type::isGlobalType(_element* pXSDType)
	{
		VERIFY_POINTER(pXSDType);

		return pXSDType->hasAttribute(name_attr);
	}

	/*static*/ bool _xsd_type::isNestedType(_element* pXSDType)
	{
		VERIFY_POINTER(pXSDType);

		return !pXSDType->hasAttribute(name_attr);
	}

	/*static*/ enumXSDContentType _xsd_type::getContentType(_element* pXSDType)
	{
		VERIFY_POINTER(pXSDType);

		if (_utils::getChild(pXSDType, vector<string>{ simpleContent_elem }) != nullptr)
		{
			return enumXSDContentType::simple;
		}
		else if (_utils::getChild(pXSDType, vector<string>{ complexContent_elem }) != nullptr)
		{
			return enumXSDContentType::complex;
		}

		return enumXSDContentType::na;
	}

	// ********************************************************************************************
	/*static*/ void _utils::readTag(_stream_reader* pReader, string& strTag)
	{
		VERIFY_POINTER(pReader);
		VERIFY_STLOBJ_IS_EMPTY(strTag);

		bool bCheckForSpecialTag = true;

		char ch = pReader->getChar();
		if (ch != LESS_THAN)
		{
			THROW_INTERNAL_ERROR();
		}

		ch = pReader->getNextChar(true);
		while (true)
		{
			if ((ch != CR) && (ch != LF))
			{
				strTag += ch;
			}
			else
			{
				if (!strTag.empty() && (strTag.back() != SPACE))
				{
					strTag += SPACE;
				}
			}

			// VERSION_START, COMMENT_START
			if (bCheckForSpecialTag && (strTag.size() >= 3))
			{
				bCheckForSpecialTag = false;

				if (_string::startsWith(strTag, VERSION_START, false))
				{
					// <?xml...?>
					readSpecialTag(pReader, VERSION_START, VERSION_END, strTag);

					break;
				}
				else if (_string::startsWith(strTag, COMMENT_START, false))
				{
					// <!--...-->
					readSpecialTag(pReader, COMMENT_START, COMMENT_END, strTag);

					break;
				}
			} // if (bCheckForSpecialTag && ...

			ch = pReader->getNextChar(true);			

			if (ch == GREATER_THAN)
			{
				break;
			}
		} // while (true)

		pReader->goNext();
	}

	/*static*/ void _utils::readSpecialTag(_stream_reader* pReader, const string& strTagStart, const string& strTagEnd, string& strTag)
	{
		VERIFY_POINTER(pReader);

		if (!_string::startsWith(strTag, strTagStart))
		{
			THROW_ARGUMENT_ERROR();
		}

		char ch = pReader->getNextChar(true);
		while (true)
		{
			if ((ch != CR) && (ch != LF))
			{
				strTag += ch;
			}

			ch = pReader->getNextChar(true);

			if (ch == GREATER_THAN)
			{
				if (_string::endsWith(strTag, strTagEnd, false))
				{
					break;
				}
				else
				{
					continue;
				}

				break;
			} // if (ch == GREATER_THAN)
		} // while (true)
	}

	/*static*/ void _utils::readTagName(const string& strTag, string& strName)
	{
		if (strTag.empty() || !strName.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		strName = strTag;

		size_t iPos = strTag.find_first_of(SPACE_TAB);
		if (iPos != string::npos)
		{
			strName = strTag.substr(0, iPos);
		}

		if (strName.back() == SLASH)
		{
			strName = strName.substr(0, strName.size() - 1);
		}

		_string::trim(strName);
	}

	/*static*/ void _utils::readTagAttributes(const string& strTag, string& strAttributes)
	{
		if (strTag.empty() || !strAttributes.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		size_t iPos = strTag.find_first_of(SPACE_TAB);
		if (iPos != string::npos)
		{
			strAttributes = strTag.substr(iPos);
			if (strAttributes.back() == SLASH)
			{
				strAttributes = strAttributes.substr(0, strAttributes.size() - 1);
			}

			_string::trim(strAttributes);
		}
	}

	/*static*/ void _utils::getName(const string& strUniqueName, string& strName)
	{
		if (strUniqueName.empty() || !strName.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		strName = strUniqueName;

		vector<string> vecTokens;
		_string::tokenize(strUniqueName, CHR2STR(COLON), vecTokens);

		if (vecTokens.empty() || (vecTokens.size() > 2))
		{
			THROW_INTERNAL_ERROR();
		}

		if (vecTokens.size() == 2)
		{
			strName = vecTokens[1];
		}
	}

	/*static*/ void _utils::getPrefixAndName(const string& strUniqueName, string& strPrefix, string& strName)
	{
		if (strUniqueName.empty() || !strPrefix.empty() || !strName.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		strName = strUniqueName;

		vector<string> vecTokens;
		_string::tokenize(strUniqueName, CHR2STR(COLON), vecTokens);

		if (vecTokens.empty() || (vecTokens.size() > 2))
		{
			THROW_INTERNAL_ERROR();
		}

		if (vecTokens.size() == 2)
		{
			strPrefix = vecTokens[0];
			strName = vecTokens[1];
		}
	}

	/*static*/ _element* _utils::getChild(_element* pElement, const string& strName)
	{
		VERIFY_POINTER(pElement);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strName);

		auto itChild = find_if(pElement->m_vecChildren.begin(), pElement->m_vecChildren.end(),
			[&strName](_element* pChild) { return pChild->getName() == strName; });

		if (itChild != pElement->m_vecChildren.end())
		{
			return pElement->m_vecChildren[distance(pElement->m_vecChildren.begin(), itChild)];
		}

		return nullptr;
	}

	/*static*/ _element* _utils::getChildByAttribute(
		_element* pElement, 
		const string& strName, 
		const string& strAttributeName, 
		const string& strAttributeValue)
	{
		VERIFY_POINTER(pElement);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strName);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strAttributeName);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strAttributeValue);

		vector<_xml::_element*> vecChildren;
		_xml::_utils::getChildren(pElement, strName, vecChildren);

		for (auto pChild : vecChildren)
		{
			if (pChild->getAttributeValue(strAttributeName) == strAttributeValue)
			{
				return pChild;
			}
		}

		return nullptr;
	}

	/*static*/ _element* _utils::getChild(_element* pElement, const vector<string>& vecPath)
	{
		VERIFY_POINTER(pElement);
		VERIFY_STLOBJ_IS_NOT_EMPTY(vecPath);

		auto pChild = pElement;
		for (auto strElement : vecPath)
		{
			pChild = _utils::getChild(pChild, strElement);
			if (pChild == nullptr)
			{
				return nullptr;
			}
		}

		return pChild;
	}

	/*static*/ bool _utils::hasChild(_element* pElement, const vector<string>& vecChildren)
	{
		VERIFY_POINTER(pElement);
		VERIFY_STLOBJ_IS_NOT_EMPTY(vecChildren);

		for (auto strChild : vecChildren)
		{
			if (getChild(pElement, strChild) != nullptr)
			{
				return true;
			}
		}

		return false;
	}

	/*static*/ string _utils::getChildAttribute(_element* pElement, const vector<string>& vecPath, const string& strAttribute)
	{
		VERIFY_POINTER(pElement);
		VERIFY_STLOBJ_IS_NOT_EMPTY(vecPath);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strAttribute);

		auto pChild = _utils::getChild(pElement, vecPath);
		if ((pChild == nullptr) ||
			!pChild->hasAttribute(strAttribute))
		{
			return "";
		}

		return pChild->getAttributeValue(strAttribute);
	}

	/*static*/ void _utils::getChildren(_element* pElement, const string& strName, vector<_element*>& vecChildren)
	{
		VERIFY_POINTER(pElement);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strName);

		vecChildren.clear();

		for (auto pChild : pElement->m_vecChildren)
		{
			if (pChild->getName() == strName)
			{
				vecChildren.push_back(pChild);
			}
		}
	}	
};