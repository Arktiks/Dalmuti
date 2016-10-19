#pragma once
#include "Cards.h"
#include <string>

// Hands always consist cards of the same value.
// e.g. Three Knights (6)
struct Hand
{
    int value;
    int amount;
    int jesters;
    std::string player;
};