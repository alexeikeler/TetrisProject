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


TetrisGame::TetrisGame(TerminalManager *tm) {
  tm_ = tm;

  // draw the game field, statistics and next
  drawGameField();
  drawLevelText();
  drawScoreText();
  drawNextTetrominoText();
  drawStatistics();
  drawDestroyedLinesText();
  updateSurface();

  // Fill logical game field
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
  
  // To avoid repeating tetrominos we check if
  // current == next
  // and if
  // current == previous
  while((currentRandomNumber == previousRandomNumber) || (currentRandomNumber == nextRandomNumber))
  {
    // Generate new number if currentRandomNumber == nextRandomNumber.
    currentRandomNumber = generateRandomNumber(a, b);
  }
    
  previousRandomNumber = nextRandomNumber;

}


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

  // later: smart pointer ?
  // Main game loop
  int temp;
    
  while(true)
  {
    // Because we need to know next tetromino immideatly (to print it on the screen)
    // we need to generate two rundom numbers (r1 != r2) and use deque
    // to code correct behaviour.

    // Generate initial random numbers and put them at the back of the deque.
    generateCurrentAndNext();
    deque.push_back(currentRandomNumber);
    deque.push_back(nextRandomNumber);
    
    // Until we havent used our two random Tetrominos:
    while(!deque.empty())
    {
      // Get first element of the deque and create current tetromino
      temp = deque.front();
      NewAbstractTetromino *tetr = chooseTetromino(deque.front());
      
      // Remove first element from the deque
      deque.pop_front();

      // Generate next random numbers and add them to the deque
      generateCurrentAndNext();
      deque.push_back(currentRandomNumber);
      deque.push_back(nextRandomNumber);
      
      // Here we get the element that follows the elements from the line 178, 
      // since we used deque.pop_front() (line 181). 
      // Thus the resulting element is the following.
      drawNextTetromino(deque.front());


      currentTetromino = tetr;
      drawTetromino();

      // until it's alive (i.e not collided)
      while (currentTetromino != nullptr) {

        UserInput userInput = tm_->getUserInput();
        // find new surface, to check for collision
        decideAction(userInput);
        // Small delay, so that tetromino will have enought time to
        // be erased and drawn with new coordinates
        // (without the delay it's flickering)
        usleep(20'000);
      }

      updateStatistics(temp);

      // Avoid memory leaks

      delete currentTetromino;
      delete tetr;
      // Remove used number
    }

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
  // (see erfahrungen.pdf (collision for rotation) for more details)
  //-----------------------------------------------------------------------------------------
  // Tetromino I behaves strange 
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
      //tm_->drawString(20, 20, 2, "Collision: W");
      return Collision::Wall;
    }
    else if((std::find(surface.begin(), surface.end(), point) != surface.end()) && downPressed)
    {

      //tm_->drawString(20, 20, 2, "Collision: S");
      return Collision::Surface;
    }
    else if(point.row <= offset_row)
    {

      //tm_->drawString(20, 20, 2, "Collision: R");
      return Collision::Roof;
    }
    else if(gameField[point])
    {

      //tm_->drawString(20, 20, 2, "Collision: B");
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
    // We can't just write gameField[point] = true, because we out new point
    // can have new color.
    gameField.erase(point);
    gameField.insert(std::pair(point, true));
  }
}

void TetrisGame::decideAction(UserInput userInput) {

  // Save tetromino location before moving
  // in case of collision
  std::vector<Point> previousLocation = currentTetromino->getCurrentLocation();
  int previousAngle = currentTetromino->getCurrentAngle();

  if (userInput.isKeyA()) {
    // currentTetromino->rotateLeft();
  }

   else if (userInput.isKeyS()) {
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
    earnedPoints += 1;
  }

  else if(userInput.isKeyUp()){
    currentTetromino->moveUp();
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
    userInput.isKeyA(), 
    userInput.isKeyS(), 
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

    // Update number of destroyed lines
    updateDestroyedLines();

    // Update score before next tetromino appears
    updateScore();

    // Reset earned points
    earnedPoints = 0;

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
    
    // // Numerate rows
    // tm_->drawString(i, offset_col - 2, 2, (std::to_string(i).c_str()));
    
    tm_->drawPixel(i, offset_col + cols_, 2);
  }


  for (int i = offset_col; i < cols_ + offset_col + 1; i++) {
    // Draw top and bottom parts of game field
    tm_->drawPixel(offset_row, i, 2);

    // if(i % 2 == 0)
    // {

    //   tm_->drawString(offset_row - 2, i, 2, (std::to_string(i).c_str()));

    // }
    // else
    // {
    //   tm_->drawString(offset_row - 3, i, 2, (std::to_string(i).c_str()));

    // }
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
  std::string stringScore = std::to_string(statistics[tetrominoIndex]);
  int leadingZeroes = 3 - stringScore.length();
  stringScore.insert(0, leadingZeroes, '0');
  
  // Update statistics of a tetromino with given index.
  // First coordinate is a row, which value corresponds to the index row of tetromino
  // in the statistics table. Second argument is column of that tetromino shifted by 6.
  // Such shifting allows us to overwrite current score.
  tm_->drawString(statisticsRowEnd - (tetrominoIndex) * 3, statisticsColStart + 6, (int)NamedColors::WHITE, stringScore.c_str());
}

void TetrisGame::drawDestroyedLinesText()
{
  tm_->drawString(linesRow, linesCol, (int)NamedColors::WHITE, "LINES - 000");
}

void TetrisGame::updateDestroyedLines()
{
  // Get quotient and remainder. If quotient > then current level,
  // then we have enough points for the next level.
  div_t divresult = std::div(destroyedLines, 10);
  if(divresult.quot > currentLevel)
  {
    updateLevel(divresult.quot);
  }

  std::string stringScore = std::to_string(destroyedLines);
  int leadingZeroes = 3 - stringScore.length();
  stringScore.insert(0, leadingZeroes, '0'); 
  tm_->drawString(linesRow, linesCol+4 , (int)NamedColors::WHITE, stringScore.c_str());
}

void TetrisGame::drawLevelText()
{
  tm_->drawString(levelRow, levelCol, (int)NamedColors::WHITE, "LEVEL - 000");
}

void TetrisGame::updateLevel(int newLevel)
{
  currentLevel = newLevel;
  std::string stringLevel = std::to_string(currentLevel);
  int leadingZeroes = 3 - stringLevel.length();
  stringLevel.insert(0, leadingZeroes, '0');

  tm_->drawString(levelRow, levelCol + 4, (int)NamedColors::WHITE, stringLevel.c_str());
}

void TetrisGame::drawScoreText()
{
  tm_->drawString(scoreRow, scoreCol, (int)NamedColors::WHITE, "SCORE - 000000");
}

void TetrisGame::updateScore()
{
  currentPoints += earnedPoints;
  std::string stringPoints = std::to_string(currentPoints);
  int leadingZeros = 6 - stringPoints.length();
  stringPoints.insert(0, leadingZeros, '0');
  tm_->drawString(scoreRow, scoreCol + 4, (int)NamedColors::WHITE, stringPoints.c_str());
}