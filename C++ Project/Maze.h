#ifndef MAZE_H
#define MAZE_H

#include "Position.h"
#include <vector>
#include <string>

using namespace std;

class Maze {
private:
    vector<vector<char>> grid;
    int width;
    int height;
    Position ladderPos;
    
public:
    Maze();
    ~Maze();
    
    bool loadFromFile(const string& filename);
    bool isValidPosition(const Position& pos) const;
    bool isWall(const Position& pos) const;
    bool isPassable(const Position& pos) const;
    
    char getCell(const Position& pos) const;
    void setCell(const Position& pos, char value);
    
    int getWidth() const;
    int getHeight() const;
    Position getLadderPosition() const;
    
    vector<Position> getValidPositions() const;
    vector<Position> getNeighbors(const Position& pos) const;
    
    void removeWalls();
};

#endif