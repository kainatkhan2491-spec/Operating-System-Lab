
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex mutexA, mutexB;

void thread1_work() {
    mutexA.lock();

    cout << "Thread 1 acquired Lock A, waiting for Lock B..."
         << endl;

    this_thread::sleep_for(chrono::seconds(1));

    mutexB.lock();

    mutexB.unlock();
    mutexA.unlock();
}

void thread2_work() {
    mutexB.lock();

    cout << "Thread 2 acquired Lock B, waiting for Lock A..."
         << endl;

    this_thread::sleep_for(chrono::seconds(1));

    mutexA.lock();

    mutexA.unlock();
    mutexB.unlock();
}

int main() {
    thread t1(thread1_work);
    thread t2(thread2_work);

    t1.join();
    t2.join();

    cout << "Program completed" << endl;

    return 0;
}

