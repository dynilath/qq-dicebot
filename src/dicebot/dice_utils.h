#pragma once

#include <string>

namespace dicebot{
	extern void split_line(std::string & source, std::vector<std::string>& source_splits);
	extern void remove_space_and_tab(std::string & source);
	extern void quick_sort(int * origin, int * pilot, int first, int last);
}