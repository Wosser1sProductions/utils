#ifndef UTILS_OS_HPP
#define UTILS_OS_HPP

#include "utils_compiler.hpp"
#include "utils_bits.hpp"
#include "utils_string.hpp"
#include "utils_traits.hpp"

#if defined(UTILS_OS_WIN)
    #include <windows.h>
    #define UTILS_OS_ENABLE_VIRTUAL_TERMINAL 0x0004
#else
    extern char **environ;
#endif

#include <iostream>
#include <map>
#include <cstdlib>


namespace utils::os {
    namespace internal {
        // Make inline instead of static, else every translation unit has a different one...
        inline bool _virtual_console_enabled = false;

        enum _Console_commands {
            _CC_CLS       = 1L << 0,
            _CC_CLLINE    = 1L << 1,
            _CC_CURSOR    = 1L << 2,
            _CC_RESET     = 1L << 3,

            _CC_FG        = 1L << 4,
            _CC_BG        = 1L << 5,
            _CC_BRIGHT    = 1L << 6,
            _CC_UNDERLINE = 1L << 7,
            _CC_BOLD      = 1L << 8,
            _CC_ITALIC    = 1L << 9,
            _CC_REVERSED  = 1L << 10,

            _CC_BLACK     = 1L << 11,
            _CC_RED       = 1L << 12,
            _CC_GREEN     = 1L << 13,
            _CC_YELLOW    = 1L << 14,
            _CC_BLUE      = 1L << 15,
            _CC_MAGENTA   = 1L << 16,
            _CC_CYAN      = 1L << 17,
            _CC_WHITE     = 1L << 18,
        } HEDLEY_FLAGS;

        inline constexpr _Console_commands
        operator&(const _Console_commands __a, const _Console_commands __b) {
            return _Console_commands(utils::traits::to_underlying(__a) & utils::traits::to_underlying(__b));
        }

        inline constexpr _Console_commands
        operator|(const _Console_commands __a, const _Console_commands __b) {
            return _Console_commands(utils::traits::to_underlying(__a) | utils::traits::to_underlying(__b));
        }

        inline constexpr _Console_commands
        operator^(const _Console_commands __a, const _Console_commands __b) {
            return _Console_commands(utils::traits::to_underlying(__a) ^ utils::traits::to_underlying(__b));
        }

        inline constexpr _Console_commands
        operator~(const _Console_commands __a) {
            return _Console_commands(~utils::traits::to_underlying(__a));
        }

        inline const _Console_commands&
        operator|=(_Console_commands& __a, const _Console_commands __b) {
            return __a = __a | __b;
        }

        inline const _Console_commands&
        operator&=(_Console_commands& __a, const _Console_commands __b) {
            return __a = __a & __b;
        }

        inline const _Console_commands&
        operator^=(_Console_commands& __a, const _Console_commands __b) {
            return __a = __a ^ __b;
        }
    }

    /**
     *  \brief  Public console command variable type.
     */
    typedef utils::os::internal::_Console_commands command_t;

    /**
     *  Namespace withe the concole commands types.
     *  These can be aggregated with the common bitwise operators.
     */
    namespace Console {
        static constexpr command_t CLS       = utils::os::internal::_CC_CLS;       ///< Clear entire screen
        static constexpr command_t CLLINE    = utils::os::internal::_CC_CLLINE;    ///< Clear the current line
        static constexpr command_t CURSOR    = utils::os::internal::_CC_CURSOR;    ///< Set cursor to start
        static constexpr command_t RESET     = utils::os::internal::_CC_RESET;     ///< Reset formatting to black bg with gray fg

        static constexpr command_t FG        = utils::os::internal::_CC_FG;        ///< Set foreground colour options
        static constexpr command_t BG        = utils::os::internal::_CC_BG;        ///< Set background colour options
        static constexpr command_t BRIGHT    = utils::os::internal::_CC_BRIGHT;    ///< Make text brighter
        static constexpr command_t UNDERLINE = utils::os::internal::_CC_UNDERLINE; ///< Make text underlined
        static constexpr command_t BOLD      = utils::os::internal::_CC_BOLD;      ///< Make text bold
        static constexpr command_t ITALIC    = utils::os::internal::_CC_ITALIC;    ///< Make text italic
        static constexpr command_t REVERSED  = utils::os::internal::_CC_REVERSED;  ///< Reverse FG/BG colours

