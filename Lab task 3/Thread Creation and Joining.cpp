#include <iostream>
#include <thread>
using namespace std;

void child_routine() {
    cout << "Child executing with unique thread ID: "
         << this_thread::get_id() << endl;
}

int main() {
    thread t(child_routine);
    t.join();

    cout << "Main thread finished" << endl;
    return 0;
}

