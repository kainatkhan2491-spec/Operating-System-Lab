
#include <iostream>
#include <future>
using namespace std;

int heavy_calc(int n) {
    return n * n;
}

int main() {
    future<int> fut = async(launch::async, heavy_calc, 5);

    cout << "Main thread is working" << endl;

    int result = fut.get();

    cout << "Result: " << result << endl;
    return 0;
}

