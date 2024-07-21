// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used
//
// This wikipedia page was used in order to figure out rotation
// formula for the tetrominos.
//
// https://en.wikipedia.org/wiki/Rotation_matrix
//
//

#include "./AbstractTetromino.h"
#include <cmath>

void NewAbstractTetromino::moveLeft() {
  for (Point &point : currentLocation_) {
    point.col -= 1;
  }
}

void NewAbstractTetromino::moveRight() {
  for (Point &point : currentLocation_) {
    point.col += 1;
  }
}

void NewAbstractTetromino::moveDown() {
  for (Point &point : currentLocation_) {
    point.row += 1;
  }
}

void NewAbstractTetromino::moveUp() {
  for (Point &point : currentLocation_) {
    point.row -= 1;
  }
}

void NewAbstractTetromino::rotate(bool left) {
  // We will need center point of the tetromino
  // in order to rotate it properly
  Point center = currentLocation_[centerIndex];
  // If we are rotating to the left side, then the angle should be negative.
  int negate;

  if (left) {
    negate = -1;
  } else {
    negate = 1;
  }

  // Translate points first, then apply formula from wikipedia.
  for (Point &point : currentLocation_) {
    int translatedCol = point.col - center.col;
    int translatedRow = point.row - center.row;

    int rotatedCol =
        (int)(center.col + translatedCol * cos(rotation_angle * negate) -
              translatedRow * sin(rotation_angle * negate));
    int rotatedRow =
        (int)(center.row + translatedCol * sin(rotation_angle * negate) +
              translatedRow * cos(rotation_angle * negate));
    point.row = rotatedRow;
    point.col = rotatedCol;
  }

  // Keep track of the current angle.
  currentAngle_ = (currentAngle_ + (90 * negate)) % 360;
  if (currentAngle_ < 0) {
    currentAngle_ += 360;
  }
}