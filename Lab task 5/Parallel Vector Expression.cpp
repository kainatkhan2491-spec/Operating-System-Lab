
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace chrono;

void sequential(const vector<double>& A,
                const vector<double>& B,
                vector<double>& C, size_t n) {
    for (size_t i = 0; i < n; i++)
        C[i] = A[i] * B[i] + sin(A[i]);
}

void worker(const vector<double>& A,
            const vector<double>& B,
            vector<double>& C,
            size_t n, size_t tid, size_t T) {
    size_t chunk = (n + T - 1) / T;
    size_t lo = tid * chunk;
    size_t hi = min(n, lo + chunk);

    for (size_t i = lo; i < hi; i++)
        C[i] = A[i] * B[i] + sin(A[i]);
}

void threaded(const vector<double>& A,
              const vector<double>& B,
              vector<double>& C, size_t n, size_t T) {
    vector<thread> threads;
    for (size_t t = 0; t < T; t++)
        threads.emplace_back(worker, cref(A), cref(B),
                             ref(C), n, t, T);
    for (auto& th : threads)
        th.join();
}

void openmpVersion(const vector<double>& A,
                   const vector<double>& B,
                   vector<double>& C, size_t n, int T) {
    #pragma omp parallel for num_threads(T)
    for (long long i = 0; i < static_cast<long long>(n); i++)
        C[i] = A[i] * B[i] + sin(A[i]);
}

bool check(const vector<double>& X,
           const vector<double>& Y, double eps = 1e-9) {
    for (size_t i = 0; i < X.size(); i++)
        if (fabs(X[i] - Y[i]) > eps)
            return false;
    return true;
}

template <typename Function>
double measure(Function f) {
    auto start = high_resolution_clock::now();
    f();
    auto end = high_resolution_clock::now();
    return duration<double, milli>(end - start).count();
}

int main() {
    cout << fixed << setprecision(3);
    const double eps = 1e-9;

    for (size_t n : {1000ULL, 100000ULL, 1000000ULL}) {
        vector<double> A(n), B(n), Cseq(n), Cthread(n), Comp(n);

        for (size_t i = 0; i < n; i++) {
            A[i] = 0.01 * (i % 1000);
            B[i] = 0.5 + 0.001 * (i % 500);
        }

        double ts = measure([&] {
            sequential(A, B, Cseq, n);
        });

        cout << "\nArray size: " << n << '\n';
        cout << "Sequential: " << ts << " ms\n";

        for (int T : {1, 2, 4, 8}) {
            double tt = measure([&] {
                threaded(A, B, Cthread, n, T);
            });

            double to = measure([&] {
                openmpVersion(A, B, Comp, n, T);
            });

            cout << "T=" << T
                 << " | std::thread: " << tt << " ms"
                 << " | speedup: " << ts / tt
                 << " | correct: " << boolalpha
                 << check(Cseq, Cthread, eps) << '\n';

            cout << "    OpenMP: " << to << " ms"
                 << " | speedup: " << ts / to
                 << " | correct: " << check(Cseq, Comp, eps)
                 << '\n';
        }
    }

    return 0;
}

