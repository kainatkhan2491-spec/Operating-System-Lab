
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
using namespace std;

int permits = 3;
mutex mtx;
condition_variable cv;

void task(int id) {
    {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [] {
            return permits > 0;
        });

        permits--;
    }

    cout << "Task " << id << " is using resource" << endl;

    this_thread::sleep_for(chrono::milliseconds(200));

    cout << "Task " << id << " released resource" << endl;

    {
        lock_guard<mutex> lock(mtx);
        permits++;
    }

    cv.notify_one();
}

int main() {
    vector<thread> threads;

    for (int i = 1; i <= 5; i++) {
        threads.emplace_back(task, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "All tasks completed" << endl;
    return 0;
}

