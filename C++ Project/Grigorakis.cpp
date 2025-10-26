#include "Grigorakis.h"
#include <algorithm>
#include <chrono>

using namespace std;

Grigorakis::Grigorakis() : Hero('G'), stuckCounter(0) {
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(static_cast<unsigned int>(seed));
    lastPosition = Position(-1, -1);
}

Grigorakis::~Grigorakis() {}

Position Grigorakis::makeMove(const Maze& maze) {
    if (isTrapped) {
        return currentPos; // Δεν μπορεί να κινηθεί
    }
    
    Position nextMove;
    
    if (hasKey && cageFound) {
        nextMove = seekCageMove(maze);
    }
    else if (keyFound && !hasKey) {
        nextMove = seekKeyMove(maze);
    }
    else {
        nextMove = exploreMove(maze);
    }
    
    if (nextMove == lastPosition) {
        stuckCounter++;
    } else {
        stuckCounter = 0;
    }
    
    if (stuckCounter > 3) {
        nextMove = randomValidMove(maze);
        stuckCounter = 0;
    }
    
    lastPosition = currentPos;
    return nextMove;
}

Position Grigorakis::exploreMove(const Maze& maze) {
    vector<Position> possibleMoves;
    
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};
    
    for (int i = 0; i < 4; i++) {
        Position newPos(currentPos.getX() + dx[i], currentPos.getY() + dy[i]);
        if (maze.isPassable(newPos)) {
            possibleMoves.push_back(newPos);
        }
    }
    
    if (possibleMoves.empty()) {
        return currentPos;
    }
    
    vector<Position> unvisitedMoves;
    for (const Position& pos : possibleMoves) {
        if (!hasVisited(pos)) {
            unvisitedMoves.push_back(pos);
        }
    }
    
    if (!unvisitedMoves.empty()) {
        uniform_int_distribution<int> dist(0, static_cast<int>(unvisitedMoves.size()) - 1);
        return unvisitedMoves[dist(rng)];
    }
    
    uniform_int_distribution<int> dist(0, static_cast<int>(possibleMoves.size()) - 1);
    return possibleMoves[dist(rng)];
}

Position Grigorakis::seekKeyMove(const Maze& maze) {
    vector<Position> possibleMoves;
    
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};
    
    for (int i = 0; i < 4; i++) {
        Position newPos(currentPos.getX() + dx[i], currentPos.getY() + dy[i]);
        if (maze.isPassable(newPos)) {
            possibleMoves.push_back(newPos);
        }
    }
    
    if (possibleMoves.empty()) {
        return currentPos;
    }
    
    Position bestMove = possibleMoves[0];
    int minDistance = bestMove.manhattanDistance(keyPosition);
    
    for (const Position& pos : possibleMoves) {
        int distance = pos.manhattanDistance(keyPosition);
        if (distance < minDistance) {
            minDistance = distance;
            bestMove = pos;
        }
    }
    
    if (bestMove == lastPosition && possibleMoves.size() > 1) {
        Position secondBest = possibleMoves[0];
        if (secondBest == bestMove && possibleMoves.size() > 1) {
            secondBest = possibleMoves[1];
        }
        
        for (const Position& pos : possibleMoves) {
            if (pos != bestMove) {
                int distance = pos.manhattanDistance(keyPosition);
                int currentSecondDistance = secondBest.manhattanDistance(keyPosition);
                if (distance < currentSecondDistance) {
                    secondBest = pos;
                }
            }
        }
        
        int bestDistance = bestMove.manhattanDistance(keyPosition);
        int secondDistance = secondBest.manhattanDistance(keyPosition);
        
        if (secondDistance - bestDistance <= 2) {
            return secondBest;
        }
    }
    
    return bestMove;
}

Position Grigorakis::seekCageMove(const Maze& maze) {
    vector<Position> possibleMoves;
    
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};
    
    for (int i = 0; i < 4; i++) {
        Position newPos(currentPos.getX() + dx[i], currentPos.getY() + dy[i]);
        if (maze.isPassable(newPos)) {
            possibleMoves.push_back(newPos);
        }
    }
    
    if (possibleMoves.empty()) {
        return currentPos;
    }
    
    Position bestMove = possibleMoves[0];
    int minDistance = bestMove.manhattanDistance(cagePosition);
    
    for (const Position& pos : possibleMoves) {
        int distance = pos.manhattanDistance(cagePosition);
        if (distance < minDistance) {
            minDistance = distance;
            bestMove = pos;
        }
    }
    
    
    if (bestMove == lastPosition && possibleMoves.size() > 1) {
        
        Position secondBest = possibleMoves[0];
        if (secondBest == bestMove && possibleMoves.size() > 1) {
            secondBest = possibleMoves[1];
        }
        
        for (const Position& pos : possibleMoves) {
            if (pos != bestMove) {
                int distance = pos.manhattanDistance(cagePosition);
                int currentSecondDistance = secondBest.manhattanDistance(cagePosition);
                if (distance < currentSecondDistance) {
                    secondBest = pos;
                }
            }
        }
        
        
        int bestDistance = bestMove.manhattanDistance(cagePosition);
        int secondDistance = secondBest.manhattanDistance(cagePosition);
        
        if (secondDistance - bestDistance <= 2) { 
            return secondBest;
        }
    }
    
    return bestMove;
}

Position Grigorakis::randomValidMove(const Maze& maze) {
    vector<Position> possibleMoves;
    
    int dx[] = {0, 1, 0, -1, -1, -1, 1, 1};
    int dy[] = {-1, 0, 1, 0, -1, 1, -1, 1};
    
    for (int i = 0; i < 8; i++) {
        Position newPos(currentPos.getX() + dx[i], currentPos.getY() + dy[i]);
        if (maze.isPassable(newPos)) {
            possibleMoves.push_back(newPos);
        }
    }
    
    if (possibleMoves.empty()) {
        return currentPos;
    }
    
    uniform_int_distribution<int> dist(0, static_cast<int>(possibleMoves.size()) - 1);
    return possibleMoves[dist(rng)];
}

bool Grigorakis::isStuck() {
    return stuckCounter > 5;
}