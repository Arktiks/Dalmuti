#pragma once
#include "Player.h"

class Monkey : public Player
{
public:
    Monkey();
    Monkey(std::string name);
    ~Monkey();

    void Prepare();
    Hand AskMove(GameState state);
};
