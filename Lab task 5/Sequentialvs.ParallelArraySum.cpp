
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <iomanip>

using namespace std;
using namespace chrono;

const size_t N = 50000000;  // 50 million elements

vector<long long> A(N, 1);
vector<long long> partial;

void worker(size_t tid, size_t chunkSize) {
    size_t lo = tid * chunkSize;
    size_t hi = min(N, lo + chunkSize);

    long long localSum = 0;

    for (size_t i = lo; i < hi; i++) {
        localSum += A[i];
    }

    partial[tid] = localSum;
}

int main() {
    cout << "Sequential vs Parallel Array Sum\n";
    cout << "Array size: " << N << " elements\n\n";

    // Sequential sum
    long long sequentialSum = 0;

    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < N; i++) {
        sequentialSum += A[i];
    }

    auto end = high_resolution_clock::now();

    double sequentialTime =
        duration<double>(end - start).count();

    cout << "Sequential Sum: " << sequentialSum << '\n';
    cout << "Sequential Time: " << sequentialTime
         << " seconds\n\n";

    cout << left << setw(12) << "Threads"
         << setw(20) << "Parallel Time"
         << setw(15) << "Speedup"
         << "Correct Sum\n";

    for (int T : {1, 2, 4, 8}) {
        partial.assign(T, 0);

        size_t chunkSize = (N + T - 1) / T;
        vector<thread> threads;

        start = high_resolution_clock::now();

        for (int t = 0; t < T; t++) {
            threads.emplace_back(worker, t, chunkSize);
        }

        for (auto& th : threads) {
            th.join();
        }

        long long parallelSum =
            accumulate(partial.begin(), partial.end(), 0LL);

        end = high_resolution_clock::now();

        double parallelTime =
            duration<double>(end - start).count();

        double speedup = sequentialTime / parallelTime;

        cout << left << setw(12) << T
             << setw(20) << parallelTime
             << setw(15) << speedup
             << parallelSum << '\n';
    }

    return 0;
}

