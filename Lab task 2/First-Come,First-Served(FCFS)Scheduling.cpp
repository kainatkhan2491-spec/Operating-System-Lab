
#include <iostream>
using namespace std;

int main() {
    int arrival[] = {0, 1, 2};
    int burst[] = {5, 3, 8};
    int completion[3], turnaround[3], wait[3];
    int current_time = 0;
    float total_wait = 0;

    cout << "PID | Arrival | Burst | Completion | Turnaround | Wait\n";

    for (int i = 0; i < 3; i++) {
        if (current_time < arrival[i])
            current_time = arrival[i];

        completion[i] = current_time + burst[i];
        turnaround[i] = completion[i] - arrival[i];
        wait[i] = turnaround[i] - burst[i];
        current_time = completion[i];
        total_wait += wait[i];

        cout << "P" << i + 1 << "  | "
             << arrival[i] << "       | "
             << burst[i] << "     | "
             << completion[i] << "          | "
             << turnaround[i] << "          | "
             << wait[i] << endl;
    }

    cout << "Average Waiting Time: "
         << total_wait / 3 << " ms" << endl;

    return 0;
}

