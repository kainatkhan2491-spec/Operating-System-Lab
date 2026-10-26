
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

// Busy loop for the specified number of milliseconds
void Busy(int ms)
{
    auto start = steady_clock::now();

    while (duration_cast<milliseconds>(
               steady_clock::now() - start).count() < ms)
    {
        // Keep checking the elapsed time
    }
}

// Run four tasks concurrently and measure wall time
double RunTasks(const vector<int>& work)
{
    vector<thread> threads;

    auto start = steady_clock::now();

    for (int ms : work)
    {
        threads.emplace_back(Busy, ms);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    auto end = steady_clock::now();

    return duration<double, milli>(end - start).count();
}

int main()
{
    cout << fixed << setprecision(2);

    cout << "OS Lab Task 9: Fork-Join with Unequal Task Sizes\n\n";

    // Case 1: Unequal workloads
    vector<int> imbalanced = {100, 400, 100, 400};

    cout << "Case 1: Imbalanced Workload\n";
    cout << "Task times: 100, 400, 100, 400 ms\n";

    double imbalancedTime = RunTasks(imbalanced);

    cout << "Expected ideal time: 400 ms\n";
    cout << "Actual wall time: " << imbalancedTime << " ms\n\n";

    // Case 2: Equal workloads
    vector<int> balanced = {250, 250, 250, 250};

    cout << "Case 2: Balanced Workload\n";
    cout << "Task times: 250, 250, 250, 250 ms\n";

    double balancedTime = RunTasks(balanced);

    cout << "Expected ideal time: 250 ms\n";
    cout << "Actual wall time: " << balancedTime << " ms\n\n";

    cout << "Time saved after balancing: "
         << imbalancedTime - balancedTime << " ms\n";

    if (balancedTime > 0)
    {
        cout << "Imbalanced/Balanced time ratio: "
             << imbalancedTime / balancedTime << "x\n";
    }

    return 0;
}

