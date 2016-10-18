#include "GameManager.h"
#include "Data/Cards.h"
#include "Data/Roles.h"
#include "Data/GameState.h"
#include "Logging/Log.h"

#include <algorithm>
#include <chrono>

using json = nlohmann::json;
int GameManager::idCounter = 0;

///////////////////////
//      PUBLIC      ///
///////////////////////

void GameManager::GameLoop()
{
    if(AllFinished())
    {
        DEBUG_INFO(("Round is over."));
        gameOver = true;
        return;
    }

    if(players.size() == 1)
    {
        CheckFinished(players[0], true);
        return;
    }

    Player* player = players.front(); // Player in-turn.

    // Ask player his move and track spent time.
    auto start = std::chrono::high_resolution_clock::now();
    Hand move = player->AskMove(GetState()); 
    auto finish = std::chrono::high_resolution_clock::now();
    player->turnTimes.push_back(
        std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    
    if(move.value == CARDS::PASS && LastInOrder()) // If player passes and he is last player in order.
    {
        DEBUG_INFO(("(%s)(%i) starts new round.", player->name.c_str(), player->id));
        DiscardTable();
        passes = 0;
        deadlock++;
        move = player->AskMove(GetState());
    }
    else if(move.value == CARDS::PASS)
    {
        DEBUG_INFO(("(%s)(%i) passess.", player->name.c_str(), player->id));
        passes++;
        NextPlayer();
        deadlock++;
        return;
    }

    if(ConfirmHand(move, player))
    {
        DiscardTable();
        table = move;
        player->RemoveCard(move);
        passes = 0;
        deadlock = 0;
        CheckFinished(player);
    }
    else
    {
        //DEBUG_INFO(("(%s)(%i) passess because of illegal move.", player->name.c_str(), player->id));
        player->errors.push_back(FormatError(move, table, player));
        player->illegalMoves++;
        passes++;
        deadlock++;
    }

    if(deadlock > 50)
        AbortRound();

    NextPlayer();
}

void GameManager::AddPlayer(Player* player)
{
    AssignID(player);
    players.push_back(player);
}

void GameManager::Prepare()
{
    if(firstRound)
    {
        RandomSeating();
        for(auto p : players)
        {
            p->role = ROLES::NO_ROLE;
            p->roles.clear();
            p->turnTimes.clear();
            p->errors = json::array();
            p->illegalMoves = 0;
        }
    }
    else
    {
        players = finished;
        finished.clear();
    }

    PrepareRoles();
    ClearCards();
    DealCards();

    gameOver = false;
    passes = 0;
    deadlock = 0;
    table = EmptyHand();
    discard.clear();

    //PrintPlayers();

    if(!firstRound)
    {
        DalmutiPhase();
        RankSeating();
        //PrintPlayers();
    }

    for(auto i : players)
    {
        i->SortCards();
        i->Prepare();
    }

    firstRound = false;
}

bool GameManager::IsGameOver() const
{
    return gameOver;
}

std::vector<Player*> GameManager::GetPlayers() const
{
    std::vector<Player*> p = players;
    for(auto i : finished)
        p.push_back(i);
    return p;
}

void GameManager::DeInit()
{
    if(!players.empty())
    {
        for(auto i : players)
            delete i;
    }

    if(!finished.empty())
    {
        for(auto i : finished)
            delete i;
    }
}

////////////////////////
//      PRIVATE      ///
////////////////////////

std::vector<int> GameManager::CreateDeck()
{
    std::vector<int> deck = { CARDS::JESTER, CARDS::JESTER };

    for(int i = CARDS::PEASANT; i != 0; i--)
    {
        for(int j = 0; j != i; j++)
            deck.push_back(i);
    }

    std::random_shuffle(deck.begin(), deck.end());
    std::random_shuffle(deck.begin(), deck.end(), [](int random) -> int
    {
        return rand() % random;
    });

    return deck;
}

void GameManager::AbortRound()
{
    DEBUG_INFO(("Round has entered deadlock; dividing remaining roles by random."));
    RandomSeating();
    for(auto p : players)
        CheckFinished(p, true);
}

void GameManager::DealCards()
{
    std::vector<int> deck = CreateDeck();

    while(!deck.empty())
    {
        for(int i = 0; i < players.size(); i++)
        {
            players[i]->AddCard(deck.back());
            deck.pop_back();

            if(deck.empty())
                break;
        }
    }
}

void GameManager::PrepareRoles()
{
    roles.clear();
    roles.push_back(ROLES::GREATER_DALMUTI);
    roles.push_back(ROLES::MERCHANT);
    roles.push_back(ROLES::GREATER_PEON);

    if(players.size() >= 5)
    {
        roles.push_back(ROLES::LESSER_DALMUTI);
        roles.push_back(ROLES::LESSER_PEON);
    }

    int merchants = players.size() - roles.size();

    for(int i = 0; i < merchants; i++)
        roles.push_back(ROLES::MERCHANT);

    std::sort(roles.begin(), roles.end());
}

void GameManager::RandomSeating()
{
    std::random_shuffle(players.begin(), players.end());
}

void GameManager::RankSeating()
{
    std::sort(players.begin(), players.end(), [](const Player* a, const Player* b)
    {
        return a->role < b->role;
    });
}

void GameManager::NextPlayer()
{
    if(AllFinished())
        return;

    players.push_back(players.front());
    players.erase(players.begin());
}

void GameManager::DiscardTable()
{
    for(int i = 0; i < table.amount; i++)
        discard.push_back(table.value);

    for(int i = 0; i < table.jesters; i++)
        discard.push_back(CARDS::JESTER);

    table = EmptyHand();
}

void GameManager::PlayerFinished(Player* player)
{
    for(auto it = players.begin(); it < players.end(); it++)
    {
        if((*it)->id == player->id)
        {
            finished.push_back(*it);
            players.erase(it);
            break;
        }
    }
}

void GameManager::AssignID(Player * player)
{
    player->id = idCounter;
    DEBUG_INFO(("Assigned ID(%i) to (%s).", idCounter, player->name.c_str()));
    idCounter++;
}

void GameManager::CheckFinished(Player* player, bool forced)
{
    if(!player->HasCards() || forced)
    {
        AssignRole(player);
        DEBUG_INFO(("(%s)(%i) is out of cards ROLE(%i).", player->name.c_str(), player->id, player->role));
        PlayerFinished(player);
    }
}

void GameManager::AssignRole(Player* player)
{
    player->role = roles.front();
    player->roles.push_back(player->role);
    roles.erase(roles.begin());
}

void GameManager::ClearCards()
{
    for(auto i : players)
        i->handCards.clear();
}

void GameManager::DalmutiPhase()
{
    Player* great_dalmuti = FindPlayer(ROLES::GREATER_DALMUTI);
    Player* great_peon = FindPlayer(ROLES::GREATER_PEON);

    for(int i = 0; i < 2; i++)
    {
        int card = great_peon->GiveLowest();
        great_dalmuti->AddCard(card);
        DEBUG_INFO(("Greater Peon (%s) gives (%i).", great_peon->name.c_str(), card));
    }

    Player* less_dalmuti = FindPlayer(ROLES::LESSER_DALMUTI);
    Player* less_peon = nullptr;
    if(less_dalmuti != nullptr)
    {
        less_peon = FindPlayer(ROLES::LESSER_PEON);
        int card = less_peon->GiveLowest();
        less_dalmuti->AddCard(card);
        DEBUG_INFO(("Lesser Peon (%s) gives (%i).", less_peon->name.c_str(), card));

        card = less_dalmuti->GiveCard();
        if(less_dalmuti->HasCard(card))
        {
            less_peon->AddCard(card);
            less_dalmuti->RemoveCard(card);
        }
        else
        {
            card = less_dalmuti->GiveHighest();
            less_peon->AddCard(card);
        }
        DEBUG_INFO(("Lesser Dalmuti (%s) gives (%i).", less_dalmuti->name.c_str(), card));
    }

    for(int i = 0; i < 2; i++)
    {
        int card = great_dalmuti->GiveCard();
        if(great_dalmuti->HasCard(card))
        {
            great_peon->AddCard(card);
            great_dalmuti->RemoveCard(card);
        }
        else
        {
            card = great_dalmuti->GiveHighest();
            great_peon->AddCard(card);
        }
        DEBUG_INFO(("Greater Dalmuti (%s) gives (%i).", great_dalmuti->name.c_str(), card));
    }
}

Player* GameManager::FindPlayer(int role)
{
    for(auto p : players)
    {
        if(p->role == role)
            return p;
    }
    return nullptr;
}

/////////////////////////////
///    CONST FUNCTIONS    ///
/////////////////////////////

std::vector<AI> GameManager::GetSeatedPlayers() const
{
    std::vector<AI> seatedPlayers;
    for(int i = 1; i < players.size(); i++)
    {
        AI ai;
        ai.name = players[i]->name;
        ai.role = players[i]->role;
        ai.cardsInHand = players[i]->GetHandSize();
        ai.toYourLeft = i;
        ai.toYourRight = players.size() - i;
    }
    return seatedPlayers;
}

Hand GameManager::EmptyHand() const
{
    Hand hand;
    hand.value = CARDS::PASS;
    hand.amount = 0;
    hand.jesters = 0;
    return hand;
}

GameState GameManager::GetState() const
{
    GameState state;
    state.table = table;
    state.discard = discard;
    state.players = GetSeatedPlayers();
    state.passes = passes;
    return state;
}

void GameManager::PrintPlayers() const
{
    for(auto i : players)
    {
        i->PrintRole();
        i->PrintCards();
    }
}

bool GameManager::LastInOrder() const
{
    if(passes >= (PlayersLeft() - 1))
        return true;
    return false;
}

bool GameManager::ConfirmHand(const Hand& move, const Player* player) const
{
    std::string name = player->name;
    int id = player->id;

    if(move.value == CARDS::PASS) // Pass is always valid move.
    {
        return false;
    }
    else if(move.amount > 12 || (move.amount + move.jesters) > 14) // Maximum amount of cards you can play in one hand is 14.
    {
        DEBUG_INFO(("(%s)(%i): trying to play too many cards! (%i)", name.c_str(), id, (move.amount + move.jesters)));
        return false;
    }
    else if(move.value > 13) // Biggest card is 13.
    {
        DEBUG_INFO(("(%s)(%i): card value not between 0-13! (%i)", name.c_str(), id, move.value));
        return false;
    }
    else if(move.jesters > 2) // There are only two jokers in game.
    {
        DEBUG_INFO(("(%s)(%i): trying to play too many jesters! (%i)", name.c_str(), id, move.jesters));
        return false;
    }

    // Passed all preliminary checks at this point.
    // Time to check if player can actually play the hand he submitted.
    std::vector<int> handCards = player->GetCards();

    if(player->HasCard(move))
    {
        if(IsCleanTable()) // Any hand can be played on empty pile.
        {
            DEBUG_INFO(("(%s)(%i) CARD(%i) PLAYED(%i+%i)", name.c_str(), id, move.value, move.amount, move.jesters));
            return true;
        }
        else if(move.value < table.value && // Card value is smaller than current smallest on table.
            (move.amount + move.jesters) >= (table.amount + table.jesters)) // And amount of cards is same or greater.
        {
            DEBUG_INFO(("(%s)(%i) CARD(%i) PLAYED(%i+%i)", name.c_str(), id, move.value, move.amount, move.jesters));
            return true;
        }
        else // Something gone really wrong.
        {
            DEBUG_INFO(("UNEXPECTED ERROR (%s)(%i): CARD(%i) PLAYED(%i+%i)", name.c_str(), id, move.value, move.amount, move.jesters));
            return false;
        }
    }
    else
    {
        DEBUG_INFO(("ERROR (%s)(%i): does not have played cards! CARD(%i) PLAYED(%i+%i) HAS(%i+%i)",
            name.c_str(), id, move.value, move.amount, move.jesters,
            player->CountCard(move.value), player->CountCard(CARDS::JESTER)));
        return false;
    }
}

bool GameManager::IsCleanTable() const
{
    if(table.value == CARDS::PASS)
        return true;
    return false;
}

bool GameManager::AllFinished() const
{
    if(players.empty())
        return true;
    return false;
}

int GameManager::PlayersLeft() const 
{
    return players.size();
}

nlohmann::json GameManager::FormatError(const Hand& move, const Hand& table, const Player* player) const
{
    json error;
    error["name"] = player->GetName();
    error["move"] = FormatHand(move);
    error["latest_Table"] = FormatHand(table);
    error["hand_Cards"] = player->GetCards();
    return error;
}

nlohmann::json GameManager::FormatHand(const Hand& move) const
{
    json root;
    root["value"] = move.value;
    root["amount"] = move.amount;
    root["jesters"] = move.jesters;
    return root;
}
