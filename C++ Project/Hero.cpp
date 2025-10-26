#include "Hero.h"

using namespace std;

Hero::Hero(char symbol) : symbol(symbol), hasKey(false), isTrapped(false), 
                         keyFound(false), cageFound(false) {}

Hero::~Hero() {}

Position Hero::getPosition() const {
    return currentPos;
}

void Hero::setPosition(const Position& pos) {
    currentPos = pos;
    rememberVisitedPosition(pos);
}

char Hero::getSymbol() const {
    return symbol;
}

bool Hero::getHasKey() const {
    return hasKey;
}

void Hero::setHasKey(bool hasKey) {
    this->hasKey = hasKey;
}

bool Hero::getIsTrapped() const {
    return isTrapped;
}

void Hero::setIsTrapped(bool trapped) {
    this->isTrapped = trapped;
}

void Hero::rememberVisitedPosition(const Position& pos) {
    
    bool found = false;
    for (const Position& visited : visitedPositions) {
        if (visited == pos) {
            found = true;
            break;
        }
    }
    if (!found) {
        visitedPositions.insert(pos);
    }
}

bool Hero::hasVisited(const Position& pos) const {
    for (const Position& visited : visitedPositions) {
        if (visited == pos) {
            return true;
        }
    }
    return false;
}

void Hero::setKeyPosition(const Position& pos) {
    keyPosition = pos;
    keyFound = true;
}

Position Hero::getKeyPosition() const {
    return keyPosition;
}

bool Hero::isKeyFound() const {
    return keyFound;
}

void Hero::setCagePosition(const Position& pos) {
    cagePosition = pos;
    cageFound = true;
}

Position Hero::getCagePosition() const {
    return cagePosition;
}

bool Hero::isCageFound() const {
    return cageFound;
}

vector<Position> Hero::getVisiblePositions(const Maze& maze) const {
    vector<Position> visiblePositions;
    
    
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    
    visiblePositions.push_back(currentPos);
    
    for (int i = 0; i < 8; i++) {
        Position neighbor(currentPos.getX() + dx[i], currentPos.getY() + dy[i]);
        if (maze.isValidPosition(neighbor)) {
            visiblePositions.push_back(neighbor);
        }
    }
    
    return visiblePositions;
}