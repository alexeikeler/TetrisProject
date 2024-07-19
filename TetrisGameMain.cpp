// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./TerminalManager.h"
#include "./TetrisGame.h"
#include "./Tetromino.h"
#include "./ParseArguments.h"
#include <iostream>


std::vector<std::pair<Color, Color>> createColorVector()
{

    std::vector<std::pair<Color, Color>> colorVector;

  std::pair MainColorPair =
      std::pair(Color(1.0, 0.0, 0.0), Color(0.0, 0.0, 0.0));
  std::pair BlackColorPair =
      std::pair(Color(0.0, 0.0, 0.0), Color(0.0, 0.0, 0.0));
  std::pair BordersColorPair =
      std::pair(Color(0, 0.522, 1), Color(0.0, 0.0, 0.0));
  std::pair TetrominoTColorPair = 
      std::pair(Color(0.153, 0.659, 0.047), Color(0.0, 0.0, 0.0));
  std::pair TetrominoLColorPair = 
      std::pair(Color(0.004, 0, 1), Color(0.0, 0.0, 0.0));
  std::pair TetrominoJColorPair = 
      std::pair(Color(0.906, 1, 0), Color(0.0, 0.0, 0.0));
  std::pair TetrominoOColorPair = 
      std::pair(Color(1.0, 0.6, 0.0), Color(0.0, 0.0, 0.0));
  std::pair TetrominoIColorPair = 
      std::pair(Color(1, 0, 0.886), Color(0.0, 0.0, 0.0));
  std::pair TetrominoZColorPair = 
      std::pair(Color(0.455, 0, 1), Color(0.0, 0.0, 0.0));
  std::pair TetrominoSColorPair = 
      std::pair(Color(0, 0.859, 0.655), Color(0.0, 0.0, 0.0));

  std::pair WhileColorPair(Color(1, 1, 1), Color(0.0, 0.0, 0.0));

  colorVector.push_back(MainColorPair);
  colorVector.push_back(BlackColorPair);
  colorVector.push_back(BordersColorPair);
  colorVector.push_back(TetrominoTColorPair);
  colorVector.push_back(TetrominoLColorPair);
  colorVector.push_back(TetrominoJColorPair);
  colorVector.push_back(TetrominoOColorPair);
  colorVector.push_back(TetrominoIColorPair);
  colorVector.push_back(TetrominoZColorPair);
  colorVector.push_back(TetrominoSColorPair);
  colorVector.push_back(WhileColorPair);

  return colorVector;
}


int main(int argc, char **argv) {
  // Create color vector.
  std::vector<std::pair<Color, Color>> colorVector = createColorVector();
  
  //std::cout << *argv << std::endl;

  // Create parser in order to parse arguments.
  Parser parser;
  parser.parseArguments(argc, argv);
  
  // Get values from parsed args.
  int level = parser.getLevel();
  char rightRotationKey = parser.getRightRotationKey();
  char leftRotationKey = parser.getLeftRotationKey();

  // Create new terminal manager with colors and start the game.
  TerminalManager *tm = new TerminalManager(colorVector);
  TetrisGame game(tm, level, rightRotationKey, leftRotationKey);
  game.play();
}