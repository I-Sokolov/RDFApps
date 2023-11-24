#include "pch.h"

#include "_reader.h"
#include "_errors.h"

// ********************************************************************************************
_stream_reader::_stream_reader()
	: m_pStream(nullptr)
{
}

/*virtual*/ _stream_reader::~_stream_reader()
{}

void _stream_reader::init(istream* pStream)
{
	VERIFY_POINTER(pStream);

	m_pStream = pStream;

	/* BOM */
	int iChar = getCharT<unsigned char>();
	if (iChar == 0xEF)
	{
		iChar = getNextCharT<unsigned char>(true);
		if (iChar != 0xBB)
		{
			THROW_INTERNAL_ERROR();
		}

		iChar = getNextCharT<unsigned char>(true);
		if (iChar != 0xBF)
		{
			THROW_INTERNAL_ERROR();
		}

		getNextChar(false);
	}
}

char _stream_reader::getChar()
{
	return (char)m_pStream->rdbuf()->sgetc();
}

char _stream_reader::getCharGoNext()
{
	return (char)m_pStream->rdbuf()->sbumpc();
}

char _stream_reader::getNextChar(bool bThrowError)
{
	char ch = (char)m_pStream->rdbuf()->snextc();
	if (bThrowError && (ch == EOF))
	{
		THROW_ERROR(EOF_ERR);
	}

	return ch;
}

void _stream_reader::goNext()
{
	m_pStream->rdbuf()->snextc();
}

string _stream_reader::getNextChars(int iCount)
{
	string strChars;
	for (int i = 0; i < iCount; i++)
	{
		strChars += getNextChar(true);
	}

	return strChars;
}

void _stream_reader::goToNextChar(char chTarget)
{
	char ch = getChar();
	while (ch != chTarget)
	{
		ch = getNextChar(true);
	}
}

char _stream_reader::getPrevChar(bool bThrowError)
{
	char ch = (char)m_pStream->rdbuf()->sungetc();
	if (bThrowError && (ch == EOF))
	{
		THROW_ERROR(SOF_ERR);
	}

	return ch;
}

void _stream_reader::goPrev()
{
	m_pStream->rdbuf()->sungetc();
}

// ********************************************************************************************
_file_reader::_file_reader()
	: _stream_reader()
	, m_pFile(nullptr)
{}

/*virtual*/ _file_reader::~_file_reader()
{
	close();
}

/*virtual*/ bool _file_reader::open(const char* szFile)
{
	close();

	m_pFile = new ifstream(szFile);
	if (m_pFile->is_open())
	{
		_stream_reader::init(m_pFile);

		return true;
	}

	return false;
}

/*virtual*/ void _file_reader::close()
{
	delete m_pFile;
	m_pFile = nullptr;
}
