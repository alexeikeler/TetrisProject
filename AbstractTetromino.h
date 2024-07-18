// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once

#include "./Point.h"
#include <vector>

class NewAbstractTetromino{
  public:
    
    virtual ~NewAbstractTetromino() = default;

    void moveLeft();
    void moveRight();
    void moveDown();
    void moveUp();
    // We need to slightly modify rotation for
    // O, S, Z and I
    virtual void rotate(bool left);

    // Getters
    std::vector<Point> getCurrentLocation() const {return currentLocation_;}
    NamedColors getTetrominoColor() const {return color_;}
    int getTetrominoSize() const {return size_;};
    int getCurrentAngle() const {return currentAngle_;}

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

    // Vector with points that define current position of the tetromino.
    std::vector<Point> currentLocation_;
    // Size of the tetrominos.
    const int size_ = 4;
    // Start angle.
    int currentAngle_ = 0;  
    // Rotation angle (always 90 degrees). We do need to convert it from
    // radians to degrees.
    const double rotation_angle = 90.0 * (M_PI / 180);
};
