#ifndef NUMBERS_H
#define NUMBERS_H

#include <vector>

/// @brief Checks whether an integer is prime.
/// @param number Value to test.
/// @return True if prime, false otherwise.
bool isnum(int number);

/// @brief Squares every prime number in the vector.
/// @param nums Vector modified in place.
void square(std::vector<int>& nums);

/// @brief Reorders numbers: odds ascending first, then evens descending.
/// @param nums Original values (unchanged).
/// @return New vector with the required order.
std::vector<int> mixed(const std::vector<int>& nums);

/// @brief Extracts unique numbers inside [low, high].
/// @param nums Source values.
/// @param low Inclusive lower bound.
/// @param high Inclusive upper bound.
/// @return Sorted unique values from the range.
std::vector<int> pickrange(const std::vector<int>& nums, int low, int high);

/// @brief Prints vector elements on one line.
/// @param nums Values to display.
void prnums(const std::vector<int>& nums);

#endif