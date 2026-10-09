
#include <iostream>
using namespace std;

int main() {
    cout << "Timeline: [P1: 0-1] [P2: 1-5] "
         << "[P1: 5-10] [P3: 10-12]" << endl;
    cout << endl;

    cout << "PID | Completion | Waiting Time" << endl;
    cout << "P1  |     10     |      4" << endl;
    cout << "P2  |      5     |      0" << endl;
    cout << "P3  |     12     |      7" << endl;

    return 0;
}

