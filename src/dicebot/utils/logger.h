#include <functional>
#include <string>

namespace dicebot::logger {
    extern std::function<void(std::string const& scope,
                              std::string const& content)>
        _log;
    extern void log(std::string const& scope, std::string const& content);
} // namespace dicebot::logger