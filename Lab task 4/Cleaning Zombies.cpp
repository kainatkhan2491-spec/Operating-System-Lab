
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION child[5] = {};
    si.cb = sizeof(si);

    for (int i = 0; i < 5; i++) {
        char command[] =
            "cmd.exe /c \"timeout /t 2 /nobreak > nul & exit 0\"";

        if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
            CREATE_NO_WINDOW, NULL, NULL, &si, &child[i])) {
            cout << "Child creation failed!" << endl;
            continue;
        }

        cout << "Child created with PID: "
             << child[i].dwProcessId << endl;
    }

    int remaining = 5;

    while (remaining > 0) {
        for (int i = 0; i < 5; i++) {
            if (child[i].hProcess != NULL) {
                DWORD result = WaitForSingleObject(
                    child[i].hProcess, 0
                );

                if (result == WAIT_OBJECT_0) {
                    cout << "Asynchronously reaped a terminated child." << endl;

                    CloseHandle(child[i].hProcess);
                    CloseHandle(child[i].hThread);

                    child[i].hProcess = NULL;
                    remaining--;
                }
            }
        }

        Sleep(200);
    }

    cout << "All child processes completed." << endl;

    return 0;
}

