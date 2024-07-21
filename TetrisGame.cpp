// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./TetrisGame.h"
#include "./TerminalManager.h"
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <stdlib.h>
#include <thread>
#include <stdio.h>

TetrisGame::TetrisGame(TerminalManager *tm, int level, char rrk, char lrk) {
  
  tm_ = tm;
  currentLevel += level;
  rightRotationKey = rrk;
  leftRotationKey = lrk;


  // draw the game field, current level, score, next tetromino, statistic
  // and destroyed lines texts.
  drawGameField();
  drawLevelText();
  updateLevelAndSpeed();
  updateLevelAndSpeedText();
  drawScoreText();
  drawNextTetrominoText();
  drawStatistics();
  drawDestroyedLinesText();

  // Update surface block to provide correct collision.
  updateSurface();

  // Fill logical game field. Initially all points are false,
  // because the player haven't placed anything yet.
    for(int i = offset_row; i < offset_row + rows_; i++)
    {
      
      for(int j = offset_col; j < offset_col + cols_; j++)
      { 
        gameField[Point{i, j, NamedColors::BLACK}] = false;
      }
    }

  // Create shapes for "Next" tetromino box.
  for(int i = 0; i < numberOfTetrominos; i++)
  {
    std::vector<Point> shape;
    // I'm starting from the third NamedColor value because it corresponds to the first color
    // of the tetromino.
    // Here I'm using map with functions to avoid manual creation of all shapes.
    // I'm also creating shapes this way because I need them to be created more then once
    // (for example for statistics).
    shapeMapper[i](nextTetrominoRowStart + 1, nextTetrominoColStart + 2, &shape, static_cast<NamedColors>(i + 3));
    shapes.push_back(shape);
  }
  
}



int TetrisGame::generateRandomNumber(int a, int b)
{
  // Use current time as seed to get a new number each time
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::default_random_engine generator (seed);
  
  // Create uniforme distribution of numbers from 0 to 6
  std::uniform_int_distribution<int> distribution (a,b);
  
  // Return num in [0, 6] with p(num) = 1/7.
  return distribution(generator);
}


void TetrisGame::generateCurrentAndNext(int a, int b)
{
  currentRandomNumber = generateRandomNumber(a, b);
  nextRandomNumber = generateRandomNumber(a, b);
  
  // To avoid repeating tetrominos we need check if
  // current == next and if current == previous.
  // We will repeat this process until we'll get different numbers.
  while((currentRandomNumber == previousRandomNumber) || (currentRandomNumber == nextRandomNumber))
  {
    // Generate new number if currentRandomNumber == nextRandomNumber.
    currentRandomNumber = generateRandomNumber(a, b);
  }
  
  // Save nextRandomNumber to avoid situation where tetromino from
  // the old cycle repeats last tetromino from the previous cycle.
  previousRandomNumber = nextRandomNumber;

}

// Choose tetromino based on the generated numbers
NewAbstractTetromino* TetrisGame::chooseTetromino(int randomNumber)
{
  if(randomNumber == 0)
  {
    return new TetrominoI();
  }
  else if(randomNumber == 1)
  {
    return new TetrominoJ();
  }
  else if(randomNumber == 2)
  {
    return new TetrominoL();
  }
  else if(randomNumber == 3)
  {
    return new TetrominoO();
  }
  else if(randomNumber == 4)
  {
    return new TetrominoS();
  }
  else if(randomNumber == 5)
  {
    return new TetrominoZ();
  }
  else if(randomNumber == 6)
  {
    return new TetrominoT();
  }

  // Just to supress warning
  return new TetrominoO();
};


