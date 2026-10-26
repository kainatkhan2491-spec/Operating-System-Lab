
#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

const int K = 5;          // Buffer capacity
const int N = 20;         // Number of items to produce
const int SENTINEL = -1;  // Stop signal

array<int, K> B;
int in = 0;
int out = 0;
int count = 0;

mutex m;
condition_variable notFull;
condition_variable notEmpty;

int consumedCount = 0;
long long totalSum = 0;

// Producer function
void Producer(int n)
{
    for (int i = 0; i < n; i++)
    {
        unique_lock<mutex> lock(m);

        notFull.wait(lock, [] {
            return count < K;
        });

        B[in] = i;
        in = (in + 1) % K;
        count++;

        notEmpty.notify_one();
    }

    // Insert sentinel after producing all items
    {
        unique_lock<mutex> lock(m);

        notFull.wait(lock, [] {
            return count < K;
        });

        B[in] = SENTINEL;
        in = (in + 1) % K;
        count++;

        notEmpty.notify_one();
    }
}

// Consumer function
void Consumer()
{
    while (true)
    {
        int x;

        {
            unique_lock<mutex> lock(m);

            notEmpty.wait(lock, [] {
                return count > 0;
            });

            x = B[out];
            out = (out + 1) % K;
            count--;

            notFull.notify_one();
        }

        if (x == SENTINEL)
        {
            break;
        }

        // Process item outside the critical section
        totalSum += x;
        consumedCount++;

        cout << "Consumed: " << x << '\n';
    }
}

int main()
{
    cout << "OS Lab Task 7: Producer-Consumer Problem\n";
    cout << "Buffer capacity: " << K << '\n';
    cout << "Items to produce: " << N << "\n\n";

    thread producer(Producer, N);
    thread consumer(Consumer);

    producer.join();
    consumer.join();

    cout << "\nAll items consumed successfully.\n";
    cout << "Total items consumed: " << consumedCount << '\n';
    cout << "Sum of consumed items: " << totalSum << '\n';
    cout << "Expected sum: " << (1LL * N * (N - 1) / 2) << '\n';
    cout << "Buffer items remaining: " << count << '\n';

    return 0;
}

