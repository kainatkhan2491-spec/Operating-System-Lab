
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace chrono;

const int N = 512;

using Matrix = vector<double>;

void sequential(const Matrix& A, const Matrix& B, Matrix& C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i * N + k] * B[k * N + j];
            C[i * N + j] = sum;
        }
    }
}

void rowWorker(const Matrix& A, const Matrix& B, Matrix& C,
               int startRow, int endRow) {
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i * N + k] * B[k * N + j];
            C[i * N + j] = sum;
        }
    }
}

void threaded(const Matrix& A, const Matrix& B,
              Matrix& C, int T) {
    vector<thread> threads;
    int rowsPerThread = (N + T - 1) / T;

    for (int t = 0; t < T; t++) {
        int lo = t * rowsPerThread;
        int hi = min(N, lo + rowsPerThread);

        if (lo < N)
            threads.emplace_back(rowWorker, cref(A), cref(B),
                                 ref(C), lo, hi);
    }

    for (auto& th : threads)
        th.join();
}

void openmpVersion(const Matrix& A, const Matrix& B,
                   Matrix& C, int T) {
    #pragma omp parallel for collapse(2) num_threads(T)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i * N + k] * B[k * N + j];
            C[i * N + j] = sum;
        }
    }
}

template <typename Function>
double measure(Function f) {
    auto start = high_resolution_clock::now();
    f();
    auto end = high_resolution_clock::now();
    return duration<double, milli>(end - start).count();
}

bool check(const Matrix& A, const Matrix& B,
           double eps = 1e-8) {
    for (size_t i = 0; i < A.size(); i++)
        if (fabs(A[i] - B[i]) > eps)
            return false;
    return true;
}

int main() {
    cout << fixed << setprecision(2);
    Matrix A(N * N), B(N * N);
    Matrix Cseq(N * N), Cthread(N * N), Comp(N * N);

    for (int i = 0; i < N * N; i++) {
        A[i] = (i % 10) * 0.1;
        B[i] = (i % 7) * 0.2;
    }

    double ts = measure([&] {
        sequential(A, B, Cseq);
    });

    cout << "Matrix size: " << N << " x " << N << '\n';
    cout << "Sequential time: " << ts << " ms\n\n";

    cout << "Threads | std::thread (ms) | OpenMP (ms)"
         << " | Thread correct | OpenMP correct\n";

    for (int T : {1, 2, 4, 8}) {
        double tt = measure([&] {
            threaded(A, B, Cthread, T);
        });

        double to = measure([&] {
            openmpVersion(A, B, Comp, T);
        });

        cout << setw(7) << T << " | "
             << setw(16) << tt << " | "
             << setw(11) << to << " | "
             << boolalpha << check(Cseq, Cthread) << " | "
             << check(Cseq, Comp) << '\n';

        cout << "  std::thread speedup: " << ts / tt
             << "x, OpenMP speedup: " << ts / to << "x\n";
    }

    return 0;
}

