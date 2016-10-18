#include "Parser.h"
#include <ctime>
#include <fstream>
using json = nlohmann::json;

void Parser::WriteLog(const std::vector<Player*>& players) const
{
    json root;
    root["error_Log"] = FormatErrors(players);
    root["players"]   = FormatPlayers(players);
    root["linechart"] = FormatLinechart(players);
    root["date"]      = GetDate();
    WriteFile(root);
}

json Parser::FormatErrors(const std::vector<Player*>& players) const
{
    json errors;
    for(auto i : players)
    {
        for(json::iterator it = i->errors.begin(); it != i->errors.end(); ++it)
            errors.push_back(*it);
    }
    return errors;
}

json Parser::FormatPlayers(const std::vector<Player*>& players) const
{
    json data;
    for(auto p : players)
    {
        json player;
        player["name"]             = p->GetName();
        player["greater_Dalmuti"]  = std::count(p->roles.begin(), p->roles.end(), ROLES::GREATER_DALMUTI);
        player["lesser_Dalmuti"]   = std::count(p->roles.begin(), p->roles.end(), ROLES::LESSER_DALMUTI);
        player["merchant"]         = std::count(p->roles.begin(), p->roles.end(), ROLES::MERCHANT);
        player["lesser_Peon"]      = std::count(p->roles.begin(), p->roles.end(), ROLES::LESSER_PEON);
        player["greater_Peon"]     = std::count(p->roles.begin(), p->roles.end(), ROLES::GREATER_PEON);
        player["average_Turntime"] = ConvertDouble(AverageTurnTime(p->turnTimes));
        player["illegal_Moves"]    = p->illegalMoves;
        data.push_back(player);
    }
    return data;
}

nlohmann::json Parser::FormatLinechart(const std::vector<Player*>& players) const
{
    json linechart;
    for(int r = 0; r < players[0]->roles.size(); r++)
    {
        json round;
        round["r"] = r;
        for(int i = 0; i < players.size(); i++)
        {
            round[std::to_string(i)] = players[i]->roles[r]; // Player Number = Role
        }
        linechart["rounds"].push_back(round);
    }

    for(int i = 0; i < players.size(); i++)
    {
        linechart["ykeys"].push_back(i);
        linechart["labels"].push_back(players[i]->GetName());
    }
    return linechart;
}

std::string Parser::GetDate() const
{
    time_t now = time(0);
    char* dt = ctime(&now);
    tm* gmtm = gmtime(&now);
    gmtm->tm_hour += 3;
    dt = asctime(gmtm);
    std::string date(dt);
    date.pop_back();
    return date;
}

std::string Parser::ConvertDouble(double d) const
{
    std::stringstream s;
    s << std::fixed << std::setprecision(5) << d;
    return s.str();
}

double Parser::AverageTurnTime(const std::vector<double>& times) const
{
    double sum = 0;
    for(const double &d : times)
        sum += d;
    double ms = (sum / times.size()) * 0.000001;
    return ms;
}

void Parser::WriteFile(const json& log) const
{
    std::ofstream file;
    file.open("Log.json");
    file << std::setw(4) << log;
    file.close();
}