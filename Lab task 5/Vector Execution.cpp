
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION child = {};
    si.cb = sizeof(si);

    char command[] =
        "cmd.exe /c \"echo Program: reporter arg1 arg2 & "
        "echo LAB_MODULE=OS_PROC & "
        "echo SEM=FALL2026\"";

    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
        0, NULL, NULL, &si, &child)) {
        cout << "Process execution failed!" << endl;
        return 1;
    }

    WaitForSingleObject(child.hProcess, INFINITE);

    cout << "Parent: Child execution completed." << endl;

    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);

    return 0;
}

