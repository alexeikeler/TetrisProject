// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./MockTerminalManager.h"
#include "./TetrisGame.h"
#include "./AbstractTetromino.h"
#include "./Point.h"
#include "./Tetromino.h"
#include <gtest/gtest.h>
#include <vector>

TEST(Tetromino_I_Creation, TetrominoI) {
  // Test creation
  AbstractTetromino *tetrI = new TetrominoI();

  ASSERT_EQ(4, tetrI->getTetrominoSize());
  ASSERT_EQ(90, tetrI->getTetrominoAngle());
  ASSERT_FALSE(tetrI->getCurrentLocation().empty());

  // Avoid memory leaks!
  delete tetrI;
}

TEST(Tetromino_I_Moving, TetrominoI) {
  // Test moving
  AbstractTetromino *tetrI = new TetrominoI();
  std::vector<Point> locationBeforeMoving = tetrI->getCurrentLocation();

  tetrI->moveLeft();
  tetrI->moveLeft();
  tetrI->moveDown();
  tetrI->moveRight();

  std::vector<Point> locationAfterMoving = tetrI->getCurrentLocation();

  for (int i = 0; i < tetrI->getTetrominoSize(); i++) {
    ASSERT_EQ(locationBeforeMoving[i].col - 1, locationAfterMoving[i].col);
    ASSERT_EQ(locationBeforeMoving[i].row + 1, locationAfterMoving[i].row);
  }

  // Avoid memory leaks!
  delete tetrI;
}

TEST(Tetromino_I_RightRotation, TetrominoI) {
  AbstractTetromino *tetrI = new TetrominoI();

  // Test right rotation 90 --> 180
  std::vector<Point> locationBeforeRotation = tetrI->getCurrentLocation();
  int angleBeforeRotation = tetrI->getTetrominoAngle();

  tetrI->rotateRight();

  std::vector<Point> locationAfterRotation = tetrI->getCurrentLocation();
  int angleAfterRotation = tetrI->getTetrominoAngle();

  // Compare angles and coordinates after one right rotation
  ASSERT_EQ(angleBeforeRotation + 90, angleAfterRotation);

  ASSERT_EQ(locationBeforeRotation[0].col -= 1, locationAfterRotation[0].col);
  ASSERT_EQ(locationBeforeRotation[0].row += 1, locationAfterRotation[0].row);

  ASSERT_EQ(locationBeforeRotation[1].col, locationAfterRotation[1].col);
  ASSERT_EQ(locationBeforeRotation[1].row, locationAfterRotation[1].row);

  ASSERT_EQ(locationBeforeRotation[2].col -= 1, locationAfterRotation[2].col);
  ASSERT_EQ(locationBeforeRotation[2].row, locationAfterRotation[2].row);

  ASSERT_EQ(locationBeforeRotation[3].col, locationAfterRotation[3].col);
  ASSERT_EQ(locationBeforeRotation[3].row -= 1, locationAfterRotation[3].row);

  // Test full circle right rotation 180 ---> 0 ---> 90 ---> 180;
  std::vector<Point> locationBeforeFullCircleRotation =
      tetrI->getCurrentLocation();
  int angleBeforeFullCircleRotation = tetrI->getTetrominoAngle();

  tetrI->rotateRight();
  tetrI->rotateRight();
  tetrI->rotateRight();

  std::vector<Point> locationAfterFullCircleRotation =
      tetrI->getCurrentLocation();
  int angleAfterFullCircleRotation = tetrI->getTetrominoAngle();

  // After full cycle coordinates and angle should remain the same
  ASSERT_EQ(angleBeforeFullCircleRotation, angleAfterFullCircleRotation);

  for (int i = 0; i < tetrI->getTetrominoSize(); i++) {
    ASSERT_EQ(locationBeforeFullCircleRotation[i].col,
              locationAfterFullCircleRotation[i].col);
    ASSERT_EQ(locationBeforeFullCircleRotation[i].row,
              locationAfterFullCircleRotation[i].row);
  }

  // Avoid memory leaks!
  delete tetrI;
}

TEST(Tetromino_I_LeftRotation, TetrominoI) {
  AbstractTetromino *tetrI = new TetrominoI();

  // Test left rotation
  std::vector<Point> locationBeforeRotation = tetrI->getCurrentLocation();
  int angleBeforeRotation = tetrI->getTetrominoAngle();

  tetrI->rotateLeft();

  std::vector<Point> locationAfterRotation = tetrI->getCurrentLocation();
  int angleAfterRotation = tetrI->getTetrominoAngle();

  ASSERT_EQ(angleBeforeRotation - 90, angleAfterRotation);

  // Compare coordinates after left rotation
  ASSERT_EQ(locationBeforeRotation[0].row += 2, locationAfterRotation[0].row);
  ASSERT_EQ(locationBeforeRotation[0].col -= 2, locationAfterRotation[0].col);

  ASSERT_EQ(locationBeforeRotation[1].row += 1, locationAfterRotation[1].row);
  ASSERT_EQ(locationBeforeRotation[1].col -= 1, locationAfterRotation[1].col);

  ASSERT_EQ(locationBeforeRotation[3].row -= 1, locationAfterRotation[3].row);
  ASSERT_EQ(locationBeforeRotation[3].col += 1, locationAfterRotation[3].col);

  // Test full circle left rotation 0 <--- 90 <--- 180 <--- 0;
  std::vector<Point> locationBeforeFullCircleRotation =
      tetrI->getCurrentLocation();
  int angleBeforeFullCircleRotation = tetrI->getTetrominoAngle();

  tetrI->rotateLeft();
  tetrI->rotateLeft();
  tetrI->rotateLeft();

  std::vector<Point> locationAfterFullCircleRotation =
      tetrI->getCurrentLocation();
  int angleAfterFullCircleRotation = tetrI->getTetrominoAngle();

  // After full cycle coordinates and angle should remain the same
  ASSERT_EQ(angleBeforeFullCircleRotation, angleAfterFullCircleRotation);

  for (int i = 0; i < tetrI->getTetrominoSize(); i++) {
    ASSERT_EQ(locationBeforeFullCircleRotation[i].col,
              locationAfterFullCircleRotation[i].col);
    ASSERT_EQ(locationBeforeFullCircleRotation[i].row,
              locationAfterFullCircleRotation[i].row);
  }

  // Avoid memory leaks!
  delete tetrI;
}

TEST(MockTerminalManagerWork, MockTerminalManager)
{
  MockTerminalManager mtm(10, 10);

  ASSERT_EQ(10, mtm.numCols());
  ASSERT_EQ(10, mtm.numRows());

  ASSERT_TRUE(mtm.getDrawnPixels().empty());

  mtm.drawPixel(3, 3, 0);
  mtm.drawPixel(3, 5, 0);
  mtm.drawPixel(5, 3, 0);
  
  ASSERT_TRUE(mtm.isPixelDrawn(3, 3));
  ASSERT_TRUE(mtm.isPixelDrawn(5, 3));
  ASSERT_TRUE(mtm.isPixelDrawn(3, 5));

  ASSERT_FALSE(mtm.isPixelDrawn(9, 9));
}