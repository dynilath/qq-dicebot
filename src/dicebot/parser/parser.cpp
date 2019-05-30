#include "./parser.h"
#include <iostream>

using namespace dicebot;
using namespace diceparser;

enum class action_t : uint8_t { stack, resolve, error, accept };

#define s action_t::stack
#define r action_t::resolve
#define e action_t::error
#define a action_t::accept

struct action_item {
    action_t operation;
    uint8_t status;
} constexpr ACTION[][14] = {
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {s, 9}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {a, 0}},
    {{e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 16}, {s, 17}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 1}},
    {{e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 18}, {s, 19}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 2}},
    {{e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 20}, {s, 21}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 3}},
    {{e, 0}, {e, 0}, {r, 6}, {r, 6}, {r, 6}, {r, 6}, {s, 22}, {e, 0}, {e, 0}, {s, 23}, {e, 0}, {r, 6}, {r, 6}, {r, 6}},
    {{e, 0}, {e, 0}, {r, 4}, {r, 4}, {r, 4}, {r, 4}, {r, 4}, {r, 4}, {r, 4}, {r, 4}, {e, 0}, {r, 4}, {r, 4}, {r, 4}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {s, 9}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 8}, {s, 27}, {r, 8}, {r, 8}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 8}, {r, 8}, {r, 8}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 17}, {r, 17}, {r, 17}, {r, 17}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 17}, {r, 17}, {r, 17}},
    {{s, 6}, {s, 32}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 21}, {s, 33}, {r, 21}, {r, 21}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 21}, {r, 21}, {r, 21}},
    {{e, 0}, {e, 0}, {r, 37}, {r, 37}, {r, 37}, {r, 37}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 37}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {s, 39}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 43}, {s, 42}, {r, 43}, {r, 43}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 43}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 32}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {s, 64}, {e, 0}, {s, 16}, {s, 17}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {s, 65}, {e, 0}, {s, 18}, {s, 19}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {s, 66}, {e, 0}, {s, 20}, {s, 21}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 9}, {s, 27}, {r, 9}, {r, 9}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 9}, {r, 9}, {r, 9}},
    {{e, 0}, {e, 0}, {r, 22}, {s, 33}, {r, 22}, {r, 22}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 22}, {r, 22}, {r, 22}},
    {{e, 0}, {e, 0}, {r, 44}, {s, 42}, {r, 44}, {r, 44}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 44}},
    {{e, 0}, {e, 0}, {r, 12}, {r, 12}, {r, 12}, {r, 12}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 12}, {r, 12}, {r, 12}},
    {{s, 6}, {s, 32}, {e, 0}, {e, 0}, {s, 73}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 77}, {s, 78}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 33}, {r, 33}, {e, 0}},
    {{e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 79}, {s, 80}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 34}, {r, 34}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 6}, {r, 6}, {r, 6}, {r, 6}, {s, 22}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 6}, {r, 6}, {r, 6}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {s, 39}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 8}, {s, 83}, {r, 8}, {r, 8}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 8}, {r, 8}, {r, 8}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 21}, {s, 86}, {r, 21}, {r, 21}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 21}, {r, 21}, {r, 21}},
    {{e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 87}, {s, 88}, {e, 0}},
    {{s, 6}, {s, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {s, 14}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 11}, {s, 27}, {r, 11}, {r, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 11}, {r, 11}, {r, 11}},
    {{e, 0}, {e, 0}, {r, 24}, {s, 33}, {r, 24}, {r, 24}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 24}, {r, 24}, {r, 24}},
    {{e, 0}, {e, 0}, {r, 46}, {s, 42}, {r, 46}, {r, 46}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 46}},
    {{e, 0}, {e, 0}, {r, 10}, {s, 27}, {r, 10}, {r, 10}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 10}, {r, 10}, {r, 10}},
    {{e, 0}, {e, 0}, {r, 23}, {s, 33}, {r, 23}, {r, 23}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 23}, {r, 23}, {r, 23}},
    {{e, 0}, {e, 0}, {r, 45}, {s, 42}, {r, 45}, {r, 45}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 45}},
    {{e, 0}, {e, 0}, {r, 26}, {s, 27}, {r, 26}, {r, 26}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 26}, {r, 26}, {r, 26}},
    {{e, 0}, {e, 0}, {r, 28}, {s, 33}, {r, 28}, {r, 28}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 28}, {r, 28}, {r, 28}},
    {{e, 0}, {e, 0}, {r, 48}, {s, 42}, {r, 48}, {r, 48}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 48}},
    {{e, 0}, {e, 0}, {r, 25}, {s, 27}, {r, 25}, {r, 25}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 25}, {r, 25}, {r, 25}},
    {{e, 0}, {e, 0}, {r, 27}, {s, 33}, {r, 27}, {r, 27}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 27}, {r, 27}, {r, 27}},
    {{e, 0}, {e, 0}, {r, 47}, {s, 42}, {r, 47}, {r, 47}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 47}},
    {{e, 0}, {e, 0}, {r, 50}, {s, 27}, {r, 50}, {r, 50}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 50}},
    {{e, 0}, {e, 0}, {r, 52}, {s, 33}, {r, 52}, {r, 52}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 52}},
    {{e, 0}, {e, 0}, {r, 54}, {s, 42}, {r, 54}, {r, 54}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 54}},
    {{e, 0}, {e, 0}, {r, 49}, {s, 27}, {r, 49}, {r, 49}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 49}},
    {{e, 0}, {e, 0}, {r, 51}, {s, 33}, {r, 51}, {r, 51}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 51}},
    {{e, 0}, {e, 0}, {r, 53}, {s, 42}, {r, 53}, {r, 53}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 53}},
    {{e, 0}, {e, 0}, {r, 13}, {r, 13}, {r, 13}, {r, 13}, {e, 0}, {s, 92}, {s, 93}, {e, 0}, {e, 0}, {r, 13}, {r, 13}, {r, 13}},
    {{e, 0}, {e, 0}, {r, 29}, {r, 29}, {r, 29}, {r, 29}, {s, 22}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 29}},
    {{e, 0}, {e, 0}, {r, 30}, {r, 30}, {r, 30}, {r, 30}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 30}},
    {{e, 0}, {e, 0}, {r, 5}, {r, 5}, {r, 5}, {r, 5}, {r, 5}, {r, 5}, {r, 5}, {r, 5}, {e, 0}, {r, 5}, {r, 5}, {r, 5}},
    {{e, 0}, {e, 0}, {r, 16}, {r, 16}, {r, 16}, {r, 16}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 16}, {r, 16}, {r, 16}},
    {{e, 0}, {e, 0}, {r, 32}, {r, 32}, {r, 32}, {r, 32}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 32}},
    {{e, 0}, {e, 0}, {r, 7}, {r, 7}, {r, 7}, {r, 7}, {s, 22}, {e, 0}, {e, 0}, {s, 23}, {e, 0}, {r, 7}, {r, 7}, {r, 7}},
    {{e, 0}, {e, 0}, {r, 18}, {r, 18}, {r, 18}, {r, 18}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 18}, {r, 18}, {r, 18}},
    {{e, 0}, {e, 0}, {r, 38}, {r, 38}, {r, 38}, {r, 38}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 38}},
    {{e, 0}, {e, 0}, {s, 64}, {e, 0}, {s, 94}, {s, 95}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 6}, {r, 6}, {r, 6}, {r, 6}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 6}, {r, 6}, {r, 6}},
    {{e, 0}, {e, 0}, {r, 8}, {s, 96}, {r, 8}, {r, 8}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 8}, {r, 8}, {r, 8}},
    {{s, 6}, {s, 32}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 19}, {r, 19}, {r, 19}, {r, 19}, {s, 22}, {e, 0}, {e, 0}, {s, 23}, {e, 0}, {r, 19}, {r, 19}, {r, 19}},
    {{e, 0}, {e, 0}, {r, 20}, {r, 20}, {r, 20}, {r, 20}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 20}, {r, 20}, {r, 20}},
    {{e, 0}, {e, 0}, {r, 39}, {r, 39}, {r, 39}, {r, 39}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 39}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {s, 64}, {e, 0}, {s, 77}, {s, 78}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {s, 65}, {e, 0}, {s, 79}, {s, 80}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 9}, {s, 83}, {r, 9}, {r, 9}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 9}, {r, 9}, {r, 9}},
    {{e, 0}, {e, 0}, {r, 22}, {s, 86}, {r, 22}, {r, 22}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 22}, {r, 22}, {r, 22}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 31}, {r, 31}, {r, 31}, {r, 31}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 31}},
    {{s, 6}, {s, 37}, {e, 0}, {e, 0}, {s, 39}, {e, 0}, {s, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 40}, {r, 40}, {r, 40}, {r, 40}, {s, 22}, {e, 0}, {e, 0}, {s, 23}, {e, 0}, {e, 0}, {e, 0}, {r, 40}},
    {{e, 0}, {e, 0}, {r, 41}, {r, 41}, {r, 41}, {r, 41}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 41}},
    {{e, 0}, {e, 0}, {r, 42}, {r, 42}, {r, 42}, {r, 42}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 42}},
    {{s, 6}, {s, 32}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 32}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 32}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 32}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{s, 6}, {s, 32}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 9}, {s, 96}, {r, 9}, {r, 9}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 9}, {r, 9}, {r, 9}},
    {{e, 0}, {e, 0}, {r, 11}, {s, 83}, {r, 11}, {r, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 11}, {r, 11}, {r, 11}},
    {{e, 0}, {e, 0}, {r, 24}, {s, 86}, {r, 24}, {r, 24}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 24}, {r, 24}, {r, 24}},
    {{e, 0}, {e, 0}, {r, 10}, {s, 83}, {r, 10}, {r, 10}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 10}, {r, 10}, {r, 10}},
    {{e, 0}, {e, 0}, {r, 23}, {s, 86}, {r, 23}, {r, 23}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 23}, {r, 23}, {r, 23}},
    {{e, 0}, {e, 0}, {r, 26}, {s, 83}, {r, 26}, {r, 26}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 26}, {r, 26}, {r, 26}},
    {{e, 0}, {e, 0}, {r, 28}, {s, 86}, {r, 28}, {r, 28}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 28}, {r, 28}, {r, 28}},
    {{e, 0}, {e, 0}, {r, 25}, {s, 83}, {r, 25}, {r, 25}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 25}, {r, 25}, {r, 25}},
    {{e, 0}, {e, 0}, {r, 27}, {s, 86}, {r, 27}, {r, 27}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 27}, {r, 27}, {r, 27}},
    {{e, 0}, {e, 0}, {r, 7}, {r, 7}, {r, 7}, {r, 7}, {s, 22}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 7}, {r, 7}, {r, 7}},
    {{e, 0}, {e, 0}, {r, 19}, {r, 19}, {r, 19}, {r, 19}, {s, 22}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 19}, {r, 19}, {r, 19}},
    {{e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 77}, {s, 78}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 35}, {r, 35}, {e, 0}},
    {{e, 0}, {e, 0}, {e, 0}, {e, 0}, {s, 79}, {s, 80}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 36}, {r, 36}, {e, 0}},
    {{e, 0}, {e, 0}, {r, 14}, {r, 14}, {r, 14}, {r, 14}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 14}, {r, 14}, {r, 14}},
    {{e, 0}, {e, 0}, {r, 15}, {r, 15}, {r, 15}, {r, 15}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 15}, {r, 15}, {r, 15}},
    {{e, 0}, {e, 0}, {r, 11}, {s, 96}, {r, 11}, {r, 11}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 11}, {r, 11}, {r, 11}},
    {{e, 0}, {e, 0}, {r, 10}, {s, 96}, {r, 10}, {r, 10}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 10}, {r, 10}, {r, 10}},
    {{e, 0}, {e, 0}, {r, 7}, {r, 7}, {r, 7}, {r, 7}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {e, 0}, {r, 7}, {r, 7}, {r, 7}}};

