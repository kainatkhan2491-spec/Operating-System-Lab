
#include <iostream>
#include <thread>
using namespace std;

int counter = 0;

void increment_task() {
    for (int i = 1; i <= 100000; i++) {
        counter++;
    }
}

int main() {
    thread thread1(increment_task);
    thread thread2(increment_task);

    thread1.join();
    thread2.join();

    cout << "Expected: 200000 | Actual counter: "
         << counter << endl;

    return 0;
}

