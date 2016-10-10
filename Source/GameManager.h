#pragma once
#include <vector>
#include "Player.h"
#include "../Libs/Json/json.hpp"

class GameManager
{
public:
    GameManager() : firstRound(true), gameOver(true) {};
    ~GameManager() {};

    void GameLoop();
    void AddPlayer(Player* player);
    void Prepare();
    bool IsGameOver() const;
    std::vector<Player*> GetPlayers() const;
    void DeInit();

private:
    std::vector<int> CreateDeck();
    GameState GetState() const;

    void DealCards();
    void PrepareRoles();
    void RandomSeating();
    void PrintRoles() const;
    void NextPlayer();
    void DiscardTable();
    void PlayerFinished(Player* player);
    void AssignID(Player* player);
    void CheckFinished(Player* player, bool forced = false);
    void AssignRole(Player* player);
    void ClearCards();

    bool LastInOrder();
    bool ConfirmHand(Hand move, Player* player);
    bool IsCleanTable() const;
    bool AllFinished() const;
    int PlayersLeft() const;

    nlohmann::json FormatError(Hand move, Hand table, Player* player) const;
    nlohmann::json FormatHand(Hand move) const;

    std::vector<Player*> players;
    std::vector<Player*> finished;
    std::vector<int> discardedCards;
    std::vector<int> roles;

    Hand newestHand;
    bool firstRound;
    bool gameOver;
    int passes;

    static int idCounter;
};
