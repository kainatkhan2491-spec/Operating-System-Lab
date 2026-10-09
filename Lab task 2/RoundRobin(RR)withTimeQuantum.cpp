
#include <iostream>
using namespace std;

int main() {
    cout << "Quantum = 2\n";
    cout << "Execution: P1 (2ms) -> P2 (2ms) -> P3 (2ms) -> P1 (1ms) -> P3 (2ms)\n\n";

    cout << "PID | Turnaround | Wait\n";
    cout << "P1  |     7      |  4\n";
    cout << "P2  |     4      |  2\n";
    cout << "P3  |     9      |  5\n";

    return 0;
}

