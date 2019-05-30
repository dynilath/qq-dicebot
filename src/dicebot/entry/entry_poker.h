#include "./entry_base.h"
namespace dicebot::entry {
    class entry_poker : public entry_base {
    public:
        entry_poker() noexcept;
        bool resolve_request(std::string const &message, event_info &event, std::string &response) noexcept override;
    };
} // namespace dicebot::entry