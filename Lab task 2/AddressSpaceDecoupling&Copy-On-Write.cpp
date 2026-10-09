
#include <iostream>
#include <windows.h>
#include <cstdio>
using namespace std;

int shared_counter = 100;

int main(int argc, char* argv[]) {

    if (argc > 1) {
        shared_counter = shared_counter + 50;

        cout << "Child sees shared_counter = "
             << shared_counter << endl;

        return 0;
    }

    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);

    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    char command[MAX_PATH + 10];
    snprintf(command, sizeof(command), "\"%s\" child", path);

    if (!CreateProcessA(
        NULL, command, NULL, NULL, FALSE,
        0, NULL, NULL, &si, &pi
    )) {
        cout << "Process creation failed" << endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    cout << "Parent sees shared_counter = "
         << shared_counter << endl;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

