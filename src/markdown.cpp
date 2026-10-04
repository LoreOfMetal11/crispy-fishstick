#include "markdown.h"

#include <fstream>
#include <iostream>
#include <regex>

using namespace std;

namespace
{
/**
 * @brief Removes carriage returns and trailing spaces from a line.
 * @param value Line to normalize.
 */
void cleanline(string& value)
{
    while (!value.empty() &&
           (value.back() == '\r' ||
            value.back() == ' ' ||
            value.back() == '\t'))
    {
        value.pop_back();
    }
}

/**
 * @brief Classifies one physical Markdown line.
 * @param line Source line.
 * @param marker Receives heading level or list indentation.
 * @return Detected line type.
 */
linekind classify(const string& line, int& marker)
{
    static const regex heading(
        R"(^([ ]{0,3})(#{1,6})[ ]+(.+?)[ ]*#*[ ]*$)");
    static const regex bullet(
        R"(^([ ]{0,3})([-+*])[ ]+(.+)$)");
    static const regex ordered(
        R"(^([ ]{0,3})([0-9]+)[.)][ ]+(.+)$)");
    static const regex fence(
        R"(^[ ]{0,3}(`{3,}|~{3,})(.*)$)");

    marker = 0;

    if (line.empty() || line.find_first_not_of(' ') == string::npos)
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

/**
 * @brief Checks whether a line is an ordered or unordered list item.
 * @param kind Line type to check.
 * @return True for both supported list types.
 */
bool islist(linekind kind)
{
    return kind == linekind::Bullet || kind == linekind::Ordered;
}

/**
 * @brief Reads parameters of a Markdown code fence.
 * @param value Source line containing a fence.
 * @param symbol Receives the fence character.
 * @param length Receives the fence length.
 * @param rest Receives text after the fence.
 */
void fenceinfo(const string& value, char& symbol, int& length, string& rest)
{
    size_t first = value.find_first_not_of(' ');
    symbol = value[first];

    size_t end = value.find_first_not_of(symbol, first);

    if (end == string::npos)
    {
        length = static_cast<int>(value.size() - first);
        rest.clear();
    }
    else
    {
        length = static_cast<int>(end - first);
        rest = value.substr(end);
    }
}
}

vector<dataline> readmd(const string& file)
{
    ifstream input(file, ios::binary);
    vector<dataline> result;

    if (!input)
    {
        cerr << "Cannot open file: " << file << '\n';
        return result;
    }

    const int blocksize = 4096;
    char buffer[blocksize];
    string pending;
    int number = 1;

    while (input.read(buffer, blocksize) || input.gcount() > 0)
    {
        streamsize count = input.gcount();
        pending.append(buffer, static_cast<size_t>(count));

        size_t end;

        while ((end = pending.find('\n')) != string::npos)
        {
            string line = pending.substr(0, end);
            pending.erase(0, end + 1);
            cleanline(line);

            int marker = 0;
            linekind kind = classify(line, marker);

            result.push_back({line, kind, number, marker});
            number++;
        }
    }

    if (!pending.empty())
    {
        cleanline(pending);

        int marker = 0;
        linekind kind = classify(pending, marker);

        result.push_back({pending, kind, number, marker});
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
            char symbol;
            int length;
            string rest;
            fenceinfo(line.value, symbol, length, rest);

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
    paragraph current{};
    bool opened = false;
    bool infence = false;
    char fencesymb = 0;
    int fencelength = 0;

    for (const dataline& line : lines)
    {
        if (line.kind == linekind::Fence)
        {
            char symbol;
            int length;
            string rest;
            fenceinfo(line.value, symbol, length, rest);

            if (!infence)
            {
                infence = true;
                fencesymb = symbol;
                fencelength = length;
            }
            else if (symbol == fencesymb &&
                     length >= fencelength &&
                     rest.find_first_not_of(' ') == string::npos)
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
            char symbol;
            int length;
            string rest;
            fenceinfo(line.value, symbol, length, rest);

            if (!infence)
            {
                infence = true;
                fencesymb = symbol;
                fencelength = length;
            }
            else if (symbol == fencesymb &&
                     length >= fencelength &&
                     rest.find_first_not_of(' ') == string::npos)
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
        cout << "line " << line.number << ": " << line.value << '\n';
    }

    cout << "Found headings: " << data.size() << '\n';
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
        cout << "lines " << item.firstline << "-" << item.lastline
             << ": " << item.value << '\n';
    }

    cout << "Found paragraphs: " << data.size() << '\n';
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
        cout << "depth " << line.marker << ", line " << line.number
             << ": " << line.value << '\n';
    }

    cout << "Found list items: " << data.size() << '\n';
}
