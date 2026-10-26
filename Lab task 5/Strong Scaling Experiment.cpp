
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace chrono;

const size_t N = 20000000;

long long SumRange(const vector<int>& A, size_t start, size_t end)
{
    long long sum = 0;

    for (size_t i = start; i < end; i++)
        sum += A[i];

    return sum;
}

long long ParallelSum(const vector<int>& A, int T)
{
    vector<thread> threads;
    vector<long long> partial(T, 0);

    size_t chunk = (A.size() + T - 1) / T;

    for (int t = 0; t < T; t++)
    {
        size_t left = t * chunk;
        size_t right = min(left + chunk, A.size());

        if (left >= A.size())
            break;

        threads.emplace_back([&, t, left, right]()
        {
            partial[t] = SumRange(A, left, right);
        });
    }

    for (auto& th : threads)
        th.join();

    long long total = 0;

    for (long long value : partial)
        total += value;

    return total;
}

double MeasureOverhead(int T)
{
    const int repetitions = 10;
    auto start = steady_clock::now();

    for (int r = 0; r < repetitions; r++)
    {
        vector<thread> threads;

        for (int t = 0; t < T; t++)
            threads.emplace_back([]() {});

        for (auto& th : threads)
            th.join();
    }

    auto end = steady_clock::now();

    return duration<double>(end - start).count()
           / repetitions;
}

int main()
{
    cout << fixed << setprecision(6);
    cout << "Task 13: Strong Scaling Experiment\n";
    cout << "Fixed array size: " << N << "\n\n";

    vector<int> A(N);

    for (size_t i = 0; i < N; i++)
        A[i] = static_cast<int>(i % 100) + 1;

    ofstream file("task13_results.csv");

    if (!file)
    {
        cerr << "Error: Could not create CSV file.\n";
        return 1;
    }

    file << "Threads,TimeSeconds,Speedup,Efficiency,"
            "CreateJoinOverheadSeconds\n";

    double T1 = 0.0;
    long long expectedSum = SumRange(A, 0, A.size());

    cout << "Threads\tTime(s)\t\tSpeedup\t\tEfficiency"
         << "\tOverhead(s)\n";

    cout << "------------------------------------------------"
            "------------------\n";

    for (int T = 1; T <= 8; T++)
    {
        auto start = steady_clock::now();

        long long sum = ParallelSum(A, T);

        auto end = steady_clock::now();

        double timeTaken =
            duration<double>(end - start).count();

        if (sum != expectedSum)
        {
            cerr << "Error: Incorrect sum for T = "
                 << T << '\n';
            return 1;
        }

        if (T == 1)
            T1 = timeTaken;

        double speedup = T1 / timeTaken;
        double efficiency = speedup / T;
        double overhead = MeasureOverhead(T);

        cout << T << "\t"
             << timeTaken << "\t"
             << speedup << "\t\t"
             << efficiency << "\t\t"
             << overhead << '\n';

        file << T << ','
             << timeTaken << ','
             << speedup << ','
             << efficiency << ','
             << overhead << '\n';
    }

    file.close();

    cout << "\nCorrectness check: PASSED\n";
    cout << "Results saved to task13_results.csv\n";

    return 0;
}

