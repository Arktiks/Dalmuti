#pragma once
#include <vector>
#include "Player.h"
#include "Data/AI.h"
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
    void AbortRound();

    void DealCards();
    void PrepareRoles();
    void RandomSeating();
    void RankSeating();
    void NextPlayer();
    void DiscardTable();
    void PlayerFinished(Player* player);
    void AssignID(Player* player);
    void CheckFinished(Player* player, bool forced = false);
    void AssignRole(Player* player);
    void ClearCards();
    void DalmutiPhase();
    void SetPlayerPasses();
    void SetTable(Hand move);
    Player* FindPlayer(int role);

    std::vector<AI> GetSeatedPlayers() const;
    Hand EmptyHand() const;
    GameState GetState() const;
    void PrintPlayers() const;
    bool LastInOrder() const;
    bool ConfirmHand(const Hand& move, const Player* player) const;
    bool IsCleanTable() const;
    bool AllFinished() const;
    int PlayersLeft() const;

    nlohmann::json FormatError(const Hand& move, const Hand& table, const Player* player) const;
    nlohmann::json FormatHand(const Hand& move) const;

    std::vector<Player*> players;
    std::vector<Player*> finished;
    std::vector<int> played;
    std::vector<Hand> stack;
    std::vector<int> roles;

    Hand table;
    bool firstRound;
    bool gameOver;
    int passes;
    int deadlock;

    static int idCounter;
};
