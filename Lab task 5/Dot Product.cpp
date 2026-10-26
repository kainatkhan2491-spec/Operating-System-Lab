
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace chrono;

const int N = 1000000;
const int T = 8;

vector<long long> A(N, 2);
vector<long long> B(N, 3);

long long dot = 0;
mutex dotMutex;

void buggyWorker(int tid) {
    int chunk = (N + T - 1) / T;
    int lo = tid * chunk;
    int hi = min(N, lo + chunk);

    long long p = 0;

    for (int i = lo; i < hi; i++)
        p += A[i] * B[i];

    // BUG: Multiple threads update shared dot without a lock.
    dot = dot + p;
}

void mutexWorker(int tid) {
    int chunk = (N + T - 1) / T;
    int lo = tid * chunk;
    int hi = min(N, lo + chunk);

    long long p = 0;

    for (int i = lo; i < hi; i++)
        p += A[i] * B[i];

    lock_guard<mutex> lock(dotMutex);
    dot += p;
}

long long sequentialDot() {
    long long result = 0;

    for (int i = 0; i < N; i++)
        result += A[i] * B[i];

    return result;
}

int main() {
    cout << "Dot Product Race Condition Demo\n";
    cout << "Array size: " << N << ", Threads: " << T << "\n\n";

    long long correct = sequentialDot();
    cout << "Sequential result: " << correct << "\n\n";

    // 1. Buggy version: repeated trials.
    cout << "1. Buggy version (no lock):\n";

    for (int trial = 1; trial <= 5; trial++) {
        dot = 0;
        vector<thread> threads;

        for (int t = 0; t < T; t++)
            threads.emplace_back(buggyWorker, t);

        for (auto& th : threads)
            th.join();

        cout << "Trial " << trial << ": " << dot
             << " | Correct: " << boolalpha
             << (dot == correct) << '\n';
    }

    // 2. Fixed with mutex.
    dot = 0;
    vector<thread> mutexThreads;

    for (int t = 0; t < T; t++)
        mutexThreads.emplace_back(mutexWorker, t);

    for (auto& th : mutexThreads)
        th.join();

    cout << "\n2. Mutex result: " << dot
         << " | Correct: " << boolalpha
         << (dot == correct) << '\n';

    // 3. Fixed with OpenMP atomic.
    long long ompDot = 0;

    #pragma omp parallel num_threads(T)
    {
        long long p = 0;

        #pragma omp for
        for (int i = 0; i < N; i++)
            p += A[i] * B[i];

        #pragma omp atomic
        ompDot += p;
    }

    cout << "3. OpenMP atomic result: " << ompDot
         << " | Correct: " << boolalpha
         << (ompDot == correct) << '\n';

    return 0;
}

