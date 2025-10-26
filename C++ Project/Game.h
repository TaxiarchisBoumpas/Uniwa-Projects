#ifndef GAME_H
#define GAME_H

#include "Maze.h"
#include "Hero.h"
#include "Grigorakis.h"
#include "Asimenia.h"
#include <ncurses.h>
#include <vector>
#include <random>

using namespace std;

class Game {
private:
    Maze maze;
    Grigorakis* grigorakis;
    Asimenia* asimenia;
    
    Position keyPosition;
    vector<Position> trapPositions;
    vector<Position> cagePositions;
    
    int currentTurn;
    bool gameWon;
    bool gameLost;
    bool wallsRemoved;
    
    mt19937 rng;
    
public:
    Game();
    ~Game();
    
    bool initialize(const string& mapFile);
    void run();
    
private:
    void initializeNCurses();
    void cleanupNCurses();
    
    bool placeHeroes();
    bool placeObjects();
    Position getRandomValidPosition();
    bool isPositionOccupied(const Position& pos);
    
    void gameLoop();
    void updateGame();
    void updateHeroesAwareness();
    void renderGame();
    void renderMaze();
    void renderHeroes();
    void renderObjects();
    void renderUI();
    
    bool checkTraps();
    bool checkKey();
    bool checkCages();
    bool checkWinCondition();
    bool checkLoseCondition();
    
    bool canMoveToPosition(const Position& pos, Hero* hero);
    
    void handleWin();
    void handleLoss();
    void removeWallsAnimation();
    void moveToLadder();
    
    char getDisplayChar(const Position& pos);
    int getColorPair(const Position& pos);
    
    void initializeColors();
    void delay(int milliseconds);
};

// Color definitions
enum Colors {
    COLOR_WALL = 1,
    COLOR_GRIGORAKIS,
    COLOR_ASIMENIA,
    COLOR_KEY,
    COLOR_TRAP,
    COLOR_CAGE,
    COLOR_LADDER,
    COLOR_UI
};

#endif