
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

int main()
{
    cout << fixed << setprecision(4);

    cout << "Task 14: Amdahl vs Gustafson\n";

    double p;

    cout << "Enter estimated parallel fraction p (0 < p <= 1): ";
    cin >> p;

    if (!cin || p <= 0.0 || p > 1.0)
    {
        cout << "Invalid input. Enter p between 0 and 1.\n";
        return 1;
    }

    ifstream file("task13_results.csv");

    if (!file)
    {
        cout << "Error: task13_results.csv not found.\n";
        cout << "Run Task13.exe in this folder first.\n";
        return 1;
    }

    string line;

    // Skip CSV header
    getline(file, line);

    cout << "\nParallel fraction p = " << p << "\n\n";

    cout << "Threads\tAmdahl\t\tGustafson\tMeasured\n";
    cout << "------------------------------------------------\n";

    while (getline(file, line))
    {
        stringstream ss(line);

        int N;
        double timeTaken, measuredSpeedup;
        double efficiency, overhead;
        char comma;

        if (!(ss >> N >> comma
                 >> timeTaken >> comma
                 >> measuredSpeedup >> comma
                 >> efficiency >> comma
                 >> overhead))
        {
            continue;
        }

        // Standard Amdahl's Law: fixed problem size
        double amdahl =
            1.0 / ((1.0 - p) + p / N);

        // Gustafson's Law: scaled problem size
        double gustafson =
            N - (1.0 - p) * (N - 1);

        cout << N << "\t"
             << amdahl << "\t\t"
             << gustafson << "\t\t"
             << measuredSpeedup << '\n';
    }

    file.close();

    return 0;
}

