
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;

queue<int> q;
mutex mtx;
condition_variable cv;

void producer() {
    {
        lock_guard<mutex> lock(mtx);
        q.push(100);
        cout << "Produced: 100" << endl;
    }
    cv.notify_one();
}

void consumer() {
    unique_lock<mutex> lock(mtx);

    cv.wait(lock, [] { return !q.empty(); });

    cout << "Consumed: " << q.front() << endl;
    q.pop();
}

int main() {
    thread c(consumer);
    thread p(producer);

    p.join();
    c.join();

    return 0;
}

