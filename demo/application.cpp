#include "application.h"

#include <iostream>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

Application::Application(const string& fileName) : file(fileName)
{
    encoding();
    lines = readmd(file);
}

void Application::encoding()
{
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    setlocale(LC_ALL, "");
}

void Application::menu() const
{
    cout << "\n1 Check markdown\n";
    cout << "2 Find headings\n";
    cout << "3 Find paragraphs\n";
    cout << "4 Find lists\n";
    cout << "5 Check profile\n";
    cout << "6 Generate profile\n";
    cout << "0 Exit\n";
    cout << "> ";
}

void Application::printTime(
    Clock::time_point start,
    Clock::time_point finish) const
{
    auto elapsed = chrono::duration_cast<chrono::microseconds>(finish - start);

    cout << "Time: " << elapsed.count() << " microseconds\n";
}

Profile Application::inputProfile()
{
    string rest;
    getline(cin, rest);

    cout << "Enter last name, first name, middle name,\n";
    cout << "birth date (DD.MM.YYYY), phone and e-mail.\n";
    cout << "Write every value on a new line:\n";

    return readprof(cin);
}

void Application::handleChoice(int choice)
{
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
        finish = Clock::now();

        prhead(result);
        break;
    }
    case 3:
    {
        start = Clock::now();
        vector<paragraph> result = findparag(lines);
        finish = Clock::now();

        prparag(result);
        break;
    }
    case 4:
    {
        start = Clock::now();
        vector<dataline> result = findlist(lines);
        finish = Clock::now();

        prlist(result);
        break;
    }
    case 5:
    {
        Profile profile = inputProfile();

        start = Clock::now();
        bool valid = checkprof(profile);
        finish = Clock::now();

        cout << (valid ? "Profile is valid\n" : "Profile has errors\n");
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
        printTime(start, finish);
    }
}

void Application::run()
{
    int choice = -1;

    do
    {
        menu();

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "Enter an integer from 0 to 6\n";
            choice = -1;
            continue;
        }

        if (choice < 0 || choice > 6)
        {
            cout << "Unknown menu item\n";
            continue;
        }

        handleChoice(choice);
    }
    while (choice != 0);
}
