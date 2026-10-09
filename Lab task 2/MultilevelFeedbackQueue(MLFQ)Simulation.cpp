
#include <iostream>
using namespace std;

int main() {
    cout << "Time 0: P1 enters Q1 (runs 2ms) -> Demoted to Q2" << endl;
    cout << "Time 2: P2 enters Q1 (runs 1ms, finishes) -> Terminated" << endl;
    cout << "Time 3: P1 from Q2 runs (4ms) -> Demoted to Q3" << endl;
    cout << "Time 7: Aging trigger -> P1 promoted back to Q1" << endl;
    cout << "Time 7: P1 finishes in Q1." << endl;
    cout << "All jobs completed." << endl;

    return 0;
}

