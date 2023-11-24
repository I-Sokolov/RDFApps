#pragma once

#include <string>
#include <vector>
#include <algorithm> 
#include <functional>
using namespace std;

#include <assert.h>

#define CHR2STR(c) string(1, c)

// ************************************************************************************************
class _string
{

public: //  Methods
		
	static inline void ltrim(string& strInput, std::function<int(int)> fnTrim = [](int c) { return isblank((unsigned char)c); })
	{
		strInput.erase(strInput.begin(), find_if(strInput.begin(), strInput.end(), not1(fnTrim)));
	}

	static inline void ltrim(string& strInput, char cToTrim)
	{
		std::function<int(int)> fnTrim = [cToTrim](int c) { return c == cToTrim; };

		strInput.erase(strInput.begin(), find_if(strInput.begin(), strInput.end(), not1(fnTrim)));
	}

	static inline void rtrim(string& strInput, std::function<int(int)> fnTrim = [](int c) { return isblank((unsigned char)c); })
	{
		strInput.erase(find_if(strInput.rbegin(), strInput.rend(), not1(fnTrim)).base(), strInput.end());
	}

	static inline void rtrim(string& strInput, char cToTrim)
	{
		std::function<int(int)> fnTrim = [cToTrim](int c) { return c == cToTrim; };

		strInput.erase(find_if(strInput.rbegin(), strInput.rend(), not1(fnTrim)).base(), strInput.end());
	}

	static inline void trim(string& strInput)
	{
		rtrim(strInput);
		ltrim(strInput);
	}

	static inline void trim(vector<string>& vecInput)
	{
		for (auto& strInput : vecInput)
		{
			rtrim(strInput);
			ltrim(strInput);
		}		
	}

	static inline void trim(string& strInput, char cToTrim)
	{
		std::function<int(int)> fnTrim = [cToTrim](int c) { return c == cToTrim; };

		rtrim(strInput, fnTrim);
		ltrim(strInput, fnTrim);
	}

	static inline string ltrim_copy(string strInput)
	{
		ltrim(strInput);

		return strInput;
	}

	static inline string rtrim_copy(string strInput)
	{
		rtrim(strInput);

		return strInput;
	}

	static inline string trim_copy(string strInput)
	{
		trim(strInput);

		return strInput;
	}

	static inline void toUpper(string& strInput)
	{
		std::transform(strInput.begin(), strInput.end(), strInput.begin(),
			[](unsigned char c) { return (unsigned char)std::toupper(c); });
	}

	static inline bool equal(const string& strInput1, const string& strInput2, bool bIgnoreCase)
	{
		if (bIgnoreCase)
		{
			string strInput1Copy = strInput1;
			toUpper(strInput1Copy);

			string strInput2Copy = strInput2;
			toUpper(strInput2Copy);

			return strInput1Copy == strInput2Copy;
		}

		return strInput1 == strInput2;
	}

	static inline bool contains(const string& strInput, const string& strValue, bool bIgnoreCase = true)
	{
		if (bIgnoreCase)
		{
			string strInputCopy = strInput;
			toUpper(strInputCopy);

			string strValueCopy = strValue;
			toUpper(strValueCopy);

			return strInputCopy.find(strValueCopy) != string::npos;
		}

		return strInput.find(strValue) != string::npos;
	}

	static inline bool startsWith(const string& strInput, const string& strValue, bool bIgnoreCase= true)
	{
		if (bIgnoreCase)
		{
			string strInputCopy = strInput;
			toUpper(strInputCopy);			

			string strValueCopy = strValue;
			toUpper(strValueCopy);

			return strInputCopy.find(strValueCopy) == 0;
		}		

		return strInput.find(strValue) == 0;
	}

	static inline bool endsWith(const string& strInput, const string& strValue, bool bIgnoreCase = true)
	{
		if (strValue.size() > strInput.size())
		{
			return false;
		}

		if (bIgnoreCase)
		{
			string strInputCopy = strInput;
			toUpper(strInputCopy);

			string strValueCopy = strValue;
			toUpper(strValueCopy);

			return strInputCopy.rfind(strValueCopy) == (strInputCopy.size() - strValueCopy.size());
		}

		return strInput.rfind(strValue) == (strInput.size() - strValue.size());
	}

