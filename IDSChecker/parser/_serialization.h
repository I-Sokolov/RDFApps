#pragma once

#include "_io.h"
#include "_schema_storage.h"

#include <string>
#include <vector>
#include <map>

namespace _srln
{
	// ********************************************************************************************
	class _storage;
	class _archive;

	// ********************************************************************************************
	class _serializable
	{

	public: // Methods

		virtual void serialize(_archive* pArchive) = 0;
		virtual void deserialize(_archive* pArchive) = 0;
	};

	// ********************************************************************************************
	class _archive
	{

	protected: // Members

		_storage* m_pStorage;
		string m_strName;
		map<string, vector<string>> m_mapData; // Tag : Data

		// serialization; pointer => 1-based index; 
		map<int64_t, string> m_mapObject2Tag;

	public: // Methods

		// serialization
		_archive(_storage* pStorage, const string& strName);

		// deserialization
		_archive(_storage* pStorage);

		virtual ~_archive();

		// serialization
		string getObjectTag(void* pObject);
		virtual void writeData(const string& strTag, const vector<string>& vecData);
		virtual string writeObject(void* pObject, const vector<string>& vecData);

		// deserialization
		virtual const vector<string>& readData(const string& strTag) const { return m_mapData.at(strTag); }

		const string& getName() const { return m_strName; }
		map<string, vector<string>>& data() { return m_mapData; }
	};

	// ********************************************************************************************
	class _namespace
	{

	private: // Members

		_storage* m_pStorage;
		string m_strName;
		map<std::string, _archive*> m_mapArchives;

	public: // Methods

		_namespace(_storage* pStorage, const string& strName);
		virtual ~_namespace();

		_archive* getArchive(const string& strArchive, bool bCreateNewIfNeeded);

		const string& getName() const { return m_strName; }
		map<std::string, _archive*>& archives() { return m_mapArchives; }		
	};

	// ********************************************************************************************
	class _storage
	{

	protected: // Members

		string m_strName;
		map<std::string, _namespace*> m_mapNamespaces; 

		// deserialization
		map<string, vector<string>> m_mapIndex; // namespace : vector<archive>

	public: // Methods
		
		// serialization
		_storage(const string& strName);

		// deserialization
		_storage(); 

		virtual ~_storage();

		
		_namespace* getNamespace(const string& strNamespace);		
		_archive* getArchive(const string& strNamespace, const string& strArchive, bool bCreateNewIfNeeded);

		const string& getName() const { return m_strName; }
		map<std::string, _namespace*>& getNamespaces() { return m_mapNamespaces; }
		const map<string, vector<string>>& getIndex() const { return m_mapIndex; }

	protected: // Methods

		// deserialization
		void deserialize();
		virtual _archive* createArchive(const string& /*strNamespace*/, const string& /*strArchive*/) { THROW_NOT_SUPPORTED_ERROR(); }
	};

	class _schema_storage 
		: public _schema_storage_metadata
		, public _storage
	{

	public: // Methods
		
		// serialization
		_schema_storage(const string& strName);

		// deserialization
		_schema_storage();

		virtual ~_schema_storage();
	};
};



