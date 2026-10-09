
#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    HANDLE readPipe, writePipe;
    SECURITY_ATTRIBUTES sa = {sizeof(sa), NULL, TRUE};

    if (!CreatePipe(&readPipe, &writePipe, &sa, 0)) {
        cout << "Pipe creation failed";
        return 1;
    }

    const char message[] = "Hello Child from Kernel Pipe";
    DWORD written;

    WriteFile(writePipe, message, sizeof(message), &written, NULL);
    CloseHandle(writePipe);

    char buffer[100];
    DWORD bytesRead;

    ReadFile(readPipe, buffer, sizeof(buffer) - 1,
             &bytesRead, NULL);
    buffer[bytesRead] = '\0';

    cout << "Child read from pipe: " << buffer << endl;

    CloseHandle(readPipe);
    return 0;
}