	static inline bool startsEndsWith(const string& strInput, const string& strStartValue, const string& strEndValue, bool bIgnoreCase)
	{
		if ((strStartValue.size() > strInput.size()) || 
			(strEndValue.size() > strInput.size()))
		{
			return false;
		}

		if (bIgnoreCase)
		{
			string strInputCopy = strInput;
			toUpper(strInputCopy);

			string strStartValueCopy = strStartValue;
			toUpper(strStartValueCopy);

			if (strInputCopy.find(strStartValueCopy) != 0)
			{
				return false;
			}

			string strEndValueCopy = strEndValue;
			toUpper(strEndValueCopy);

			return(strInputCopy.rfind(strEndValueCopy) == (strInputCopy.size() - strEndValueCopy.size()));
		}

		return (strInput.find(strStartValue) == 0) &&
			(strInput.rfind(strEndValue) == (strInput.size() - strEndValue.size()));
	}

	static inline void tokenize(const string& strInput, const string& strDelimter, vector<string>& vecTokens, bool bFirstOnly = false)
	{
		vecTokens.clear();

		string strInputCopy = strInput;
		trim(strInputCopy);

		size_t iPos = 0;		
		while ((iPos = strInputCopy.find(strDelimter)) != string::npos)
		{
			string strToken = strInputCopy.substr(0, iPos);
			vecTokens.push_back(strToken);

			strInputCopy.erase(0, iPos + strDelimter.length());

			if (bFirstOnly)
			{
				break;
			}
		}

		vecTokens.push_back(strInputCopy);
	}

	static inline void tokenizeN(const string& strInput, const string& strDelimter, vector<string>& vecTokens, int iTokensCount)
	{
		assert(iTokensCount >= 1);

		vecTokens.clear();

		string strInputCopy = strInput;
		trim(strInputCopy);

		size_t iPos = 0;
		while ((iPos = strInputCopy.find(strDelimter)) != string::npos)
		{
			string strToken = strInputCopy.substr(0, iPos);
			vecTokens.push_back(strToken);

			strInputCopy.erase(0, iPos + strDelimter.length());

			if ((int)vecTokens.size() == iTokensCount)
			{
				break;
			}
		}

		if (!strInputCopy.empty())
		{
			if ((int)vecTokens.size() == iTokensCount)
			{
				vecTokens[vecTokens.size() - 1] += strDelimter;
				vecTokens[vecTokens.size() - 1] += strInputCopy;
			}
			else
			{
				vecTokens.push_back(strInputCopy);
			}
		}
	}

	static inline void split(const string& strInput, const string& strDelimter, vector<string>& vecTokens, bool bRemoveEmpty = true)
	{
		tokenize(strInput, strDelimter, vecTokens);

		if (bRemoveEmpty)
		{
			trim(vecTokens);

			auto isEmptyOrBlank = [](const std::string& s) {
				return s.find_first_not_of(" \t") == std::string::npos;
			};

			vecTokens.erase(std::remove_if(vecTokens.begin(), vecTokens.end(), isEmptyOrBlank), vecTokens.end());
		}		
	}

	static inline void replace(string& strInput, const string& strOldValue, const string& strNewValue)
	{
		size_t iIndex = 0;
		while (true) 
		{
			iIndex = strInput.find(strOldValue, iIndex);
			if (iIndex == string::npos)
			{
				break;
			}

			strInput.replace(iIndex, strOldValue.size(), strNewValue);

			iIndex += strNewValue.size();
		} // while (true)
	}

	template<typename... Arguments> 
	static inline string format(const char* szInput, Arguments... args)
	{
		char szBuffer[1024];
		sprintf_s(szBuffer, 1024, szInput, args...);

		return szBuffer;
	}

	template<typename... Arguments>
	static inline string sformat(const string& strInput, Arguments... args)
	{
		char szBuffer[1024];
		sprintf_s(szBuffer, 1024, strInput.c_str(), args...);

		return szBuffer;
	}

	static inline bool isInteger(const std::string& strInput)
	{
		return !strInput.empty() && 
			std::find_if(strInput.begin(),
			strInput.end(), 
			[](unsigned char c) { return !std::isdigit(c); }) == strInput.end();
	}

	static inline bool isXInteger(const std::string& strInput)
	{
		return !strInput.empty() &&
			std::find_if(strInput.begin(),
				strInput.end(),
				[](unsigned char c) { return !std::isxdigit(c); }) == strInput.end();
	}
};

