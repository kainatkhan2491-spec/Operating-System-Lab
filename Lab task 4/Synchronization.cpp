
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    STARTUPINFOA si = {};
    PROCESS_INFORMATION child = {};
    si.cb = sizeof(si);

    char command[] =
        "cmd.exe /c \"for /L %i in (1,1,5) do "
        "(echo Child working... step %i & "
        "timeout /t 1 /nobreak > nul)\"";

    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
        0, NULL, NULL, &si, &child)) {
        cout << "Child creation failed!" << endl;
        return 1;
    }

    cout << "Parent waiting for child completion..." << endl;

    WaitForSingleObject(child.hProcess, INFINITE);

    cout << "Parent resumed: Child terminated successfully." << endl;

    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);

    return 0;
}

