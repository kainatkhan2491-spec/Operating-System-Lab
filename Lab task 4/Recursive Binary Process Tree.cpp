
#include <iostream>
#include <windows.h>
using namespace std;

void buildTree(int level, int maxDepth) {
    if (level >= maxDepth)
        return;

    STARTUPINFOA si = {};
    PROCESS_INFORMATION left = {}, right = {};
    si.cb = sizeof(si);

    char leftCmd[] = "cmd.exe /c echo Left child running";
    char rightCmd[] = "cmd.exe /c echo Right child running";

    if (CreateProcessA(NULL, leftCmd, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &left)) {
        cout << "Level " << level + 1
             << " Left Child PID: " << left.dwProcessId << endl;
    }

    if (CreateProcessA(NULL, rightCmd, NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &right)) {
        cout << "Level " << level + 1
             << " Right Child PID: " << right.dwProcessId << endl;
    }

    if (left.hProcess != NULL) {
        WaitForSingleObject(left.hProcess, INFINITE);
        CloseHandle(left.hProcess);
        CloseHandle(left.hThread);
    }

    if (right.hProcess != NULL) {
        WaitForSingleObject(right.hProcess, INFINITE);
        CloseHandle(right.hProcess);
        CloseHandle(right.hThread);
    }
}

int main() {
    int depth = 2;

    cout << "Binary Process Tree Started" << endl;
    buildTree(0, depth);
    cout << "All child processes completed." << endl;

    return 0;
}

