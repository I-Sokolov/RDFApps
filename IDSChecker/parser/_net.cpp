#include "pch.h"

#include "_io.h"
#include "_net.h"
#include "_errors.h"
#include "_reader.h"

#ifdef _WINDOWS
#pragma comment(lib, "Winhttp.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

// ------------------------------------------------------------------------------------------------
namespace _net
{
    // ********************************************************************************************
    _http_client::_http_client()
        : _log_client()
        , m_hSession(NULL)
        , m_hConnect(NULL)
        , m_hRequest(NULL)
    {
        // Use WinHttpOpen to obtain a session handle
        m_hSession = WinHttpOpen(
            L"GISEngine",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS,
            0);

        if (m_hSession == NULL)
        {
            LOG_THROW_ERROR_F("WinHttpOpen() failed; error code: %u", GetLastError());
        }
    }

    /*virtual*/ _http_client::~_http_client()
    {
        clean();

        if (m_hSession != NULL)
        {
            WinHttpCloseHandle(m_hSession);
            m_hSession = NULL;
        }
    }

    void _http_client::get(const wchar_t* szUrl)
    {
        logInfof("Downloading '%s' ...", (LPCSTR)CW2A(szUrl));

        clean();

        // Extract Host & Path
        URL_COMPONENTS urlComps;
        ZeroMemory(&urlComps, sizeof(urlComps));
        urlComps.dwStructSize = sizeof(urlComps);
        urlComps.dwSchemeLength = (DWORD)-1;
        urlComps.dwHostNameLength = (DWORD)-1;
        urlComps.dwUrlPathLength = (DWORD)-1;
        urlComps.dwExtraInfoLength = (DWORD)-1;
        if (!WinHttpCrackUrl(szUrl, (DWORD)wcslen(szUrl), 0, &urlComps))
        {
            LOG_THROW_ERROR_F("WinHttpCrackUrl() failed; error code: %d.", GetLastError());
        }

        wstring strHost(urlComps.lpszHostName, urlComps.dwHostNameLength);
        wstring strPath(urlComps.lpszUrlPath, urlComps.dwUrlPathLength);

        // Specify an HTTP server
        m_hConnect = WinHttpConnect(
            m_hSession,
            strHost.c_str(),
            INTERNET_DEFAULT_PORT,
            0);

        if (m_hConnect == NULL)
        {
            LOG_THROW_ERROR_F("WinHttpConnect() failed; error code: %d.", GetLastError());
        }
        
        // Create an HTTP request handle
        m_hRequest = WinHttpOpenRequest(
            m_hConnect,
            L"GET",
            strPath.c_str(),
            NULL,
            WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            _string::startsWith((LPCSTR)CW2A(szUrl), HTTPS, false) ? WINHTTP_FLAG_SECURE : 0);
        if (m_hRequest == NULL)
        {
            LOG_THROW_ERROR_F("WinHttpOpenRequest() failed; error code: %d.", GetLastError());
        }

        // Send a request
        BOOL bResults = WinHttpSendRequest(
            m_hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0, 
            WINHTTP_NO_REQUEST_DATA, 
            0,
            0, 
            0);
        if (!bResults)
        {
            LOG_THROW_ERROR_F("WinHttpSendRequest() failed; error code: %d.", GetLastError());
        }

        // End the request
        bResults = WinHttpReceiveResponse(m_hRequest, NULL);
        if (!bResults)
        {
            LOG_THROW_ERROR_F("WinHttpReceiveResponse() failed; error code: %d.", GetLastError());
        }

        // Retrieve HTTP status code
        DWORD dwStatusCode = 0;
        DWORD dwSize = sizeof(dwStatusCode);
        WinHttpQueryHeaders(m_hRequest,
            WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX,
            &dwStatusCode, 
            &dwSize, 
            WINHTTP_NO_HEADER_INDEX);

        if (dwStatusCode != 200)
        {
            LOG_THROW_ERROR_F("WinHttpReceiveResponse() failed; error code: %d.", dwStatusCode);
        }

        // Keep checking for data until there is nothing left
        dwSize = 0;
        do
        {
            // Check for available data            
            if (!WinHttpQueryDataAvailable(m_hRequest, &dwSize))
            {
                LOG_THROW_ERROR_F("WinHttpQueryDataAvailable() failed; error code: %d.", GetLastError());
            }

            LPSTR pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer)
            {
                LOG_THROW_ERROR_F("Out of memory; error code: %d.", GetLastError());
            }            
            
            ZeroMemory(pszOutBuffer, dwSize + 1);

            // Read the Data
            DWORD dwDownloaded = 0;
            if (!WinHttpReadData(
                m_hRequest,
                (LPVOID)pszOutBuffer,
                dwSize,
                &dwDownloaded))
            {
                LOG_THROW_ERROR_F("WinHttpReadData() failed; error code: %d.", GetLastError());
            }
                
            m_ssPayload << pszOutBuffer;

            delete[] pszOutBuffer;
        } while (dwSize > 0);
    }

    /*virtual*/ void _http_client::clean()
    {
        if (m_hConnect != NULL)
        {
            WinHttpCloseHandle(m_hConnect);
            m_hConnect = NULL;
        }

        if (m_hRequest != NULL)
        {
            WinHttpCloseHandle(m_hRequest);
            m_hRequest = NULL;
        }

        m_ssPayload.clear();
    }
};
