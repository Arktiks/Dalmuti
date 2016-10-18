#include "k00pa.h"
#include <iostream>

k00pa::k00pa() : Player()
{
    SetName("Jorma");
}

k00pa::~k00pa()
{
}

void k00pa::Prepare()
{
}

Hand k00pa::AskMove(GameState state)
{
    Hand table = state.table;
   Hand myHand;

    if(table.value == CARDS::PASS)
    {
        int highest = GetHighestCard();
        int amount = CountCard(highest);

        myHand.value = highest;
        myHand.amount = amount;
        myHand.jesters = 0;
        return myHand;
    }
    else if(table.value == CARDS::DALMUTI)
    {
        return Hand();
    }
    else
    {
       
        //myHand.value = i; ei toimi
        //myHand.amount = CountCard(i);
        myHand.jesters = 0;
        return myHand;

    }

    return Hand();
}

int k00pa::GiveCard()
{
    //std::cout << "hahaa\n";
    return CARDS::PASS;
}