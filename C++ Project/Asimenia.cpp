#include "Asimenia.h"
#include <algorithm>
#include <chrono>

using namespace std;

Asimenia::Asimenia() : Hero('S'), currentPathIndex(0), followingWall(false) {
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count() + 12345;
    rng.seed(static_cast<unsigned int>(seed));
}

Asimenia::~Asimenia() {}

Position Asimenia::makeMove(const Maze& maze) {
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
        nextMove = systematicExploreMove(maze);
    }
    
    return nextMove;
}

Position Asimenia::systematicExploreMove(const Maze& maze) {
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
        
        Position bestMove = unvisitedMoves[0];
        int minDistance = bestMove.manhattanDistance(currentPos);
        
        for (const Position& pos : unvisitedMoves) {
            int distance = pos.manhattanDistance(currentPos);
            if (distance < minDistance) {
                minDistance = distance;
                bestMove = pos;
            }
        }
        
        return bestMove;
    }
    
    
    Position bestMove = possibleMoves[0];
    
    
    for (const Position& pos : possibleMoves) {
        
        if (!explorationPath.empty() && explorationPath.back() != pos) {
            bestMove = pos;
            break;
        }
    }
    
    
    explorationPath.push_back(currentPos);
    
    
    if (explorationPath.size() > 10) {
        explorationPath.erase(explorationPath.begin());
    }
    
    return bestMove;
}

Position Asimenia::wallFollowMove(const Maze& maze) {
    vector<Position> possibleMoves;
    
    
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};
    
    for (int i = 0; i < 4; i++) {
        Position newPos(currentPos.getX() + dx[i], currentPos.getY() + dy[i]);
        if (maze.isPassable(newPos)) {
            possibleMoves.push_back(newPos);
        }
    }
    
    if (possibleMoves.empty()) {
        return currentPos;
    }
    
    
    return possibleMoves[0];
}

Position Asimenia::seekKeyMove(const Maze& maze) {
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
    
    
    if (!explorationPath.empty() && explorationPath.back() == bestMove && possibleMoves.size() > 1) {
        
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

Position Asimenia::seekCageMove(const Maze& maze) {
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
    
    
    if (!explorationPath.empty() && explorationPath.back() == bestMove && possibleMoves.size() > 1) {

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

Position Asimenia::getNextUnvisitedPosition(const Maze& maze) {
    
    vector<Position> visiblePositions = getVisiblePositions(maze);
    
    for (const Position& pos : visiblePositions) {
        if (maze.isPassable(pos) && !hasVisited(pos)) {
            return pos;
        }
    }
    
    
    return Position(-1, -1);
}