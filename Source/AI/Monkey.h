#pragma once
#include "../Player.h"

class Monkey : public Player
{
public:
    Monkey();
    Monkey(std::string name);
    ~Monkey();

    void Prepare() override;
    Hand AskMove(GameState state) override;
    int GiveCard() override;
};
