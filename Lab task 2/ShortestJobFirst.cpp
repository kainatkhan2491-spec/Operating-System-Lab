
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    int burst[] = {6, 8, 2};
    int wait[3], turnaround[3];
    string pid[] = {"P1", "P2", "P3"};

    // Execution order: P1 -> P3 -> P2
    wait[0] = 0;
    turnaround[0] = burst[0];

    wait[2] = turnaround[0];
    turnaround[2] = wait[2] + burst[2];

    wait[1] = turnaround[2];
    turnaround[1] = wait[1] + burst[1];

    cout << "Execution Order: P1 -> P3 -> P2\n\n";

    cout << "PID | Burst | Wait Time | Turnaround Time\n";
    cout << "P1  |   " << burst[0] << "   |     "
         << wait[0] << "     |       " << turnaround[0] << "\n";

    cout << "P3  |   " << burst[2] << "   |     "
         << wait[2] << "     |       " << turnaround[2] << "\n";

    cout << "P2  |   " << burst[1] << "   |     "
         << wait[1] << "     |      " << turnaround[1] << "\n";

    double average = (wait[0] + wait[1] + wait[2]) / 3.0;

    cout << fixed << setprecision(2);
    cout << "Average Waiting Time: " << average << " ms\n";

    return 0;
}