#undef s
#undef r
#undef e
#undef a

struct goto_item {
    uint8_t status;
} constexpr GOTO[][12] = {{{0}, {1}, {2}, {3}, {4}, {5}, {8}, {10}, {12}, {13}, {0}, {15}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {24}, {25}, {26}, {5}, {8}, {10}, {12}, {13}, {0}, {15}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {5}, {28}, {10}, {29}, {13}, {0}, {30}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {31}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {34}, {35}, {0}, {36}, {38}, {10}, {40}, {0}, {41}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {5}, {43}, {10}, {44}, {13}, {0}, {45}},
                          {{0}, {0}, {0}, {0}, {0}, {5}, {46}, {10}, {47}, {13}, {0}, {48}},
                          {{0}, {0}, {0}, {0}, {0}, {5}, {49}, {10}, {50}, {13}, {0}, {51}},
                          {{0}, {0}, {0}, {0}, {0}, {5}, {52}, {10}, {53}, {13}, {0}, {54}},
                          {{0}, {0}, {0}, {0}, {0}, {5}, {55}, {10}, {56}, {13}, {0}, {57}},
                          {{0}, {0}, {0}, {0}, {0}, {5}, {58}, {10}, {59}, {13}, {0}, {60}},
                          {{0}, {0}, {0}, {0}, {0}, {61}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {62}, {0}, {63}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {67}, {0}, {68}, {0}, {69}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {70}, {0}, {0}, {71}, {72}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {74}, {0}, {75}, {0}, {76}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {81}, {82}, {0}, {36}, {38}, {10}, {40}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {36}, {84}, {10}, {85}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {89}, {0}, {90}, {0}, {91}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {71}, {97}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {36}, {98}, {10}, {99}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {36}, {100}, {10}, {101}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {36}, {102}, {10}, {103}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {36}, {104}, {10}, {105}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {106}, {0}, {68}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {107}, {0}, {75}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {108}, {109}, {0}, {36}, {38}, {10}, {40}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {110}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {111}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {71}, {112}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {71}, {113}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {114}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}};

