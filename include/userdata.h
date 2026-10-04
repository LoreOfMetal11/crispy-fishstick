#ifndef USERDATA_H
#define USERDATA_H

#include "lab3_export.h"

#include <istream>
#include <string>

/** @brief User profile data. */
struct Profile
{
    std::string lastname;
    std::string firstname;
    std::string middlename;
    std::string birthdate;
    std::string phone;
    std::string email;
};

/**
 * @brief Reads all profile fields from an input stream.
 * @param input Stream containing one field per line.
 * @return Filled profile structure.
 */
LAB3_API Profile readprof(std::istream& input);

/**
 * @brief Checks all fields of a user profile.
 * @param profile Profile to check.
 * @return True if every field is valid.
 */
LAB3_API bool checkprof(const Profile& profile);

/**
 * @brief Generates a random valid profile.
 * @return Generated profile.
 */
LAB3_API Profile randprof();

/**
 * @brief Prints a profile to the console.
 * @param profile Profile to print.
 */
LAB3_API void prprof(const Profile& profile);

#endif
