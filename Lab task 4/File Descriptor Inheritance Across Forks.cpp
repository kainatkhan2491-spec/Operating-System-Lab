
#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

int main() {
    ofstream file("log.txt", ios::out | ios::trunc);

    if (!file) {
        cout << "File opening failed!" << endl;
        return 1;
    }

    file << "[Child Data]" << endl;
    file << "[Parent Data]" << endl;
    file.close();

    cout << "Data written successfully to log.txt" << endl;

    return 0;
}

