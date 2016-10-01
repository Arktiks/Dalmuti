#pragma once
#include "Hand.h"
#include <vector>

class Player;

struct GameState
{
    Hand newestHand;
    std::vector<int> discardedCards;
    std::vector<Player*> players;
    int passes;
};