#ifdef _WIN32
#    define NOMINMAX 1
#    define WIN32_LEAN_AND_MEAN 1
#    include <Windows.h>
#else
#    include <sys/ioctl.h>
#    include <unistd.h>
#endif // _WIN32

#include <cstdlib>
#include <iostream>

namespace {

#ifdef _WIN32

    unsigned terminal_width () noexcept {
        HANDLE soh = ::GetStdHandle (STD_OUTPUT_HANDLE);
        if (soh == INVALID_HANDLE_VALUE) {
            return 0U;
        }
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        if (::GetConsoleScreenBufferInfo (soh, &csbi) == 0) {
            return 0U;
        }
        auto const & window = csbi.srWindow;
        if (window.Right < window.Left) {
            return 0U;
        }
        return window.Right - window.Left + 1U;
    }

#else

    unsigned terminal_width () noexcept {
        winsize w{};
        return ::ioctl (STDOUT_FILENO, TIOCGWINSZ, &w) == -1 ? 0U : w.ws_col;
    }

#endif

} // end anonymous namespace

int main () {
    auto const width = terminal_width ();
    if (width == 0) {
        std::cout << "Terminal width is unknown. Is stdout redirected?\n";
    } else {
        std::cout << "Terminal width is " << width << " characters.\n";
    }
    return EXIT_SUCCESS;
}