struct production_item {
    production_type ptype;
    nterminals ntype;
    uint8_t len;
} constexpr PRODUCTION[] = {
    {production_type::S_acc, nterminals::nterminal_S, 1},
    {production_type::acc_const_expr, nterminals::nterminal_acc, 1},
    {production_type::acc_rand_expr, nterminals::nterminal_acc, 1},
    {production_type::acc_dicelet_expr, nterminals::nterminal_acc, 1},
    {production_type::const_unit_number, nterminals::nterminal_const_unit, 1},
    {production_type::const_unit_lparenthesis_const_expr_rparenthesis, nterminals::nterminal_const_unit, 3},
    {production_type::const_mul_const_unit, nterminals::nterminal_const_mul, 1},
    {production_type::const_mul_const_mul_muldvi_const_unit, nterminals::nterminal_const_mul, 3},
    {production_type::const_expr_const_mul, nterminals::nterminal_const_expr, 1},
    {production_type::const_expr_minus_const_mul, nterminals::nterminal_const_expr, 2},
    {production_type::const_expr_const_expr_plus_const_mul, nterminals::nterminal_const_expr, 3},
    {production_type::const_expr_const_expr_minus_const_mul, nterminals::nterminal_const_expr, 3},
    {production_type::rand_unit_d_const_unit, nterminals::nterminal_rand_unit, 2},
    {production_type::rand_unit_const_unit_d_const_unit, nterminals::nterminal_rand_unit, 3},
    {production_type::rand_unit_const_unit_d_const_unit_k_const_unit, nterminals::nterminal_rand_unit, 5},
    {production_type::rand_unit_const_unit_d_const_unit_kl_const_unit, nterminals::nterminal_rand_unit, 5},
    {production_type::rand_unit_lparenthesis_rand_expr_rparenthesis, nterminals::nterminal_rand_unit, 3},
    {production_type::rand_mul_rand_unit, nterminals::nterminal_rand_mul, 1},
    {production_type::rand_mul_const_mul_muldvi_rand_unit, nterminals::nterminal_rand_mul, 3},
    {production_type::rand_mul_rand_mul_muldvi_const_unit, nterminals::nterminal_rand_mul, 3},
    {production_type::rand_mul_rand_mul_muldvi_rand_unit, nterminals::nterminal_rand_mul, 3},
    {production_type::rand_expr_rand_mul, nterminals::nterminal_rand_expr, 1},
    {production_type::rand_expr_minus_rand_mul, nterminals::nterminal_rand_expr, 2},
    {production_type::rand_expr_const_expr_plus_rand_mul, nterminals::nterminal_rand_expr, 3},
    {production_type::rand_expr_const_expr_minus_rand_mul, nterminals::nterminal_rand_expr, 3},
    {production_type::rand_expr_rand_expr_plus_const_mul, nterminals::nterminal_rand_expr, 3},
    {production_type::rand_expr_rand_expr_minus_const_mul, nterminals::nterminal_rand_expr, 3},
    {production_type::rand_expr_rand_expr_plus_rand_mul, nterminals::nterminal_rand_expr, 3},
    {production_type::rand_expr_rand_expr_minus_rand_mul, nterminals::nterminal_rand_expr, 3},
    {production_type::dicelet_u_const_unit_sharp_const_unit, nterminals::nterminal_dicelet_u, 3},
    {production_type::dicelet_u_const_unit_sharp_rand_unit, nterminals::nterminal_dicelet_u, 3},
    {production_type::dicelet_u_lbrace_dicelet_ct_rbrace, nterminals::nterminal_dicelet_u, 3},
    {production_type::dicelet_u_lparenthesis_dicelet_expr_rparenthesis, nterminals::nterminal_dicelet_u, 3},
    {production_type::dicelet_ct_const_expr, nterminals::nterminal_dicelet_ct, 1},
    {production_type::dicelet_ct_rand_expr, nterminals::nterminal_dicelet_ct, 1},
    {production_type::dicelet_ct_dicelet_ct_comma_const_expr, nterminals::nterminal_dicelet_ct, 3},
    {production_type::dicelet_ct_dicelet_ct_comma_rand_expr, nterminals::nterminal_dicelet_ct, 3},
    {production_type::dicelet_mul_dicelet_u, nterminals::nterminal_dicelet_mul, 1},
    {production_type::dicelet_mul_const_mul_muldvi_dicelet_u, nterminals::nterminal_dicelet_mul, 3},
    {production_type::dicelet_mul_rand_mul_muldvi_dicelet_u, nterminals::nterminal_dicelet_mul, 3},
    {production_type::dicelet_mul_dicelet_mul_muldvi_const_unit, nterminals::nterminal_dicelet_mul, 3},
    {production_type::dicelet_mul_dicelet_mul_muldvi_rand_unit, nterminals::nterminal_dicelet_mul, 3},
    {production_type::dicelet_mul_dicelet_mul_muldvi_dicelet_u, nterminals::nterminal_dicelet_mul, 3},
    {production_type::dicelet_expr_dicelet_mul, nterminals::nterminal_dicelet_expr, 1},
    {production_type::dicelet_expr_minus_dicelet_mul, nterminals::nterminal_dicelet_expr, 2},
    {production_type::dicelet_expr_const_expr_plus_dicelet_mul, nterminals::nterminal_dicelet_expr, 3},
    {production_type::dicelet_expr_const_expr_minus_dicelet_mul, nterminals::nterminal_dicelet_expr, 3},
    {production_type::dicelet_expr_rand_expr_plus_dicelet_mul, nterminals::nterminal_dicelet_expr, 3},
    {production_type::dicelet_expr_rand_expr_minus_dicelet_mul, nterminals::nterminal_dicelet_expr, 3},
    {production_type::dicelet_expr_dicelet_expr_plus_const_mul, nterminals::nterminal_dicelet_expr, 3},
    {production_type::dicelet_expr_dicelet_expr_minus_const_mul, nterminals::nterminal_dicelet_expr, 3},
    {production_type::dicelet_expr_dicelet_expr_plus_rand_mul, nterminals::nterminal_dicelet_expr, 3},
    {production_type::dicelet_expr_dicelet_expr_minus_rand_mul, nterminals::nterminal_dicelet_expr, 3},
    {production_type::dicelet_expr_dicelet_expr_plus_dicelet_mul, nterminals::nterminal_dicelet_expr, 3},
    {production_type::dicelet_expr_dicelet_expr_minus_dicelet_mul, nterminals::nterminal_dicelet_expr, 3}};

