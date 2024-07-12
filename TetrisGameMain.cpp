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

  std::vector<std::pair<Color, Color>> colorVector;

  colorVector.push_back(MainColorPair);
  colorVector.push_back(BlackColorPair);
  colorVector.push_back(BordersColorPair);
  colorVector.push_back(TetrominoTColorPair);

  TerminalManager *tm = new TerminalManager(colorVector);
  TetrisGame game(tm);
  game.play();
}