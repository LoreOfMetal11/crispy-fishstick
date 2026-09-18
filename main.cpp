#include <chrono>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "markdown.h"
#include "userdata.h"

using namespace std;
using Clock = chrono::steady_clock;

void encoding()
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    setlocale(LC_ALL, "");
}

/// @brief Prints the application menu.
void menu()
{
    cout << "\n1  Check markdown\n";
    cout << "2  Find headings\n";
    cout << "3  Find paragraphs\n";
    cout << "4  Find lists\n";
    cout << "5  Check profile\n";
    cout << "6  Generate profile\n";
    cout << "0  Exit\n";
    cout << " ";
}

/// @brief Prints elapsed time in microseconds.
/// @param start Start timestamp.
/// @param finish Finish timestamp.
void prtime(Clock::time_point start,
    Clock::time_point finish)
{
    auto elapsed = chrono::duration_cast<chrono::microseconds>(finish - start);
    cout << "Time: " << elapsed.count() << " microseconds\n";
}

/// @brief Reads a profile after removing the menu input newline.
/// @return Profile entered by the user.
Profile inputprof()
{
    string rest;
    getline(cin, rest);

    cout << "Last name\n";
    cout << "First name\n";
    cout << "Middle name\n";
    cout << "Birth date\n";
    cout << "Phone\n";
    cout << "E-mail\n";

    return readprof(cin);
}

int main()
{
    encoding();
    const string file = "text.md";
    vector<dataline> lines = readmd(file);
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
            bool valid = checkmd(lines);
            finish = Clock::now();
            cout << (valid ? "Markdown is valid\n" : "Markdown has errors\n");
            break;
        }
        case 2:
        {
            start = Clock::now();
            vector<dataline> result = findhead(lines);
            prhead(result);
            finish = Clock::now();
            break;
        }
        case 3:
        {
            start = Clock::now();
            vector<paragraph> result = findparag(lines);
            prparag(result);
            finish = Clock::now();
            break;
        }
        case 4:
        {
            start = Clock::now();
            vector<dataline> result = findlist(lines);
            prlist(result);
            finish = Clock::now();
            break;
        }
        case 5:
        {
            Profile profile = inputprof();

            start = Clock::now();

            bool valid = checkprof(profile);

            finish = Clock::now();

            cout << (valid
                ? "Profile is valid\n"
                : "Profile has errors\n");

            break;
        }
        case 6:
        {
            start = Clock::now();
            Profile profile = randprof();
            finish = Clock::now();
            prprof(profile);
            break;
        }
        default:
            break;
        }

        if (choice >= 1 && choice <= 6)
        {
            prtime(start, finish);
        }
    } while (choice != 0);

    return 0;
}