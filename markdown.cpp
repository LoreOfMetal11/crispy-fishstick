#include "markdown.h"
#include <fstream>
#include <iostream>
#include <regex>

using namespace std;

namespace
{
    /// @brief Removes carriage returns and trailing spaces from a line.
    /// @param value Line to normalize.
    void cleanline(string& value)
    {
        while (!value.empty() &&
            (value.back() == '\n' ||
                value.back() == ' ' ||
                value.back() == '\t'))
        {
            value.pop_back();
        }
    }

    /// @brief Classifies one physical Markdown line.
    /// @param line Source line without trailing invisible characters.
    /// @param marker Receives heading level or list indentation depth.
    /// @return Detected line kind.
    linekind classify(const string& line, int& marker)
    {
        static const regex heading(R"(^([ ]{0,3})(#{1,6})[ ]+(.+?)[ ]*#*[ ]*$)");

        static const regex bullet(R"(^([ ]{0,3})([-+*])[ ]+(.+)$)");

        static const regex ordered(R"(^([ ]{0,3})([0-9]+)[.)][ ]+(.+)$)");

        static const regex fence(R"(^[ ]{0,3}(`{3,}|~{3,})(.*)$)");

        marker = 0;

        if (line.empty() ||
            line.find_first_not_of(' ') == string::npos)
        {
            return linekind::Empty;
        }

        smatch match;

        if (regex_match(line, match, heading))
        {
            marker = static_cast<int>(match[2].str().size());

            return linekind::Heading;
        }

        if (regex_match(line, match, bullet))
        {
            marker = static_cast<int>(match[1].str().size());

            return linekind::Bullet;
        }

        if (regex_match(line, match, ordered))
        {
            marker = static_cast<int>(match[1].str().size());

            return linekind::Ordered;
        }

        if (regex_match(line, fence))
        {
            return linekind::Fence;
        }

        return linekind::Text;
    }

    /// @brief Returns true for both supported list types.
    /// @param kind Line kind to inspect.
    /// @return True for bullet and ordered list items.
    bool islist(linekind kind)
    {
        return kind == linekind::Bullet || kind == linekind::Ordered;
    }
}

vector<dataline> readmd(const string& file)
{
    ifstream input(file);
    vector<dataline> result;

    string value;
    int number = 1;

    while (getline(input, value))
    {
        cleanline(value);

        int marker = 0;
        linekind kind = classify(value, marker);

        dataline line;
        line.value = value;
        line.kind = kind;
        line.number = number;
        line.marker = marker;

        result.push_back(line);
        number++;
    }

    cout << "Loaded lines: " << result.size() << '\n';

    return result;
}

bool checkmd(const vector<dataline>& lines)
{
    bool valid = true;
    bool infence = false;

    char fencesymb = 0;
    int fencelength = 0;

    linekind activelist = linekind::Empty;
    int depth = -1;

    for (const dataline& line : lines)
    {
        if (line.kind == linekind::Fence)
        {
            size_t first = line.value.find_first_not_of(' ');

            if (first == string::npos)
            {
                continue;
            }

            char symbol = line.value[first];

            size_t end = line.value.find_first_not_of(symbol, first);

            int length;

            if (end == string::npos)
            {
                length = static_cast<int>(line.value.size() - first);
            }
            else
            {
                length = static_cast<int>(end - first);
            }

            string rest;

            if (end != string::npos)
            {
                rest = line.value.substr(end);
            }

            if (!infence)
            {
                infence = true;
                fencesymb = symbol;
                fencelength = length;

                activelist = linekind::Empty;
                depth = -1;
            }
            else
            {
                bool correctsymb = symbol == fencesymb;

                bool correctlength = length >= fencelength;

                bool spaceonly = rest.find_first_not_of(' ') == string::npos;

                if (correctsymb && correctlength && spaceonly)
                {
                    infence = false;
                }
            }

            continue;
        }

        if (infence)
        {
            continue;
        }

        if (line.kind == linekind::Empty)
        {
            activelist = linekind::Empty;
            depth = -1;
            continue;
        }

        if (islist(line.kind))
        {
            if (activelist == linekind::Empty)
            {
                activelist = line.kind;
                depth = line.marker;
            }
            else if (line.kind != activelist && line.marker <= depth)
            {
                cerr << "List type changes at line " << line.number << '\n';

                valid = false;
            }

            continue;
        }

        activelist = linekind::Empty;
        depth = -1;
    }

    if (infence)
    {
        cerr << "Unclosed code fence\n";
        valid = false;
    }

    return valid;
}

