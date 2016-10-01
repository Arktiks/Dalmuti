#include "GameManager.h"
#include "Data\Cards.h"
#include "Data\Roles.h"
#include "Data\GameState.h"
#include "Debug\Log.h"

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
        DEBUG_INFO(("Game is over."));
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
        move = player->AskMove(GetState());
    }
    else if(move.value == CARDS::PASS)
    {
        DEBUG_INFO(("(%s)(%i) passess.", player->name.c_str(), player->id));
        passes++;
        NextPlayer();
        return;
    }

    if(ConfirmHand(move, player))
    {
        DiscardTable();
        newestHand = move;
        player->RemoveCard(move);
        passes = 0;
        CheckFinished(player);
    }
    else
    {
        //DEBUG_INFO(("(%s)(%i) passess because of illegal move.", player->name.c_str(), player->id));
        player->errors.push_back(FormatError(move, newestHand, player));
        player->illegalMoves++;
        passes++;
    }

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
        firstRound = false;
        for(auto i : players)
            i->illegalMoves = 0;
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
    newestHand = Hand();
    discardedCards.clear();

    for(auto i : players)
        i->Prepare();
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

    for(size_t i = CARDS::PEASANT; i != 0; i--)
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

GameState GameManager::GetState() const
{
    GameState state;
    state.newestHand = newestHand;
    state.discardedCards = discardedCards;
    state.players = players;
    state.passes = passes;
    return state;
}

void GameManager::DealCards()
{
    std::vector<int> deck = CreateDeck();

    while(!deck.empty())
    {
        for(size_t i = 0; i < players.size(); i++)
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

    for(size_t i = 0; i < merchants; i++)
        roles.push_back(ROLES::MERCHANT);

    std::sort(roles.begin(), roles.end());
}

void GameManager::RandomSeating()
{
    std::random_shuffle(players.begin(), players.end());
}

void GameManager::PrintRoles() const
{
    //DEBUG_INFO(("Roles:"));
    for(auto i : players)
        printf("(%s)(%i): %i \n", i->name.c_str(), i->id, i->role);
    printf("\n");
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
    for(size_t i = 0; i < newestHand.amount; i++)
        discardedCards.push_back(newestHand.value);

    for(size_t i = 0; i < newestHand.jesters; i++)
        discardedCards.push_back(CARDS::JESTER);

    newestHand = Hand();
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

bool GameManager::LastInOrder()
{
    if(passes >= (PlayersLeft() - 1))
        return true;
    return false;
}

bool GameManager::ConfirmHand(Hand move, Player* player)
{
    std::string name = player->name;
    int id = player->id;

    if(move.value == CARDS::PASS) // Pass is always valid move.
    {
        //DEBUG_INFO(("(%s)(%i) passess.", name.c_str(), id));
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
        else if(move.value < newestHand.value && // Card value is smaller than current smallest on table.
            (move.amount + move.jesters) >= (newestHand.amount + newestHand.jesters)) // And amount of cards is same or greater.
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
    if(newestHand.value == CARDS::PASS)
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

nlohmann::json GameManager::FormatError(Hand move, Hand table, Player* player) const
{
    json error;
    error["name"] = player->GetName();
    error["move"] = FormatHand(move);
    error["latest_Table"] = FormatHand(table);
    error["hand_Cards"] = player->GetCards();
    return error;
}

nlohmann::json GameManager::FormatHand(Hand move) const
{
    json root;
    root["value"] = move.value;
    root["amount"] = move.amount;
    root["jesters"] = move.jesters;
    return root;
}
