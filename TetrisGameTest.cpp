// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./TetrisGame.h"
#include "./AbstractTetromino.h"
#include "./Point.h"
#include "./Tetromino.h"
#include <gtest/gtest.h>
#include <vector>

TEST(TetrominoCreation, Tetromino)
{
  // Test creation
  NewAbstractTetromino *tetrI = new TetrominoI();

  ASSERT_EQ(4, tetrI->getTetrominoSize());
  ASSERT_EQ(0, tetrI->getCurrentAngle());
  ASSERT_FALSE(tetrI->getCurrentLocation().empty());

  // Avoid memory leaks!
  delete tetrI;

  tetrI = new TetrominoT();

  ASSERT_EQ(4, tetrI->getTetrominoSize());
  ASSERT_EQ(0, tetrI->getCurrentAngle());
  ASSERT_FALSE(tetrI->getCurrentLocation().empty());

  // Avoid memory leaks!
  delete tetrI;

  tetrI = new TetrominoO();

  ASSERT_EQ(4, tetrI->getTetrominoSize());
  ASSERT_EQ(0, tetrI->getCurrentAngle());
  ASSERT_FALSE(tetrI->getCurrentLocation().empty());

  // Avoid memory leaks!
  delete tetrI;

  tetrI = new TetrominoZ();

  ASSERT_EQ(4, tetrI->getTetrominoSize());
  ASSERT_EQ(0, tetrI->getCurrentAngle());
  ASSERT_FALSE(tetrI->getCurrentLocation().empty());

  // Avoid memory leaks!
  delete tetrI;

  tetrI = new TetrominoS();

  ASSERT_EQ(4, tetrI->getTetrominoSize());
  ASSERT_EQ(0, tetrI->getCurrentAngle());
  ASSERT_FALSE(tetrI->getCurrentLocation().empty());

  // Avoid memory leaks!
  delete tetrI;

  tetrI = new TetrominoJ();

  ASSERT_EQ(4, tetrI->getTetrominoSize());
  ASSERT_EQ(0, tetrI->getCurrentAngle());
  ASSERT_FALSE(tetrI->getCurrentLocation().empty());

  // Avoid memory leaks!
  delete tetrI;

  tetrI = new TetrominoL();

  ASSERT_EQ(4, tetrI->getTetrominoSize());
  ASSERT_EQ(0, tetrI->getCurrentAngle());
  ASSERT_FALSE(tetrI->getCurrentLocation().empty());

  // Avoid memory leaks!
  delete tetrI;
}