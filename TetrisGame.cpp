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
  // // Fill logical game field

  // for(int i = offset_row; i < offset_row + rows_; i++)
  // {
  //   for(int j = offset_col; j < offset_col + cols_; i++)
  //   {
  //     gameField[Point{i, j}] = false;
  //   }
  // }

}

void TetrisGame::play() {

  // later: smart pointer ?

  // Main game loop
  while(true)
  {
    // Create current tetromino
    AbstractTetromino *tetrI = new TetrominoI();
    currentTetromino = tetrI;
    drawTetromino();

    // until it's alive (i.e not collided with other block or floor)
    while (currentTetromino != nullptr) {

      UserInput userInput = tm_->getUserInput();
      decideAction(userInput);
      // Small delay, so that tetromino will have enought time to
      // be erased and drawn with new coordinates
      // (without the delay it's flickering)
      usleep(35'000);
    }

    // Avoid memory leaks
    delete currentTetromino;
    delete tetrI;

  }
  
}

Collision TetrisGame::isColliding() {

  for(auto point : currentTetromino->getCurrentLocation())
  {
    if(point.col >= (offset_col + cols_) || point.col <= offset_col)
    {
        return Collision::Wall;
    }
    else if(point.row >= (offset_row + rows_))
    {
      return Collision::Floor;
    }
    else if(point.row <= offset_row)
    {
      return Collision::Roof;
    }
    
  }

  return Collision::Nothing;
}

void TetrisGame::placeTetromino()
{
  for(auto point : currentTetromino->getCurrentLocation())
  {
    gameField[point] = true;
  }
}

void TetrisGame::decideAction(UserInput userInput) {

  // For now it's a simple collision with walls
  // later I'll add something more complicated
  // to check for collision with other tetrominos.
  // Current solution for the collision:
  // if we hit the wall then, we simply return to the previous position
  // (It's probably not the most optimal way to do it)

  // This is not 100% correct, because we need to be able
  // to define custom keycodes for rotation, but for now I'll
  // leave it like this for the testing purposes


  std::vector<Point> previousLocation = currentTetromino->getCurrentLocation();
  int previousAngle = currentTetromino->getCurrentAngle();

  removeTetrominoFromScreen();

  // Save tetromino location before moving
  // in case of collision

  if (userInput.isKeyA()) {
    currentTetromino->rotateLeft();
  }

   else if (userInput.isKeyS()) {
    currentTetromino->rotateRight();
  }

  if (userInput.isKeyLeft()) {
    currentTetromino->moveLeft();
  } 
  
  else if (userInput.isKeyRight()) {
    currentTetromino->moveRight();
  } 

  else if (userInput.isKeyDown()) {
    currentTetromino->moveDown();
  }

  // Return Tetromino to previous location if there is a collision
  
  Collision collision = isColliding();
  if(collision == Collision::Floor)
  {
    currentTetromino->setCurrentLocation(previousLocation);
    currentTetromino->setCurrentAngle(previousAngle);
    drawTetromino();

    // If we have a collision with floor tetromino dies
    currentTetromino = nullptr;
    return;
  }

  else if(collision != Collision::Nothing)
  {
    currentTetromino->setCurrentLocation(previousLocation);
    currentTetromino->setCurrentAngle(previousAngle);
  }
  
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