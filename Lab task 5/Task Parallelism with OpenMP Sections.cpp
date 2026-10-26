
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <iomanip>
#include <functional>

using namespace std;
using namespace chrono;

const long long SQRT_COUNT = 50000000;
const size_t ARRAY_SIZE = 5000000;

double resultX = 0.0;
long long resultY = 0;

// Task X: CPU-intensive square-root calculations
void TaskX()
{
    double sum = 0.0;

    for (long long i = 1; i <= SQRT_COUNT; i++)
    {
        sum += sqrt(static_cast<double>(i) + 0.25);
    }

    resultX = sum;
}

// Task Y: Sort an array of integers
void TaskY(vector<int>& data)
{
    sort(data.begin(), data.end());

    resultY = data.empty() ? 0 : data.back();
}

int main()
{
    cout << fixed << setprecision(3);
    cout << "OS Lab Task 10: OpenMP Sections\n\n";

    // Generate the same input data for every method
    vector<int> original(ARRAY_SIZE);

    mt19937 rng(12345);
    uniform_int_distribution<int> dist(1, 1000000000);

    for (int& value : original)
    {
        value = dist(rng);
    }

    // --------------------------------
    // 1. Sequential execution
    // --------------------------------
    vector<int> sequentialData = original;

    auto start = steady_clock::now();

    TaskX();
    TaskY(sequentialData);

    auto end = steady_clock::now();

    double sequentialTime =
        duration<double>(end - start).count();

    double sequentialX = resultX;
    long long sequentialY = resultY;

    cout << "Sequential Execution:\n";
    cout << "Task X result: " << sequentialX << '\n';
    cout << "Task Y maximum: " << sequentialY << '\n';
    cout << "Time: " << sequentialTime << " seconds\n\n";

    // --------------------------------
    // 2. Pthread-style execution
    //    using std::thread
    // --------------------------------
    vector<int> pthreadData = original;

    start = steady_clock::now();

    thread t1(TaskX);
    thread t2(TaskY, ref(pthreadData));

    t1.join();
    t2.join();

    end = steady_clock::now();

    double pthreadTime =
        duration<double>(end - start).count();

    double pthreadX = resultX;
    long long pthreadY = resultY;

    cout << "Pthread (std::thread) Execution:\n";
    cout << "Task X result: " << pthreadX << '\n';
    cout << "Task Y maximum: " << pthreadY << '\n';
    cout << "Time: " << pthreadTime << " seconds\n\n";

    // --------------------------------
    // 3. OpenMP parallel sections
    // --------------------------------
    vector<int> openmpData = original;

    start = steady_clock::now();

    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            TaskX();
        }

        #pragma omp section
        {
            TaskY(openmpData);
        }
    }

    end = steady_clock::now();

    double openmpTime =
        duration<double>(end - start).count();

    double openmpX = resultX;
    long long openmpY = resultY;

    cout << "OpenMP Sections Execution:\n";
    cout << "Task X result: " << openmpX << '\n';
    cout << "Task Y maximum: " << openmpY << '\n';
    cout << "Time: " << openmpTime << " seconds\n\n";

    // --------------------------------
    // Comparison
    // --------------------------------
    cout << "Performance Comparison:\n";
    cout << "Sequential time: " << sequentialTime << " seconds\n";
    cout << "Pthread time:    " << pthreadTime << " seconds\n";
    cout << "OpenMP time:     " << openmpTime << " seconds\n\n";

    cout << "Pthread results correct: " << boolalpha
         << (pthreadX == sequentialX &&
             pthreadY == sequentialY) << '\n';

    cout << "OpenMP results correct: "
         << (openmpX == sequentialX &&
             openmpY == sequentialY) << '\n';

    cout << "OpenMP/Pthread time ratio: "
         << openmpTime / pthreadTime << "x\n";

    return 0;
}

