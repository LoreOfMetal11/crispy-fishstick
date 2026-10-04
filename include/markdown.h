#ifndef MARKDOWN_H
#define MARKDOWN_H

#include "lab3_export.h"

#include <string>
#include <vector>

/** @brief Types of lines supported by the Markdown parser. */
enum class linekind
{
    Empty,
    Heading,
    Bullet,
    Ordered,
    Fence,
    Text
};

/** @brief Information about one line of a Markdown file. */
struct dataline
{
    std::string value;
    linekind kind;
    int number;
    int marker;
};

/** @brief Logical paragraph and its line range. */
struct paragraph
{
    std::string value;
    int firstline;
    int lastline;
};

/**
 * @brief Reads a Markdown file and classifies its lines.
 * @param file Name of the input file.
 * @return Vector of classified lines.
 */
LAB3_API std::vector<dataline> readmd(const std::string& file);

/**
 * @brief Checks the supported Markdown structure.
 * @param lines Classified Markdown lines.
 * @return True if the document structure is valid.
 */
LAB3_API bool checkmd(const std::vector<dataline>& lines);

/**
 * @brief Finds all ATX headings in the document.
 * @param lines Classified Markdown lines.
 * @return Vector containing the found headings.
 */
LAB3_API std::vector<dataline> findhead(const std::vector<dataline>& lines);

/**
 * @brief Joins neighbouring text lines into paragraphs.
 * @param lines Classified Markdown lines.
 * @return Vector containing the found paragraphs.
 */
LAB3_API std::vector<paragraph> findparag(const std::vector<dataline>& lines);

/**
 * @brief Finds ordered and unordered list items.
 * @param lines Classified Markdown lines.
 * @return Vector containing the found list items.
 */
LAB3_API std::vector<dataline> findlist(const std::vector<dataline>& lines);

/**
 * @brief Prints headings to the console.
 * @param data Headings to print.
 */
LAB3_API void prhead(const std::vector<dataline>& data);

/**
 * @brief Prints paragraphs to the console.
 * @param data Paragraphs to print.
 */
LAB3_API void prparag(const std::vector<paragraph>& data);

/**
 * @brief Prints list items to the console.
 * @param data List items to print.
 */
LAB3_API void prlist(const std::vector<dataline>& data);

#endif
