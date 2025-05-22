//
// Created by Rikka on 2025/5/17.
//

#ifndef POS_H
#define POS_H

class Pos {
public:
    int x;
    int y;

    Pos(const int x, const int y) : x(x), y(y) {}

    Pos() : x(0), y(0) {}

    bool operator==(const Pos& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Pos& other) const { return !(*this == other); }

    Pos operator+(const Pos& other) const {
        return Pos(x + other.x, y + other.y);
    }

    Pos operator-(const Pos& other) const {
        return Pos(x - other.x, y - other.y);
    }
};

#endif // POS_H
