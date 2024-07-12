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

class NewAbstractTetromino{
  public:
    
    virtual ~NewAbstractTetromino() = default;

    void moveLeft();
    void moveRight();
    void moveDown();
    void moveUp();
    void rotate(bool left);
    // rotationLeft
    // rotationRight

    // Getters
    std::vector<Point> getCurrentLocation() {return currentLocation_;}
    NamedColors getTetrominoColor() {return color_;}
    int getTetrominoSize() {return size_;}
    int getCurrentAngle() {return currentAngle_;}

    // Setters
    void setCurrentLocation(std::vector<Point> location) {currentLocation_ = location;}
    void setCurrentAngle(int angle) {currentAngle_ = angle;}



  protected:
    // Each tetromino should define these separetly
    NamedColors color_;
    int startRow_;
    int startCol_;
    int centerIndex;
    //

    std::vector<Point> currentLocation_;
    const int size_ = 4;
    int currentAngle_ = 0;  
    double rotation_angle = 90.0 * (M_PI / 180);
};
