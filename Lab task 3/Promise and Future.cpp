
#include <iostream>
#include <thread>
#include <future>
using namespace std;

int main() {
    promise<int> prom;
    future<int> fut = prom.get_future();

    thread worker([&prom]() {
        prom.set_value(42);
    });

    int result = fut.get();

    cout << "Result: " << result << endl;

    worker.join();
    return 0;
}

