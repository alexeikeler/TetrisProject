// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./TerminalManager.h"
#include "./TetrisGame.h"
#include "./Tetromino.h"

int main() {
  // TODO: Move this to TerminalManager.h and create
  // enum to avoid magic numbers when using colors
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

  std::vector<std::pair<Color, Color>> colorVector;

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
  

  TerminalManager *tm = new TerminalManager(colorVector);
  TetrisGame game(tm);
  game.play();
}