#pragma once

#include "_string.h"
#include "_time.h"

#include <string>

// ************************************************************************************************
enum class enumLogEvent
{
	info = 0,
	warning,
	error,
};

// ************************************************************************************************
class _log
{

public: // Methods

	_log()
	{}

	virtual ~_log()
	{}

public: // Methods

	virtual void logWrite(enumLogEvent enLogEvent, const std::string& strEvent) = 0;
};

// ************************************************************************************************
typedef void(__stdcall* _log_callback)(enumLogEvent enLogEvent, const char* szEvent);

class _c_log : public _log
{

private: // _log_callback

	_log_callback m_pLogCallback;

public: // Methods

	_c_log(_log_callback pLogCallback)
		: _log()
		, m_pLogCallback(pLogCallback)
	{}

	virtual ~_c_log()
	{}

public: // Methods

	virtual void logWrite(enumLogEvent enLogEvent, const std::string& strEvent)
	{
		if (m_pLogCallback != nullptr)
		{
			(*m_pLogCallback)(enLogEvent, strEvent.c_str());
		}
	}
};

// ************************************************************************************************
class _log_client
{

private: // Members

	_log* m_pLog;

public: // Methods

	_log_client() 
		: m_pLog(nullptr)
	{}

	virtual ~_log_client()
	{}

	void setLog(_log* pLog) { m_pLog = pLog; }
	_log* getLog() { return m_pLog; }

	void logInfo(const std::string& strEvent) 
	{ 
		if (m_pLog != nullptr)
		{
			m_pLog->logWrite(enumLogEvent::info, _time::addDateTimeStamp(strEvent));
		}
	}

	template<typename... Arguments>
	void logInfof(const std::string& strEvent, Arguments... args)
	{
		if (m_pLog != nullptr)
		{
			string strFormattedEvent = _string::sformat(strEvent, args...);
			m_pLog->logWrite(enumLogEvent::info, _time::addDateTimeStamp(strFormattedEvent));
		}
	}

	void logWarn(const std::string& strEvent)
	{
		if (m_pLog != nullptr)
		{
			m_pLog->logWrite(enumLogEvent::warning, _time::addDateTimeStamp(strEvent));
		}
	}

	template<typename... Arguments>
	void logWarnf(const std::string& strEvent, Arguments... args)
	{
		if (m_pLog != nullptr)
		{
			string strFormattedEvent = _string::sformat(strEvent, args...);
			m_pLog->logWrite(enumLogEvent::warning, _time::addDateTimeStamp(strFormattedEvent));
		}
	}

	void logErr(const std::string& strEvent)
	{
		if (m_pLog != nullptr)
		{
			m_pLog->logWrite(enumLogEvent::error, _time::addDateTimeStamp(strEvent));
		}
	}

	template<typename... Arguments>
	void logErrf(const std::string& strEvent, Arguments... args)
	{
		if (m_pLog != nullptr)
		{
			string strFormattedEvent = _string::sformat(strEvent, args...);
			m_pLog->logWrite(enumLogEvent::error, _time::addDateTimeStamp(strFormattedEvent));
		}
	}
};