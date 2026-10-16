
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION child = {};
    si.cb = sizeof(si);

    char command[] = "cmd.exe /c \"timeout /t 3 /nobreak > nul & exit 0\"";

    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &child)) {
        cout << "Child creation failed!" << endl;
        return 1;
    }

    while (true) {
        DWORD result = WaitForSingleObject(child.hProcess, 0);

        if (result == WAIT_TIMEOUT) {
            cout << "Parent computing independent background tasks..." << endl;
            Sleep(500);
        }
        else if (result == WAIT_OBJECT_0) {
            cout << "Child finished! Parent wrapping up." << endl;
            break;
        }
        else {
            cout << "Error checking child process." << endl;
            break;
        }
    }

    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);

    return 0;
}

