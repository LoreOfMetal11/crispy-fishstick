#include "userdata.h"

#include <cstdio>
#include <iostream>
#include <random>
#include <regex>
#include <vector>

using namespace std;

Profile readprof(istream& input)
{
    Profile profile;

    getline(input, profile.surname);
    getline(input, profile.name);
    getline(input, profile.middlename);
    getline(input, profile.birth);
    getline(input, profile.phone);
    getline(input, profile.email);

    return profile;
}

namespace
{
    /// @brief Checks whether a byte is a UTF-8 continuation byte.
    /// @param value Byte to inspect.
    /// @return True if the byte has the 10xxxxxx form.
    bool iscondy(unsigned char value)
    {
        return (value & 0xC0) == 0x80;
    }

    /// @brief Checks a UTF-8 string for Cyrillic letters only.
    /// @param value String to inspect.
    /// @return True if the string contains only Russian letters.
    bool isutf8(const string& value)
    {
        if (value.empty())
        {
            return false;
        }

        size_t position = 0;
        int letters = 0;

        while (position < value.size())
        {
            unsigned char first = static_cast<unsigned char>(value[position]);

            if (first < 0x80)
            {
                return false;
            }

            if ((first & 0xE0) == 0xC0)
            {
                if (position + 1 >= value.size())
                {
                    return false;
                }

                unsigned char second =
                    static_cast<unsigned char>(
                        value[position + 1]);

                if (!iscondy(second))
                {
                    return false;
                }

                position += 2;
            }
            else
            {
                return false;
            }

            letters++;
        }

        return letters >= 2;
    }

    /// @brief Check one field using a regular expression.
    /// @param value Field value.
    /// @param pattern Required regular expression.
    /// @param name Field name.
    /// @return True if the complete field matches.
    bool checkregex(const string& value,
        const regex& pattern,
        const string& name)
    {
        bool valid = regex_match(value, pattern);

        if (!valid)
        {
            cerr << "Invalid " << name
                << ": " << value << '\n';
        }

        return valid;
    }

    /// @brief Checks one Russian personal name.
    /// @param value Name value.
    /// @param name Field description.
    /// @return True if the value contains Russian letters.
    bool checkname(const string& value,
        const string& name)
    {
        bool valid = isutf8(value);

        if (!valid)
        {
            cerr << "Invalid " << name
                << ": " << value << '\n';
        }

        return valid;
    }
}

bool checkprof(const Profile& profile)
{
    const regex date(
        R"(^([0-9]{2}\.[0-9]{2}\.[0-9]{4}|[0-9]{4}-[0-9]{2}-[0-9]{2})$)");

    const regex phone(
        R"(^((\+7[0-9]{10})|(8[0-9]{10}))$)");

    const regex email(
        R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

    bool valid = true;

    valid = checkname(
        profile.surname,
        "last name") && valid;

    valid = checkname(
        profile.name,
        "first name") && valid;

    valid = checkname(
        profile.middlename,
        "middle name") && valid;

    valid = checkregex(
        profile.birth,
        date,
        "birth date") && valid;

    valid = checkregex(
        profile.phone,
        phone,
        "phone") && valid;

    valid = checkregex(
        profile.email,
        email,
        "email") && valid;

    return valid;
}

Profile randprof()
{
    static const vector<string> surname1 =
    {
        "Сидоров",
        "Иванов",
        "Петров",
        "Кузнецов"
    };

    static const vector<string> name1 =
    {
        "Иван",
        "Пётр",
        "Никита",
        "Алексей"
    };

    static const vector<string> middle1name =
    {
        "Иванович",
        "Петрович",
        "Никитич",
        "Алексеевич"
    };

    random_device device;
    mt19937 generator(device());

    auto choose = [&](const vector<string>& values) -> string
        {
            uniform_int_distribution<size_t> distribution(0, values.size() - 1);

            return values[distribution(generator)];
        };

    Profile profile;

    profile.surname = choose(surname1);
    profile.name = choose(name1);
    profile.middlename = choose(middle1name);

    uniform_int_distribution<int> day(1, 30);
    uniform_int_distribution<int> month(1, 12);
    uniform_int_distribution<int> year(1980, 2026);

    char date[11];

    snprintf(
        date,
        sizeof(date),
        "%02d.%02d.%04d",
        day(generator),
        month(generator),
        year(generator));

    profile.birth = date;

    uniform_int_distribution<int> digit(0, 9);

    profile.phone = "+7";

    for (int index = 0; index < 10; index++)
    {
        profile.phone += static_cast<char>('0' + digit(generator));
    }

    profile.email = "user";

    for (int index = 0; index < 4; index++)
    {
        profile.email += static_cast<char>('a' + digit(generator));
    }

    profile.email += "@mail.ru";

    return profile;
}

void prprof(const Profile& profile)
{
    cout << profile.surname << ' '
        << profile.name << ' '
        << profile.middlename << '\n';

    cout << profile.birth << '\n';
    cout << profile.phone << '\n';
    cout << profile.email << '\n';
}