#include <exception> // for std::exception
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <winreg.h>
#include <winuser.h>
#include "message_box.h"

#define THREADCOUNT 1

using std::cin;
using std::cout;
using std::endl;

HANDLE ghMutex;

UINT g_uQueryCancelAutoPlay = 0;

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

int start_message_box() {
	//RegSetKeyValueW(HKEY_LOCAL_MACHINE, NULL, L"\Software\Microsoft\Windows\CurrentVersion\RunOnce", NULL, NULL, NULL);
	UINT g_uQueryCancelAutoPlay = 0;
	
	HANDLE aThread[THREADCOUNT];
	DWORD ThreadID;
	int i;

	// Create a mutex with no initial owner
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

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
	
	// Wait for all threads to terminate

	CloseHandle(aThread[0]);
	CloseHandle(ghMutex);

	return 0;
}

