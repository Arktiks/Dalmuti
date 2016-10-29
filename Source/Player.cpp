#include "Player.h"
#include "Logging/Log.h"
#include "Data/Cards.h"
#include "Data/Roles.h"
#include <algorithm>
#include <cassert>

///////////////////////
//      PUBLIC      ///
///////////////////////

std::string Player::GetName() const
{
    return name;
}

bool Player::HasCards() const
{
    if(handCards.empty())
        return false;
    return true;
}

int Player::GetHandSize() const
{
    return handCards.size();
}

int Player::GetRole() const
{
    return role;
}

//////////////////////////
//      PROTECTED      ///
//////////////////////////

void Player::Prepare()
{
}

Hand Player::AskMove(GameState state)
{
    return Hand();
}

int Player::GiveCard()
{
    return 0;
}

std::vector<int> Player::GetCards() const
{
    return handCards;
}

bool Player::HasCard(int value) const
{
    auto it = std::find(handCards.begin(), handCards.end(), value);
    if(it != handCards.end())
        return true;

    return false;
}

bool Player::HasCard(Hand hand) const
{
    if(CountCard(hand.value) < hand.amount)
        return false;

    if(CountCard(CARDS::JESTER) < hand.jesters)
        return false;

    return true;
}

int Player::CountCard(int value) const
{
    if(value > -1 && value < 14)
        return std::count(handCards.begin(), handCards.end(), value);
    else
    {
        DEBUG_INFO(("CountCard() invalid value: %i", value));
        return 0;
    }
}

int Player::GetHighestCard() const
{
    int largest = 0;
    for(auto i : handCards)
    {
        if(i > largest && i != CARDS::JESTER)
            largest = i;
    }
    return largest;
}

int Player::GetLowestCard() const
{
    int lowest = 12;
    for(auto i : handCards)
    {
        if(i < lowest)
            lowest = i;
    }
    return lowest;
}

int Player::GetJesterAmount() const
{
    return std::count(handCards.begin(), handCards.end(), CARDS::JESTER);
}

void Player::SetName(std::string name)
{
    if(!(this->name).empty())
        return;

    if(name.size() > 15)
        name = "ERROR";
    (this->name) = name;
}

///////////////////////
//      PRIVATE     ///
///////////////////////

void Player::SortCards()
{
    std::sort(handCards.begin(), handCards.end(),
        [](const int &a, const int &b)->bool
    {
        return a > b;
    });
}

void Player::PrintRole() const
{
    printf("(%s)(%i): %i \n", name.c_str(), id, role);
}

void Player::PrintCards() const
{
    printf("(%s)(%i): ", name.c_str(), id);
    for(int i = 0; i < handCards.size(); i++)
    {
        printf("%i ", handCards[i]);
    }
    printf("\n");
}

void Player::AddCard(int value)
{
    handCards.push_back(value);
}

void Player::RemoveCard(int value)
{
    std::vector<int>::iterator it;
    for(it = handCards.begin(); it < handCards.end(); it++)
    {
        if((*it) == value)
            break;
    }
    assert(it != handCards.end()); // Trying to remove nonexistent card is a critical error.
    handCards.erase(it);
}

void Player::RemoveCard(Hand hand)
{
    for(int i = 0; i < hand.amount; i++)
        RemoveCard(hand.value);

    for(int i = 0; i < hand.jesters; i++)
        RemoveCard(CARDS::JESTER);
}

int Player::GiveLowest()
{
    int lowest = GetLowestCard();
    RemoveCard(lowest);
    return lowest;
}

int Player::GiveHighest()
{
    int highest = GetHighestCard();
    RemoveCard(highest);
    return highest;
}