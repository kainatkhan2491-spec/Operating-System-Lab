
#include <iostream>
#include <thread>
using namespace std;

int matrix[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

int vector1[3] = {1, 2, 3};
int result[3];

void worker_multiply_row(int row) {
    result[row] = 0;

    for (int col = 0; col < 3; col++) {
        result[row] += matrix[row][col] * vector1[col];
    }
}

int main() {
    thread t1(worker_multiply_row, 0);
    thread t2(worker_multiply_row, 1);
    thread t3(worker_multiply_row, 2);

    t1.join();
    t2.join();
    t3.join();

    cout << "Result Vector: [";

    for (int i = 0; i < 3; i++) {
        cout << result[i];
        if (i < 2) cout << ", ";
    }

    cout << "]" << endl;

    return 0;
}

