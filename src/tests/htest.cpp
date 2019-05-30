#include <chrono>
#include <iostream>
#include <string>
#include "dicebot/dicebot.h"

int main(int argc, char const *argv[]) {
    std::string in;

    dicebot::event_info ei(123456);
    ei.nickname = "dynilath";
    dicebot::initialize("./build/test_db/");

    bool flag = true;
    while (flag) {
        std::cout << "> ";
        char buf[1024];
        std::cin.getline(buf, 1024);
        in.assign(buf);
        if (in.size() < 1) continue;
        if (in == "quit" | in == "q") break;
        std::string out;
        auto t1 = std::chrono::high_resolution_clock::now();
        dicebot::try_fill_nickname(ei);
        dicebot::message_pipeline(in, ei, out);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << out << std::endl;
        std::cout << "full time : " << (t2 - t1) / std::chrono::nanoseconds(1) / 1000.0 << "us" << std::endl;
    }

    dicebot::salvage();
    return 0;
}
