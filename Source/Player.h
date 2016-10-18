#pragma once
#include "Data/Hand.h"
#include "Data/GameState.h"
#include "Data/Roles.h"
#include "../Libs/Json/json.hpp"
#include <vector>
#include <string>

class Player
{
    friend class GameManager;
    friend class Parser;

public:
    virtual ~Player() {};

protected:
    Player() {};
    Player(std::string name) { SetName(name); }

    virtual void Prepare();
    virtual Hand AskMove(GameState state) = 0;
    virtual int GiveCard() = 0;

    std::vector<int> GetCards() const; // Return: Copy of current hand cards.
    bool HasCard(int value)     const; // Return: True if you have the given card.
    bool HasCard(Hand hand)     const; // Return: True if you have the given cards.
    int  CountCard(int value)   const; // Return: How many copies of the given card you have.
    int  GetHighestCard()       const; // Return: Highest card you have that is not jester.
    int  GetLowestCard()        const; // Return: Lowest card you have that is not jester.
    int  GetJesterAmount()      const; // Return: How many jesters you have.
    void SetName(std::string name);    // Set name of the player.

private:
    std::string GetName()       const; // Return: Name of the player.
    bool        HasCards()      const; // Return: True if player has cards.
    int         GetHandSize()   const; // Return: How many cards player has in hand.
    int         GetRole()       const; // Return: Players role (Roles.h)

    void SortCards();
    void PrintRole() const;
    void PrintCards() const;
    void AddCard(int value);
    void RemoveCard(int value);
    void RemoveCard(Hand hand);
    int GiveLowest();
    int GiveHighest();
    
    std::vector<int> handCards;
    std::string name;
    int role;
    int id;

    /// STATISTICS ///
    std::vector<int> roles;
    std::vector<double> turnTimes;
    nlohmann::json errors;
    int illegalMoves;
};
