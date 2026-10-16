
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION child = {};
    si.cb = sizeof(si);

    char command[] = "cmd.exe /c exit 42";

    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &child)) {
        cout << "Child creation failed!" << endl;
        return 1;
    }

    WaitForSingleObject(child.hProcess, INFINITE);

    DWORD status;
    GetExitCodeProcess(child.hProcess, &status);

    if (status == 42) {
        cout << "Child exited normally with status: " << status << endl;
    }
    else {
        cout << "Child exited with status: " << status << endl;
    }

    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);

    return 0;
}

