
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    double p;

    cout << "Task 11: Theoretical Speedup Calculator\n";
    cout << "Enter parallel fraction p (0 < p <= 1): ";
    cin >> p;

    if (p <= 0 || p > 1)
    {
        cout << "Invalid input! Enter p between 0 and 1.\n";
        return 1;
    }

    cout << "\nCore Count\tSpeedup\n";
    cout << "--------------------------\n";
    cout << fixed << setprecision(4);

    for (int N = 1; N <= 16; N++)
    {
        double S;

        if (p == 1.0)
        {
            // Avoid division by zero in the given formula.
            cout << N << "\t\tUndefined (division by zero)\n";
            continue;
        }

        S = 1.0 / (1.0 - p) + p / N;

        cout << N << "\t\t" << S << '\n';
    }

    return 0;
}

