
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION a = {}, b = {};
    si.cb = sizeof(si);

    char cmdA[] = "cmd.exe /c \"timeout /t 4 /nobreak > nul & exit 10\"";
    char cmdB[] = "cmd.exe /c \"timeout /t 1 /nobreak > nul & exit 20\"";

    if (!CreateProcessA(NULL, cmdA, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &a)) {
        cout << "Child A creation failed!" << endl;
        return 1;
    }

    if (!CreateProcessA(NULL, cmdB, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &b)) {
        cout << "Child B creation failed!" << endl;
        WaitForSingleObject(a.hProcess, INFINITE);
        CloseHandle(a.hProcess);
        CloseHandle(a.hThread);
        return 1;
    }

    DWORD status;

    // Wait for Child A first
    WaitForSingleObject(a.hProcess, INFINITE);
    GetExitCodeProcess(a.hProcess, &status);
    cout << "Child A reaped with status: " << status << endl;

    // Then wait for Child B
    WaitForSingleObject(b.hProcess, INFINITE);
    GetExitCodeProcess(b.hProcess, &status);
    cout << "Child B reaped with status: " << status << endl;

    CloseHandle(a.hProcess);
    CloseHandle(a.hThread);
    CloseHandle(b.hProcess);
    CloseHandle(b.hThread);

    return 0;
}

