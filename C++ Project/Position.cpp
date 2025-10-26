#include "Position.h"
#include <cmath>

using namespace std;

Position::Position() : x(0), y(0) {}

Position::Position(int x, int y) : x(x), y(y) {}

int Position::getX() const {
    return x;
}

int Position::getY() const {
    return y;
}

void Position::setX(int x) {
    this->x = x;
}

void Position::setY(int y) {
    this->y = y;
}

void Position::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

bool Position::operator<(const Position& other) const {
    if (y != other.y) {
        return y < other.y;
    }
    return x < other.x;
}

double Position::distanceTo(const Position& other) const {
    int dx = x - other.x;
    int dy = y - other.y;
    return sqrt(dx * dx + dy * dy);
}

int Position::manhattanDistance(const Position& other) const {
    return abs(x - other.x) + abs(y - other.y);
}