#ifndef APPLICATION_H
#define APPLICATION_H

#include "markdown.h"
#include "userdata.h"

#include <chrono>
#include <string>
#include <vector>

/** @brief Console application demonstrating library calls. */
class Application
{
private:
    using Clock = std::chrono::steady_clock;

    std::string file;
    std::vector<dataline> lines;

    /** @brief Configures console encoding and locale. */
    void encoding();

    /** @brief Prints the main menu. */
    void menu() const;

    /**
     * @brief Prints elapsed operation time.
     * @param start Operation start time.
     * @param finish Operation finish time.
     */
    void printTime(Clock::time_point start, Clock::time_point finish) const;

    /**
     * @brief Reads a profile entered by the user.
     * @return Entered profile.
     */
    Profile inputProfile();

    /**
     * @brief Runs one menu command.
     * @param choice Selected menu item.
     */
    void handleChoice(int choice);

public:
    /**
     * @brief Creates the application and reads a Markdown file.
     * @param fileName Name of the Markdown file.
     */
    explicit Application(const std::string& fileName);

    /** @brief Runs the application's menu loop. */
    void run();
};

#endif
