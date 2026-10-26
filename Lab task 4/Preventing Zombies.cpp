
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION child = {};
    si.cb = sizeof(si);

    char command[] = "cmd.exe /c echo Child terminating immediately.";

    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &child)) {
        cout << "Child creation failed!" << endl;
        return 1;
    }

    cout << "Child process created. PID: "
         << child.dwProcessId << endl;

    cout << "Parent sleeping for 10 seconds..." << endl;
    Sleep(10000);

    WaitForSingleObject(child.hProcess, INFINITE);

    cout << "Child process completed. Parent finished waiting." << endl;

    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);

    return 0;
}

