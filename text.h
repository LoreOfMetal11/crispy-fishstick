#ifndef TEXT_H
#define TEXT_H
#include <map>
#include <string>
#include <vector>

struct textinf
{
    std::map<std::string, int> counts;
    std::map<std::string, std::vector<int>> places;
    std::vector<int> lengths;
};

/// @brief Reads the file once and builds word counts, positions, and word lengths.
/// @param file Path to the text file.
/// @return Filled textinf structure ready for all tasks.
textinf readtext(const std::string& file);

/// @brief Prints word frequencies in the required format: "word - count".
/// @param text Prepared text data.
void prcount(const textinf& text);

/// @brief Prints word positions in the required format: "word - 0, 2, ...".
/// @param text Prepared text data with stored positions.
void prplaces(const textinf& text);

#endif