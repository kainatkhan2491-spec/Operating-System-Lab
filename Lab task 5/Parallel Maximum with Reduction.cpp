
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>

using namespace std;
using namespace chrono;

const size_t N = 10000000; // 10 million elements

vector<int> A;
vector<int> localMax;

void sequentialMax(size_t n, int& result) {
    result = A[0];

    for (size_t i = 1; i < n; i++) {
        if (A[i] > result)
            result = A[i];
    }
}

void worker(size_t tid, size_t n, size_t T) {
    size_t chunk = (n + T - 1) / T;
    size_t lo = tid * chunk;
    size_t hi = min(n, lo + chunk);

    if (lo >= hi) {
        localMax[tid] = A[0];
        return;
    }

    int m = A[lo];

    for (size_t i = lo + 1; i < hi; i++) {
        if (A[i] > m)
            m = A[i];
    }

    // Each thread writes only to its own localMax slot.
    localMax[tid] = m;
}

int parallelMax(size_t n, size_t T) {
    localMax.assign(T, 0);
    vector<thread> threads;

    for (size_t t = 0; t < T; t++) {
        threads.emplace_back(worker, t, n, T);
    }

    // Wait until all threads finish.
    for (auto& th : threads) {
        th.join();
    }

    // Sequential reduction after join.
    int result = localMax[0];

    for (size_t t = 1; t < T; t++) {
        result = max(result, localMax[t]);
    }

    return result;
}

int main() {
    A.resize(N);

    mt19937 rng(42);
    uniform_int_distribution<int> dist(-1000000, 1000000);

    for (size_t i = 0; i < N; i++)
        A[i] = dist(rng);

    // Ensure a known maximum exists.
    A[N / 2] = 2000000;

    int seqResult = 0;

    auto start = high_resolution_clock::now();
    sequentialMax(N, seqResult);
    auto end = high_resolution_clock::now();

    double seqTime = duration<double, milli>(end - start).count();

    cout << fixed << setprecision(3);
    cout << "Parallel Maximum with Reduction\n";
    cout << "Array size: " << N << '\n';
    cout << "Sequential maximum: " << seqResult << '\n';
    cout << "Sequential time: " << seqTime << " ms\n\n";

    cout << "Threads | Maximum | Time (ms) | Speedup | Correct\n";

    for (size_t T : {1, 2, 4, 8}) {
        start = high_resolution_clock::now();
        int result = parallelMax(N, T);
        end = high_resolution_clock::now();

        double parTime =
            duration<double, milli>(end - start).count();

        cout << setw(7) << T << " | "
             << setw(7) << result << " | "
             << setw(9) << parTime << " | "
             << setw(7) << seqTime / parTime << " | "
             << boolalpha << (result == seqResult) << '\n';
    }

    return 0;
}

