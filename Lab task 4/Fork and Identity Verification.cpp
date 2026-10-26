
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    DWORD parentPID = GetCurrentProcessId();

    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};

    si.cb = sizeof(si);

    char command[] = "cmd.exe /c echo Child process running";

    if (!CreateProcessA(
        NULL, command, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &pi
    )) {
        cout << "Process creation failed!" << endl;
        return 1;
    }

    cout << "Parent Process -> PID: " << parentPID
         << " | Child PID: " << pi.dwProcessId << endl;

    cout << "Child Process -> PID: " << pi.dwProcessId
         << " | Parent PID: " << parentPID << endl;

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

