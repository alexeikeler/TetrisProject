// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once

#include "./AbstractTetrisGame.h"

class MockTetrisGame : public AbstractTetrisGame {
public:
  // Instead of writing tons of getters I've decided to use friend test.
  friend class MockTetrisGameSimpleMovement_MockTetrisGame_Test;
  friend class MockTetrisGamePlacement_MockTetrisGame_Test;
  friend class MockTetrisGameIsGameOver_MockTetrisGame_Test;
  friend class MockTetrisGameCollision_MockTetrisGame_Test;
  friend class MockTetrisGameLineRemoving_MockTetrisGame_Test;

  // We don't need terminal manager for this.
  MockTetrisGame(int level, char rrk, char lrk);
  ~MockTetrisGame(){};

  // We will need to override some of the methods
  // for test purposes.
  void play() override;
  void gameOver() override;
  void decideAction(UserInput userInput, bool isAritificialMovement) override;
  void placeTetromino() override;
  void reshapeGameField() override;

private:
  // Some additional variables for testing.
  bool isGameOver = false;
  Collision lastCollision = Collision::Nothing;
  bool isCurrentTetrominoPlaced = false;
};