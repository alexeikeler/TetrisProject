// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./TetrisGame.h"
#include "./TerminalManager.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

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
  for (int i = offset_row; i < offset_row + rows_; i++) {

    for (int j = offset_col; j < offset_col + cols_; j++) {
      gameField[Point{i, j, NamedColors::BLACK}] = false;
    }
  }

  // Create shapes for "Next" tetromino box.
  for (int i = 0; i < numberOfTetrominos; i++) {
    std::vector<Point> shape;
    // I'm starting from the third NamedColor value because it corresponds to
    // the first color of the tetromino. Here I'm using map with functions to
    // avoid manual creation of all shapes. I'm also creating shapes this way
    // because I need them to be created more then once (for example for
    // statistics).
    shapeMapper[i](nextTetrominoRowStart + 1, nextTetrominoColStart + 2, &shape,
                   static_cast<NamedColors>(i + 3));
    shapes.push_back(shape);
  }
}

void TetrisGame::play() {

  // We will treat currentSpeed as a "time" variable.
  // For example: if we start the game with level 0 we will wait 48/60 <=> 0.8
  // sec <=> 800 ms. for user input and if we won't get any then we will move
  // tetromino down.
  int timer = currentSpeed;

  // Main game loop

  // We will need to save current tetromino id
  // to be able to update statistics later.
  int current;

  // Because we need to know next tetromino immideatly (to print it on the
  // screen) we need to generate two rundom numbers (r1 != r2) and use deque to
  // code correct behaviour.

  // Generate initial random numbers and put them at the back of the deque.
  generateCurrentAndNext();
  deque.push_back(currentRandomNumber);
  deque.push_back(nextRandomNumber);

  // Main game loop.
  while (!deque.empty()) {

    // Get first element of the deque and create current tetromino
    // Save current element for the statistics.

    current = deque.front();
    NewAbstractTetromino *tetr = chooseTetromino(current);

    // Remove first element from the deque
    deque.pop_front();

    // Generate next random numbers and add them to the deque.
    // We need to add them only if we have 1 element left, because
    // otherwise our deque will be growing infinitely.
    if (deque.size() == 1) {
      generateCurrentAndNext();
      deque.push_back(currentRandomNumber);
      deque.push_back(nextRandomNumber);
    }

    // Here we will get the element that follows the element that is stored in
    // "current", since deque.pop_front() was used. Thus the resulting element
    // is the following.
    drawNextTetromino(deque.front());

    // Assign current tetromino.
    currentTetromino = tetr;
    drawTetromino();

    // until it's alive (i.e not collided)
    while (currentTetromino != nullptr) {
      UserInput userInput = tm_->getUserInput();

      // Wait for input
      if (userInput.keycode_ == -1) {
        timer -= 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
      // If we get input from user
      else {
        decideAction(userInput, false);
        usleep(20'000);
      }
      // If we didn't get user input and the time is up
      if (timer == 0) {
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

void TetrisGame::gameOver() {
  for (int i = 0; i < tm_->numRows(); i++) {
    for (int j = 0; j < tm_->numCols(); j++) {
      tm_->drawPixel(i, j, (int)NamedColors::BLACK);
    }
  }

  tm_->drawString(tm_->numRows() / 2, tm_->numCols() / 2,
                  (int)NamedColors::WHITE, "GAME OVER!");
  tm_->refresh();

  std::this_thread::sleep_for(std::chrono::milliseconds(gameOverTimeroutMs));
  // Call destructor to avoid ncurses terminal bug
  tm_->~TerminalManager();

  exit(0);
}

void TetrisGame::reshapeGameField() {
  // Variable to store number of alive points per line
  int acc = 0;

  // vector with lines index
  std::vector<int> rowsToRemove;

  // Go through every line
  for (int i = offset_row; i < offset_row + rows_; i++) {
    acc = 0;

    for (int j = offset_col + 1; j < offset_col + cols_; j++) {
      acc += gameField[Point{i, j, NamedColors::BLACK}];
    }

    // If this holds, then line is full and we need to remove it
    // (- 1 because of the border, we don't want to count it)
    if (acc == cols_ - 1) {
      rowsToRemove.push_back(i);
    }
  }

  // Nothing to remove
  if (rowsToRemove.empty()) {
    return;
  }

  destroyedLines += rowsToRemove.size();
  earnedPoints +=
      ((currentLevel + 1) * pointsForRemovedRows[rowsToRemove.size()]);

  // Remove all points from collected rows
  for (int row : rowsToRemove) {

    // Remove point everywhere
    for (int j = offset_col + 1; j < offset_col + cols_; j++) {
      Point pointToRemove = Point{row, j, NamedColors::BLACK};

      auto it = std::find(surface.begin(), surface.end(), pointToRemove);
      if (it != surface.end()) {
        surface.erase(it);
      }

      gameField[pointToRemove] = false;
      removePointFromScreen(pointToRemove);
    }
  }
  bool flag = false;

  // Now we need to move all drawn the points that are above the lines to be
  // deleted

  for (int row : rowsToRemove) {
    for (int i = row; i > offset_row; i--) {
      for (int j = offset_col + 1; j < offset_col + cols_; j++) {

        Point currentPoint = Point{i, j, NamedColors::BLACK};
        // If point is drawn
        if (gameField[currentPoint]) {
          auto pointInGameField = gameField.find(currentPoint);

          // Set it to false
          gameField[currentPoint] = false;

          // Immitates "falling"
          usleep(15'000);

          // Remove current point from screen
          removePointFromScreen(currentPoint);

          // Remove current point from surface set. We need to do this
          // to avoid false collisions. We also need flag variable to know for
          // sure, that we have deleted the point.
          auto it = std::find(surface.begin(), surface.end(), currentPoint);
          if (it != surface.end()) {
            flag = true;
            surface.erase(it);
          }

          // Move point one row down
          currentPoint.row += 1;

          currentPoint.color = pointInGameField->first.color;

          // Put point with new coordinates back in surface set. This will
          // provide correct collision
          if (flag) {
            surface.insert(currentPoint);
          }

          // Put new on the screen and add it to the "logical screen".
          gameField[currentPoint] = true;
          tm_->drawPixel(currentPoint.row, currentPoint.col,
                         (int)currentPoint.color);
        }

        flag = false;
      }
    }
  }
}

void TetrisGame::placeTetromino() {
  for (auto point : currentTetromino->getCurrentLocation()) {
    // If we are trying to place a tetromino
    // on the roof level it's game over.
    if (point.row == offset_row + 1) {
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
    if (!isArtificialMovement) {
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
  // current "surface" (see .h for exact information about surface points)
  // point, which will result in wrong collision.

  Collision collision = isColliding(
      userInput.isKeyDown(), userInput.isLeftRotationKey(leftRotationKey),
      userInput.isRightRotationKey(rightRotationKey), previousLocation);

  if (collision == Collision::Surface) {

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
    if (divresult.quot > previousQuotient) {
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

  else if (collision == Collision::Wall || collision == Collision::Block) {
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

void TetrisGame::removePointFromScreen(Point point) {
  tm_->drawPixel(point.row, point.col, (int)NamedColors::BLACK);
  tm_->refresh();
}

void TetrisGame::drawTetromino() {
  for (auto point : currentTetromino->getCurrentLocation()) {
    tm_->drawPixel(point.row, point.col,
                   (int)currentTetromino->getTetrominoColor());
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
    // tm_->drawPixel(offset_row, i, 2);
    tm_->drawPixel(offset_row + rows_, i, 2);
  }

  tm_->refresh();
}

void TetrisGame::drawNextTetrominoText() {
  tm_->drawString(nextTetrominoRowStart - 1, nextTetrominoColStart,
                  (int)NamedColors::WHITE, "Next Tetromino :");
}

void TetrisGame::drawNextTetromino(int tetrominoIndex) {
  // Clear 4x4 box
  for (int i = nextTetrominoRowStart + 1; i < nextTetrominoRowEnd; i++) {
    for (int j = nextTetrominoColStart + 1; j < nextTetrominoColEnd; j++) {
      removePointFromScreen(Point{i, j, NamedColors::BLACK});
    }
  }

  // Draw next tetromino.
  for (Point point : shapes[tetrominoIndex]) {
    tm_->drawPixel(point.row, point.col, (int)point.color);
  }
}

void TetrisGame::drawStatistics() {
  // Draw word "Statistics"
  tm_->drawString(statisticsRowStart, statisticsColStart + 2,
                  (int)NamedColors::WHITE, "Statistics");

  // We will need this offset to draw everything more or less alligned
  int offset = 3;

  // Draw each tetromino and current score.
  // (I'm drawing them from last to first so that it looks
  // like they have even amout of space between each other)

  for (int i = numberOfTetrominos - 1; i >= 0; i--) {
    std::vector<Point> shape;

    // Create shape according to index.
    shapeMapper[i](statisticsRowStart + offset, statisticsColStart, &shape,
                   static_cast<NamedColors>(i + 3));
    // Move offset so that the current shape will be drawn properly without
    // collision.
    offset += 3;

    for (Point point : shape) {
      tm_->drawPixel(point.row, point.col, (int)point.color);
    }

    // Draw the number of the placed tetrominos of certain type on the right
    // side.
    tm_->drawString(statisticsRowStart + offset - 3, statisticsColStart + 6,
                    (int)NamedColors::WHITE, "000");
  }
}

void TetrisGame::updateStatisticsText(int tetrominoIndex) {
  tm_->drawString(statisticsRowEnd - (tetrominoIndex)*3, statisticsColStart + 6,
                  (int)NamedColors::WHITE,
                  intToString(statistics[tetrominoIndex], 3).c_str());
}

void TetrisGame::drawDestroyedLinesText() {
  tm_->drawString(linesRow, linesCol, (int)NamedColors::WHITE, "LINES - 000");
}

void TetrisGame::updateDestroyedLinesText() {
  tm_->drawString(linesRow, linesCol + 4, (int)NamedColors::WHITE,
                  intToString(destroyedLines, 3).c_str());
}

void TetrisGame::drawLevelText() {
  tm_->drawString(levelRow, levelCol, (int)NamedColors::WHITE, "LEVEL - 000");
}

void TetrisGame::updateLevelAndSpeedText() {
  tm_->drawString(levelRow, levelCol + 4, (int)NamedColors::WHITE,
                  intToString(currentLevel, 3).c_str());
}

void TetrisGame::drawScoreText() {
  tm_->drawString(scoreRow, scoreCol, (int)NamedColors::WHITE,
                  "SCORE - 000000");
}

void TetrisGame::updateScoreText() {
  tm_->drawString(scoreRow, scoreCol + 4, (int)NamedColors::WHITE,
                  intToString(currentPoints, 6).c_str());
}
