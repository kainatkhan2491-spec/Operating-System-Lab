
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
bool ready = false;

void worker(int id) {
    cout << "Worker " << id << " is waiting" << endl;

    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return ready; });
    lock.unlock();

    cout << "Worker " << id << " started" << endl;
}

int main() {
    thread t1(worker, 1);
    thread t2(worker, 2);

    cout << "Main prepares resources" << endl;

    {
        lock_guard<mutex> lock(mtx);
        ready = true;
    }
    cv.notify_all();

    t1.join();
    t2.join();

    return 0;
}

