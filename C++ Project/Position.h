#ifndef POSITION_H
#define POSITION_H


class Position {
private:
    int x;
    int y;

public:
    Position();
    Position(int x, int y);
    
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
    void setPosition(int x, int y);
    
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    bool operator<(const Position& other) const;
    double distanceTo(const Position& other) const;
    int manhattanDistance(const Position& other) const;
};

#endif