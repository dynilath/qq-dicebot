#include "stdafx.h"
#include "manualdice.h"
#include <regex>

std::regex regex_filter_manual_dice("^((\\+)?\\d*d\\d+)");

manualdice::manualdice()
{
}


manualdice::~manualdice()
{
}

manualdice::manualdice(const std::string source)
{
}
