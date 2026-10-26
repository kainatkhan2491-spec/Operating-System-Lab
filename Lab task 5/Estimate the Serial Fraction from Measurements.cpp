
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;

int main()
{
    cout << fixed << setprecision(6);

    cout << "Task 15: Estimate Serial Fraction\n\n";

    ifstream file("task13_results.csv");

    if (!file)
    {
        cerr << "Error: task13_results.csv not found.\n";
        cerr << "Run Task13.exe first in the same folder.\n";
        return 1;
    }

    string line;

    // Skip CSV header
    getline(file, line);

    cout << "Threads\tMeasured Speedup\tEstimated f\n";
    cout << "-----------------------------------------------\n";

    double sumF = 0.0;
    int validCount = 0;

    while (getline(file, line))
    {
        stringstream ss(line);

        int N;
        double timeTaken, speedup, efficiency, overhead;
        char comma;

        if (!(ss >> N >> comma
                 >> timeTaken >> comma
                 >> speedup >> comma
                 >> efficiency >> comma
                 >> overhead))
        {
            continue;
        }

        if (N <= 1 || speedup <= 0.0)
            continue;

        double f =
            (1.0 / speedup - 1.0 / N)
            / (1.0 - 1.0 / N);

        cout << N << "\t"
             << speedup << "\t\t"
             << f << '\n';

        sumF += f;
        validCount++;
    }

    file.close();

    if (validCount > 0)
    {
        double averageF = sumF / validCount;

        cout << "\nAverage estimated serial fraction: "
             << averageF << '\n';

        cout << "Average estimated parallel fraction: "
             << 1.0 - averageF << '\n';
    }
    else
    {
        cout << "\nNo valid measurements found.\n";
    }

    return 0;
}

