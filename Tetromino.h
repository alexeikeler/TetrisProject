// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once
#include "./AbstractTetromino.h"
#include "./TerminalManager.h"
#include <string>
#include <vector>

// Structure to create tetromino shapes and place them inside the vector with
// Points.
struct TetrominoShape {

  // Create T shape tetromino.
  static void createTShape(int startRow, int startCol,
                           std::vector<Point> *container, NamedColors color) {
    container->push_back(Point{startRow, startCol, color});
    container->push_back(Point{startRow, startCol + 1, color});
    container->push_back(Point{startRow + 1, startCol + 1, color});
    container->push_back(Point{startRow, startCol + 2, color});
  }
  // Create L shape tetromino.
  static void createLShape(int startRow, int startCol,
                           std::vector<Point> *container, NamedColors color) {
    container->push_back(Point{startRow, startCol, color});
    container->push_back(Point{startRow, startCol + 1, color});
    container->push_back(Point{startRow, startCol + 2, color});
    container->push_back(Point{startRow + 1, startCol + 2, color});
  }

  // Create J shape tetromino.
  static void createJShape(int startRow, int startCol,
                           std::vector<Point> *container, NamedColors color) {
    container->push_back(Point{startRow + 1, startCol, color});
    container->push_back(Point{startRow, startCol, color});
    container->push_back(Point{startRow, startCol + 1, color});
    container->push_back(Point{startRow, startCol + 2, color});
  }

  // Create O shape tetromino.
  static void createOShape(int startRow, int startCol,
                           std::vector<Point> *container, NamedColors color) {
    container->push_back(Point{startRow, startCol, color});
    container->push_back(Point{startRow, startCol + 1, color});
    container->push_back(Point{startRow + 1, startCol, color});
    container->push_back(Point{startRow + 1, startCol + 1, color});
  }

  // Create I shape tetromino.
  static void createIShape(int startRow, int startCol,
                           std::vector<Point> *container, NamedColors color) {
    container->push_back(Point{startRow, startCol, color});
    container->push_back(Point{startRow, startCol + 1, color});
    container->push_back(Point{startRow, startCol + 2, color});
    container->push_back(Point{startRow, startCol + 3, color});
  }

  // Create Z shape tetromino.
  static void createZShape(int startRow, int startCol,
                           std::vector<Point> *container, NamedColors color) {
    container->push_back(Point{startRow, startCol, color});
    container->push_back(Point{startRow, startCol + 1, color});
    container->push_back(Point{startRow + 1, startCol + 1, color});
    container->push_back(Point{startRow + 1, startCol + 2, color});
  }

  // Create S shape tetromino.
  static void createSShape(int startRow, int startCol,
                           std::vector<Point> *container, NamedColors color) {
    container->push_back(Point{startRow + 1, startCol, color});
    container->push_back(Point{startRow + 1, startCol + 1, color});
    container->push_back(Point{startRow, startCol + 1, color});
    container->push_back(Point{startRow, startCol + 2, color});
  }
};

// Tetromino classes. I've decided to use inheritence here
// to avoid code duplication, because for the most part it's the same stuff.
class TetrominoT : public NewAbstractTetromino {
public:
  TetrominoT();
  ~TetrominoT() = default;
};

class TetrominoL : public NewAbstractTetromino {
public:
  TetrominoL();
  ~TetrominoL() = default;
};

class TetrominoJ : public NewAbstractTetromino {
public:
  TetrominoJ();
  ~TetrominoJ() = default;
};

class TetrominoO : public NewAbstractTetromino {
public:
  TetrominoO();
  ~TetrominoO() = default;
  void rotate(bool left) override;
};

class TetrominoI : public NewAbstractTetromino {
public:
  TetrominoI();
  ~TetrominoI() = default;
  void rotate(bool left) override;
};

class TetrominoZ : public NewAbstractTetromino {
public:
  TetrominoZ();
  ~TetrominoZ() = default;
  void rotate(bool left) override;
};

class TetrominoS : public NewAbstractTetromino {
public:
  TetrominoS();
  ~TetrominoS() = default;
  void rotate(bool left) override;
};