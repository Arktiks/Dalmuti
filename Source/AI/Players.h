#include "../GameManager.h"

#include "Base.h"
#include "Monkey.h"
#include "k00pa.h"

static int rounds = 10;

GameManager SetGame()
{
    GameManager manager;
    manager.AddPlayer(new k00pa("k00pa"));
    manager.AddPlayer(new Monkey("Mielikki"));
    manager.AddPlayer(new Monkey("Koiro"));
    manager.AddPlayer(new Monkey("Hessu Hopo"));
    manager.AddPlayer(new Monkey("Aurinko"));
    return manager;
}
