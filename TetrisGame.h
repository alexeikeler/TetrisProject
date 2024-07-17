// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./TerminalManager.h"
#include "./Tetromino.h"
#include <unordered_map>
#include <deque>
#include <set>
#include <functional>

enum class Collision {Roof, Wall, Block, Nothing, Surface, Floor};

class TetrisGame {
public:
  TetrisGame(TerminalManager *tm, int level, char rrk, char lrk);
  ~TetrisGame(){};

  void drawLevelText();
  void updateLevelAndSpeed(int increaseLevelBy=0);

  void drawScoreText();
  void updateScore();

  void drawDestroyedLinesText();
  void updateDestroyedLines();

  void drawStatistics();
  void updateStatistics(int tetrominoIndex);
  
  void drawNextTetrominoText();
  void drawNextTetromino(int tetrominoIndex);

  void drawGameField();
  void play();


  // Removing in this context means
  // drawing black pixels on top of the current coordinates
  // Usually this method should be called
  // before tetromino will change it's location,
  // which will result in "moving-like" behaviour
  // when combined with drawTetromino()
  void removeTetrominoFromScreen(std::vector<Point> location);
  
  // "Remove" point from SCREEN (paint it black).
  void removePointFromScreen(Point point);

  void drawTetromino();
  // "Place" tetromino in gameField
  void placeTetromino();
  // Decide what to do with the current tetromino
  void decideAction(UserInput userInput);  
  
  // Check if tetromino colliding
  Collision isColliding(bool downPressed, bool leftRotaion, bool rightRotation, std::vector<Point> previousLocation);
  
  // Method for finding and updating surface
  void updateSurface();

  // Remove lines
  void reshapeGameField();

  // Generate random tetromino
  void generateCurrentAndNext(int a = 0, int b = 6);
  
  int generateRandomNumber(int a, int b);

  NewAbstractTetromino* chooseTetromino(const int randomNumber);

  // Convert int to string with leading zeroes
  std::string intToString(int number, int maxLength);

private:
  TerminalManager *tm_;
  std::deque<int> deque;
  const int numberOfTetrominos = 7;
  static const int rows_ = 21;
  static const int cols_ = 11;

  // Logical game "screen" where all blocks
  // will be stored. Based on values of this array
  // we will draw the picture on the screen.
  // int gameField[rows_][cols_];
  std::unordered_map<Point, bool> gameField;
  
  // vector of highest points in columns
  std::set<Point> surface;


  // To put our "game screen" in the middle
  // we will need some offset
  const int offset_row = 14;
  const int offset_col = 40;

  // Current tetromino which will be displayed on the screen
  NewAbstractTetromino *currentTetromino;

  // To generate new tetrominos
  int previousRandomNumber;
  int currentRandomNumber;
  int nextRandomNumber;

  std::unordered_map<int, double> fallingSpeed = {
    {0, (double)48/60},
    {1, (double)43/60},
    {2, (double)38/60},
    {3, (double)33/60},
    {4, (double)28/60},
    {5, (double)23/60},
    {6, (double)18/60},
    {7, (double)13/60},
    {8, (double)8/60},
    {9, (double)6/60},
    {10, (double)5/60},
    {11, (double)5/60},
    {12, (double)5/60},
    {13, (double)4/60},
    {14, (double)4/60},
    {15, (double)4/60},
    {16, (double)3/60},
    {17, (double)3/60},
    {18, (double)3/60},
    {19, (double)2/60},
    {20, (double)2/60},
    {21, (double)2/60},
    {22, (double)2/60},
    {23, (double)2/60},
    {24, (double)2/60},
    {25, (double)2/60},
    {26, (double)2/60},
    {27, (double)2/60},
    {28, (double)2/60},
    {29, (double)1/60},
  };
  // Falling speed (depends from currentLevel)
  double currentSpeed = fallingSpeed[0];

  // keys for rotation
  // default a & s
  char leftRotationKey = 's';

  char rightRotationKey = 'a';

  // Coordinates of the "Box" for the next tetromino
  int nextTetrominoRowStart = 20;
  int nextTetrominoRowEnd = 25;
  
  int nextTetrominoColStart = 54;
  int nextTetrominoColEnd = 60;

  // All tetromino shapes. We need them all at once
  // to be able to draw next tetromino.
  std::vector<std::vector<Point>> shapes;

  // Statistics map. First integer is the integer code for concrete tetromino,
  // second integer corresponds to the number of placed tetrominos.
  std::unordered_map<int, int> statistics = {
    {0, 0},
    {1, 0},
    {2, 0},
    {3, 0},
    {4, 0},
    {5, 0},
    {6, 0}
  };


  using shapeFunctions = std::function<void(int, int, std::vector<Point>*, NamedColors)>;
  std::unordered_map<int, shapeFunctions> shapeMapper = {
    {0, TetrominoShape::createIShape},
    {1, TetrominoShape::createJShape},
    {2, TetrominoShape::createLShape},
    {3, TetrominoShape::createOShape},
    {4, TetrominoShape::createSShape},
    {5, TetrominoShape::createZShape},
    {6, TetrominoShape::createTShape}
  };
  
  // Coordinates of the statistics
  int statisticsRowStart = 13;
  int statisticsColStart = 25;
  int statisticsRowEnd = 34;

  // Coordinates of the "LINES-xxx" line and number of
  // the removed lines 
  int linesRow = 12;
  int linesCol = 43;
  // We will need this variable to update the level
  // properly
  int previousQuotient = 0;
  int destroyedLines = 0;

  // Coordinates of the "LEVEL-xxx" line and current level.
  int levelRow = 10;
  int levelCol = 43;
  int currentLevel = 0;

  // Coordinates of the "Score - xxxxxx" line,
  // and variables to store earned points per tetromino
  // life cycle and current points.
  int scoreRow = 17;
  int scoreCol = 54;
  int earnedPoints = 0;
  int currentPoints = 0;

  // Map for points which player gets
  // after removing certain number of rows.
  std::unordered_map<int, int> pointsForRemovedRows ={
    {1, 40},
    {2, 100},
    {3, 300},
    {4, 1200}
  };
};