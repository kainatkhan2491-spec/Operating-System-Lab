
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION child = {}, grandchild = {};
    si.cb = sizeof(si);

    char cmd1[] = "cmd.exe /c echo Grandchild process completed";

    // Parent creates Child
    char cmd2[] = "cmd.exe /c echo Child process completed";

    if (!CreateProcessA(NULL, cmd2, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &child)) {
        cout << "Child creation failed!" << endl;
        return 1;
    }

    if (!CreateProcessA(NULL, cmd1, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &grandchild)) {
        cout << "Grandchild creation failed!" << endl;
        WaitForSingleObject(child.hProcess, INFINITE);
        CloseHandle(child.hProcess);
        CloseHandle(child.hThread);
        return 1;
    }

    cout << "[Grandchild] Process created." << endl;
    WaitForSingleObject(grandchild.hProcess, INFINITE);
    cout << "[Child] Process completed." << endl;

    WaitForSingleObject(child.hProcess, INFINITE);
    cout << "[Parent] All descendants have completed." << endl;

    CloseHandle(grandchild.hProcess);
    CloseHandle(grandchild.hThread);
    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);

    return 0;
}

