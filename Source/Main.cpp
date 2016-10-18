#include "AI/Players.h"
#include "Parser.h"

int main(int argc, char** argv)
{
    srand(unsigned(std::time(0)));
    GameManager manager = SetGame();

    for(int i = 0; i < rounds; i++)
    {
        manager.Prepare();
        while(!manager.IsGameOver())
            manager.GameLoop();
    }

    std::vector<Player*> players = manager.GetPlayers();
    Parser parser;
    parser.WriteLog(players);

    manager.DeInit();
    return 0;
}