terminals translate(token_index what) {
    switch (what) {
    case token_index::index_stop:
        return terminals::terminal_end;
    case token_index::index_number:
        return terminals::terminal_number;
    case token_index::keyword_d:
        return terminals::terminal_roll_d;
    case token_index::keyword_k:
        return terminals::terminal_roll_k;
    case token_index::keyword_kl:
        return terminals::terminal_roll_kl;
    case token_index::punct_add:
        return terminals::terminal_plus;
    case token_index::punct_sub:
        return terminals::terminal_minus;
    case token_index::punct_mul:
    case token_index::punct_dvi:
        return terminals::terminal_mul_dvi;
    case token_index::punct_lbrace:
        return terminals::terminal_lbrace;
    case token_index::punct_rbrace:
        return terminals::terminal_rbrace;
    case token_index::punct_comma:
        return terminals::terminal_comma;
    case token_index::punct_lparenthesis:
        return terminals::terminal_lparenthesis;
    case token_index::punct_rparenthesis:
        return terminals::terminal_rparenthesis;
    case token_index::punct_sharp:
        return terminals::terminal_sharp;
    default:
        return terminals::terminal_end;
    }
}

parser::parser(tokenizer const& tknz) noexcept : tknzer(tknz){};
p_syntax_item parser::parse() {
    std::deque<uint8_t> _status = {0};
    this->vec_symbols = std::make_unique<std::vector<p_syntax_item>>();

    while (tknzer.next_token()->id != token_index::index_stop)
        ;

    tokenizer::token_container_t& deque_input_tokens = tknzer.token_container;

    bool not_finished = true;
    bool err = false;
    while (not_finished) {
        token_t next = deque_input_tokens.front();
        uint8_t status = _status.back();
        terminals next_terminal = translate(next.id);
        action_item const& ai = ACTION[status][next_terminal];
        switch (ai.operation) {
        case action_t::stack: {
            _status.push_back(ai.status);
            this->vec_symbols->emplace_back(stack(next, tknzer.token_string(next)));
            deque_input_tokens.pop_front();
            break;
        }
        case action_t::resolve: {
            production_item pi = PRODUCTION[ai.status];
            p_syntax_item p = resolve(ai.status);
            int lbeta = pi.len;
            while (lbeta--) {
                this->vec_symbols->pop_back();
                _status.pop_back();
            }
            uint8_t status_new = _status.back();
            this->vec_symbols->emplace_back(std::move(p));
            _status.push_back(GOTO[status_new][this->vec_symbols->back()->type].status);
            break;
        }
        case action_t::accept:
            not_finished = false;
            break;
        default: {
            if (err && !this->vec_symbols->empty()) {
                this->vec_symbols->pop_back();
                _status.pop_back();
            } else if (!err) {
                deque_input_tokens.push_front({token_index::index_stop, 0, npos, npos});
                err = true;
            } else
                not_finished = false;
            break;
        }
        }
        if (!not_finished) break;
    }
    if (this->vec_symbols->empty()) return nullptr;
    this->tail = tknzer.tail(this->vec_symbols->front()->source_token);
    return this->vec_symbols->front();
}

p_syntax_item parser::resolve(uint8_t prod_index) const {
    token_t last_pos = this->vec_symbols->back()->source_token;

    auto item = std::make_shared<syntax_nterminal>(last_pos, PRODUCTION[prod_index].ntype, PRODUCTION[prod_index].ptype);

    int prodl = PRODUCTION[prod_index].len;
    auto iter = this->vec_symbols->rbegin();

    while (prodl--) {
        item->items[prodl] = *iter;
        iter++;
    }
    return item;
}

p_syntax_item parser::stack(token_t target, std::string&& src) const {
    auto item = std::make_shared<syntax_terminal>(target, translate(target.id), std::move(src));
    return item;
}