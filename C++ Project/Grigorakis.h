#ifndef GRIGORAKIS_H
#define GRIGORAKIS_H

#include "Hero.h"
#include <random>

using namespace std;

class Grigorakis : public Hero {
private:
    mt19937 rng;
    Position lastPosition;
    int stuckCounter;
    
public:
    Grigorakis();
    virtual ~Grigorakis();
    
    virtual Position makeMove(const Maze& maze) override;
    
private:
    Position exploreMove(const Maze& maze);
    Position seekKeyMove(const Maze& maze);
    Position seekCageMove(const Maze& maze);
    Position randomValidMove(const Maze& maze);
    bool isStuck();
};

#endif