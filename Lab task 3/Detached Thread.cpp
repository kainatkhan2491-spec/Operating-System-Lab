#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void logger() {
    for (int i = 1; i <= 3; i++) {
        cout << "Background logger: " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

int main() {
    thread t(logger);
    t.detach();

    cout << "Main thread continues" << endl;
    this_thread::sleep_for(chrono::seconds(1));

    return 0;
}

