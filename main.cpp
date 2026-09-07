#include <chrono>
#include <iostream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include "numbers.h"
#include "text.h"
using namespace std;
using Clock = chrono::steady_clock;

void menu()
{
    cout << "\n1  Word count\n";
    cout << "2  Word positions\n";
    cout << "3  Square primes\n";
    cout << "4  Mixed sorting\n";
    cout << "5  Values in range\n";
    cout << "0  Exit\n";
    cout << " ";
}

void printTime(Clock::time_point start, Clock::time_point finish)
{
    auto elapsed = chrono::duration_cast<chrono::microseconds>(finish - start);
    cout << "Time: " << elapsed.count() << " microseconds\n";
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // File is read once; all tasks reuse the same in-memory data.
    textinf text = readtext("war_and_peace.ru.txt");

    // Numbers for tasks 3-5 come from word lengths of the same text.
    vector<int> nums = text.lengths;

    int choice;

    do
    {
        menu();
        cin >> choice;

        Clock::time_point start;
        Clock::time_point finish;

        switch (choice)
        {
        case 1:
        {
            start = Clock::now();
            prcount(text);
            finish = Clock::now();
            break;
        }
        case 2:
        {
            start = Clock::now();
            prplaces(text);
            finish = Clock::now();
            break;
        }
        case 3:
        {
            vector<int> data = nums;
            start = Clock::now();
            square(data);
            prnums(data);
            finish = Clock::now();
            break;
        }
        case 4:
        {
            start = Clock::now();
            vector<int> sorted = mixed(nums);
            prnums(sorted);
            finish = Clock::now();
            break;
        }
        case 5:
        {
            int low;
            int high;

            cout << "Minimum: ";
            cin >> low;

            cout << "Maximum: ";
            cin >> high;

            start = Clock::now();
            vector<int> picked = pickrange(nums, low, high);
            prnums(picked);
            finish = Clock::now();
            break;
        }
        }

        if (choice >= 1 && choice <= 5)
        {
            printTime(start, finish);
        }
    } while (choice != 0);

    return 0;
}