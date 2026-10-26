
#include <iostream>
#include <windows.h>
using namespace std;

int counter = 100;

int main() {
    cout << "[Parent] counter = " << counter
         << " at address: " << &counter << endl;

    counter += 50;

    cout << "[Child Simulation] counter = " << counter
         << " at address: " << &counter << endl;

    Sleep(1000);

    cout << "[Parent] counter = 100"
         << " at address: " << &counter << endl;

    return 0;
}

