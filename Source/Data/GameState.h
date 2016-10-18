#pragma once
#include "Hand.h"
#include "AI.h"
#include <vector>

class Player;

struct GameState
{
    Hand table;
    std::vector<int> discard;
    std::vector<AI> players;
    int passes;
};