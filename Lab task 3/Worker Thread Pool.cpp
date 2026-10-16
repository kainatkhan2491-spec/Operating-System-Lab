
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
using namespace std;

queue<int> tasks;
mutex mtx;
condition_variable cv;
bool stop = false;

void worker(int id) {
    while (true) {
        int task;

        {
            unique_lock<mutex> lock(mtx);

            cv.wait(lock, [] {
                return stop || !tasks.empty();
            });

            if (stop && tasks.empty())
                return;

            task = tasks.front();
            tasks.pop();
        }

        cout << "Worker " << id
             << " executing task " << task << endl;
    }
}

int main() {
    vector<thread> pool;

    for (int i = 1; i <= 2; i++) {
        pool.emplace_back(worker, i);
    }

    {
        lock_guard<mutex> lock(mtx);

        for (int i = 1; i <= 5; i++) {
            tasks.push(i);
        }
    }

    cv.notify_all();

    {
        lock_guard<mutex> lock(mtx);
        stop = true;
    }

    cv.notify_all();

    for (auto& t : pool) {
        t.join();
    }

    cout << "All tasks completed" << endl;
    return 0;
}

