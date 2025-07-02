#ifndef MY_MUTEX_H
#define MY_MUTEX_H
#include <iostream>
#include <windows.h>
#include <synchapi.h>


class MY_MUTEX {
public:
	MY_MUTEX();

	//Get Functions
	HANDLE getHandleMutex() const;

	//Destructor
	~MY_MUTEX() { ReleaseMutex(my_ghMutex); std::puts("Released Mutex"); };


private:
	HANDLE my_ghMutex;
};

#endif //MY_MUTEX_H