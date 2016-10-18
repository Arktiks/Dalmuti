#include "Monkey.h"

Monkey::Monkey() : Player()
{
    SetName("Monkey");
}

Monkey::Monkey(std::string name) : Player()
{
    SetName(name);
}

Monkey::~Monkey()
{
}

void Monkey::Prepare()
{
}

Hand Monkey::AskMove(GameState state)
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
        for(int i = (table.value - 1); i != 0; i--)
        {
            if(CountCard(i) != 0) // Does player have good enough cards.
            {
                if((CountCard(i) + GetJesterAmount()) >= (table.amount + table.jesters))
                {
                    if(CountCard(i) < (table.amount + table.jesters)) // Needs jesters.
                    {
                        int jesters = (table.amount /*+ table.jesters*/) - CountCard(i);

                        myHand.value = i;
                        myHand.amount = CountCard(i);
                        myHand.jesters = jesters;
                        return myHand;
                    }
                    else // No jesters needed.
                    {
                        myHand.value = i;
                        myHand.amount = CountCard(i);
                        myHand.jesters = 0;
                        return myHand;
                    }
                }
            }
        }
    }

    return Hand();
}

int Monkey::GiveCard()
{
    return CARDS::PASS;
}