        static constexpr command_t BLACK     = utils::os::internal::_CC_BLACK;     ///< Set colour
        static constexpr command_t RED       = utils::os::internal::_CC_RED;       ///< Set colour
        static constexpr command_t GREEN     = utils::os::internal::_CC_GREEN;     ///< Set colour
        static constexpr command_t YELLOW    = utils::os::internal::_CC_YELLOW;    ///< Set colour
        static constexpr command_t BLUE      = utils::os::internal::_CC_BLUE;      ///< Set colour
        static constexpr command_t MAGENTA   = utils::os::internal::_CC_MAGENTA;   ///< Set colour
        static constexpr command_t CYAN      = utils::os::internal::_CC_CYAN;      ///< Set colour
        static constexpr command_t WHITE     = utils::os::internal::_CC_WHITE;     ///< Set colour
    };

    /**
     *  \brief  Process the given commands \p cmd and write the appropriate
     *          virtual terminal codes to the given stream.
     *
     *          For Windows, call EnableVirtualConsole() once (standard output handle only).
     *
     *  \param  cmd
     *      The commands to process.
     *  \param out
     *      The stream to write the virtual codes to. (default: std::cout)
     */
    template<typename TChar, typename TCharTraits> ATTR_MAYBE_UNUSED
    static void Command(const command_t cmd, std::basic_ostream<TChar, TCharTraits>& out = std::cout) {
        if (HEDLEY_UNLIKELY(!utils::os::internal::_virtual_console_enabled))
            return;

        #define BASE_ "\033["
        out.flush();

        if (cmd & Console::RESET) {
            out << BASE_ "0m";
        }

        if (cmd & Console::CLS) {
            out << BASE_ "2J";
        } else if (cmd & Console::CLLINE) {
            out << BASE_ "2K";
        }

        if (cmd & Console::CURSOR) {
            out << BASE_ "H";
        }

        if (cmd & Console::BOLD) {
            out << BASE_ "1m";
        }

        if (cmd & Console::UNDERLINE) {
            out << BASE_ "4m";
        }

        if (cmd & Console::ITALIC) {
            out << BASE_ "3m";
        }

        if (cmd >= Console::BLACK) {
            out << BASE_;
            // A colour will be set
            if (cmd & Console::BG) {
                out << "4"; // Background
            } else {
                out << "3"; // Foreground
            }

            out << (utils::bits::msb(uint64_t(cmd) / uint64_t(Console::BLACK)) - 1);

            if (cmd & Console::BRIGHT) {
                out << ";1";
            }

            out << "m";
        }
    }

    /**
     *  \brief  Set the title of the current terminal window
     *          by calling the corresponding virtual code.
     *
     *  \param  title
     *      The value to set the title to.
     *  \param  out
     *      The stream to write the virtual codes to. (default: std::cout)
     */
    template<typename TChar, typename TCharTraits> ATTR_MAYBE_UNUSED
    static void SetScreenTitle(const std::string_view& title, std::basic_ostream<TChar, TCharTraits>& out = std::cout) {
        if (HEDLEY_LIKELY(utils::os::internal::_virtual_console_enabled)) {
            out << "\033]2;" << title << "\007";
        } else {
            #if defined(UTILS_OS_WIN)
                SetConsoleTitleA(LPCSTR(title.data()));
            #endif
        }
    }

    /**
     *  \brief  Pause the console and wait for user input.
     *
     *  \param  out
     *      The stream to write the virtual codes to. (default: std::cout)
     */
    ATTR_MAYBE_UNUSED
    static void PromptChar(const char expected = '\n') {
        switch (expected) {
            case '\r': ATTR_FALLTHROUGH
            case '\n':
                std::cout << "Press <RETURN> to continue...";
                break;
            default:
                std::cout << utils::string::format("Press %c to continue... ", expected);
        }
        while (std::cin.get() != expected);
    }

