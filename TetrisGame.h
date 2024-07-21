// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once
#include "./TerminalManager.h"
#include "./Tetromino.h"
#include "AbstractTetrisGame.h"
#include <deque>
#include <functional>
#include <set>
#include <unordered_map>

class TetrisGame : public AbstractTetrisGame {
public:
  // Constructor & destructor
  // rrk - right rotation key
  // lrk - left rotation key
  TetrisGame(TerminalManager *tm, int level, char rrk, char lrk);
  ~TetrisGame(){};

  // Function for drawing data / game field / tetrominos on the screen.
  // --------------------------------------------
  void drawLevelText();

  void updateLevelAndSpeedText();

  void drawScoreText();
  void updateScoreText();

  void drawDestroyedLinesText();
  void updateDestroyedLinesText();

  void drawStatistics();
  void updateStatisticsText(int tetrominoIndex);

  void drawNextTetrominoText();

  void drawNextTetromino(int tetrominoIndex);

  void drawTetromino();

  void drawGameField();
  // --------------------------------------------

  // Main game loop
  void play() override;

  // Exit the game if it's over
  // and draw a "GAME OVER!".
  void gameOver() override;

  // Removing in this context means
  // drawing black pixels on top of the current coordinates
  // Usually this method should be called
  // before tetromino will change it's location,
  // which will result in "moving-like" behaviour
  // when combined with drawTetromino()
  void removeTetrominoFromScreen(std::vector<Point> location);

  // "Remove" point from SCREEN (paint it black).
  void removePointFromScreen(Point point);

  // "Place" tetromino in gameField
  void placeTetromino() override;

  // Decide what to do with the current tetromino
  void decideAction(UserInput userInput, bool isArtificialMovement) override;

  // Remove lines
  void reshapeGameField() override;

private:
  TerminalManager *tm_;

  // We will need some additional variable to be able to update
  // data on the screen.

  // Coordinates of the "Box" for the next tetromino
  const int nextTetrominoRowStart = 20;
  const int nextTetrominoRowEnd = 25;

  const int nextTetrominoColStart = 54;
  const int nextTetrominoColEnd = 60;

  // All tetromino shapes. We need them all at once
  // to be able to draw next tetromino.
  std::vector<std::vector<Point>> shapes;

  // It's a type that will store functions from struct TetrominoShape.
  using shapeFunctions =
      std::function<void(int, int, std::vector<Point> *, NamedColors)>;

  // map with functions that create concrete tetrominos shape.
  std::unordered_map<int, shapeFunctions> shapeMapper = {
      {0, TetrominoShape::createIShape}, {1, TetrominoShape::createJShape},
      {2, TetrominoShape::createLShape}, {3, TetrominoShape::createOShape},
      {4, TetrominoShape::createSShape}, {5, TetrominoShape::createZShape},
      {6, TetrominoShape::createTShape}};

  // Coordinates of the statistics
  const int statisticsRowStart = 13;
  const int statisticsColStart = 25;
  const int statisticsRowEnd = 34;

  // Coordinates of the "LINES-xxx" line and number of
  // the removed lines
  const int linesRow = 12;
  const int linesCol = 43;

  // Coordinates of the "LEVEL-xxx" line and current level.
  const int levelRow = 10;
  const int levelCol = 43;

  // Coordinates of the "Score - xxxxxx" line,
  // and variables to store earned points per tetromino
  // life cycle and current points.
  const int scoreRow = 17;
  const int scoreCol = 54;

  // Hold "Game over" for 1.5 sec.
  const int gameOverTimeroutMs = 1500;
};