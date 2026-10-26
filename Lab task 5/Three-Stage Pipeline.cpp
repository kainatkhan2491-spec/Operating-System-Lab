
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

const int N = 1000000;
const int K = 64;
const long long SENTINEL = -1;

// Bounded queue with mutex and condition variables
class BoundedQueue
{
private:
    queue<long long> q;
    int capacity;
    mutex m;
    condition_variable notFull;
    condition_variable notEmpty;

public:
    BoundedQueue(int size) : capacity(size) {}

    void Put(long long value)
    {
        unique_lock<mutex> lock(m);

        notFull.wait(lock, [this] {
            return q.size() < static_cast<size_t>(capacity);
        });

        q.push(value);

        lock.unlock();
        notEmpty.notify_one();
    }

    long long Get()
    {
        unique_lock<mutex> lock(m);

        notEmpty.wait(lock, [this] {
            return !q.empty();
        });

        long long value = q.front();
        q.pop();

        lock.unlock();
        notFull.notify_one();

        return value;
    }
};

BoundedQueue Q1(K);
BoundedQueue Q2(K);

long long pipelineSum = 0;

// Stage 1: Generate numbers
void Stage1(int n)
{
    for (int i = 0; i < n; i++)
    {
        Q1.Put(i);
    }

    Q1.Put(SENTINEL);
}

// Stage 2: Square each number
void Stage2()
{
    while (true)
    {
        long long x = Q1.Get();

        if (x == SENTINEL)
        {
            Q2.Put(SENTINEL);
            break;
        }

        Q2.Put(x * x);
    }
}

// Stage 3: Sum the squared numbers
void Stage3()
{
    long long sum = 0;

    while (true)
    {
        long long y = Q2.Get();

        if (y == SENTINEL)
        {
            break;
        }

        sum += y;
    }

    pipelineSum = sum;
}

int main()
{
    cout << "OS Lab Task 8: Three-Stage Pipeline\n";
    cout << "Numbers: " << N << '\n';
    cout << "Queue capacity: " << K << "\n\n";

    // Sequential execution
    long long sequentialSum = 0;

    auto start = steady_clock::now();

    for (long long i = 0; i < N; i++)
    {
        sequentialSum += i * i;
    }

    auto end = steady_clock::now();

    double sequentialTime =
        duration<double>(end - start).count();

    // Pipeline execution
    start = steady_clock::now();

    thread t1(Stage1, N);
    thread t2(Stage2);
    thread t3(Stage3);

    t1.join();
    t2.join();
    t3.join();

    end = steady_clock::now();

    double pipelineTime =
        duration<double>(end - start).count();

    cout << fixed << setprecision(6);

    cout << "Sequential Execution:\n";
    cout << "Sum of squares: " << sequentialSum << '\n';
    cout << "Time: " << sequentialTime << " seconds\n\n";

    cout << "Pipeline Execution:\n";
    cout << "Sum of squares: " << pipelineSum << '\n';
    cout << "Time: " << pipelineTime << " seconds\n\n";

    cout << "Results match: " << boolalpha
         << (sequentialSum == pipelineSum) << '\n';

    cout << "Sequential throughput: "
         << N / sequentialTime << " items/second\n";

    cout << "Pipeline throughput: "
         << N / pipelineTime << " items/second\n";

    cout << "Speedup (sequential / pipeline): "
         << sequentialTime / pipelineTime << "x\n";

    return 0;
}

