
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace chrono;

const long long SQRT_COUNT = 50000000;
const size_t ARRAY_SIZE = 5000000;

volatile double resultX = 0.0;
long long resultY = 0;

void TaskX() {
    double sum = 0.0;

    for (long long i = 1; i <= SQRT_COUNT; i++) {
        sum += sqrt(static_cast<double>(i) + 0.25);
    }

    resultX = sum;
}

void TaskY(vector<int>& data) {
    sort(data.begin(), data.end());

    // Save a result so the task's output is observable.
    resultY = data.empty() ? 0 : data.back();
}

int main() {
    cout << fixed << setprecision(3);
    cout << "OS Lab Task 6: Two Independent Tasks\n";
    cout << "sqrt calculations: " << SQRT_COUNT << '\n';
    cout << "Array elements: " << ARRAY_SIZE << "\n\n";

    // Prepare the same input before measuring either approach.
    vector<int> original(ARRAY_SIZE);
    mt19937 rng(12345);
    uniform_int_distribution<int> dist(1, 1000000000);

    for (auto& value : original)
        value = dist(rng);

    // Version 1: Sequential execution.
    vector<int> sequentialData = original;

    auto start = steady_clock::now();

    TaskX();
    TaskY(sequentialData);

    auto end = steady_clock::now();

    double sequentialTime =
        duration<double>(end - start).count();

    double sequentialX = resultX;
    long long sequentialY = resultY;

    cout << "Sequential execution:\n";
    cout << "Task X result: " << sequentialX << '\n';
    cout << "Task Y maximum: " << sequentialY << '\n';
    cout << "Total wall time: " << sequentialTime << " seconds\n\n";

    // Version 2: Run both tasks concurrently.
    vector<int> parallelData = original;

    start = steady_clock::now();

    thread p1(TaskX);
    thread p2(TaskY, ref(parallelData));

    p1.join();
    p2.join();

    end = steady_clock::now();

    double parallelTime =
        duration<double>(end - start).count();

    cout << "Concurrent execution:\n";
    cout << "Task X result: " << resultX << '\n';
    cout << "Task Y maximum: " << resultY << '\n';
    cout << "Total wall time: " << parallelTime << " seconds\n\n";

    cout << "Sequential/Concurrent ratio: "
         << sequentialTime / parallelTime << "x\n";

    cout << "Task X result matches: "
         << boolalpha << (sequentialX == resultX) << '\n';

    cout << "Task Y result matches: "
         << (sequentialY == resultY) << '\n';

    return 0;
}

