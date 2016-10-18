#pragma once
#include "../Player.h"

class k00pa : public Player
{
public:
    k00pa();
    k00pa(std::string name) : Player() { SetName(name); }
    ~k00pa();

    // Called at the start of each game before any cards have been played.
    // Your role and hand cards are known at this point.
    void Prepare() override;

    // Ask AI what cards to play.
    // Parameter: GameState gives information about current state of the game.
    // Return: Hand contains a set of cards the AI wishes to play.
    Hand AskMove(GameState state) override;

    // Ask Dalmuti what card he wishes to give to corresponding Peon.
    // Return: Valid card in hand.
    int GiveCard() override;
};
