#pragma once

#include "_errors.h"

#include <iostream>
#include <fstream>

#include <codecvt>
#include <fcntl.h>
#include <io.h>

#include <string>
using namespace std;

// ********************************************************************************************
const char LF					= '\n'; // Line Feed
const char CR					= '\r'; // Carriage Return
const char TAB					= '\t';
const char SPACE				= ' ';
const char SLASH				= '/';
const char BACK_SLASH			= '\\';
const char EXCLAMATION_MARK		= '!';
const char DASH					= '-';
const char COLON				= ':';
const char COMMA				= ',';
const char EQUAL				= '=';
const char SINGLE_QUOT_MARK		= '\'';
const char DOULE_QUOT_MARK		= '"';
const char SPACE_TAB[]			= " \t";

// ********************************************************************************************
const string SOF_ERR = "Start of file.";
const string EOF_ERR = "End of file.";

// ********************************************************************************************
class _stream_reader
{

private:// Members

	istream* m_pStream;

public: // Methods

	_stream_reader();
	virtual ~_stream_reader();

	void init(istream* m_pStream);

	/// <summary>
	/// https://cplusplus.com/reference/streambuf/basic_streambuf/snextc/
	/// https://cplusplus.com/reference/streambuf/basic_streambuf/sungetc/
	/// </summary>
	/// <returns></returns>
	char getChar();
	template<typename T>
	T getCharT() { return (T)m_pStream->rdbuf()->sgetc(); }
	char getCharGoNext();
	char getNextChar(bool bThrowError);
	template<typename T>
	T getNextCharT(bool bThrowError)
	{
		T ch = (T)m_pStream->rdbuf()->snextc();
		if (bThrowError && (ch == EOF))
		{
			THROW_ERROR(EOF_ERR);
		}

		return ch;
	}
	void goNext();
	string getNextChars(int iCount);
	void goToNextChar(char ch);
	char getPrevChar(bool bThrowError);
	void goPrev();
};

// ********************************************************************************************
class _file_reader : public _stream_reader	
{

private:// Members

	ifstream* m_pFile;

public: // Methods

	_file_reader();
	virtual ~_file_reader();

	virtual bool open(const char* szFile);

protected: // Methods

	virtual void close();
};

