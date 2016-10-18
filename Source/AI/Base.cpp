#include "Base.h"

void Base::Prepare()
{
}

Hand Base::AskMove(GameState state)
{
    Hand hand;
    hand.value = 0;
    return hand;
}

int Base::GiveCard()
{
    return CARDS::PASS;
}
