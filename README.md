### How To
1. Place your AI into `<Source/AI>`
2. Edit `<Source/AI/Players.h>` to include your AI.
3. Tamper with the round amount and players as you see fit.
4. If the game finishes without crashing `Log.json` file will be created to `(Solution-Dir)` with game statistics.

### Visual Studio 2015
Drag and drop your AI into Solution Explorer.

![VisualStudio2015](http://i.imgur.com/RnMKJPX.jpg)

Build and make a sanity check on how to debug your AI.

### Linux
`make DEBUG=0` or `make DEBUG=1`

### Disclaimer
Some of the game mechanics are due to change very soon, as there are obvious exploits in the code at the moment.