vector<dataline> findhead(const vector<dataline>& lines)
{
    vector<dataline> result;

    for (const dataline& line : lines)
    {
        if (line.kind == linekind::Heading)
        {
            result.push_back(line);
        }
    }

    return result;
}

vector<paragraph> findparag(const vector<dataline>& lines)
{
    vector<paragraph> result;
    paragraph current;

    bool opened = false;
    bool infence = false;

    char fencesymb = 0;
    int fencelength = 0;

    for (const dataline& line : lines)
    {
        if (line.kind == linekind::Fence)
        {
            size_t first = line.value.find_first_not_of(' ');

            if (first == string::npos)
            {
                continue;
            }

            char symbol = line.value[first];

            size_t end = line.value.find_first_not_of(symbol, first);

            int length;

            if (end == string::npos)
            {
                length = static_cast<int>(line.value.size() - first);
            }
            else
            {
                length = static_cast<int>(end - first);
            }

            string rest;

            if (end != string::npos)
            {
                rest = line.value.substr(end);
            }

            if (!infence)
            {
                infence = true;
                fencesymb = symbol;
                fencelength = length;
            }
            else if (symbol == fencesymb &&
                length >= fencelength &&
                rest.find_first_not_of(' ') ==
                string::npos)
            {
                infence = false;
            }

            if (opened)
            {
                result.push_back(current);
                opened = false;
            }

            continue;
        }

        if (infence)
        {
            continue;
        }

        if (line.kind == linekind::Text)
        {
            if (!opened)
            {
                current.value.clear();
                current.firstline = line.number;
                opened = true;
            }

            if (!current.value.empty())
            {
                current.value += ' ';
            }

            current.value += line.value;
            current.lastline = line.number;
        }
        else if (opened)
        {
            result.push_back(current);
            opened = false;
        }
    }

    if (opened)
    {
        result.push_back(current);
    }

    return result;
}

vector<dataline> findlist(const vector<dataline>& lines)
{
    vector<dataline> result;
    bool infence = false;

    char fencesymb = 0;
    int fencelength = 0;

    for (const dataline& line : lines)
    {
        if (line.kind == linekind::Fence)
        {
            size_t first = line.value.find_first_not_of(' ');

            if (first == string::npos)
            {
                continue;
            }

            char symbol = line.value[first];

            size_t end = line.value.find_first_not_of(symbol, first);

            int length;

            if (end == string::npos)
            {
                length = static_cast<int>(line.value.size() - first);
            }
            else
            {
                length = static_cast<int>(end - first);
            }

            string rest;

            if (end != string::npos)
            {
                rest = line.value.substr(end);
            }

            if (!infence)
            {
                infence = true;
                fencesymb = symbol;
                fencelength = length;
            }
            else if (symbol == fencesymb &&
                length >= fencelength &&
                rest.find_first_not_of(' ') ==
                string::npos)
            {
                infence = false;
            }

            continue;
        }

        if (!infence && islist(line.kind))
        {
            result.push_back(line);
        }
    }

    return result;
}

void prhead(const vector<dataline>& data)
{
    if (data.empty())
    {
        cout << "No headings found\n";
        return;
    }

    for (const dataline& line : data)
    {
        cout << "line " << line.number
            << ": " << line.value << '\n';
    }

    cout << "Found headings: "
        << data.size() << '\n';
}

void prparag(const vector<paragraph>& data)
{
    if (data.empty())
    {
        cout << "No paragraphs found\n";
        return;
    }

    for (const paragraph& item : data)
    {
        cout << "lines " << item.firstline
            << "-" << item.lastline
            << ": " << item.value << '\n';
    }

    cout << "Found paragraphs: "
        << data.size() << '\n';
}

void prlist(const vector<dataline>& data)
{
    if (data.empty())
    {
        cout << "No list items found\n";
        return;
    }

    for (const dataline& line : data)
    {
        cout << "depth " << line.marker
            << ", line " << line.number
            << ": " << line.value << '\n';
    }

    cout << "Found list items: "
        << data.size() << '\n';
}