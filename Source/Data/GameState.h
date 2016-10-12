#pragma once
#include "Hand.h"
#include <vector>

class Player;

struct GameState
{
    Hand table;
    std::vector<int> discard;
    std::vector<Player*> players;
    int passes;
};