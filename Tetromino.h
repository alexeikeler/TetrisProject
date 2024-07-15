// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once
#include "./AbstractTetromino.h"
#include "./TerminalManager.h"
#include <string>
#include <vector>


struct TetrominoShape{
    
    static void createTShape(int startRow, int startCol, std::vector<Point> *container, NamedColors color)
    {
      container->push_back(Point{startRow, startCol, color});
      container->push_back(Point{startRow, startCol + 1, color});
      container->push_back(Point{startRow + 1, startCol + 1, color});
      container->push_back(Point{startRow, startCol + 2, color});
    }

    static void createLShape(int startRow, int startCol, std::vector<Point> *container, NamedColors color)
    {
      container->push_back(Point{startRow, startCol, color});
      container->push_back(Point{startRow, startCol + 1, color});
      container->push_back(Point{startRow, startCol + 2, color});
      container->push_back(Point{startRow + 1, startCol + 2, color});
    }

    static void createJShape(int startRow, int startCol, std::vector<Point> *container, NamedColors color)
    {
      container->push_back(Point{startRow + 1, startCol, color});
      container->push_back(Point{startRow, startCol, color});
      container->push_back(Point{startRow, startCol + 1, color});
      container->push_back(Point{startRow, startCol + 2, color});
    }

    static void createOShape(int startRow, int startCol, std::vector<Point> *container, NamedColors color)
    {
      container->push_back(Point{startRow, startCol, color});
      container->push_back(Point{startRow, startCol + 1, color});
      container->push_back(Point{startRow + 1, startCol, color});
      container->push_back(Point{startRow + 1, startCol + 1, color});
    }

    static void createIShape(int startRow, int startCol, std::vector<Point> *container, NamedColors color)
    {
      container->push_back(Point{startRow, startCol, color});
      container->push_back(Point{startRow, startCol + 1, color});
      container->push_back(Point{startRow, startCol + 2, color});
      container->push_back(Point{startRow, startCol + 3, color});
    }

    static void createZShape(int startRow, int startCol, std::vector<Point> *container, NamedColors color)
    {
      container->push_back(Point{startRow, startCol, color});
      container->push_back(Point{startRow, startCol + 1, color});
      container->push_back(Point{startRow + 1, startCol + 1, color});
      container->push_back(Point{startRow + 1, startCol + 2, color});
    }

    static void createSShape(int startRow, int startCol, std::vector<Point> *container, NamedColors color)
    {
      container->push_back(Point{startRow + 1, startCol, color});
      container->push_back(Point{startRow + 1, startCol + 1, color});
      container->push_back(Point{startRow, startCol + 1, color});
      container->push_back(Point{startRow, startCol + 2, color});
    }
};


class TetrominoT : public NewAbstractTetromino{
  public:
    TetrominoT();
    ~TetrominoT() = default;  
};

class TetrominoL : public NewAbstractTetromino{
  public:
    TetrominoL();
    ~TetrominoL() = default;
};

class TetrominoJ : public NewAbstractTetromino{
  public:
    TetrominoJ();
    ~TetrominoJ() = default;
};

class TetrominoO : public NewAbstractTetromino{
  public:
    TetrominoO();
    ~TetrominoO() = default;
    void rotate(bool left) override;
};

class TetrominoI : public NewAbstractTetromino{
  public:
  TetrominoI();
  ~TetrominoI() = default;
  void rotate(bool left) override;
};

class TetrominoZ : public NewAbstractTetromino{
  public:
  TetrominoZ();
  ~TetrominoZ() = default;
  void rotate(bool left) override;

};

class TetrominoS : public NewAbstractTetromino{
  public:
  TetrominoS();
  ~TetrominoS() = default;
  void rotate(bool left) override;
};