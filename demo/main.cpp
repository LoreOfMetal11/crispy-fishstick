#include "application.h"

/**
 * @brief Starts the demonstration program.
 * @return Zero after normal completion.
 */
int main()
{
    Application application("text.md");
    application.run();

    return 0;
}
