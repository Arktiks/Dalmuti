#pragma once
#include <string>

struct AI
{
    std::string name;
    int role;
    int cardsInHand;
    int toYourLeft;
    int toYourRight;
    bool passed;
};