#include "userdata.h"

#include <array>
#include <cctype>
#include <ctime>
#include <iostream>
#include <random>
#include <regex>

using namespace std;

namespace
{
/**
 * @brief Checks whether a year is a leap year.
 * @param year Year to check.
 * @return True if the year is a leap year.
 */
bool isleap(int year)
{
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

/**
 * @brief Checks a date written in DD.MM.YYYY format.
 * @param value Date string to check.
 * @return True if the date exists and has the required format.
 */
bool checkdate(const string& value)
{
    static const regex format(R"(^([0-9]{2})\.([0-9]{2})\.([0-9]{4})$)");
    smatch match;

    if (!regex_match(value, match, format))
    {
        return false;
    }

    int day = stoi(match[1].str());
    int month = stoi(match[2].str());
    int year = stoi(match[3].str());

    if (year < 1900 || year > 2100 || month < 1 || month > 12)
    {
        return false;
    }

    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (isleap(year))
    {
        days[1] = 29;
    }

    return day >= 1 && day <= days[month - 1];
}

/**
 * @brief Checks a person's name.
 * @param value Name to check.
 * @return True if the name contains only letters or a hyphen.
 */
bool checkname(const string& value)
{
    if (value.size() < 2 || value.size() > 40)
    {
        return false;
    }

    for (unsigned char ch : value)
    {
        if (!isalpha(ch) && ch != '-' && ch < 128)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Returns a random integer in the specified range.
 * @param first Lowest possible value.
 * @param last Highest possible value.
 * @return Generated integer.
 */
int randomint(int first, int last)
{
    static mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    uniform_int_distribution<int> dist(first, last);
    return dist(gen);
}

/**
 * @brief Adds a leading zero to a one-digit number.
 * @param value Number to convert.
 * @return Two-character number string.
 */
string twodigits(int value)
{
    if (value < 10)
    {
        return "0" + to_string(value);
    }

    return to_string(value);
}
}

Profile readprof(istream& input)
{
    Profile profile;

    getline(input, profile.lastname);
    getline(input, profile.firstname);
    getline(input, profile.middlename);
    getline(input, profile.birthdate);
    getline(input, profile.phone);
    getline(input, profile.email);

    return profile;
}

bool checkprof(const Profile& profile)
{
    static const regex phone(R"(^\+?[0-9][0-9 ()-]{8,18}$)");
    static const regex email(
        R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

    bool valid = true;

    if (!checkname(profile.lastname))
    {
        cerr << "Invalid last name\n";
        valid = false;
    }

    if (!checkname(profile.firstname))
    {
        cerr << "Invalid first name\n";
        valid = false;
    }

    if (!checkname(profile.middlename))
    {
        cerr << "Invalid middle name\n";
        valid = false;
    }

    if (!checkdate(profile.birthdate))
    {
        cerr << "Invalid birth date\n";
        valid = false;
    }

    if (!regex_match(profile.phone, phone))
    {
        cerr << "Invalid phone\n";
        valid = false;
    }

    if (!regex_match(profile.email, email))
    {
        cerr << "Invalid e-mail\n";
        valid = false;
    }

    return valid;
}

Profile randprof()
{
    const array<string, 5> lastnames = {
        "Ivanov", "Petrov", "Sidorov", "Smirnov", "Orlov"
    };
    const array<string, 5> firstnames = {
        "Ivan", "Petr", "Alexey", "Nikolay", "Mikhail"
    };
    const array<string, 5> middlenames = {
        "Ivanovich", "Petrovich", "Alexeevich", "Nikolaevich", "Mikhailovich"
    };

    int year = randomint(1970, 2005);
    int month = randomint(1, 12);
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (isleap(year))
    {
        days[1] = 29;
    }

    int day = randomint(1, days[month - 1]);
    string number;

    for (int i = 0; i < 10; i++)
    {
        number += static_cast<char>('0' + randomint(0, 9));
    }

    int id = randomint(100, 999);

    Profile profile;
    profile.lastname = lastnames[randomint(0, 4)];
    profile.firstname = firstnames[randomint(0, 4)];
    profile.middlename = middlenames[randomint(0, 4)];
    profile.birthdate = twodigits(day) + "." + twodigits(month) + "." + to_string(year);
    profile.phone = "+7" + number;
    profile.email = "student" + to_string(id) + "@example.com";

    return profile;
}

void prprof(const Profile& profile)
{
    cout << "Last name: " << profile.lastname << '\n';
    cout << "First name: " << profile.firstname << '\n';
    cout << "Middle name: " << profile.middlename << '\n';
    cout << "Birth date: " << profile.birthdate << '\n';
    cout << "Phone: " << profile.phone << '\n';
    cout << "E-mail: " << profile.email << '\n';
}
