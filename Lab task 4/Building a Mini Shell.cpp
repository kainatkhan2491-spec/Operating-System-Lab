
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
using namespace std;

int main() {
    string line;

    while (true) {
        cout << "myshell > ";
        getline(cin, line);

        if (line == "exit")
            break;

        if (line.empty())
            continue;

        string command = "cmd.exe /c " + line;

        STARTUPINFOA si = {};
        PROCESS_INFORMATION pi = {};
        si.cb = sizeof(si);

        if (CreateProcessA(NULL, &command[0], NULL, NULL,
            FALSE, 0, NULL, NULL, &si, &pi)) {

            WaitForSingleObject(pi.hProcess, INFINITE);

            DWORD status;
            GetExitCodeProcess(pi.hProcess, &status);

            cout << "Command exit status: " << status << endl;

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else {
            cout << "Command failed!" << endl;
        }
    }

    cout << "Shell closed." << endl;
    return 0;
}

