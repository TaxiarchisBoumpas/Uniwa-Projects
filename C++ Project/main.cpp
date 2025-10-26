#include "Game.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <maze_file>" << endl;
        return 1;
    }
    
    string mapFile = argv[1];
    
    Game game;
    if (!game.initialize(mapFile)) {
        cerr << "Failed to initialize game with map file: " << mapFile << endl;
        return 1;
    }
    
    game.run();
    
    return 0;
}