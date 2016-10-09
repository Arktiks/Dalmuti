#include "AI\Monkey.h"

#include "GameManager.h"
#include "Debug\Memory.h"

#include <CmdParser\cmdparser.hpp>
#include <Json\json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <fstream>

double TurnTime(const std::vector<double>& times);
std::string ConvertDouble(double d);
void configParser(cli::Parser& parser);

int main(int argc, char** argv)
{
    cli::Parser parser(argc, argv);
    configParser(parser);
    parser.run_and_exit_if_error();

    srand(unsigned(std::time(0)));

    GameManager manager;
    manager.AddPlayer(NEW Monkey("Loiri"));
    manager.AddPlayer(NEW Monkey("Mielikki"));
    manager.AddPlayer(NEW Monkey("Koiro"));
    manager.AddPlayer(NEW Monkey("Hessu Hopo"));
    manager.AddPlayer(NEW Monkey("Aurinko"));

    for(int i = 0; i < 20; i++)
    {
        manager.Prepare();
        //std::cin.get();

        while(!manager.IsGameOver())
        {
            manager.GameLoop();
            //std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::vector<Player*> players = manager.GetPlayers();
    
    json root;
    root["players"] = json::array();
    root["rounds"] = json::array();
    root["error_Log"] = json::array();

    for(auto i : players)
    {
        json add;
        add["name"]                  = i->GetName();
        add["greater_Dalmuti"]       = std::count(i->roles.begin(), i->roles.end(), ROLES::GREATER_DALMUTI);
        add["lesser_Dalmuti"]        = std::count(i->roles.begin(), i->roles.end(), ROLES::LESSER_DALMUTI);
        add["merchant"]              = std::count(i->roles.begin(), i->roles.end(), ROLES::MERCHANT);
        add["lesser_Peon"]           = std::count(i->roles.begin(), i->roles.end(), ROLES::LESSER_PEON);
        add["greater_Peon"]          = std::count(i->roles.begin(), i->roles.end(), ROLES::GREATER_PEON);
        add["average_Turntime"]      = ConvertDouble(TurnTime(i->turnTimes));
        add["illegal_Moves"]         = i->illegalMoves;
        root["players"].push_back(add);
        
        for(json::iterator it = i->errors.begin(); it != i->errors.end(); ++it)
        {
            root["error_Log"].push_back(*it);
        }
    }

    for(int r = 0; r < players[0]->roles.size(); r++) // Round amount.
    {
        json add;
        add["r"] = r; // Round
        for(int i = 0; i < players.size(); i++)
            add[std::to_string(i)] = players[i]->roles[r]; // Player_X = Role
        root["rounds"].push_back(add);
    }

    std::vector<int> ykeys;
    std::vector<std::string> labels;
    for(int i = 0; i < players.size(); i++)
    {
        ykeys.push_back(i);
        labels.push_back(players[i]->GetName());
    }
    root["ykeys"] = ykeys;
    root["labels"] = labels;

    std::ofstream file_id;
    file_id.open("Log.json");
    file_id << std::setw(4) << root;
    file_id.close();

    manager.DeInit();

#ifdef _DEBUG
    Memory::Print();
#endif

    return 0;
}

double TurnTime(const std::vector<double>& times)
{
    double sum = 0;
    for(const double &d : times)
        sum += d;

    double ms = (sum / times.size()) * 0.000001;
    //std::cout << sum << " " << times.size() << " " << ms << std::endl;
    return ms;
}

std::string ConvertDouble(double d)
{
    std::stringstream s;
    s << std::fixed << std::setprecision(5) << d;
    return s.str();
}

void configParser(cli::Parser& parser)
{
}