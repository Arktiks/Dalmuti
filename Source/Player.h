#pragma once
#include "Data\Hand.h"
#include "Data\GameState.h"
#include "Data\Roles.h"
#include <vector>
#include <string>
#include <Json\json.hpp>

class Player
{
    friend class GameManager;

public:
    std::string  GetName()       const; // Return: Name of the player.
    bool         HasCards()      const; // Return: True if player has cards.
    int          GetHandSize()   const; // Return: How many cards player has in hand.
    int          GetRole()       const; // Return: Players role (Roles.h)

    /// STATISTICS ///
    std::vector<int> roles;
    std::vector<double> turnTimes;
    nlohmann::json errors;
    int illegalMoves;

    Player() {};
    Player(std::string name) { SetName(name); }
    virtual ~Player() {};

protected:
    // Called at the start of each game before any cards have been played.
    // Your role and hand cards are known at this point.
    virtual void Prepare();

    // Ask player what cards he wishes to play.
    // Parameter:  GameState gives access to current state of the game.
    // Return:     Hand contains a set of cards the player wishes to play.
    virtual Hand AskMove(GameState state) = 0;

    std::vector<int> GetCards()  const; // Return: Copy of current hand cards.
    bool   HasCard(int value)    const; // Return: True if you have the given card.
    bool   HasCard(Hand hand)    const; // Return: True if you have the given cards.
    int    CountCard(int value)  const; // Return: How many copies of the given card you have.
    int    GetHighestCard()      const; // Return: Highest card you have that is not jester.
    int    GetLowestCard()       const; // Return: Lowest card you have that is not jester.
    int    GetJesterAmount()     const; // Return: How many jesters you have.
    void   SortCards();                 // Sort hand cards from highest to lowest.
    void   SetName(std::string name);   // Set name of the player.

private:
    void   PrintCards() const;
    void   AddCard(int value);
    void   RemoveCard(int value);
    void   RemoveCard(Hand hand);
    
    std::vector<int> handCards;
    std::string name;
    int role;
    int id;
};