void TetrisGame::play() {

  // UserInput testInput;
  // testInput.keycode_ = 258;
  // We will treat currentSpeed as a "time" variable.
  // For example: if we start the game with level 0 we will wait 48/60 <=> 0.8 sec <=> 800 ms.
  // for user input and if we won't get any then we will move tetromino down.
  int timer = currentSpeed;

  // later: smart pointer ?
  // Main game loop
  
  // We will need to save current tetromino id
  // to be able to update statistics later.
  int current;

  // Because we need to know next tetromino immideatly (to print it on the screen)
  // we need to generate two rundom numbers (r1 != r2) and use deque
  // to code correct behaviour.

  // Generate initial random numbers and put them at the back of the deque.
  generateCurrentAndNext();
  deque.push_back(currentRandomNumber);
  deque.push_back(nextRandomNumber);

  // Main game loop.
  while(!deque.empty())
  {
   
    // Get first element of the deque and create current tetromino
    // Save current element for the statistics.

    current = deque.front();
    NewAbstractTetromino *tetr = chooseTetromino(current);

    // Remove first element from the deque
    deque.pop_front();

    // Generate next random numbers and add them to the deque.
    // We need to add them only if we have 1 element left, because
    // otherwise our deque will be growing infinitely.
    if(deque.size() == 1)
    {
      generateCurrentAndNext();
      deque.push_back(currentRandomNumber);
      deque.push_back(nextRandomNumber);
    }
    
    // Here we will get the element that follows the element that is stored in "current", 
    // since deque.pop_front() was used.  
    // Thus the resulting element is the following.
    drawNextTetromino(deque.front());

    // Assign current tetromino.
    currentTetromino = tetr;
    drawTetromino();

    // until it's alive (i.e not collided)
    while (currentTetromino != nullptr) {
        UserInput userInput = tm_->getUserInput();
        
        // Wait for input
        if(userInput.keycode_ == -1)
        {
          timer -= 1;
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        // If we get input from user
        else
        {
          decideAction(userInput, false);
          usleep(20'000);
        }
        // If we didn't get user input and the time is up
        if(timer == 0)
        {
          // Set keycode to Down
          userInput.keycode_ = 258;
          decideAction(userInput, true);
          usleep(20'000);
          timer = currentSpeed;
        }
    }

    // Update stats
    updateStatistics(current);
    updateStatisticsText(current);

    // Avoid memory leaks
    delete currentTetromino;
    delete tetr;
  }

}

void TetrisGame::gameOver()
{
  for(int i = 0; i < tm_->numRows(); i++)
  {
    for(int j = 0; j < tm_->numCols(); j++)
    {
      tm_->drawPixel(i, j, (int)NamedColors::BLACK);
    }
  }

  tm_->drawString(tm_->numRows() / 2, tm_->numCols() / 2, (int)NamedColors::WHITE, "GAME OVER!");
  tm_->refresh();

  std::this_thread::sleep_for(std::chrono::milliseconds(gameOverTimeroutMs));
  // Call destructor to avoid ncurses terminal bug
  tm_->~TerminalManager();

  exit(0);
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
      removePointFromScreen(pointToRemove);
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
          usleep(15'000);

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
          
          currentPoint.color = pointInGameField->first.color;
          
          // Put point with new coordinates back in surface set. This will provide correct
          // collision
          if(flag)
          {
            surface.insert(currentPoint);
          }

          // Put new on the screen and add it to the "logical screen".
          gameField[currentPoint] = true;
          tm_->drawPixel(currentPoint.row, currentPoint.col, (int)currentPoint.color);
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
        if(gameField[Point{i, j, NamedColors::BLACK}])
        {
          index = i;
          break;
        }
    }

    // If index == 0 it means that the column is empty
    // and we will set point from last row as the highest point in this column.
    if(index == 0)
    {
      surface.insert(Point{offset_row + rows_, j, NamedColors::BLACK});

    }
    // If index != 0 then column i have the highest point at (i, j)
    else
    {
      surface.insert(Point{index, j, NamedColors::BLACK});
    }
  }
}

 
Collision TetrisGame::isColliding(
  bool downPressed,  bool leftRotaion, bool rightRotation, std::vector<Point> previousLocation
  ) {

  
  // currentLocation is different from previousLocation because we have
  // performed moving by this point
  std::vector<Point> currentLocation = currentTetromino->getCurrentLocation();

  // If tetromino is being rotated
  // we need to check left and right sides (depends on rotation)
  // to avoid rotation "through" points. In other words:
  // we don't want to perform rotation if something is on the way.
  //-----------------------------------------------------------------------------------------
  
  if(leftRotaion || rightRotation)
  {
    for(int i = 0; i < currentTetromino->getTetrominoSize(); i++)
    {
      Point previousPoint = previousLocation[i];
      Point currentPoint =  currentLocation[i];
      
      int y = 1;
      int x = 1;
      int distRow = std::abs(previousPoint.row - currentPoint.row);
      int distCol = std::abs(currentPoint.col - previousPoint.col);
      
      // Case 1
      if(currentPoint.row <= previousPoint.row && currentPoint.col >= previousPoint.col)
      {

        if(distRow != 0)
        {
          // Move previvousPoint to the same y level
          while(y != distRow)
          {
            // Decreasing row means putting point higher
            previousPoint.row -= 1;
            // If in the process of rotation, we came across a block ---> collision
            if(gameField[previousPoint])
            {
              return Collision::Block;
            }
            y += 1;
          }

        }

        // Here is "<" instead of "!=" because we don't want to
        // bump into currentPoint
        if(distCol != 0)
        {
          while(x < distCol)
          {
            previousPoint.col += 1;
            if(gameField[previousPoint])
            {
              return Collision::Block;
            }
            x += 1;
          }
        }

      }

      // Case 2
      else if(currentPoint.row <= previousPoint.row && currentPoint.col <= previousPoint.col)
      {

        if(distRow != 0)
        {
          // Move previvousPoint to the same y level
          while(y != distRow)
          {
            // Decreasing row means putting point higher
            previousPoint.row -= 1;
            // If in the process of rotation, we came across a block ---> collision
            if(gameField[previousPoint])
            {
              return Collision::Block;
            }
            y += 1;
          }

        }      

        if(distCol != 0)
        {
          while(x < distCol)
          {
            previousPoint.col -= 1;
            if(gameField[previousPoint])
            {
              return Collision::Block;
            }
            x += 1;
          }
        
        }
      }

      // Case 3
      else if(previousPoint.row <= currentPoint.row && previousPoint.col >= currentPoint.col)
      {

        if(distCol != 0)
        {
          while(x != (-distCol))
          {
            previousPoint.col -= 1;
            if(gameField[previousPoint])
            {
              return Collision::Block;
            }
            x -= 1;
          }

        }
        if(distRow != 0)
        {
          while(y < distRow)
          {
            previousPoint.row += 1;
            if(gameField[previousPoint])
            {
              return Collision::Block;
            }

            y += 1;
          }
        }
      }

      else if(previousPoint.row <= currentPoint.row && previousPoint.col <= currentPoint.col)
      {

        if(distCol != 0)
        {
          while(x != distCol)
          {
            previousPoint.col += 1;
            if(gameField[previousPoint])
            {
              return Collision::Block;
            }
            x += 1;
          }
        }

        if(distRow != 0)
        {
          while(y < distRow)
          {
            previousPoint.row -= 1;
            if(gameField[previousPoint])
            {
              return Collision::Block;
            }
            y -= 1;
          }
        }
      }

    }  

  }
  
  //-----------------------------------------------------------------------------------------

  // Check if there is a collision after changing the coordinates
  // of the currentTetromino
  for(auto point : currentTetromino->getCurrentLocation())
  {
    if(point.col >= (offset_col + cols_) || point.col <= offset_col)
    {
      return Collision::Wall;
    }
    else if((std::find(surface.begin(), surface.end(), point) != surface.end()) && downPressed)
    {

      return Collision::Surface;
    }
    else if(point.row <= offset_row)
    {
      return Collision::Roof;
    }
    else if(gameField[point])
    {

      return Collision::Block;
    }
    else if(point.row >= offset_row + rows_)
    {
      return Collision::Floor;
    }
  }

  return Collision::Nothing;
}

void TetrisGame::placeTetromino()
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

void TetrisGame::decideAction(UserInput userInput, bool isArtificialMovement) {

  // Save tetromino location before moving
  // in case of collision
  std::vector<Point> previousLocation = currentTetromino->getCurrentLocation();
  int previousAngle = currentTetromino->getCurrentAngle();

  
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


    // Update number of destroyed lines and leve if needed
    div_t divresult = std::div(destroyedLines, 10);
    if(divresult.quot > previousQuotient)
    {
      updateLevelAndSpeed(1);
      updateLevelAndSpeedText();
      previousQuotient = divresult.quot;
    }

    updateDestroyedLinesText();


    // Update score before next tetromino appears
    updateScore();
    updateScoreText();

    // Reset earned points
    earnedPoints = 0;

    // If we have a collision with floor tetromino dies
    currentTetromino = nullptr;
    return;
  }

  else if(collision == Collision::Wall || collision == Collision::Block)
  {
    currentTetromino->setCurrentLocation(previousLocation);
    currentTetromino->setCurrentAngle(previousAngle);
  }  

  else if(collision == Collision::Roof)
  {
    tm_->drawString(10, 0, 0, "In roof!");
  }


  removeTetrominoFromScreen(previousLocation);
  drawTetromino();
}



void TetrisGame::removeTetrominoFromScreen(std::vector<Point> location) {
  for (auto point : location) {
    tm_->drawPixel(point.row, point.col, (int)NamedColors::BLACK);
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
    tm_->drawPixel(point.row, point.col, (int)currentTetromino->getTetrominoColor());
  }

  tm_->refresh();
}

void TetrisGame::drawGameField() {
  for (int i = offset_row + 1; i < rows_ + offset_row; i++) {
    // Draw "walls"
    tm_->drawPixel(i, offset_col, 2);
    tm_->drawPixel(i, offset_col + cols_, 2);
  }


  for (int i = offset_col; i < cols_ + offset_col + 1; i++) {
    // Draw top and bottom parts of game field
    //tm_->drawPixel(offset_row, i, 2);
    tm_->drawPixel(offset_row + rows_, i, 2);
  }

  tm_->refresh();
}

void TetrisGame::drawNextTetrominoText()
{
  tm_->drawString(
    nextTetrominoRowStart - 1, 
    nextTetrominoColStart, 
    (int)NamedColors::WHITE,
    "Next Tetromino :");
}

void TetrisGame::drawNextTetromino(int tetrominoIndex)
{
  // Clear 4x4 box
  for(int i = nextTetrominoRowStart + 1; i < nextTetrominoRowEnd; i++)
  {
    for(int j = nextTetrominoColStart + 1; j < nextTetrominoColEnd; j++)
    {
      removePointFromScreen(Point{i, j, NamedColors::BLACK});
    }
  } 

  for(Point point : shapes[tetrominoIndex])
  {
    tm_->drawPixel(point.row, point.col, (int)point.color);
  }
}

void TetrisGame::drawStatistics()
{
  // Draw word "Statistics"
  tm_->drawString(
    statisticsRowStart,
    statisticsColStart + 2,
    (int)NamedColors::WHITE,
    "Statistics"
  );
  
  // We will need this offset to draw everything more or less alligned
  int offset = 3;

  // Draw each tetromino and current score.
  // (I'm drawing them from last to first so that it looks 
  // like they have even amout of space between each other)

  for(int i = numberOfTetrominos - 1; i >= 0; i--)
  {
    std::vector<Point> shape;
    shapeMapper[i](statisticsRowStart + offset, statisticsColStart, &shape, static_cast<NamedColors>(i + 3));
    offset += 3;

    for(Point point : shape)
    {
      tm_->drawPixel(point.row, point.col, (int)point.color);
    }

    tm_->drawString(statisticsRowStart + offset-3, statisticsColStart + 6, (int)NamedColors::WHITE, "000");
  }

}

void TetrisGame::updateStatistics(int tetrominoIndex)
{
  // Update score
  statistics[tetrominoIndex] += 1;

  // Convert score to string and add leading zeroes.
  // std::string stringScore = std::to_string(statistics[tetrominoIndex]);
  // int leadingZeroes = 3 - stringScore.length();
  // stringScore.insert(0, leadingZeroes, '0');
  
  // Update statistics of a tetromino with given index.
  // First coordinate is a row, which value corresponds to the index row of tetromino
  // in the statistics table (We are starting from bottom). 
  // Second argument is column of that tetromino shifted by 6.
  // Such shifting allows us to overwrite current score.

  // Should be separated
  // tm_->drawString(
  //   statisticsRowEnd - (tetrominoIndex) * 3, 
  //   statisticsColStart + 6, 
  //   (int)NamedColors::WHITE, 
  //   intToString(statistics[tetrominoIndex], 3).c_str()
  // );
}

void TetrisGame::updateStatisticsText(int tetrominoIndex)
{
   tm_->drawString(
    statisticsRowEnd - (tetrominoIndex) * 3, 
    statisticsColStart + 6, 
    (int)NamedColors::WHITE, 
    intToString(statistics[tetrominoIndex], 3).c_str()
  );
}

void TetrisGame::drawDestroyedLinesText()
{
  tm_->drawString(linesRow, linesCol, (int)NamedColors::WHITE, "LINES - 000");
}

// void TetrisGame::updateDestroyedLines()
// {

//   // Get quotient and remainder. If quotient > previousQuotient,
//   // then we have enough points for the next level.
//   div_t divresult = std::div(destroyedLines, 10);
//   if(divresult.quot > previousQuotient)
//   {
//     updateLevelAndSpeed(1);
//     updateLevelAndSpeedText();
//     previousQuotient = divresult.quot;
//   }
  
//   // Should be separated
//   // tm_->drawString(linesRow, linesCol+4 , (int)NamedColors::WHITE, intToString(destroyedLines, 3).c_str());
// }

void TetrisGame::updateDestroyedLinesText()
{
   tm_->drawString(linesRow, linesCol+4 , (int)NamedColors::WHITE, intToString(destroyedLines, 3).c_str()); 
}

void TetrisGame::drawLevelText()
{
  tm_->drawString(levelRow, levelCol, (int)NamedColors::WHITE, "LEVEL - 000");
}

void TetrisGame::updateLevelAndSpeed(int increaseLevelBy)
{
  currentLevel += increaseLevelBy;

  if(currentLevel <= maxLevel)
  {
    currentSpeed = fallingSpeed[currentLevel];
  }
  else
  {
    currentSpeed = fallingSpeed[maxLevel];
  }

  // Should be separated
  // tm_->drawString(levelRow, levelCol + 4, (int)NamedColors::WHITE, intToString(currentLevel, 3).c_str());
}

void TetrisGame::updateLevelAndSpeedText()
{
  tm_->drawString(levelRow, levelCol + 4, (int)NamedColors::WHITE, intToString(currentLevel, 3).c_str());
}

void TetrisGame::drawScoreText()
{
  tm_->drawString(scoreRow, scoreCol, (int)NamedColors::WHITE, "SCORE - 000000");
}

void TetrisGame::updateScore()
{
  currentPoints += earnedPoints;
  // Should be separated.
  // tm_->drawString(scoreRow, scoreCol + 4, (int)NamedColors::WHITE, intToString(currentPoints, 6).c_str());
}

void TetrisGame::updateScoreText()
{
  tm_->drawString(scoreRow, scoreCol + 4, (int)NamedColors::WHITE, intToString(currentPoints, 6).c_str());  
}

std::string TetrisGame::intToString(int number, int maxLength)
{
  std::string stringNumber = std::to_string(number);
  int leadingZeroes =  maxLength - stringNumber.length();
  stringNumber.insert(0, leadingZeroes, '0');
  return stringNumber;
}