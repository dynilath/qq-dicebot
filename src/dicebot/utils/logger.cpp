#include "./logger.h"
#include <functional>

namespace dicebot::logger {
    std::function<void(std::string const &, std::string const &)> _log;
    void log(std::string const &scope, std::string const &content) {
        if (_log) _log(scope, content);
    }
} // namespace dicebot::logger
