
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int counter = 0;
mutex mtx;

void safe_increment() {
    for (int i = 1; i <= 100000; i++) {
        mtx.lock();
        counter++;
        mtx.unlock();
    }
}

int main() {
    thread thread1(safe_increment);
    thread thread2(safe_increment);

    thread1.join();
    thread2.join();

    cout << "Final counter with Mutex: "
         << counter << endl;

    return 0;
}

