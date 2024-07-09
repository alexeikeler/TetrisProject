// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used
#pragma once

#include <unordered_map>

// Simple class to represent a point on a screen
// with row, col coordinates
class Point {
public:
  int row;
  int col;

  bool operator==(const Point &other) const{
      return row == other.row && col == other.col;
  }
};

// Custom hash function for unordered_map<Point, int>
template <> class std::hash<Point>{
    public:
    int operator()(const Point &p) const{
        auto hash1 = std::hash<int>{}(p.row);
        auto hash2 = std::hash<int>{}(p.col);
        return 2 * hash1 ^ hash2;
    }
};
