// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./TetrisGame.h"
#include "./TerminalManager.h"
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <vector>


TetrisGame::TetrisGame(TerminalManager *tm) {
  tm_ = tm;

  // draw the game field
  drawGameField();
  updateSurface();

  // Fill logical game field
    for(int i = offset_row; i < offset_row + rows_; i++)
    {
      
      for(int j = offset_col; j < offset_col + cols_; j++)
      { 
        gameField[Point{i, j}] = false;
      }
    }
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
      // find new surface, to check for collision
      decideAction(userInput);
      // Small delay, so that tetromino will have enought time to
      // be erased and drawn with new coordinates
      // (without the delay it's flickering)
      usleep(40'000);
    }

    // Avoid memory leaks
    delete currentTetromino;
    delete tetrI;

    // Check for filled line on game screen and remove them
    // according to the rules of the game
    // ...

  }
  
}

void TetrisGame::reshapeGameField()
{
  // Variable to store number of alive points per line
  int acc = 0;

  // vector with lines index
  std::vector<int> rowsToRemove;
  
  // Go through every line
  for(int i = offset_row; i < offset_row + rows_; i++)
  {
    acc = 0;

    for(int j = offset_col + 1; j < offset_col + cols_; j++)
    {
      acc += gameField[Point{i, j}];
    }
    
    // If this holds, then line is full and we need to remove it
    // (- 1 because of the border, we don't want to count it)
    if(acc == cols_ - 1)
    {
      rowsToRemove.push_back(i);
    }

  }

  // Nothing to remove
  if(rowsToRemove.empty())
  {
    return;
  }

  int t = 1;
  // Remove all points from collected rows 
  for(int row : rowsToRemove)
  {
    tm_->drawString(t, 40, 2, ("To remove: " + std::to_string(row)).c_str());
    t+=1;
    // Remove point everywhere
    for(int j = offset_col + 1; j < offset_col + cols_; j++)
    {
      Point pointToRemove = Point{row, j};
      auto it = std::find(surface.begin(), surface.end(), pointToRemove);
      if(it != surface.end())
      {
        surface.erase(it);
      }
      gameField[pointToRemove] = false;
      removePointFromScreen(pointToRemove);


    }
  }

  // int tempR = 0;
  // int tempC = 2;
  // for(auto point : surface)
  // {
  //   tm_->drawString(tempR, tempC, 2, ("(" + std::to_string(point.row) + ", " + std::to_string(point.col) + ") ").c_str());
  //   tempR += 1;

  //   if(tempR % 30 == 0)
  //   {
  //     tempR = 0;
  //     tempC += 5;
  //   }
  // }

  bool flag = false;

  //Now we need to move all drawn the points that are above the lines to be deleted
  for(int row : rowsToRemove)
  {
    for(int i = row; i > offset_row; i--)
    {
      for(int j = offset_col + 1; j < offset_col + cols_; j++)
      {
        
        Point currentPoint = Point{i, j};
        // If point is drawn 
        if(gameField[currentPoint])
        {
          // Set it to false
          gameField[currentPoint] = false;
          // Immitates "falling"
          usleep(70'000);

          // Remove current point from screen
          removePointFromScreen(currentPoint);
          
          // Remove current point from surface set. We need to do this
          // to avoid false collisions. We also need flag variable to know for sure,
          // that we have deleted the point.
          auto it = std::find(surface.begin(), surface.end(), currentPoint);
          if(it != surface.end())
          {
            flag = true;
            surface.erase(it);
          }

          // Move point one row down
          currentPoint.row += 1;

          // Put point with new coordinates back in surface set. This will provide correct
          // collision
          if(flag)
          {
            surface.insert(currentPoint);
          }

          // Put new on the screen and add it to the "logical screen".
          gameField[currentPoint] = true;
          tm_->drawPixel(currentPoint.row, currentPoint.col, 0);
        }

        flag = false;
      }
    }
  }

}

void TetrisGame::updateSurface()
{
  // We are traversing gameField from column to column
  // and searching for the first drawn pixel in each column
  // and then going to the next column. Vector of such pixels
  // will form the "surface" of the gameField, which we will
  // need for the collision 
  
  for(int j = offset_col; j < offset_col + cols_; j++)
  {
    // Temporary variable to store index i
    char index = 0;
    
    for(int i = offset_row; i < offset_row + rows_; i++)
    {
        // If there exists (Point{i, j} = true) then we found our first
        // non zero point in column and we can store it's index and go to the next column
        if(gameField[Point{i, j}])
        {
          index = i;
          break;
        }
    }

    // If index == 0 it means that the column is empty
    // and we will set point from last row as the highest point in this column.
    if(index == 0)
    {
      surface.insert(Point{offset_row + rows_, j});

    }
    // If index != 0 then column i have the highest point at (i, j)
    else
    {
      surface.insert(Point{index, j});
    }
  }
}


Collision TetrisGame::isColliding() {

  for(auto point : currentTetromino->getCurrentLocation())
  {
    if(point.col >= (offset_col + cols_) || point.col <= offset_col)
    {
      tm_->drawString(20, 20, 2, "Collision: W");
      return Collision::Wall;
    }
    else if(std::find(surface.begin(), surface.end(), point) != surface.end())
    {

      tm_->drawString(20, 20, 2, "Collision: S");
      return Collision::Surface;
    }
    else if(point.row <= offset_row)
    {

      tm_->drawString(20, 20, 2, "Collision: R");
      return Collision::Roof;
    }
    else if(gameField[point])
    {

      tm_->drawString(20, 20, 2, "Collision: B");
      return Collision::Block;
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

  // Save tetromino location before moving
  // in case of collision

  std::vector<Point> previousLocation = currentTetromino->getCurrentLocation();
  int previousAngle = currentTetromino->getCurrentAngle();

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

  else if(userInput.isKeyUp()){
    currentTetromino->moveUp();
  }

  // Return Tetromino to previous location if there is a collision
  
  // Not quite accurate behaviour, because we actually need to press
  // one more time and acually colide, then the new tetromino will spawn
  
  // Falling should fix that however

  // Currently checking only floor, we should of course also check
  // other blocks! And there should be a small time gap between collsion,
  // which is propbably also will be achived with the falling speed

  Collision collision = isColliding();
  
  if(collision == Collision::Surface)
  {

    currentTetromino->setCurrentLocation(previousLocation);
    currentTetromino->setCurrentAngle(previousAngle);
    
    // Draw tetromino on the screen
    drawTetromino();

    // Place the tetromino in "logical" screen
    placeTetromino(); 
  
    // Update surface to find prepare for new possible collisions
    updateSurface();

    // Remove full rows
    reshapeGameField();
    // If we have a collision with floor tetromino dies
    currentTetromino = nullptr;
    return;
  }

  else if(collision == Collision::Wall || collision == Collision::Block || collision == Collision::Roof)
  {
    currentTetromino->setCurrentLocation(previousLocation);
    currentTetromino->setCurrentAngle(previousAngle);
  }

  removeTetrominoFromScreen(previousLocation);
  drawTetromino();
}

void TetrisGame::removeTetrominoFromScreen(std::vector<Point> location) {
  for (auto point : location) {
    tm_->drawPixel(point.row, point.col, 1);
  }

  tm_->refresh();
}

void TetrisGame::removePointFromScreen(Point point)
{
  tm_->drawPixel(point.row, point.col, 1);
  tm_->refresh();
}

void TetrisGame::drawTetromino() {
  for (auto point : currentTetromino->getCurrentLocation()) {
    tm_->drawPixel(point.row, point.col, currentTetromino->getTetrominoColor());
  }

  tm_->refresh();
}

void TetrisGame::drawGameField() {
  for (int i = offset_row + 1; i < rows_ + offset_row; i++) {
    // Draw "walls"
    tm_->drawPixel(i, offset_col, 2);
    
    // Numerate rows
    tm_->drawString(i, offset_col - 2, 2, (std::to_string(i).c_str()));
    
    tm_->drawPixel(i, offset_col + cols_, 2);
  }


  for (int i = offset_col; i < cols_ + offset_col + 1; i++) {
    // Draw top and bottom parts of game field
    tm_->drawPixel(offset_row, i, 2);

    if(i % 2 == 0)
    {

      tm_->drawString(offset_row - 2, i, 2, (std::to_string(i).c_str()));

    }
    else
    {
      tm_->drawString(offset_row - 3, i, 2, (std::to_string(i).c_str()));

    }
    tm_->drawPixel(offset_row + rows_, i, 2);
  }

  tm_->refresh();
}