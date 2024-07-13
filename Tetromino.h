// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once
#include "./AbstractTetromino.h"
#include "./TerminalManager.h"
#include <string>
#include <vector>

/*


// class TetrominoI : public AbstractTetromino {
// public:
//   TetrominoI();
//   ~TetrominoI() = default;

//   // See AbstractTetromino.h
//   void moveLeft() override;
//   void moveRight() override;
//   void moveDown() override;
//   void moveUp() override;
//   void rotateLeft() override;
//   void rotateRight() override;

//   std::vector<Point> getCurrentLocation() override { return currentLocation_; }
//   NamedColors getTetrominoColor() const override { return color_; }
//   int getTetrominoSize() const override { return size_; }
//   int getCurrentAngle() const override { return currentAngle_; }

//   void setCurrentLocation(std::vector<Point> location) override;
//   void setCurrentAngle(int angle) override;

// private:
//   // Color of the tetromino, later will be changed to a enum
//   //const int color_ = 0;

//   NamedColors color_ = NamedColors::RED;

//   // For the start position of the tetromino
//   // (temporary)
//   const int startRow_ = 17;
//   const int startCol_ = 45;

//   // Current idea:
//   // Represent tetromino as an
//   // vector of Points
//   std::vector<Point> currentLocation_;
//   // Despite we are using vector, it's convinient to
//   // use size_ variable to avoid magic numbers in code
//   const int size_ = 4;

//   // Current idea:
//   // Represent state of tetramino (for the correct rotation)
//   // as the angle
//   //
//   // Something to think about:
//   // With this particular tetromino it's a bit more compicated
//   // due to the fact that it will return to the starting shape
//   // after 3 rotations whereas all other tetrominons need 4 rotations
//   // (0 --> 90 --> 180 --> 270)
//   //  ^                     |
//   //  |---------------------|
//   //
//   // Current solution:
//   // Start as usual: 0 --> 90 --> 180
//   // but set currentAngle_ = 0 after 180
//   int currentAngle_ = 90;
// };
*/


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