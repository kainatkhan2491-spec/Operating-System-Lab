
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION b = {}, c = {};
    si.cb = sizeof(si);

    char cmdB[] = "cmd.exe /c exit 0";
    char cmdC[] =
        "cmd.exe /c \"timeout /t 1 /nobreak > nul & "
        "echo Process C is still running\"";

    // Process A creates Process B
    if (!CreateProcessA(NULL, cmdB, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &b)) {
        cout << "Process B creation failed!" << endl;
        return 1;
    }

    // Process A creates Process C
    if (!CreateProcessA(NULL, cmdC, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &c)) {
        cout << "Process C creation failed!" << endl;
        WaitForSingleObject(b.hProcess, INFINITE);
        CloseHandle(b.hProcess);
        CloseHandle(b.hThread);
        return 1;
    }

    cout << "Process A PID: " << GetCurrentProcessId() << endl;
    cout << "Process B PID: " << b.dwProcessId << endl;
    cout << "Process C PID: " << c.dwProcessId << endl;

    WaitForSingleObject(b.hProcess, INFINITE);
    cout << "Process B exited." << endl;

    WaitForSingleObject(c.hProcess, INFINITE);
    cout << "Process C completed." << endl;

    CloseHandle(b.hProcess);
    CloseHandle(b.hThread);
    CloseHandle(c.hProcess);
    CloseHandle(c.hThread);

    return 0;
}

