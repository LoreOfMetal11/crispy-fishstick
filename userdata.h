#ifndef USERDATA_H
#define USERDATA_H
#include <iosfwd>
#include <string>

struct Profile
{
    std::string surname;
    std::string name;
    std::string middlename;
    std::string birth;
    std::string phone;
    std::string email;
};

/// @brief Reads a profile from six separate input lines.
/// @param input Source stream containing profile fields.
/// @return Filled profile structure.
Profile readprof(std::istream& input);

/// @brief Validates every profile field.
/// @param profile Profile to validate
/// @return True if all fields are valid.
bool checkprof(const Profile& profile);

/// @brief Generates a pseudo-random valid profile.
/// @return Generated profile.
Profile randprof();

/// @brief Prints all profile fields.
/// @param profile Profile to print.
void prprof(const Profile& profile);

#endif