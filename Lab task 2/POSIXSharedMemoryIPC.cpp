
#include <iostream>
#include <windows.h>
#include <cstring>
using namespace std;

int main() {
    HANDLE hMap = CreateFileMappingA(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
        0, 1024, "OS_SharedMemory"
    );

    if (hMap == NULL) {
        cout << "Error creating shared memory";
        return 1;
    }

    char* ptr = (char*)MapViewOfFile(
        hMap, FILE_MAP_ALL_ACCESS, 0, 0, 1024
    );

    if (ptr == NULL) {
        cout << "Error mapping memory";
        CloseHandle(hMap);
        return 1;
    }

    strcpy(ptr, "OS Shared Memory Payload");

    cout << "Child read from SHM: " << ptr << endl;

    UnmapViewOfFile(ptr);
    CloseHandle(hMap);

    return 0;
}

