
#include <iostream>
#include <windows.h>
using namespace std;

int main(int argc, char* argv[]) {

    if (argc > 1) {
        cout << "Child executing task..." << endl;

        Sleep(2000);

        cout << "Child exiting with code 42" << endl;

        return 42;
    }

    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};

    si.cb = sizeof(si);

    char command[] = "child.exe child";

    // Create the child process using the same program
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    char childCommand[MAX_PATH + 20];
    snprintf(childCommand, sizeof(childCommand),
             "\"%s\" child", path);

    if (!CreateProcessA(
        NULL, childCommand, NULL, NULL, FALSE,
        0, NULL, NULL, &si, &pi
    )) {
        cout << "Process creation failed" << endl;
        return 1;
    }

    cout << "Parent waiting for child..." << endl;

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    cout << "Parent: Child terminated with exit status "
         << exitCode << endl;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

