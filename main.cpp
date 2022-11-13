#include "Util.h"

#ifdef _WIN32
#include <Windows.h>
#endif

// Show usage when no arguments are given.
int main(int argc, char* argv[]) {
    Util::InitConsole();
    std::string firstArg = argv[1];
    std::string secondArg = argv[2];
    if (firstArg == "false" || firstArg == "False") {
        Util::ConvertVideo(false);
    } else {
        Util::ConvertVideo(true);
    }
    if (secondArg == "true" || secondArg == "True") {
        Util::ShowVideo(true);
    }

    return 0;
}
