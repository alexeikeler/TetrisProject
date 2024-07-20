// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./MockTetrisGame.h"
#include <iostream>

MockTetrisGame::MockTetrisGame(int level, char rrk, char lrk)
{
    // Mostly the same as the usual TetrisGame class
    // except for the drawing methods.
    currentLevel += level;

    rightRotationKey = rrk;
    leftRotationKey = lrk;

    updateLevelAndSpeed();
    updateSurface();

    for(int i = offset_row; i < offset_row + rows_; i++)
    {
        for(int j = offset_col; j < offset_col + cols_; j++)
        {
            gameField[Point{i, j, NamedColors::BLACK}] = false;
        }
    }
}

void MockTetrisGame::play()
{
    // We will "emulate" playing in tests.
}

void MockTetrisGame::gameOver()
{
    isGameOver = true;
}

void MockTetrisGame::placeTetromino()
{
  for(auto point : currentTetromino->getCurrentLocation())
  {
    // If we are trying to place a tetromino
    // on the roof level it's game over.
    if(point.row == offset_row+1)
    {
      gameOver();
    }

    // We can't just write gameField[point] = true, because we out new point
    // can have new color.
    gameField.erase(point);
    gameField.insert(std::pair(point, true));
  }
}

void MockTetrisGame::decideAction(UserInput userInput, bool isArtificialMovement) {

  // Same as in the TetrisGame, but without drawing.

  // Save tetromino location before moving
  // in case of collision
  std::vector<Point> previousLocation = currentTetromino->getCurrentLocation();
  int previousAngle = currentTetromino->getCurrentAngle();

  // We will need this variable for testing.
  isCurrentTetrominoPlaced = false;    
  
  if (userInput.isLeftRotationKey(leftRotationKey)) {
    currentTetromino->rotate(true);
  }

   else if (userInput.isRightRotationKey(rightRotationKey)) {
    currentTetromino->rotate(false);
  }

  if (userInput.isKeyLeft()) {
    currentTetromino->moveLeft();
  } 
  
  else if (userInput.isKeyRight()) {
    currentTetromino->moveRight();
  } 

  else if (userInput.isKeyDown()) {
    currentTetromino->moveDown();
    // Add additional point for moving down faster.
    // We are counting only movement from player.
    if(!isArtificialMovement)
    {
      earnedPoints += 1;
    }
  }

  // Return Tetromino to previous location if there is a collision
  //
  // To decide correct collision type we need additional information,
  // namely if isKeyDown() true. This helps to avoid situations like this:
  //   4
  //   3
  //   2 *
  //   1 *
  //     *
  // ____*______
  // (1234 is current figure) If we press -> then the 2 will collide with
  // current "surface" (see .h for exact information about surface points) point,
  // which will result in wrong collision.

  Collision collision = isColliding(
    userInput.isKeyDown(), 
    userInput.isLeftRotationKey(leftRotationKey), 
    userInput.isRightRotationKey(rightRotationKey), 
    previousLocation);
  

  // We will need this variable for testing.
  lastCollision = collision;

  if(collision == Collision::Surface)
  {

    currentTetromino->setCurrentLocation(previousLocation);
    currentTetromino->setCurrentAngle(previousAngle);

    // Place the tetromino in "logical" screen
    placeTetromino(); 
    
    // Update surface to find prepare for new possible collisions
    updateSurface();

    // Remove full rows
    reshapeGameField();


    // Update number of destroyed lines and leve if needed
    div_t divresult = std::div(destroyedLines, 10);
    if(divresult.quot > previousQuotient)
    {
      updateLevelAndSpeed(1);
      previousQuotient = divresult.quot;
    }

    // Update score before next tetromino appears
    updateScore();

    // Reset earned points
    earnedPoints = 0;

    isCurrentTetrominoPlaced = true;

    return;
  }

  else if(collision == Collision::Wall || collision == Collision::Block)
  {
    currentTetromino->setCurrentLocation(previousLocation);
    currentTetromino->setCurrentAngle(previousAngle);
  }

}

void MockTetrisGame::reshapeGameField()
{

  // Mostly the same as the usual TetrisGame class
  // except for the drawing methods.
  
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
      acc += gameField[Point{i, j, NamedColors::BLACK}];
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

  destroyedLines += rowsToRemove.size();
  earnedPoints += ((currentLevel + 1) * pointsForRemovedRows[rowsToRemove.size()]);

  // Remove all points from collected rows 
  for(int row : rowsToRemove)
  {

    // Remove point everywhere
    for(int j = offset_col + 1; j < offset_col + cols_; j++)
    {
      Point pointToRemove = Point{row, j, NamedColors::BLACK};

      auto it = std::find(surface.begin(), surface.end(), pointToRemove);
      if(it != surface.end())
      {
        surface.erase(it);
      }

      gameField[pointToRemove] = false;
    }
  }
  bool flag = false;

  //Now we need to move all drawn the points that are above the lines to be deleted

  for(int row : rowsToRemove)
  {
    for(int i = row; i > offset_row; i--)
    {
      for(int j = offset_col + 1; j < offset_col + cols_; j++)
      {
        // Point currentPoint = Point{i, j, NamedColors::BLACK};
        Point currentPoint = Point{i, j, NamedColors::BLACK};
        // If point is drawn 
        if(gameField[currentPoint])
        {
          auto pointInGameField = gameField.find(currentPoint);

          // Set it to false
          gameField[currentPoint] = false;
          // Immitates "falling"

          // Remove current point from screen
          
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
          
          currentPoint.color = pointInGameField->first.color;
          
          // Put point with new coordinates back in surface set. This will provide correct
          // collision
          if(flag)
          {
            surface.insert(currentPoint);
          }

          // Put new on the screen and add it to the "logical screen".
          gameField[currentPoint] = true;
        }

        flag = false;
      }
    }
  }


}
