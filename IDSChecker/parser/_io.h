#pragma once

#include "_errors.h"
#include "_string.h"

#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

namespace fs = std::experimental::filesystem;

// ************************************************************************************************
namespace _io
{
	// ********************************************************************************************
	class _file
	{

	public: // Methods

		static inline string getFileName(const string& strPath, const string& strDelimiters = "/\\")
		{
			return _getFileName<string>(strPath, strDelimiters);
		}

		static inline string getFileNameWithoutExt(const string& strPath, const string& strDelimiters = "/\\")
		{
			string strFileName = _getFileName<string>(strPath, strDelimiters);

			size_t iPos = strFileName.find_last_of('.');
			if (iPos != string::npos)
			{
				strFileName = strFileName.substr(0, iPos);
			}

			return strFileName;
		}

		static inline wstring getFileName(const wstring& strPath, const wstring& strDelimiters = L"/\\")
		{
			return _getFileName<wstring>(strPath, strDelimiters);
		}

		static inline wstring getFileNameWithoutExt(const wstring& strPath, const wstring& strDelimiters = L"/\\")
		{
			wstring strFileName = _getFileName<wstring>(strPath, strDelimiters);

			size_t iPos = strFileName.find_last_of(L'.');
			if (iPos != string::npos)
			{
				strFileName = strFileName.substr(0, iPos);
			}

			return strFileName;
		}

		template<class T>
		static inline T _getFileName(const T& strPath, const T& strDelimiters)
		{
			return strPath.substr(strPath.find_last_of(strDelimiters) + 1);
		}

		static inline bool createFolder(const std::string& strFolder)
		{
			if (!fs::exists(strFolder))
			{
				return fs::create_directories(strFolder);
			}

			return true;
		}		
	};	

	// ********************************************************************************************
	class _file_storage
	{

	private: // Members

		std::string m_strRootFolder;

	public: // Methods

		_file_storage(const std::string& strRootFolder = "_cache")
			: m_strRootFolder(strRootFolder)
		{
			if (m_strRootFolder.empty())
			{
				THROW_ARGUMENT_ERROR();
			}			
		}

		virtual ~_file_storage()
		{}

		std::string getFilePath(
			const std::string& strFileName)
		{
			return getFilePath("", "", strFileName);
		}

		std::string getFilePath(
			const std::string& strNamespace,
			const std::string& strFileName)
		{
			return getFilePath(strNamespace, "", strFileName);
		}

		std::string getFilePath(
			const std::string& strNamespace,
			const std::string& strSubNamespace,
			const std::string& strFileName)
		{
			fs::path pthFile = buildFilePath(strNamespace, strSubNamespace, strFileName);
			if (fs::exists(pthFile.string()))
			{
				return pthFile.string();
			}

			return "";
		}

		void saveFile(
			const std::string& strFileName,
			const string& strContent)
		{
			saveFile("", "", strFileName, strContent);
		}

		void saveFile(
			const std::string& strNamespace,
			const std::string& strFileName,
			const string& strContent)
		{
			saveFile(strNamespace, "", strFileName, strContent);
		}

		void saveFile(
			const std::string& strNamespace,
			const std::string& strSubNamespace,
			const std::string& strFileName,
			const string& strContent)
		{
			if (strFileName.empty())
			{
				THROW_ARGUMENT_ERROR();
			}

			fs::path pthFile = buildFilePath(strNamespace, strSubNamespace, strFileName);

			std::ofstream file(pthFile.c_str());
			file << strContent;
			file.close();
		}

		fs::path buildFilePath(
			const std::string& strNamespace,
			const std::string& strFileName)
		{
			return buildFilePath(
				strNamespace,
				"",
				strFileName);
		}

		fs::path buildFilePath(
			const std::string& strNamespace, 
			const std::string& strSubNamespace /*optional*/,
			const std::string& strFileName)
		{
			if (strFileName.empty())
			{
				THROW_ARGUMENT_ERROR();
			}

			fs::path pthFile = m_strRootFolder;

			if (!strNamespace.empty())
			{
				pthFile.append(strNamespace);
			}

			if (!strSubNamespace.empty())
			{
				pthFile.append(strSubNamespace);
			}
			
			pthFile.append(strFileName);

			auto ptPath = pthFile.parent_path();
			if (!_file::createFolder(ptPath.string()))
			{
				THROW_INTERNAL_ERROR();
			}

			return pthFile;
		}
	};	
};

