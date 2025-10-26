#ifndef HERO_H
#define HERO_H

#include "Position.h"
#include "Maze.h"
#include <vector>
#include <set>

using namespace std;

class Hero {
protected:
    Position currentPos;
    char symbol;
    set<Position> visitedPositions;
    bool hasKey;
    bool isTrapped;
    Position keyPosition;
    bool keyFound;
    Position cagePosition;
    bool cageFound;
    
public:
    Hero(char symbol);
    virtual ~Hero();
    
    Position getPosition() const;
    void setPosition(const Position& pos);
    char getSymbol() const;
    
    bool getHasKey() const;
    void setHasKey(bool hasKey);
    
    bool getIsTrapped() const;
    void setIsTrapped(bool trapped);
    
    void rememberVisitedPosition(const Position& pos);
    bool hasVisited(const Position& pos) const;
    
    void setKeyPosition(const Position& pos);
    Position getKeyPosition() const;
    bool isKeyFound() const;
    
    void setCagePosition(const Position& pos);
    Position getCagePosition() const;
    bool isCageFound() const;
    
    vector<Position> getVisiblePositions(const Maze& maze) const;
    virtual Position makeMove(const Maze& maze) = 0;
};

#endif