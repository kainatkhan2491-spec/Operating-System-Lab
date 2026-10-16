
#include <iostream>
#include <thread>
#include <functional>
using namespace std;

void worker(int val, int& ref) {
    val += 10;
    ref += 10;

    cout << "Worker value: " << val << endl;
    cout << "Worker reference: " << ref << endl;
}

int main() {
    int x = 5, y = 5;

    thread t(worker, x, ref(y));
    t.join();

    cout << "Original x: " << x << endl;
    cout << "Original y: " << y << endl;

    return 0;
}

