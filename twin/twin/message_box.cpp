#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <winreg.h>
#include <winuser.h>
#include <synchapi.h>
#include "messagebox.h"

#define THREADCOUNT 1
#define PATH "\\\\VBOXSVR\\WINDOWS_Xp_Folder\\twin2.exe"
#define SUBKEY "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"

using std::cin;
using std::cout;
using std::endl;

HANDLE ghMutex;

UINT g_uQueryCancelAutoPlay = 0;
BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int start_message_box() {
	HKEY runKey = NULL;
	int error_code = RegOpenKeyExA(HKEY_CURRENT_USER, SUBKEY, 0, KEY_ALL_ACCESS, &runKey);
	if (error_code != ERROR_SUCCESS)
	{
		cout << "Open did not work " << error_code << endl;
	}
	error_code = RegSetKeyValueA(runKey, NULL, "Message Box", REG_SZ, PATH, strlen(PATH) + 1);
	if (error_code != ERROR_SUCCESS)
	{
		cout << "Set key did not work " << error_code << endl;
	}
	UINT g_uQueryCancelAutoPlay = 0;
	RegCloseKey(runKey);

	HANDLE aThread[THREADCOUNT];
	DWORD ThreadID;
	int i;

	ghMutex = CreateMutexW(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		L"MessageBox");

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(ghMutex);
		return 1;
	}
	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return 1;
	}

	// Create worker threads

	for (i = 0; i < THREADCOUNT; i++)
	{
		aThread[i] = CreateThread(
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE)MessageBox(NULL, L"MANAGEMENT PROGRAM IS UP", L"Message Box", MB_OK),
			NULL,       // no thread function arguments
			0,          // default creation flags
			&ThreadID); // receive thread identifier

		if (aThread[i] == NULL)
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
	}
	Sleep(3600 * 60);
	// Wait for all threads to terminate

	CloseHandle(aThread[0]);
	CloseHandle(ghMutex);

	return 0;
}

BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	default:
		if (!g_uQueryCancelAutoPlay)
		{
			g_uQueryCancelAutoPlay = RegisterWindowMessage(TEXT("QueryCancelAutoPlay"));
		}
		if (uMsg == g_uQueryCancelAutoPlay)
		{
			MessageBox(NULL, L"MANAGEMENT PROGRAM IS UP", L"Message Box", MB_OK);
			return 1;
		}
	}
}

int main()
{
	start_message_box();
	return 0;
}
