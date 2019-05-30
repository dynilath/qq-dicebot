#include "./poker_manager.h"

using namespace dicebot;
using namespace poker;

std::unique_ptr<poker_manager> poker_manager::instance = nullptr;

poker_manager* poker_manager::create_instance() noexcept {
    poker_manager::instance = std::make_unique<poker_manager>();
    return poker_manager::instance.get();
}

poker_manager* poker_manager::get_instance() noexcept { return instance.get(); }

void poker_manager::destroy_instance() noexcept {
    poker_manager::instance = nullptr;
}

poker_deck* poker_manager::get_deck(const int64_t group_id) {
    auto iter = this->poker_map.find(group_id);
    if (iter != this->poker_map.end()) {
        return &iter->second;
    } else {
        auto iter_insert = this->poker_map.insert({group_id, poker_deck()});
        return &iter_insert.first->second;
    }
}