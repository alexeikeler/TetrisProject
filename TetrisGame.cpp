// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./TetrisGame.h"
#include "./TerminalManager.h"
#include <iostream>
#include <unistd.h>

TetrisGame::TetrisGame(TerminalManager *tm) {
  tm_ = tm;

  // draw the game field
  drawGameField();
}

void TetrisGame::play() {
  // Currently it's just a setup to move and rotate TetrominoI

  AbstractTetromino *tetrI = new TetrominoI();
  currentTetromino = tetrI;

  drawTetromino();

  while (true) {
    UserInput userInput = tm_->getUserInput();
    decideAction(userInput);
    // Small delay, so that tetromino will have enought time to
    // be erased and drawn with new coordinates
    // (without the delay it's flickering)
    usleep(35'000);
  }
}

bool TetrisGame::isColliding() {
  tm_->drawString(1, 1, 0, "In isColliding.");
  for (auto point : currentTetromino->getCurrentLocation()) {
    if (point.col >= (offset_col + cols_) || point.col <= offset_col ||
        (point.row >= (offset_row + rows_)) || point.row <= offset_row) {

      tm_->drawString(2, 1, 0, "Collision!");
      return true;
    }
  }
  return false;
}

void TetrisGame::decideAction(UserInput userInput) {

  removeTetrominoFromScreen();

  // For now it's a simple collision with walls
  // later I'll add something more complicated
  // to check for collision with other tetrominos.
  // Current solution for the collision:
  // if we hit the wall then, we simply return to the previous position
  // (It's probably not the most optimal way to do it)

  // This is not 100% correct, because we need to be able
  // to define custom keycodes for rotation, but for now I'll
  // leave it like this for the testing purposes
  if (userInput.isKeyA()) {
    currentTetromino->rotateLeft();
    if (isColliding()) {
      currentTetromino->rotateRight();
    }
  } else if (userInput.isKeyS()) {
    currentTetromino->rotateRight();
    if (isColliding()) {
      currentTetromino->rotateLeft();
    }
  }

  if (userInput.isKeyLeft()) {
    currentTetromino->moveLeft();
    if (isColliding()) {
      currentTetromino->moveRight();
    }

  } else if (userInput.isKeyRight()) {
    currentTetromino->moveRight();
    if (isColliding()) {
      currentTetromino->moveLeft();
    }

  } else if (userInput.isKeyDown()) {
    currentTetromino->moveDown();
    if (isColliding()) {
      currentTetromino->moveUp();
    }
  }

  // For testing
  // else if (userInput.isKeyUp()) {
  //   currentTetromino->moveUp();
  //   if (isColliding()) {
  //     currentTetromino->moveDown();
  //   }
  // }

  drawTetromino();
}

void TetrisGame::removeTetrominoFromScreen() {
  for (auto point : currentTetromino->getCurrentLocation()) {
    tm_->drawPixel(point.row, point.col, 1);
  }

  tm_->refresh();
}

void TetrisGame::drawTetromino() {
  for (auto point : currentTetromino->getCurrentLocation()) {
    tm_->drawPixel(point.row, point.col, currentTetromino->getTetrominoColor());
  }

  tm_->refresh();
}

void TetrisGame::drawGameField() {
  for (int i = offset_row; i < rows_ + offset_row; i++) {
    // Draw "walls"
    tm_->drawPixel(i, offset_col, 2);
    tm_->drawPixel(i, offset_col + cols_, 2);
  }

  for (int i = offset_col; i < cols_ + offset_col + 1; i++) {
    // Draw top and bottom parts of game field
    tm_->drawPixel(offset_row, i, 2);
    tm_->drawPixel(offset_row + rows_, i, 2);
  }

  tm_->refresh();
}