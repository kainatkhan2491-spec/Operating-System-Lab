
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    DWORD parentPID = GetCurrentProcessId();

    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};

    si.cb = sizeof(si);

    char command[] = "cmd.exe /c exit";

    if (!CreateProcessA(
        NULL,
        command,
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        cout << "Process creation failed" << endl;
        return 1;
    }

    DWORD childPID = pi.dwProcessId;

    cout << "Parent Process: PID = " << parentPID
         << ", Created Child PID = " << childPID << endl;

    cout << "Child Process: PID = " << childPID
         << ", Parent PID = " << parentPID << endl;

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

