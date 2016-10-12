#include "../Logging/Memory.h"
#include "../GameManager.h"

#include "Base.h"
#include "Monkey.h"


GameManager SetGame()
{
    GameManager manager;
    manager.AddPlayer(NEW Monkey("Loiri"));
    manager.AddPlayer(NEW Monkey("Mielikki"));
    manager.AddPlayer(NEW Monkey("Koiro"));
    manager.AddPlayer(NEW Monkey("Hessu Hopo"));
    manager.AddPlayer(NEW Monkey("Aurinko"));
    return manager;
}
