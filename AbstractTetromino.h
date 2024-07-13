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
