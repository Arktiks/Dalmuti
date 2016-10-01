#pragma once
#include "Cards.h"

// Hands always consist cards of the same value.
// e.g. Three Knights (6)
struct Hand
{
    int value;
    int amount;
    int jesters;

    Hand() : value(CARDS::PASS), amount(0), jesters(0) {};
};