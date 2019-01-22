#pragma once

#include <string>
#include <list>

namespace dicebot::utils{
    extern void split_line(std::string const & source, std::list<std::string> & source_splits);
    extern void remove_space_and_tab(std::string & source);
    extern void quick_sort(int * origin, int * pilot, int first, int last);
    extern bool find_point(std::string const & source, size_t & content_begin);
    extern bool find_space(std::string const & source, size_t & space);
}