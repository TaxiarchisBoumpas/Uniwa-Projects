#include "Maze.h"
#include <fstream>
#include <iostream>

using namespace std;

Maze::Maze() : width(0), height(0) {}

Maze::~Maze() {}

bool Maze::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return false;
    }
    
    grid.clear();
    string line;
    int row = 0;
    
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        vector<char> gridRow;
        for (int col = 0; col < static_cast<int>(line.length()); col++) {
            char cell = line[col];
            
            if (cell != '*' && cell != ' ' && cell != 'L') {
                cerr << "Error: Invalid character '" << cell << "' (ASCII: " << (int)cell 
                         << ") at position (" << col << ", " << row << ")" << endl;
                return false;
            }
            
            gridRow.push_back(cell);
            
            if (cell == 'L') {
                ladderPos.setPosition(col, row);
            }
        }
        
        if (!gridRow.empty()) {
            grid.push_back(gridRow);
            row++;
        }
    }
    
    file.close();
    
    if (grid.empty()) {
        cerr << "Error: Empty maze file" << endl;
        return false;
    }
    
    height = static_cast<int>(grid.size());
    width = static_cast<int>(grid[0].size());
    
    for (const auto& row : grid) {
        if (static_cast<int>(row.size()) != width) {
            cerr << "Error: Inconsistent row lengths in maze" << endl;
            return false;
        }
    }
    
    return true;
}

bool Maze::isValidPosition(const Position& pos) const {
    return pos.getX() >= 0 && pos.getX() < width && 
           pos.getY() >= 0 && pos.getY() < height;
}

bool Maze::isWall(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return true;
    }
    return grid[pos.getY()][pos.getX()] == '*';
}

bool Maze::isPassable(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return false;
    }
    return grid[pos.getY()][pos.getX()] != '*';
}

char Maze::getCell(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return '*';
    }
    return grid[pos.getY()][pos.getX()];
}

void Maze::setCell(const Position& pos, char value) {
    if (isValidPosition(pos)) {
        grid[pos.getY()][pos.getX()] = value;
    }
}

int Maze::getWidth() const {
    return width;
}

int Maze::getHeight() const {
    return height;
}

Position Maze::getLadderPosition() const {
    return ladderPos;
}

vector<Position> Maze::getValidPositions() const {
    vector<Position> validPositions;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Position pos(x, y);
            if (isPassable(pos)) {
                validPositions.push_back(pos);
            }
        }
    }
    
    return validPositions;
}

vector<Position> Maze::getNeighbors(const Position& pos) const {
    vector<Position> neighbors;
    
    // 8 γειτονικές θέσεις
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < 8; i++) {
        Position neighbor(pos.getX() + dx[i], pos.getY() + dy[i]);
        if (isValidPosition(neighbor)) {
            neighbors.push_back(neighbor);
        }
    }
    
    return neighbors;
}

void Maze::removeWalls() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == '*') {
                grid[y][x] = ' ';
            }
        }
    }
}