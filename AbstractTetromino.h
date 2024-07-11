// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once

#include "./Point.h"
#include <vector>

// Abstract class that represents a Tetromino
class AbstractTetromino {
public:
  virtual ~AbstractTetromino() = default;

  // Move tetromino to the left
  virtual void moveLeft() = 0;
  // Move tetromino to the right
  virtual void moveRight() = 0;
  // Move tetromino down
  virtual void moveDown() = 0;
  // Move tetromino up
  virtual void moveUp() = 0;

  // Rotate left
  virtual void rotateLeft() = 0;
  // Rotate right
  virtual void rotateRight() = 0;
 
  virtual void setCurrentLocation(std::vector<Point> location) = 0;
  virtual void setCurrentAngle(int angle) = 0;

  // Get current coordinates, color, size (number of pixels), angle
  virtual std::vector<Point> getCurrentLocation() = 0;
  virtual NamedColors getTetrominoColor() const = 0;
  virtual int getTetrominoSize() const = 0;
  virtual int getCurrentAngle() const = 0;
};
