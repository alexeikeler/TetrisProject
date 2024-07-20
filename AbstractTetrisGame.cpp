#include "./AbstractTetrisGame.h"
#include "./Tetromino.h"
#include <chrono>
#include <random>
#include <vector>
#include <string>

void AbstractTetrisGame::updateLevelAndSpeed(int increaseLevelBy)
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

}

void AbstractTetrisGame::updateDestroyedLines()
{
  div_t divresult = std::div(destroyedLines, 10);
  if(divresult.quot > previousQuotient)
  {
    updateLevelAndSpeed(1);
    previousQuotient = divresult.quot;
  }
}

void AbstractTetrisGame::updateStatistics(int tetrominoIndex)
{
    statistics[tetrominoIndex] += 1;
}

void AbstractTetrisGame::updateScore()
{
  currentPoints += earnedPoints;
}

Collision AbstractTetrisGame::isColliding(
      bool downPressed,  bool leftRotaion, bool rightRotation, std::vector<Point> previousLocation
)
{

  
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

void AbstractTetrisGame::updateSurface()
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

NewAbstractTetromino* AbstractTetrisGame::chooseTetromino(int randomNumber)
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
}

int AbstractTetrisGame::generateRandomNumber(int a, int b)
{
  // Use current time as seed to get a new number each time
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::default_random_engine generator (seed);
  
  // Create uniforme distribution of numbers from 0 to 6
  std::uniform_int_distribution<int> distribution (a,b);
  
  // Return num in [0, 6] with p(num) = 1/7.
  return distribution(generator);
}

void AbstractTetrisGame::generateCurrentAndNext(int a, int b)
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

std::string AbstractTetrisGame::intToString(int number, int maxLength)
{
  std::string stringNumber = std::to_string(number);
  int leadingZeroes =  maxLength - stringNumber.length();
  stringNumber.insert(0, leadingZeroes, '0');
  return stringNumber;
}


