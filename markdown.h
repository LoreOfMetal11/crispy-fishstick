#ifndef MARKDOWN_H
#define MARKDOWN_H

#include <string>
#include <vector>

enum class linekind
{
    Empty,
    Heading,
    Bullet,
    Ordered,
    Fence,
    Text
};

struct dataline
{
    std::string value;
    linekind kind;
    int number;
    int marker;
};

struct paragraph
{
    std::string value;
    int firstline;
    int lastline;
};

/// @brief Reads a Markdown file and converts its lines into lexical records.
/// @param file Name of the input Markdown file.
/// @return Vector of classified source lines.
std::vector<dataline> readmd(const std::string& file);

/// @brief Checks the supported Markdown subset using a state machine.
/// @param lines Previously classified Markdown lines.
/// @return True when all structural rules are satisfied.
bool checkmd(const std::vector<dataline>& lines);

/// @brief Extracts ATX headings from the document.
/// @param lines Classified Markdown lines.
/// @return All heading records in their original order.
std::vector<dataline> findhead(const std::vector<dataline>& lines);

/// @brief Joins neighbouring text lines into logical paragraphs.
/// @param lines Classified Markdown lines.
/// @return Paragraph records with source line ranges.
std::vector<paragraph> findparag(const std::vector<dataline>& lines);

/// @brief Extracts list items and preserves their nesting depth.
/// @param lines Classified Markdown lines.
/// @return List item records.
std::vector<dataline> findlist(const std::vector<dataline>& lines);

/// @brief Prints headings in the required console format.
/// @param data Heading records to print.
void prhead(const std::vector<dataline>& data);

/// @brief Prints paragraphs in the required console format.
/// @param data Paragraph records to print.
void prparag(const std::vector<paragraph>& data);

/// @brief Prints list items in the required console format.
/// @param data List records to print.
void prlist(const std::vector<dataline>& data);

#endif
