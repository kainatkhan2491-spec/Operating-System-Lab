
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION child = {};
    si.cb = sizeof(si);

    char command[] = "cmd.exe /c exit 0";

    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &child)) {
        cout << "Child creation failed!" << endl;
        return 1;
    }

    cout << "Child process created. PID: "
         << child.dwProcessId << endl;

    cout << "Parent sleeping for 20 seconds..." << endl;
    cout << "Child process will finish during this time." << endl;

    Sleep(20000);

    WaitForSingleObject(child.hProcess, INFINITE);

    cout << "Child process reaped (completed and cleaned up)." << endl;

    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);

    return 0;
}

