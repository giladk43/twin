#include "mutex_class.h"

using std::cin;
using std::cout;
using std::endl;

//In it Functions
MY_MUTEX::MY_MUTEX() {
    my_ghMutex = CreateMutexW(
        NULL,              // default security attributes
        FALSE,             // initially not owned
        L"MessageBox");


    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        ReleaseMutex(my_ghMutex);   // Need exceptions
    }
    if (my_ghMutex == NULL)
    {
        printf("CreateMutex error: %d\n", GetLastError()); // Need exceptions
    }
}

//Get Functions
HANDLE MY_MUTEX::getHandleMutex() const {
    return my_ghMutex;
}
