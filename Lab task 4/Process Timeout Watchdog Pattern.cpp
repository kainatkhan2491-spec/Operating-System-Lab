#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};

    si.cb = sizeof(si);

    char command[] = "cmd.exe /c ping 127.0.0.1 -t";

    if (!CreateProcessA(
            NULL, command, NULL, NULL, FALSE,
            CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        cout << "Failed to create child process." << endl;
        return 1;
    }

    cout << "Child process started. PID: "
         << pi.dwProcessId << endl;

    // Allow the child to run for 3 seconds
    DWORD result = WaitForSingleObject(pi.hProcess, 3000);

    if (result == WAIT_TIMEOUT) {
        cout << "Child exceeded time limit. Terminating PID: "
             << pi.dwProcessId << endl;

        // Forcefully terminate the child process
        TerminateProcess(pi.hProcess, 1);

        // Wait for the child to finish termination
        WaitForSingleObject(pi.hProcess, INFINITE);

        cout << "Child process terminated and reaped." << endl;
    } else {
        cout << "Child process completed within the time limit." << endl;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}

