
#include <iostream>
#include <thread>
using namespace std;

void worker() {
    cout << "Worker thread is running" << endl;
}

int main() {
    {
        thread jt(worker);
        cout << "Main thread is running" << endl;
        jt.join();
    }

    cout << "Thread finished" << endl;
    return 0;
}

