
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;

const size_t N = 10000000;
const double f = 0.10;

long long SumRange(const vector<int>& A, size_t start, size_t end)
{
    long long sum = 0;

    for (size_t i = start; i < end; i++)
    {
        sum += A[i];
    }

    return sum;
}

long long ParallelSum(const vector<int>& A, size_t start,
                      size_t end, int T)
{
    vector<thread> threads;
    vector<long long> partialSum(T, 0);

    size_t total = end - start;
    size_t chunk = (total + T - 1) / T;

    for (int t = 0; t < T; t++)
    {
        size_t left = start + t * chunk;
        size_t right = min(left + chunk, end);

        if (left >= end)
            break;

        threads.emplace_back([&, t, left, right]()
        {
            partialSum[t] = SumRange(A, left, right);
        });
    }

    for (auto& th : threads)
    {
        th.join();
    }

    long long totalSum = 0;

    for (long long value : partialSum)
    {
        totalSum += value;
    }

    return totalSum;
}

int main()
{
    cout << fixed << setprecision(4);

    cout << "Task 12: Serial Fraction and Amdahl's Law\n";
    cout << "Array size: " << N << '\n';
    cout << "Serial fraction f: " << f << "\n\n";

    vector<int> A(N);

    for (size_t i = 0; i < N; i++)
    {
        A[i] = static_cast<int>(i % 100) + 1;
    }

    // Fully sequential baseline
    auto start = steady_clock::now();

    long long sequentialSum = SumRange(A, 0, N);

    auto finish = steady_clock::now();

    double Tseq = duration<double>(finish - start).count();

    cout << "Sequential sum: " << sequentialSum << '\n';
    cout << "Sequential time: " << Tseq << " seconds\n\n";

    size_t nser = static_cast<size_t>(f * N);

    cout << "Threads\tSum Correct\tMeasured Speedup\tPredicted Speedup\n";
    cout << "----------------------------------------------------------\n";

    for (int T : {1, 2, 4, 8})
    {
        start = steady_clock::now();

        // Serial section
        long long serialSum = SumRange(A, 0, nser);

        // Parallel section
        long long parallelSum =
            ParallelSum(A, nser, N, T);

        long long combinedSum = serialSum + parallelSum;

        finish = steady_clock::now();

        double tT = duration<double>(finish - start).count();

        double measuredSpeedup = Tseq / tT;

        // Conventional Amdahl's Law prediction
        double predictedSpeedup =
            1.0 / (f + (1.0 - f) / T);

        cout << T << "\t"
             << boolalpha << (combinedSum == sequentialSum) << "\t\t"
             << measuredSpeedup << "\t\t"
             << predictedSpeedup << '\n';
    }

    return 0;
}

