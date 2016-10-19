#pragma once
#include "Hand.h"
#include "AI.h"
#include <vector>

class Player;

struct GameState
{
    Hand table;
    std::vector<int> played;
    std::vector<Hand> stack;
    std::vector<AI> players;
    int passes;
};