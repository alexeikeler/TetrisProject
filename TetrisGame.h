// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./TerminalManager.h"
#include "./Tetromino.h"
#include <unordered_map>

enum class Collision {Roof, Wall, Floor, Block, Nothing};

class TetrisGame {
public:
  TetrisGame(TerminalManager *tm);
  ~TetrisGame(){};

  void drawScore();
  void drawNextTetromino();
  void drawGameField();
  
  void placeTetromino();

  void play();
  void decideAction(UserInput userInput);
  Collision isColliding();
  void drawTetromino();

  // Removing in this context means
  // drawing black pixels on top of the current coordinates
  // Usually this method should be called
  // before tetromino will change it's location,
  // which will result in "moving-like" behaviour
  // when combined with drawTetromino()
  void removeTetrominoFromScreen();

  // Generate random tetromino
  void generateTetromino();

private:
  TerminalManager *tm_;

  static const int rows_ = 20;
  static const int cols_ = 10;

  // Logical game "screen" where all blocks
  // will be stored. Based on values of this array
  // we will draw the picture on the screen.
  // int gameField[rows_][cols_];
  std::unordered_map<Point, bool> gameField;


  // To put our "game screen" in the middle
  // we will need some offset
  const int offset_row = 14;
  const int offset_col = 40;

  // Color for the borders of the game
  // currenlty 1, but will be changed
  const int borderColor = 1;

  // Current tetromino which will be displayed on the screen
  AbstractTetromino *currentTetromino;

  // To store current game scrore
  int currentScore = 0;

  // To show next tetromino based on char (I, J, L, O, T, Z, S)
  char nextTetromino;

  // Falling speed (depends from currentLevel)
  float fallingSpeed;

  // current level of the game
  int currentLevel = 0;

  // keys for rotation
  // default a & s
  char leftRotationKey = 's';

  char rightRotationKey = 'a';
};