#ifndef ASIMENIA_H
#define ASIMENIA_H

#include "Hero.h"
#include <random>

using namespace std;

class Asimenia : public Hero {
private:
    mt19937 rng;
    vector<Position> explorationPath;
    int currentPathIndex;
    bool followingWall;
    Position wallFollowStart;
    
public:
    Asimenia();
    virtual ~Asimenia();
    
    virtual Position makeMove(const Maze& maze) override;
    
private:
    Position systematicExploreMove(const Maze& maze);
    Position wallFollowMove(const Maze& maze);
    Position seekKeyMove(const Maze& maze);
    Position seekCageMove(const Maze& maze);
    Position getNextUnvisitedPosition(const Maze& maze);
};

#endif