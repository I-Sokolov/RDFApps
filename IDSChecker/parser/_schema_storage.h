#pragma once

#include "_log.h"
#include "_net.h"
#include "_io.h"

#include <string>
using namespace std;

// ************************************************************************************************
class _schema_metadata
{
private: // Members

	string m_strNamespace;
	string m_strLocation;
	string m_strPath;

public: // Methods

	_schema_metadata(string strNamespace, string strLocation, string strPath)
		: m_strNamespace(strNamespace)
		, m_strLocation(strLocation)
		, m_strPath(strPath)
	{}

	virtual ~_schema_metadata()
	{}

	const string& getNamespace() const { return m_strNamespace; }
	const string& getLocation() const { return m_strLocation; }
	const string& getPath() const { return m_strPath; }
};

// ************************************************************************************************
class _schema_storage_metadata : public _log_client
{

protected: // Members

	// [http(s) -> REMOVED]namespace : ([http(s) -> REMOVED]location : [http(s)]schema metadata)
	map<string, map<string, _schema_metadata*>> m_mapNamespaceSchemas;

	// [http(s) -> REMOVED]source : [http(s)]destination
	map<string, string> m_mapNamespaceRedirections;
	map<string, string> m_mapLocationRedirections;

public: // Methods

	_schema_storage_metadata()
		: _log_client()
		, m_mapNamespaceSchemas()
		, m_mapNamespaceRedirections()
		, m_mapLocationRedirections()
	{}

	virtual ~_schema_storage_metadata()
	{
		for (auto itNamespaceSchemas : m_mapNamespaceSchemas)
		{
			for (auto itSchema : itNamespaceSchemas.second)
			{
				delete itSchema.second;
			}
		}
	}

	string getSchemas(const string& strNamespace, vector<_schema_metadata*>& vecSchemas)
	{
		if (strNamespace.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		vecSchemas.clear();

		string strTargetNamespace = getNamespaceRedirection(strNamespace);
		string strNPNamespace = _net::_url::removeProtocol(strTargetNamespace);

		auto itNamespaceSchemas = m_mapNamespaceSchemas.find(strNPNamespace);
		if (itNamespaceSchemas == m_mapNamespaceSchemas.end())
		{
			return strTargetNamespace;
		}

		for (auto itSchema : itNamespaceSchemas->second)
		{
			vecSchemas.push_back(itSchema.second);
		}

		return strTargetNamespace;
	}
	_schema_metadata* getSchema(const string& strNamespace, const string& strLocation)
	{
		if (strNamespace.empty() || strLocation.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		string strNPNamespace = _net::_url::removeProtocol(strNamespace);
		string strNPLocation = _net::_url::removeProtocol(strLocation);

		auto itNamespaceSchema = m_mapNamespaceSchemas.find(strNPNamespace);
		if (itNamespaceSchema == m_mapNamespaceSchemas.end())
		{
			return nullptr;
		}

		for (auto itSchema : itNamespaceSchema->second)
		{
			if (_net::_url::isValid(strLocation))
			{
				if (_string::equal(itSchema.first, strNPLocation, true))
				{
					return itSchema.second;
				}
			}
			else
			{
				// relative location
				string strFileName = _io::_file::getFileName(strLocation);

				strFileName.insert(strFileName.begin(), SLASH);
				if (_string::endsWith(itSchema.first, strFileName))
				{
					return itSchema.second;
				}
			}
		} // for (auto itLocation2Metadata ...

		// Location redirection: (namespace : schema) => 1 : 1
		if (itNamespaceSchema->second.size() == 1)
		{
			auto itSchema = itNamespaceSchema->second.begin();

			return itSchema->second;
		}

		return nullptr;
	}
	string getNamespaceRedirection(const string& strNamespace) const
	{
		if (strNamespace.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		string strNPNamespace = _net::_url::removeProtocol(strNamespace);

		auto itNamespaceRedirection = m_mapNamespaceRedirections.find(strNPNamespace);
		if (itNamespaceRedirection != m_mapNamespaceRedirections.end())
		{
			return itNamespaceRedirection->second;
		}

		return strNamespace;
	}
	string getLocationRedirection(const string& strLocation) const
	{
		if (strLocation.empty())
		{
			THROW_ARGUMENT_ERROR();
		}

		string strNPLocation = _net::_url::removeProtocol(strLocation);

		auto itLocationRedirection = m_mapLocationRedirections.find(strNPLocation);
		if (itLocationRedirection != m_mapLocationRedirections.end())
		{
			return itLocationRedirection->second;
		}

		return strLocation;
	}

	const map<string, map<string, _schema_metadata*>>& getNamespaceSchemas() const { return m_mapNamespaceSchemas; }
	const map<string, string>& getNamespaceRedirections() const { return m_mapNamespaceRedirections; }
	const map<string, string>& getLocationRedirections() const { return m_mapLocationRedirections; }
};

// ************************************************************************************************
class _schema_storage
	: public _schema_storage_metadata
	, public _io::_file_storage
{

public: // Methods

	_schema_storage(const string& strRootFolder)
		: _schema_storage_metadata()
		, _io::_file_storage(strRootFolder)
	{}
	virtual ~_schema_storage()
	{}	

	virtual void loadMetadata(const string& strStorage) = 0;

	string getTempFilePath(const string& strTempFolder, const string& strFile)
	{
		string strFileName = _net::_url::toFileName(strFile);

		return getFilePath(
			strTempFolder,
			strFileName);
	}
};

