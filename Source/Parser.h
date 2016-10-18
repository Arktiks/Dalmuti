#pragma once
#include "../Libs/Json/json.hpp"
#include "Player.h"

class Parser
{
public:
    void WriteLog(const std::vector<Player*>& players) const;

private:
    nlohmann::json FormatErrors(const std::vector<Player*>& players) const;
    nlohmann::json FormatPlayers(const std::vector<Player*>& players) const;
    nlohmann::json FormatLinechart(const std::vector<Player*>& players) const;
    std::string GetDate() const;
    std::string ConvertDouble(double d) const;
    double AverageTurnTime(const std::vector<double>& times) const;
    void WriteFile(const nlohmann::json& log) const;
};
