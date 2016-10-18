#include "../GameManager.h"

#include "Base.h"
#include "Monkey.h"

static int rounds = 10;

GameManager SetGame()
{
    GameManager manager;
    manager.AddPlayer(new Monkey("Loiri"));
    manager.AddPlayer(new Monkey("Mielikki"));
    manager.AddPlayer(new Monkey("Koiro"));
    manager.AddPlayer(new Monkey("Hessu Hopo"));
    manager.AddPlayer(new Monkey("Aurinko"));
    return manager;
}
