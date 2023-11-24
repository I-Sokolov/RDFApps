#pragma once

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <errno.h>

#include <atlbase.h>
using namespace ATL;

#ifdef _WINDOWS
#include <Winsock2.h>
#include <Winhttp.h>
#include <ws2tcpip.h>
#else
#error Linux/MacOS support
#endif // _WINDOWS

#include "_log.h"
#include "_reader.h"
#include "_string.h"

#include <map>
#include <string>
#include <vector>
#include <sstream>

// ------------------------------------------------------------------------------------------------
namespace _net
{
	// ********************************************************************************************
	static const char HTTP[] = "http://";
	static const char HTTPS[] = "https://";
	static const char URN[] = "urn:";

	// ********************************************************************************************
	class _url
	{

	public: // Methods

		static inline bool isValid(const string& strInput)
		{
			return 
				_string::startsWith(strInput, HTTP) ||
				_string::startsWith(strInput, HTTPS) ||
				_string::startsWith(strInput, URN);
		}

		static inline string removeProtocol(const string& strInput)
		{
			if (_string::startsWith(strInput, HTTP))
			{
				string strInputCopy = strInput;
				_string::replace(strInputCopy, HTTP, "");

				return strInputCopy;
			}
			else if (_string::startsWith(strInput, HTTPS))
			{
				string strInputCopy = strInput;
				_string::replace(strInputCopy, HTTPS, "");

				return strInputCopy;
			}

			return strInput;
		}

		static inline string toFileName(const std::string& strUrl)
		{
			/* Example:
				http://schemas.opengis.net/citygml/2.0/cityGMLBase.xsd
				=>
				schemas.opengis.net-citygml-2.0-cityGMLBase.xsd
			*/

			string strFileName = removeProtocol(strUrl);
			_string::replace(strFileName, "/", "-");

			return strFileName;
		}
	};

	// ********************************************************************************************
	class _http_client
		: public _log_client
	{

	private: // Members

		HINTERNET m_hSession;
		HINTERNET m_hConnect;
		HINTERNET m_hRequest;

		std::stringstream m_ssPayload;

	public: // Methods

		_http_client();
		virtual ~_http_client();

		// GET
		void get(const wchar_t* szUrl);

		std::stringstream const* getPayload() const { return &m_ssPayload; }

	protected: // Methods

		virtual void clean();
	};
};

