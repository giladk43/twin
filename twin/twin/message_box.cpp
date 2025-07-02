#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <winreg.h>
#include <winuser.h>
#include <synchapi.h>
#include <exception>
#include "message_box.h"
#include "mutex_class.h"

#define PATH "\\\\VBOXSVR\\WINDOWS_Xp_Folder\\twin2.exe"
#define SUBKEY "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
#define HOUR_IN_MILISECONDS (60000 * 720)
using std::cout;
using std::endl;


int start_message_box() {
    MY_MUTEX ghMutex = MY_MUTEX(); //Mutex Class with deconstructor
    HKEY runKey = NULL;
    int error_code = RegOpenKeyExA(HKEY_CURRENT_USER, SUBKEY, 0, KEY_ALL_ACCESS, &runKey);
    if (error_code != ERROR_SUCCESS)
    {
        cout << "Open did not work " << error_code << endl;
        return 1; //Need Exception
    }
    error_code = RegSetKeyValueA(runKey, NULL, "Message Box", REG_SZ, PATH, strlen(PATH) + 1);
    if (error_code != ERROR_SUCCESS)
    {
        cout << "Set key did not work " << error_code << endl;
        return 1; // Need exception
    }
    RegCloseKey(runKey);

    MessageBox(NULL, L"MANAGEMENT PROGRAM IS UP", L"Message Box", MB_OK);
    Sleep(HOUR_IN_MILISECONDS);

    return 0;
}

int main()
{
    int return_value = start_message_box();
    return return_value;
}
