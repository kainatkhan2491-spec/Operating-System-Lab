
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex A, B;

void task1() {
    lock(A, B);
    cout << "Task 1 acquired both locks" << endl;
    A.unlock();
    B.unlock();
}

void task2() {
    lock(A, B);
    cout << "Task 2 acquired both locks" << endl;
    A.unlock();
    B.unlock();
}

int main() {
    thread t1(task1);
    thread t2(task2);

    t1.join();
    t2.join();

    cout << "Deadlock avoided" << endl;
    return 0;
}

