
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
using namespace std;

mutex mtx;
condition_variable cv;
int count = 0;
int phase = 1;

void wait_for_all() {
    unique_lock<mutex> lock(mtx);
    int current_phase = phase;

    count++;

    if (count == 3) {
        count = 0;
        phase++;
        cv.notify_all();
    } else {
        cv.wait(lock, [current_phase] {
            return phase != current_phase;
        });
    }
}

void worker(int id) {
    cout << "Thread " << id << " phase 1" << endl;
    wait_for_all();

    cout << "Thread " << id << " phase 2" << endl;
    wait_for_all();
}

int main() {
    vector<thread> threads;

    for (int i = 1; i <= 3; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "All threads completed" << endl;
    return 0;
}

