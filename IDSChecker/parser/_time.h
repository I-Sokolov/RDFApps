#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

class _time
{

public: // Methods

    static std::string dateTimeStamp()
    {
        auto timePointNow = std::chrono::system_clock::now();
        auto timeNow = std::chrono::system_clock::to_time_t(timePointNow);
        auto timeNowMS = std::chrono::duration_cast<std::chrono::milliseconds>(timePointNow.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %H:%M:%S.");
        ss << std::setfill('0') << std::setw(3) << timeNowMS.count();

        return ss.str();
    }

    static std::string addDateTimeStamp(const std::string& strInput)
    {
        std::string strInputCopy = dateTimeStamp();
        strInputCopy += ": ";
        strInputCopy += strInput;

        return strInputCopy;
    }
};
