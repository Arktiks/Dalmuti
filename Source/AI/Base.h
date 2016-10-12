#pragma once
#include "../Player.h"

class Base : public Player
{
public:
    Base() {};
    Base(std::string name) : Player() { SetName(name); }
    ~Base() {};

    // Called at the start of each game before any cards have been played.
    // Your role and hand cards are known at this point.
    void Prepare();

    // Ask AI what cards to play.
    // Parameter: GameState gives information about current state of the game.
    // Return: Hand contains a set of cards the AI wishes to play.
    Hand AskMove(GameState state);
};