    /**
     *  \brief  Enable virtual commands processing for the default output handle
     *          on Windows. Unix is supported by default.
     */
    ATTR_MAYBE_UNUSED
    static void EnableVirtualConsole() {
        int error = 0;

        #ifdef UTILS_OS_ENABLE_VIRTUAL_TERMINAL
            // Set-up Windows terminal
            do { // Dummy loop to be able to break on error
                HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
                if (hOut == INVALID_HANDLE_VALUE) {
                    error = int(GetLastError());
                    break;
                }

                DWORD dwMode = 0;
                if (!GetConsoleMode(hOut, &dwMode)) {
                    error = int(GetLastError());
                    break;
                }

                dwMode |= UTILS_OS_ENABLE_VIRTUAL_TERMINAL;
                if (!SetConsoleMode(hOut, dwMode)) {
                    error = int(GetLastError());
                    break;
                }
            } while(false);
        #else
            // Unix is already good.
        #endif

        if (error) {
            std::cerr << "[utils::os::EnableVirtualConsole] Error: " << error << std::endl;
        } else {
            utils::os::internal::_virtual_console_enabled = true;
        }
    }

    /**
     *  \brief  Get the value of the variable \p name in the current
     *          calling environment.
     *  \param  name
     *      The name of the variable to get.
     *  \return Optionally returns the value as sts::string if it existed.
     */
    ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline std::optional<std::string> GetEnvironmentVar(const std::string_view name) {
        if (const auto res = std::getenv(name.data())) {
            return res;
        } else {
            return {};
        }
    }

#ifdef setenv
    /**
     *  \brief  Set the value of the variable \p name in the current
     *          calling environment. If it does not exist, it will be created,
     *          else it will be overwritten if \p overwrite == true.
     *  \param  name
     *      The name of the variable to set.
     *  \param  value
     *      The new value to set the variable to.
     *  \param  overwrite
     *      Whether to overwrite an existing variable.
     *  \return Returns true on success.
     */
    ATTR_MAYBE_UNUSED
    static inline bool SetEnvironmentVar(const std::string_view name, const std::string_view value, bool overwrite=true) {
        return std::setenv(name.data(), value.data(), int(overwrite)) == 0;
    }
#endif

#ifdef unsetenv
    /**
     *  \brief  Delete the variable \p name in the current calling environment.
     *
     *  \param  name
     *      The name of the variable to delete.
     *  \return Returns true on success.
     */
    ATTR_MAYBE_UNUSED
    static inline bool DelEnvironmentVar(const std::string_view name) {
        return std::unsetenv(name.data()) == 0;
    }
#endif

    /**
     *  \brief  Get a std::map with every variable in the current
     *          calling environment with its key and value (or "" if no value).
     *          Works on Windows and Unix systems.
     *          (Unix uses the external `char **environ` symbol).
     *
     *  \return Returns the created std::map.
     */
    ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline std::optional<std::map<std::string, std::string>>
        GetEnvironmentVars()
    {
        #if defined(UTILS_OS_WIN)
            using VarType = LPTSTR;
            #define GET_STRINGS()   GetEnvironmentStrings()
            #define GET_VAL()       var
            #define NEXT()          var += key.size()
            #define FREE()          FreeEnvironmentStrings(static_cast<VarType>(env_strings))
        #else
            using VarType = char**;
            #define GET_STRINGS()   environ
            #define GET_VAL(V)      *var
            #define NEXT()
            #define FREE()
        #endif

        if (void* env_strings = GET_STRINGS()) {
            std::map<std::string, std::string> envmap;

            for (VarType var = static_cast<VarType>(env_strings); *var; var++) {
                std::string key;

                if constexpr (std::is_same_v<std::decay_t<VarType>, wchar_t*>) {
                    const std::wstring_view wsv{reinterpret_cast<wchar_t*>(GET_VAL())};
                    key = utils::string::to_string(wsv);
                } else {
                    key = std::string{reinterpret_cast<char*>(GET_VAL())};
                }

                if (const auto equals_idx = utils::string::contains(key, '=')) {
                    envmap.emplace(std::string{key, 0, *equals_idx},
                                   std::string{key, *equals_idx + 1});
                } else {
                    envmap.emplace(key, "");
                }

                NEXT();
            }

            FREE();
            return { envmap };
        }

        return {};
    }
}

namespace std {
    template<typename TChar, typename TCharTraits>
    auto& operator<<(std::basic_ostream<TChar, TCharTraits>& stream, const utils::os::command_t& cmd) {
        utils::os::Command(cmd, stream);
        return stream;
    }
}

#undef UTILS_OS_ENABLE_VIRTUAL_TERMINAL
#endif // UTILS_OS_HPP
