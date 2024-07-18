// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used


#pragma once
#include <unordered_map>

// Plays role of a "named index" for vector of colors.
// Oder is important, because we want to use correct 
// colors from the vector.
enum class NamedColors {
    RED, 
    BLACK, 
    LIGHT_BLUE,
    TETROMINO_I,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_O,
    TETROMINO_S,
    TETROMINO_Z,
    TETROMINO_T,
    WHITE
};

// Simple class to represent a point on a screen
// with row, col coordinates and "==", ">". We need them
// for set and unordered map.
class Point {
public:
    int row;
    int col;
    NamedColors color;
  
    bool operator==(const Point &other) const{
        return row == other.row && col == other.col;
    }

    bool operator<(const Point &other) const{
        return row < other.row || (row == other.row && col < other.col);
    }
    

};

// Custom hash function for unordered map.
template <> class std::hash<Point>{
    public:

    bool operator()(const Point &p) const{
        auto hash1 = std::hash<int>{}(p.row);
        auto hash2 = std::hash<int>{}(p.col);
        return 2 * hash1 ^ hash2;
    }

};
