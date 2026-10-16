
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex A, B;

void thread1() {
    lock_guard<mutex> l1(A);
    cout << "Thread 1 locked A" << endl;

    this_thread::sleep_for(chrono::milliseconds(100));

    lock_guard<mutex> l2(B);
    cout << "Thread 1 locked B" << endl;
}

void thread2() {
    lock_guard<mutex> l1(B);
    cout << "Thread 2 locked B" << endl;

    this_thread::sleep_for(chrono::milliseconds(100));

    lock_guard<mutex> l2(A);
    cout << "Thread 2 locked A" << endl;
}

int main() {
    thread t1(thread1);
    thread t2(thread2);

    t1.join();
    t2.join();

    return 0;
}

