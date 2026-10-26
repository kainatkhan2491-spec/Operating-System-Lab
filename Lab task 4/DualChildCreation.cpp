
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION c1 = {}, c2 = {};
    si.cb = sizeof(si);

    char cmd1[] = "cmd.exe /c echo First Sibling running";
    char cmd2[] = "cmd.exe /c echo Second Sibling running";

    if (!CreateProcessA(NULL, cmd1, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &c1)) {
        cout << "First child creation failed!" << endl;
        return 1;
    }

    if (!CreateProcessA(NULL, cmd2, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &c2)) {
        cout << "Second child creation failed!" << endl;
        WaitForSingleObject(c1.hProcess, INFINITE);
        CloseHandle(c1.hProcess);
        CloseHandle(c1.hThread);
        return 1;
    }

    cout << "Parent PID: " << GetCurrentProcessId() << endl;
    cout << "First Sibling PID: " << c1.dwProcessId << endl;
    cout << "Second Sibling PID: " << c2.dwProcessId << endl;

    WaitForSingleObject(c1.hProcess, INFINITE);
    WaitForSingleObject(c2.hProcess, INFINITE);

    CloseHandle(c1.hProcess);
    CloseHandle(c1.hThread);
    CloseHandle(c2.hProcess);
    CloseHandle(c2.hThread);

    return 0;
}

