#include "Grim.h"

void Grim::Prepare()
{
}

Hand Grim::AskMove(GameState state)
{
    Hand hand;
    hand.value = 0;
    return hand;
}

int Grim::GiveCard()
{
    return CARDS::PASS;
}
