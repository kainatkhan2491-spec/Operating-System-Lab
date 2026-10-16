
#include <iostream>
#include <thread>
using namespace std;

thread_local int txn_id = 0;

void worker(int id) {
    txn_id = id;

    cout << "Thread " << id
         << " has local value " << txn_id << endl;
}

int main() {
    thread t1(worker, 1);
    thread t2(worker, 2);

    t1.join();
    t2.join();

    cout << "Main thread value: " << txn_id << endl;

    return 0;
}

