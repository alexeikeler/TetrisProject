// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./Tetromino.h"
#include "./TerminalManager.h"
#include <iostream>
#include <string>

TetrominoI::TetrominoI() {
  // Create an "I" shape
  for (int i = 0; i < size_; i++) {
    currentLocation_.push_back(Point{startRow_ + i, startCol_});
  }
};

void TetrominoI::moveLeft() {
  for (auto &point : currentLocation_) {
    point.col -= 1;
  }
}

void TetrominoI::moveRight() {
  for (auto &point : currentLocation_) {
    point.col += 1;
  }
}

void TetrominoI::moveDown() {
  for (auto &point : currentLocation_) {
    point.row += 1;
  }
}

void TetrominoI::moveUp() {
  for (auto &point : currentLocation_) {
    point.row -= 1;
  }
}

// TODO: Maybe some sort of linear transformation as a rotation?
// (i.e. multiply each Point with some sort of rotation matrix?)

void TetrominoI::rotateRight() {
  switch (currentAngle_) {
  // From ---- to I
  case 0:
    currentLocation_[0].row -= 2;
    currentLocation_[0].col += 2;

    // Move second to last to the top - 1
    currentLocation_[1].row -= 1;
    currentLocation_[1].col += 1;

    // Move second to bottom
    currentLocation_[3].row += 1;
    currentLocation_[3].col -= 1;

    currentAngle_ += 90;

    break;

  // From I to cube
  case 90:
    // Rotate tetromino to make it look like cube

    currentLocation_[0].col -= 1;
    currentLocation_[0].row += 1;
    currentLocation_[2].col -= 1;
    currentLocation_[3].row -= 1;

    currentAngle_ += 90;
    break;

  // From cube to ----
  case 180:

    // Maybe simpler ?

    currentLocation_[2].col += 2;
    currentLocation_[2].row -= 1;
    currentLocation_[3].col += 2;
    currentLocation_[3].row -= 1;

    // now we have form x1 x2 x3 x4
    // but because of the previouse rotations
    // it's shifted to the right by 1, so
    // we need to shift current tetromino coordinates to the left (means xi.col
    // -= 1) and move it down by 1 row (means xi.row += 1) to come back to the
    // initial coordinates by xi I mean currentLocation_[i]

    for (int i = 0; i < size_; i++) {
      currentLocation_[i].col -= 1;
      currentLocation_[i].row += 1;
    }

    // now we have desired shape
    currentAngle_ = 0;
  }
}

void TetrominoI::rotateLeft() {
  switch (currentAngle_) {
  // From ---- to cube
  case 0:
    currentLocation_[0].row -= 1;
    currentLocation_[0].col += 1;

    currentLocation_[1].row -= 1;
    currentLocation_[1].col += 1;

    currentLocation_[2].col -= 1;

    currentLocation_[3].col -= 1;

    currentAngle_ = 180;
    break;

  // From I to ----
  case 90:
    currentLocation_[0].row += 2;
    currentLocation_[0].col -= 2;

    currentLocation_[1].row += 1;
    currentLocation_[1].col -= 1;

    currentLocation_[3].row -= 1;
    currentLocation_[3].col += 1;

    currentAngle_ -= 90;
    break;

  // From cube to I
  case 180:
    currentLocation_[0].row -= 1;
    currentLocation_[0].col += 1;

    currentLocation_[2].col += 1;

    currentLocation_[3].row += 1;
    currentAngle_ -= 90;
    break;
  }
}