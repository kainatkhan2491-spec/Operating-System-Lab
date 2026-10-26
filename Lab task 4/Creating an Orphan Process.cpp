
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION child = {};
    si.cb = sizeof(si);

    char command[] =
        "cmd.exe /c \"timeout /t 2 /nobreak > nul & "
        "echo Child is still running after parent exit\"";

    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &child)) {
        cout << "Child creation failed!" << endl;
        return 1;
    }

    cout << "Parent PID: " << GetCurrentProcessId() << endl;
    cout << "Parent exiting immediately. Child may continue running." << endl;

    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);

    return 0;
}

