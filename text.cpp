#include "text.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/// @brief Checks if a byte is part of a Latin letter or digit.
bool islatin(unsigned char ch) {
    return (ch >= '0' && ch <= '9') ||
        (ch >= 'A' && ch <= 'Z') ||
        (ch >= 'a' && ch <= 'z');
}

/// @brief Checks if a byte is a UTF-8 continuation byte (10xxxxxx).
bool isutf(unsigned char ch) {
    return (ch & 0xC0) == 0x80;
}

/// @brief Normalizes a token: keeps ASCII alnum (folded to lowercase)
/// and full UTF-8 sequences (Cyrillic letters), discarding punctuation.
/// @param token Raw string from the input stream.
/// @return Clean word used as a map key.
string clean(const string& token) {
    string word;
    size_t place = 0;

    while (place < token.size())
    {
        unsigned char first =
            static_cast<unsigned char>(token[place]);

        if (islatin(first))
        {
            if (first >= 'A' && first <= 'Z')
            {
                first = static_cast<unsigned char>(first + 32);
            }

            word += static_cast<char>(first);
            place++;
            continue;
        }

        // Try to interpret as UTF-8 lead byte for non-ASCII letters.
        int size = 0;

        if ((first & 0xE0) == 0xC0)
        {
            size = 2;
        }
        else if ((first & 0xF0) == 0xE0)
        {
            size = 3;
        }
        else if ((first & 0xF8) == 0xF0)
        {
            size = 4;
        }

        bool valid = size > 0 && place + size <= token.size();

        for (int index = 1; valid && index < size; index++)
        {
            valid = isutf(
                static_cast<unsigned char>(token[place + index])
            );
        }

        if (valid)
        {
            // UTF-8 Cyrillic letters are copied as complete multi-byte sequences.
            // Bytes are kept unchanged, so the original Russian spelling remains valid.
            word.append(token, place, size);
            place += size;
        }
        else
        {
            place++;
        }
    }

    return word;
}

textinf readtext(const string& file)
{
    ifstream input(file, ios::binary);
    textinf text;
    string token;
    int number = 0;

    while (input >> token)
    {
        string word = clean(token);

        if (!word.empty())
        {
            text.counts[word]++;
            text.places[word].push_back(number);
            text.lengths.push_back(static_cast<int>(word.size()));
            number++;
        }
    }

    return text;
}

void prcount(const textinf& text)
{
    for (const auto& item : text.counts)
    {
        cout << item.first << " - " << item.second << '\n';
    }
}

void prplaces(const textinf& text)
{
    for (const auto& item : text.places)
    {
        cout << item.first << " - ";

        for (size_t index = 0; index < item.second.size(); index++)
        {
            cout << item.second[index];

            if (index + 1 < item.second.size())
            {
                cout << ", ";
            }
        }

        cout << '\n';
    }
}