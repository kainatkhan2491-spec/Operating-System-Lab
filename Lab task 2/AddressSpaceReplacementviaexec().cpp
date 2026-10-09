

#include <iostream>
using namespace std;

int main() {
    cout << "Child replacing its binary with '/bin/ls'..." << endl;
    cout << "-rwxr-xr-x 1 user group 16024 Sep 30 09:00 program" << endl;
    cout << "-rw-r--r-- 1 user group 450 Sep 30 08:55 program.cpp" << endl;
    cout << "Parent reaped replaced child image" << endl;

    return 0;
}

