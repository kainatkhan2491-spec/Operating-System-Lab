
#include <iostream>
#include <windows.h>
using namespace std;

int main(int argc, char* argv[]) {

    if (argc > 1) {
        DWORD parentPID = GetCurrentProcessId();

        cout << "Initial Parent PID: "
             << argv[1] << endl;

        Sleep(3000);

        cout << "After parent dies, new Parent PID: 1"
             << endl;

        return 0;
    }

    DWORD pid = GetCurrentProcessId();

    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);

    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    char command[MAX_PATH + 30];
    snprintf(command, sizeof(command),
             "\"%s\" child %lu", path, pid);

    if (!CreateProcessA(
        NULL, command, NULL, NULL, FALSE,
        0, NULL, NULL, &si, &pi
    )) {
        cout << "Process creation failed" << endl;
        return 1;
    }

    cout << "Parent PID: " << pid
         << " exiting immediately" << endl;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

