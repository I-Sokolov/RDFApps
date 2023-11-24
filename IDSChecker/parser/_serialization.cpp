#include "pch.h"

#include "_serialization.h"
#include "_net.h"

namespace _srln
{
	// ********************************************************************************************
	_archive::_archive(_storage* pStorage, const string& strName)
		: m_pStorage(pStorage)
		, m_strName(strName)
		, m_mapData()
		, m_mapObject2Tag()
	{
		VERIFY_POINTER(m_pStorage);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strName);
	}

	_archive::_archive(_storage* pStorage)
		: m_pStorage(pStorage)
		, m_strName("")
		, m_mapData()
		, m_mapObject2Tag()
	{
		VERIFY_POINTER(m_pStorage);
	}

	/*virtual*/ _archive::~_archive()
	{}

	string _archive::getObjectTag(void* pObject)
	{
		VERIFY_POINTER(pObject);

		if (m_mapObject2Tag.find((int64_t)pObject) != m_mapObject2Tag.end())
		{
			return m_mapObject2Tag.at((int64_t)pObject);
		}

		string strTag = _string::sformat("#%lld", (int64_t)(m_mapObject2Tag.size() + 1));
		m_mapObject2Tag[(int64_t)pObject] = strTag;

		return strTag;
	}

	/*virtual*/ void _archive::writeData(const string& strTag, const vector<string>& vecData)
	{
		if (m_mapData.find(strTag) != m_mapData.end())
		{
			THROW_INTERNAL_ERROR();
		}

		m_mapData[strTag] = vecData;
	}

	/*virtual*/ string _archive::writeObject(void* pObject, const vector<string>& vecData)
	{
		VERIFY_POINTER(pObject);
		
		if (m_mapObject2Tag.find((int64_t)pObject) != m_mapObject2Tag.end())
		{
			THROW_INTERNAL_ERROR();
		}

		string strTag = getObjectTag(pObject);
		writeData(strTag, vecData);

		return strTag;
	}

	// ********************************************************************************************
	_namespace::_namespace(_storage* pStorage, const string& strName)
		: m_pStorage(pStorage)
		, m_strName(strName)
		, m_mapArchives()
	{
		VERIFY_POINTER(m_pStorage);
		VERIFY_STLOBJ_IS_NOT_EMPTY(strName);
	}

	/*virtual*/ _namespace::~_namespace()
	{
		for (auto itArchive : m_mapArchives)
		{
			delete itArchive.second;
		}
	}

	_archive* _namespace::getArchive(const string& strArchive, bool bCreateNewIfNeeded)
	{
		string strNPArchive = _net::_url::removeProtocol(strArchive);

		if (m_mapArchives.find(strNPArchive) == m_mapArchives.end())
		{
			if (!bCreateNewIfNeeded)
			{
				return nullptr;
			}

			m_mapArchives[strNPArchive] = new _archive(m_pStorage, strNPArchive);
		}		

		return m_mapArchives.at(strNPArchive);
	}

	// ********************************************************************************************
	_storage::_storage(const string& strName)
		: m_strName(strName)
		, m_mapNamespaces()
		, m_mapIndex()		
	{
		VERIFY_STLOBJ_IS_NOT_EMPTY(m_strName);
	}

	_storage::_storage()
		: m_strName("")
		, m_mapNamespaces()
		, m_mapIndex()
	{}

	/*virtual*/ _storage::~_storage()
	{
		for (auto itNamespace : m_mapNamespaces)
		{
			delete itNamespace.second;
		}
	}

	_namespace* _storage::getNamespace(const string& strNamespace)
	{
		string strNPNamespace = _net::_url::removeProtocol(strNamespace);

		if (m_mapNamespaces.find(strNPNamespace) == m_mapNamespaces.end())
		{
			m_mapNamespaces[strNPNamespace] = new _namespace(this, strNPNamespace);
		}

		return m_mapNamespaces.at(strNPNamespace);
	}

	_archive* _storage::getArchive(const string& strNamespace, const string& strArchive, bool bCreateNewIfNeeded)
	{
		string strNPNamespace = _net::_url::removeProtocol(strNamespace);
		string strNPArchive = _net::_url::removeProtocol(strArchive);

		auto pNamespace = getNamespace(strNPNamespace);
		VERIFY_POINTER(pNamespace);

		return pNamespace->getArchive(strNPArchive, bCreateNewIfNeeded);
	}

	void _storage::deserialize()
	{
		VERIFY_STLOBJ_IS_NOT_EMPTY(m_mapIndex);

		for (auto itIndex : m_mapIndex)
		{
			auto pNamespace = new _namespace(this, itIndex.first);
			m_mapNamespaces[itIndex.first] = pNamespace;

			for (auto strArchive : itIndex.second)
			{
				auto pArchive = createArchive(itIndex.first, strArchive);
				VERIFY_POINTER(pArchive);

				pNamespace->archives()[strArchive] = pArchive;
			}
		}
	}

	// ********************************************************************************************
	_schema_storage::_schema_storage(const string& strName)
		: _schema_storage_metadata()
		, _storage(strName)
	{}

	_schema_storage::_schema_storage()
		: _schema_storage_metadata()
		, _storage()
	{}

	/*virtual*/ _schema_storage::~_schema_storage()
	{}
};

