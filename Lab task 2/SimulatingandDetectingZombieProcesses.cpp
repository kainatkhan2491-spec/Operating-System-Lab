#include <iostream>
#include <windows.h>
#include <cstdio>
using namespace std;

int main(int argc, char* argv[]) {

    if (argc > 1) {
        cout << "Child PID: " << GetCurrentProcessId()
             << " terminating now" << endl;
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

    cout << "Parent sleeping for 10s without collecting "
         << "child exit status." << endl;

    Sleep(10000);

    // Collect the child's exit status and clean up handles
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    WaitForSingleObject(pi.hProcess, INFINITE);

    cout << "Parent cleaned up child. Exiting." << endl;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

