//Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
//Code snippets from the lectures where used

#include "./Tetromino.h"
#include "./AbstractTetromino.h"
#include "./MockTerminalManager.h"
#include "./MockTetrisGame.h"
#include "./Point.h"
#include "./ParseArguments.h"
#include <chrono>
#include <thread>
#include <algorithm>

#include <gtest/gtest.h>
#include <vector>

TEST(MockTerminalManagerFunctionality, MockTerminalManager)
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

TEST(PointsFunctionality, Point)
{
    Point p = Point{10, 10, NamedColors::BLACK};

    ASSERT_EQ(p.color, NamedColors::BLACK);
    ASSERT_EQ(p.row, 10);
    ASSERT_EQ(p.col, 10);

    p.row += 1;
    p.col += 1;

    ASSERT_EQ(p.row, 11);
    ASSERT_EQ(p.col, 11);
    ASSERT_EQ(p.color, NamedColors::BLACK);
}

// Command Line Arguments Parser - CLAP
TEST(CLAPLongFunctionality, Parser)
{
    // Test long options
    int argc = 4;
    char programmName[] = "./TetrisGameMain";

    char longArg1[] = "--level=10";
    char longArg2[] = "--leftRotationKey=x";
    char longArg3[] = "--rightRotationKey=z";    
    char *longArgv[] = {programmName, longArg1, longArg2, longArg3};

    Parser longArgsParser;
    longArgsParser.parseArguments(argc, longArgv);

    ASSERT_EQ(10, longArgsParser.getLevel());
    ASSERT_EQ('x', longArgsParser.getLeftRotationKey());
    ASSERT_EQ('z', longArgsParser.getRightRotationKey());
}

// --------------------------------------------------------------------------------------------------------------------
// Creation tests start
// --------------------------------------------------------------------------------------------------------------------


TEST(Tetromino_I_Creation, Tetromino)
{
    NewAbstractTetromino *tetrI = new TetrominoI();
    
    ASSERT_EQ(4, tetrI->getTetrominoSize());
    ASSERT_EQ(0, tetrI->getCurrentAngle());
    ASSERT_EQ(NamedColors::TETROMINO_I, tetrI->getTetrominoColor());
    ASSERT_FALSE(tetrI->getCurrentLocation().empty());
    
    int startingRow = tetrI->getStartingRow();
    int startingCol = tetrI->getStartingCol();

    std::vector<Point> tetrIStartingLocation = {
        Point{startingRow, startingCol, NamedColors::TETROMINO_I},
        Point{startingRow, startingCol + 1, NamedColors::TETROMINO_I},
        Point{startingRow, startingCol + 2, NamedColors::TETROMINO_I},
        Point{startingRow, startingCol + 3, NamedColors::TETROMINO_I}
    };

    ASSERT_EQ(tetrIStartingLocation, tetrI->getCurrentLocation());

    delete tetrI;
}

TEST(Tetromino_T_Creation, Tetromino)
{
    NewAbstractTetromino *tetrT = new TetrominoT();
    
    ASSERT_EQ(4, tetrT->getTetrominoSize());
    ASSERT_EQ(0, tetrT->getCurrentAngle());
    ASSERT_EQ(NamedColors::TETROMINO_T, tetrT->getTetrominoColor());
    ASSERT_FALSE(tetrT->getCurrentLocation().empty());
    
    int startingRow = tetrT->getStartingRow();
    int startingCol = tetrT->getStartingCol();

    std::vector<Point> tetrTStartingLocation = {
        Point{startingRow, startingCol, NamedColors::TETROMINO_T},
        Point{startingRow, startingCol + 1, NamedColors::TETROMINO_T},
        Point{startingRow + 1, startingCol + 1, NamedColors::TETROMINO_T},
        Point{startingRow, startingCol + 2, NamedColors::TETROMINO_T}
    };

    ASSERT_EQ(tetrTStartingLocation, tetrT->getCurrentLocation());

    delete tetrT;
}

TEST(Tetromino_L_Creation, Tetromino)
{
    NewAbstractTetromino *tetrL = new TetrominoL();
    
    ASSERT_EQ(4, tetrL->getTetrominoSize());
    ASSERT_EQ(0, tetrL->getCurrentAngle());
    ASSERT_EQ(NamedColors::TETROMINO_L, tetrL->getTetrominoColor());
    ASSERT_FALSE(tetrL->getCurrentLocation().empty());
    
    int startingRow = tetrL->getStartingRow();
    int startingCol = tetrL->getStartingCol();

    std::vector<Point> tetrLStartingLocation = {
        Point{startingRow, startingCol, NamedColors::TETROMINO_L},
        Point{startingRow, startingCol + 1, NamedColors::TETROMINO_L},
        Point{startingRow, startingCol + 2, NamedColors::TETROMINO_L},
        Point{startingRow + 1, startingCol + 2, NamedColors::TETROMINO_L}
    };

    ASSERT_EQ(tetrLStartingLocation, tetrL->getCurrentLocation());

    delete tetrL;
}

TEST(Tetromino_J_Creation, Tetromino)
{
    NewAbstractTetromino *tetrJ = new TetrominoJ();
    
    ASSERT_EQ(4, tetrJ->getTetrominoSize());
    ASSERT_EQ(0, tetrJ->getCurrentAngle());
    ASSERT_EQ(NamedColors::TETROMINO_J, tetrJ->getTetrominoColor());
    ASSERT_FALSE(tetrJ->getCurrentLocation().empty());
    
    int startingRow = tetrJ->getStartingRow();
    int startingCol = tetrJ->getStartingCol();

    std::vector<Point> tetrJStartingLocation = {
        Point{startingRow + 1, startingCol, NamedColors::TETROMINO_J},
        Point{startingRow, startingCol, NamedColors::TETROMINO_J},
        Point{startingRow, startingCol + 1, NamedColors::TETROMINO_J},
        Point{startingRow, startingCol + 2, NamedColors::TETROMINO_J}
    };

    ASSERT_EQ(tetrJStartingLocation, tetrJ->getCurrentLocation());

    delete tetrJ;
}

TEST(Tetromino_O_Creation, Tetromino)
{
    NewAbstractTetromino *tetrO = new TetrominoO();
    
    ASSERT_EQ(4, tetrO->getTetrominoSize());
    ASSERT_EQ(0, tetrO->getCurrentAngle());
    ASSERT_EQ(NamedColors::TETROMINO_O, tetrO->getTetrominoColor());
    ASSERT_FALSE(tetrO->getCurrentLocation().empty());
    
    int startingRow = tetrO->getStartingRow();
    int startingCol = tetrO->getStartingCol();

    std::vector<Point> tetrOStartingLocation = {
        Point{startingRow, startingCol, NamedColors::TETROMINO_O},
        Point{startingRow, startingCol + 1, NamedColors::TETROMINO_O},
        Point{startingRow + 1, startingCol, NamedColors::TETROMINO_O},
        Point{startingRow + 1, startingCol + 1, NamedColors::TETROMINO_O}
    };

    ASSERT_EQ(tetrOStartingLocation, tetrO->getCurrentLocation());

    delete tetrO;
}

TEST(Tetromino_Z_Creation, Tetromino)
{
    NewAbstractTetromino *tetrZ = new TetrominoZ();
    
    ASSERT_EQ(4, tetrZ->getTetrominoSize());
    ASSERT_EQ(0, tetrZ->getCurrentAngle());
    ASSERT_EQ(NamedColors::TETROMINO_Z, tetrZ->getTetrominoColor());
    ASSERT_FALSE(tetrZ->getCurrentLocation().empty());
    
    int startingRow = tetrZ->getStartingRow();
    int startingCol = tetrZ->getStartingCol();

    std::vector<Point> tetrZStartingLocation = {
        Point{startingRow, startingCol, NamedColors::TETROMINO_Z},
        Point{startingRow, startingCol + 1, NamedColors::TETROMINO_Z},
        Point{startingRow + 1, startingCol + 1, NamedColors::TETROMINO_Z},
        Point{startingRow + 1, startingCol + 2, NamedColors::TETROMINO_Z}
    };

    ASSERT_EQ(tetrZStartingLocation, tetrZ->getCurrentLocation());

    delete tetrZ;
}

TEST(Tetromino_S_Creation, Tetromino)
{
    NewAbstractTetromino *tetrS = new TetrominoS();
    
    ASSERT_EQ(4, tetrS->getTetrominoSize());
    ASSERT_EQ(0, tetrS->getCurrentAngle());
    ASSERT_EQ(NamedColors::TETROMINO_S, tetrS->getTetrominoColor());
    ASSERT_FALSE(tetrS->getCurrentLocation().empty());
    
    int startingRow = tetrS->getStartingRow();
    int startingCol = tetrS->getStartingCol();

    std::vector<Point> tetrSStartingLocation = {
        Point{startingRow + 1, startingCol, NamedColors::TETROMINO_S},
        Point{startingRow + 1, startingCol + 1, NamedColors::TETROMINO_S},
        Point{startingRow, startingCol + 1, NamedColors::TETROMINO_S},
        Point{startingRow, startingCol + 2, NamedColors::TETROMINO_S}
    };

    ASSERT_EQ(tetrSStartingLocation, tetrS->getCurrentLocation());

    delete tetrS;
}
// --------------------------------------------------------------------------------------------------------------------
// Creation tests end
// --------------------------------------------------------------------------------------------------------------------


// //Not working 

// TEST(CLAPShortFunctionality, Parser)
// {
//     int argc = 4;
//     char programmName[] = "./TetrisGameMain";

//     char shortArgv1[] = "-b 15";
//     char shortArgv2[] = "-l p";
//     char shortArgv3[] = "-r k";

//     char *shortArgv[] = {programmName, shortArgv1, shortArgv2, shortArgv3};

//     Parser shortArgsParser;
//     shortArgsParser.parseArguments(argc, shortArgv);

//     ASSERT_EQ(15, shortArgsParser.getLevel());
//     ASSERT_EQ('p', shortArgsParser.getLeftRotationKey());
//     ASSERT_EQ('k', shortArgsParser.getRightRotationKey());
// }


// --------------------------------------------------------------------------------------------------------------------
// Rotation tests start
// --------------------------------------------------------------------------------------------------------------------

TEST(Tetromino_I_Rotation, Tetromino)
{  
    NewAbstractTetromino *tetrI = new TetrominoI();
    
    //RIGHT ROTATION.
    std::vector<Point> locationBeforeRotationI = tetrI->getCurrentLocation();
    //0
    int angleBeforeRotationI = tetrI->getCurrentAngle();
    
    tetrI->rotate(false);
    
    std::vector<Point> locationAfterRotationI = tetrI->getCurrentLocation();
    //90
    int angleAfterRotationI = tetrI->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotationI + 90, angleAfterRotationI);
    
    //Compare coordinates after rotation.
    ASSERT_EQ(locationBeforeRotationI[0].row - 2, locationAfterRotationI[0].row);
    ASSERT_EQ(locationBeforeRotationI[0].col + 2, locationAfterRotationI[0].col);

    ASSERT_EQ(locationBeforeRotationI[1].row - 1, locationAfterRotationI[1].row);
    ASSERT_EQ(locationBeforeRotationI[1].col + 1, locationAfterRotationI[1].col);

    ASSERT_EQ(locationBeforeRotationI[2].row, locationAfterRotationI[2].row);
    ASSERT_EQ(locationBeforeRotationI[2].col, locationAfterRotationI[2].col);

    ASSERT_EQ(locationBeforeRotationI[3].row + 1, locationAfterRotationI[3].row);
    ASSERT_EQ(locationBeforeRotationI[3].col - 1, locationAfterRotationI[3].col);

   // After two more rotation we should land at the beggining.
    tetrI->rotate(false);
    tetrI->rotate(false);

    std::vector<Point> locationAfterFullRotationCycleI =  tetrI->getCurrentLocation();
    int angleAfterFullRotationCycleI = tetrI->getCurrentAngle();
    
    ASSERT_EQ(angleBeforeRotationI, angleAfterFullRotationCycleI);

    for(int i = 0; i < tetrI->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotationI[i].row, locationAfterFullRotationCycleI[i].row);
        ASSERT_EQ(locationBeforeRotationI[i].col, locationAfterFullRotationCycleI[i].col);
    }

    //LEFT ROTATION
    //----------------------------------------------------------------------

    locationBeforeRotationI = tetrI->getCurrentLocation();
    angleBeforeRotationI = tetrI->getCurrentAngle();
    
    tetrI->rotate(true);
    
    locationAfterRotationI = tetrI->getCurrentLocation();
    
    angleAfterRotationI = tetrI->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotationI + 180, angleAfterRotationI);
    
    //Compare coordinates after rotation.
    ASSERT_EQ(locationBeforeRotationI[0].row - 1, locationAfterRotationI[0].row);
    ASSERT_EQ(locationBeforeRotationI[0].col + 1, locationAfterRotationI[0].col);

    ASSERT_EQ(locationBeforeRotationI[1].row - 1, locationAfterRotationI[1].row);
    ASSERT_EQ(locationBeforeRotationI[1].col + 1, locationAfterRotationI[1].col);

    ASSERT_EQ(locationBeforeRotationI[2].row, locationAfterRotationI[2].row);
    ASSERT_EQ(locationBeforeRotationI[2].col - 1, locationAfterRotationI[2].col);

    ASSERT_EQ(locationBeforeRotationI[3].row, locationAfterRotationI[3].row);
    ASSERT_EQ(locationBeforeRotationI[3].col - 1, locationAfterRotationI[3].col);

    //After two more rotation we should land at the beggining.
    tetrI->rotate(true);
    tetrI->rotate(true);

    locationAfterFullRotationCycleI =  tetrI->getCurrentLocation();
    angleAfterFullRotationCycleI = tetrI->getCurrentAngle();
    
    ASSERT_EQ(angleBeforeRotationI, angleAfterFullRotationCycleI);

    for(int i = 0; i < tetrI->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotationI[i].row, locationAfterFullRotationCycleI[i].row);
        ASSERT_EQ(locationBeforeRotationI[i].col, locationAfterFullRotationCycleI[i].col);
    }


   delete tetrI;
}

TEST(Tetromino_T_Rotation, Tetromino)
{    
    NewAbstractTetromino *tetrT = new TetrominoT();

    //RIGHT ROTATION
    //----------------------------------------------------------------------

    //Get values before 1 right rotation.
    std::vector<Point> locationBeforeRotation = tetrT->getCurrentLocation();
    
    int angleBeforeRotation = tetrT->getCurrentAngle();
    
    tetrT->rotate(false);
    //Get values after 1 right rotation.
    std::vector<Point> locationAfterRotation = tetrT->getCurrentLocation();
    int angleAfterRotation = tetrT->getCurrentAngle();

    //Compare angles and locations after 1 right rotation.

    ASSERT_EQ(angleBeforeRotation + 90, angleAfterRotation);

    ASSERT_EQ(locationBeforeRotation[0].row - 1, locationAfterRotation[0].row);
    ASSERT_EQ(locationBeforeRotation[0].col + 1, locationAfterRotation[0].col);

    ASSERT_EQ(locationBeforeRotation[1].row, locationAfterRotation[1].row);
    ASSERT_EQ(locationBeforeRotation[1].col, locationAfterRotation[1].col);
    
    ASSERT_EQ(locationBeforeRotation[2].row - 1, locationAfterRotation[2].row);
    ASSERT_EQ(locationBeforeRotation[2].col - 1, locationAfterRotation[2].col);
    
    ASSERT_EQ(locationBeforeRotation[3].row + 1, locationAfterRotation[3].row);
    ASSERT_EQ(locationBeforeRotation[3].col - 1, locationAfterRotation[3].col);

    //Perform 3 right rotations.
    tetrT->rotate(false);
    tetrT->rotate(false);
    tetrT->rotate(false);

    //After 4 rotations we should have the same coordinates as before the rotations.
    std::vector<Point> locationAfterFullRotationCycle =  tetrT->getCurrentLocation();
    int angleAfterFullRotationCycle = tetrT->getCurrentAngle();
    
    ASSERT_EQ(angleBeforeRotation, angleAfterFullRotationCycle);

    for(int i = 0; i < tetrT->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotation[i].row, locationAfterFullRotationCycle[i].row);
        ASSERT_EQ(locationBeforeRotation[i].col, locationAfterFullRotationCycle[i].col);
    }

    //LEFT ROTATION
    //----------------------------------------------------------------------

    //Get values before 1 left rotation.
    locationBeforeRotation = tetrT->getCurrentLocation();
    angleBeforeRotation = tetrT->getCurrentAngle();

    tetrT->rotate(true);

    //Get values after 1 left rotation.
    locationAfterRotation = tetrT->getCurrentLocation();
    angleAfterRotation = tetrT->getCurrentAngle();

    //Compare angles. Because left rotation corresponds to counter-clockwise rotation
    //our current angle should be 270.
    ASSERT_EQ(angleBeforeRotation+270, angleAfterRotation);

    ASSERT_EQ(locationBeforeRotation[0].row + 1, locationAfterRotation[0].row);
    ASSERT_EQ(locationBeforeRotation[0].col + 1, locationAfterRotation[0].col);

    ASSERT_EQ(locationBeforeRotation[1].row, locationAfterRotation[1].row);
    ASSERT_EQ(locationBeforeRotation[1].col, locationAfterRotation[1].col);
    
    ASSERT_EQ(locationBeforeRotation[2].row - 1, locationAfterRotation[2].row);
    ASSERT_EQ(locationBeforeRotation[2].col + 1, locationAfterRotation[2].col);
    
    ASSERT_EQ(locationBeforeRotation[3].row - 1, locationAfterRotation[3].row);
    ASSERT_EQ(locationBeforeRotation[3].col - 1, locationAfterRotation[3].col);

    //Perform 3 left rotations.
    tetrT->rotate(true);    
    tetrT->rotate(true);    
    tetrT->rotate(true);    

    //After 4 rotations we should have the same coordinates as before the rotations.
    locationAfterFullRotationCycle =  tetrT->getCurrentLocation();
    angleAfterFullRotationCycle = tetrT->getCurrentAngle();
    
    ASSERT_EQ(angleBeforeRotation, angleAfterFullRotationCycle);

    for(int i = 0; i < tetrT->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotation[i].row, locationAfterFullRotationCycle[i].row);
        ASSERT_EQ(locationBeforeRotation[i].col, locationAfterFullRotationCycle[i].col);
    }

    delete tetrT;
}


TEST(Tetromino_L_Rotation, Tetromino)
{
    NewAbstractTetromino *tetrL = new TetrominoL();

    std::vector<Point> locationBeforeRotation = tetrL->getCurrentLocation();
    int angleBeforeRotation = tetrL->getCurrentAngle();

    tetrL->rotate(false);

    std::vector<Point> locationAfterRotation = tetrL->getCurrentLocation();
    int angleAfterRotation = tetrL->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation + 90, angleAfterRotation);

    ASSERT_EQ(locationBeforeRotation[0].row - 1, locationAfterRotation[0].row);
    ASSERT_EQ(locationBeforeRotation[0].col + 1, locationAfterRotation[0].col);

    ASSERT_EQ(locationBeforeRotation[1].row, locationAfterRotation[1].row);
    ASSERT_EQ(locationBeforeRotation[1].col, locationAfterRotation[1].col);

    ASSERT_EQ(locationBeforeRotation[2].row + 1, locationAfterRotation[2].row);
    ASSERT_EQ(locationBeforeRotation[2].col - 1, locationAfterRotation[2].col);

    ASSERT_EQ(locationBeforeRotation[3].row, locationAfterRotation[3].row);
    ASSERT_EQ(locationBeforeRotation[3].col - 2, locationAfterRotation[3].col);
    
    //Perform 3 right rotations.
    tetrL->rotate(false);
    tetrL->rotate(false);
    tetrL->rotate(false);
    
    //After 4 rotations we should have the same coordinates as before the rotations.
    std::vector<Point> locationAfterFullRotationCycle = tetrL->getCurrentLocation();
    int angleAfterFullRotationCycle = tetrL->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation, angleAfterFullRotationCycle);

    for(int i = 0; i < tetrL->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotation[i].row, locationAfterFullRotationCycle[i].row);
        ASSERT_EQ(locationBeforeRotation[i].col, locationAfterFullRotationCycle[i].col);
    }

    //LEFT ROTATION
    //----------------------------------------------------------------------

    locationBeforeRotation = tetrL->getCurrentLocation();
    angleBeforeRotation = tetrL->getCurrentAngle();

    tetrL->rotate(true);    

    locationAfterRotation = tetrL->getCurrentLocation();
    angleAfterRotation = tetrL->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation + 270, angleAfterRotation);


    ASSERT_EQ(locationBeforeRotation[0].row + 1, locationAfterRotation[0].row);
    ASSERT_EQ(locationBeforeRotation[0].col + 1, locationAfterRotation[0].col);

    ASSERT_EQ(locationBeforeRotation[1].row, locationAfterRotation[1].row);
    ASSERT_EQ(locationBeforeRotation[1].col, locationAfterRotation[1].col);

    ASSERT_EQ(locationBeforeRotation[2].row - 1, locationAfterRotation[2].row);
    ASSERT_EQ(locationBeforeRotation[2].col - 1, locationAfterRotation[2].col);

    ASSERT_EQ(locationBeforeRotation[3].row - 2, locationAfterRotation[3].row);
    ASSERT_EQ(locationBeforeRotation[3].col, locationAfterRotation[3].col);
    
    tetrL->rotate(true);
    tetrL->rotate(true);
    tetrL->rotate(true);
    
    locationAfterFullRotationCycle = tetrL->getCurrentLocation();
    angleAfterFullRotationCycle = tetrL->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation, angleAfterFullRotationCycle);

    for(int i = 0; i < tetrL->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotation[i].row, locationAfterFullRotationCycle[i].row);
        ASSERT_EQ(locationBeforeRotation[i].col, locationAfterFullRotationCycle[i].col);
    }

    delete tetrL;   
}

TEST(Tetromino_O_Rotation, Tetromino)
{
    NewAbstractTetromino *tetrO = new TetrominoO();
    std::vector<Point> locationBeforeRotation = tetrO->getCurrentLocation();

    tetrO->rotate(false);
    tetrO->rotate(true);

    // Since cube doesn't acually rotate we don't need to test anything else.

    std::vector<Point> locationAfterRotation = tetrO->getCurrentLocation();

    ASSERT_EQ(locationBeforeRotation, locationAfterRotation);
    
    delete tetrO;
}

TEST(Tetromino_S_Rotation, Tetromino)
{
    NewAbstractTetromino *tetrS = new TetrominoS();
    
    
    std::vector<Point> locationBeforeRotation = tetrS->getCurrentLocation();
    int angleBeforeRotation = tetrS->getCurrentAngle();

    tetrS->rotate(false);

    std::vector<Point> locationAfterRotation = tetrS->getCurrentLocation();
    int angleAfterRotation = tetrS->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation + 90, angleAfterRotation);

    ASSERT_EQ(locationBeforeRotation[0].row - 2, locationAfterRotation[0].row);
    ASSERT_EQ(locationBeforeRotation[0].col + 1, locationAfterRotation[0].col);

    ASSERT_EQ(locationBeforeRotation[1].row - 1, locationAfterRotation[1].row);
    ASSERT_EQ(locationBeforeRotation[1].col, locationAfterRotation[1].col);

    ASSERT_EQ(locationBeforeRotation[2].row, locationAfterRotation[2].row);
    ASSERT_EQ(locationBeforeRotation[2].col+1, locationAfterRotation[2].col);

    ASSERT_EQ(locationBeforeRotation[3].row + 1, locationAfterRotation[3].row);
    ASSERT_EQ(locationBeforeRotation[3].col, locationAfterRotation[3].col);
    
    //Perform 1 right rotations.
    tetrS->rotate(false);
    
    //After 2 rotations we should have the same coordinates as before the rotations.
    std::vector<Point> locationAfterFullRotationCycle = tetrS->getCurrentLocation();
    int angleAfterFullRotationCycle = tetrS->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation, angleAfterFullRotationCycle);

    for(int i = 0; i < tetrS->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotation[i].row, locationAfterFullRotationCycle[i].row);
        ASSERT_EQ(locationBeforeRotation[i].col, locationAfterFullRotationCycle[i].col);
    }

    // Left rotation is the same as the right rotation.

    delete tetrS;
}

TEST(Tetromino_Z_Rotation, Tetromino)
{
    NewAbstractTetromino *tetrZ = new TetrominoZ();
    
    
    std::vector<Point> locationBeforeRotation = tetrZ->getCurrentLocation();
    int angleBeforeRotation = tetrZ->getCurrentAngle();

    tetrZ->rotate(false);

    std::vector<Point> locationAfterRotation = tetrZ->getCurrentLocation();
    int angleAfterRotation = tetrZ->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation + 90, angleAfterRotation);

    ASSERT_EQ(locationBeforeRotation[0].row - 1, locationAfterRotation[0].row);
    ASSERT_EQ(locationBeforeRotation[0].col + 2, locationAfterRotation[0].col);

    ASSERT_EQ(locationBeforeRotation[1].row, locationAfterRotation[1].row);
    ASSERT_EQ(locationBeforeRotation[1].col + 1, locationAfterRotation[1].col);

    ASSERT_EQ(locationBeforeRotation[2].row - 1, locationAfterRotation[2].row);
    ASSERT_EQ(locationBeforeRotation[2].col, locationAfterRotation[2].col);

    ASSERT_EQ(locationBeforeRotation[3].row, locationAfterRotation[3].row);
    ASSERT_EQ(locationBeforeRotation[3].col - 1, locationAfterRotation[3].col);
    
    //Perform 1 right rotations.
    tetrZ->rotate(false);
    
    //After 2 rotations we should have the same coordinates as before the rotations.
    std::vector<Point> locationAfterFullRotationCycle = tetrZ->getCurrentLocation();
    int angleAfterFullRotationCycle = tetrZ->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation, angleAfterFullRotationCycle);

    for(int i = 0; i < tetrZ->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotation[i].row, locationAfterFullRotationCycle[i].row);
        ASSERT_EQ(locationBeforeRotation[i].col, locationAfterFullRotationCycle[i].col);
    }

    // Left rotation is the same as the right rotation.

    delete tetrZ;
}

TEST(Tetromino_J_Rotation, Tetromino)
{
 NewAbstractTetromino *tetrJ = new TetrominoJ();

    std::vector<Point> locationBeforeRotation = tetrJ->getCurrentLocation();
    int angleBeforeRotation = tetrJ->getCurrentAngle();

    tetrJ->rotate(false);

    std::vector<Point> locationAfterRotation = tetrJ->getCurrentLocation();
    int angleAfterRotation = tetrJ->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation + 90, angleAfterRotation);

    ASSERT_EQ(locationBeforeRotation[0].row - 2, locationAfterRotation[0].row);
    ASSERT_EQ(locationBeforeRotation[0].col, locationAfterRotation[0].col);

    ASSERT_EQ(locationBeforeRotation[1].row - 1, locationAfterRotation[1].row);
    ASSERT_EQ(locationBeforeRotation[1].col + 1, locationAfterRotation[1].col);

    ASSERT_EQ(locationBeforeRotation[2].row, locationAfterRotation[2].row);
    ASSERT_EQ(locationBeforeRotation[2].col, locationAfterRotation[2].col);

    ASSERT_EQ(locationBeforeRotation[3].row + 1, locationAfterRotation[3].row);
    ASSERT_EQ(locationBeforeRotation[3].col - 1, locationAfterRotation[3].col);
    
    //Perform 3 right rotations.
    tetrJ->rotate(false);
    tetrJ->rotate(false);
    tetrJ->rotate(false);
    
    //After 4 rotations we should have the same coordinates as before the rotations.
    std::vector<Point> locationAfterFullRotationCycle = tetrJ->getCurrentLocation();
    int angleAfterFullRotationCycle = tetrJ->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation, angleAfterFullRotationCycle);

    for(int i = 0; i < tetrJ->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotation[i].row, locationAfterFullRotationCycle[i].row);
        ASSERT_EQ(locationBeforeRotation[i].col, locationAfterFullRotationCycle[i].col);
    }

    //LEFT ROTATION
    //----------------------------------------------------------------------

    locationBeforeRotation = tetrJ->getCurrentLocation();
    angleBeforeRotation = tetrJ->getCurrentAngle();

    tetrJ->rotate(true);    

    locationAfterRotation = tetrJ->getCurrentLocation();
    angleAfterRotation = tetrJ->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation + 270, angleAfterRotation);


    ASSERT_EQ(locationBeforeRotation[0].row, locationAfterRotation[0].row);
    ASSERT_EQ(locationBeforeRotation[0].col + 2, locationAfterRotation[0].col);

    ASSERT_EQ(locationBeforeRotation[1].row + 1, locationAfterRotation[1].row);
    ASSERT_EQ(locationBeforeRotation[1].col + 1, locationAfterRotation[1].col);

    ASSERT_EQ(locationBeforeRotation[2].row, locationAfterRotation[2].row);
    ASSERT_EQ(locationBeforeRotation[2].col, locationAfterRotation[2].col);

    ASSERT_EQ(locationBeforeRotation[3].row - 1, locationAfterRotation[3].row);
    ASSERT_EQ(locationBeforeRotation[3].col - 1, locationAfterRotation[3].col);
    
    tetrJ->rotate(true);
    tetrJ->rotate(true);
    tetrJ->rotate(true);
    
    locationAfterFullRotationCycle = tetrJ->getCurrentLocation();
    angleAfterFullRotationCycle = tetrJ->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation, angleAfterFullRotationCycle);

    for(int i = 0; i < tetrJ->getTetrominoSize(); i++)
    {
        ASSERT_EQ(locationBeforeRotation[i].row, locationAfterFullRotationCycle[i].row);
        ASSERT_EQ(locationBeforeRotation[i].col, locationAfterFullRotationCycle[i].col);
    }

    delete tetrJ;      
}

// --------------------------------------------------------------------------------------------------------------------
// Rotation tests end
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// MockTetrisGame tests start
// --------------------------------------------------------------------------------------------------------------------

TEST(MockTetrisGameSimpleMovement, MockTetrisGame)
{
    int level = 5;
    char rrk = 'x';
    char lrk = 'z';
    
    UserInput moveDown;
    moveDown.keycode_ = 258;

    UserInput moveRight;
    moveRight.keycode_ = 261;

    UserInput moveLeft;
    moveLeft.keycode_ = 260;

    UserInput rotateLeft;
    rotateLeft.keycode_ = lrk;

    UserInput rotateRight;
    rotateRight.keycode_ = rrk;


    MockTetrisGame mtg(level, rrk, lrk);

    ASSERT_EQ(5, mtg.currentLevel);
    ASSERT_EQ('x', mtg.rightRotationKey);
    ASSERT_EQ('z', mtg.leftRotationKey);
    
    // Our current speed should be equal to the falling speed from the
    // fifth level. (383 ms)
    ASSERT_EQ(mtg.currentSpeed, mtg.fallingSpeed[mtg.currentLevel]);
    
    ASSERT_FALSE(mtg.isGameOver);

    ASSERT_TRUE(mtg.deque.empty());
    
    ASSERT_FALSE(mtg.gameField.empty());
    ASSERT_FALSE(mtg.surface.empty());
    ASSERT_FALSE(mtg.fallingSpeed.empty());
    ASSERT_FALSE(mtg.statistics.empty());
    ASSERT_FALSE(mtg.pointsForRemovedRows.empty());
    
    
    // Initially all points should have false in gameField
    for(int i = mtg.offset_row; i < mtg.offset_row + mtg.rows_; i++)
    {
        for(int j = mtg.offset_col; j < mtg.offset_col + mtg.cols_; j++)
        {
            bool isAlive = mtg.gameField[Point{i, j, NamedColors::BLACK}];
            ASSERT_FALSE(isAlive);
        }
    }

    // Now we can create tetrominos and test game logic.

    // Generate some random numbers
    mtg.generateCurrentAndNext();
    
    // Check if our random numbers are different.
    ASSERT_FALSE(mtg.currentRandomNumber == mtg.nextRandomNumber);
    
    // Check if they are in the correct range.
    ASSERT_TRUE(mtg.currentRandomNumber >= 0 && mtg.currentRandomNumber <= 6);
    ASSERT_TRUE(mtg.nextRandomNumber >= 0 && mtg.nextRandomNumber <= 6);
    
    // Generate tetromino
    mtg.currentTetromino = mtg.chooseTetromino(mtg.currentRandomNumber);

    // Check if our tetromino was created succsessfully
    ASSERT_TRUE(mtg.currentTetromino != nullptr);

    // Assign "down keycode" to emulate moving down.
    // After this our tetromino should be 4 rows below it's starting position.
    // Nothing should collide.
    // Tetromino shouldn't be placed yet.
    std::vector<Point> positionBeforeMoving = mtg.currentTetromino->getCurrentLocation();
    
    mtg.decideAction(moveDown, false);
    mtg.decideAction(moveDown, false);
    mtg.decideAction(moveDown, false);
    mtg.decideAction(moveDown, false);

    std::vector<Point> positionAfterMoving = mtg.currentTetromino->getCurrentLocation();

    for(int i = 0; i < mtg.currentTetromino->getTetrominoSize(); i++)
    {
        ASSERT_EQ(positionBeforeMoving[i].row + 4, positionAfterMoving[i].row);
        ASSERT_EQ(positionBeforeMoving[i].col, positionAfterMoving[i].col);
    }
    

    // Emulate movement

    positionBeforeMoving = mtg.currentTetromino->getCurrentLocation();

    mtg.decideAction(moveLeft, false);
    mtg.decideAction(moveDown, false);
    mtg.decideAction(moveRight, false);
    mtg.decideAction(moveRight, false);
    mtg.decideAction(moveDown, false);
    
    positionAfterMoving = mtg.currentTetromino->getCurrentLocation();

    // Check new coordinates

    for(int i = 0; i < mtg.currentTetromino->getTetrominoSize(); i++)
    {
        ASSERT_EQ(positionBeforeMoving[i].row + 2, positionAfterMoving[i].row);
        ASSERT_EQ(positionBeforeMoving[i].col + 1, positionAfterMoving[i].col);
    }
    
    // Emulate rotation.
    
    // If everything is correct we should land at the same position as before the rotations.
    std::vector<Point> locationBeforeRotation = mtg.currentTetromino->getCurrentLocation();
    int angleBeforeRotation = mtg.currentTetromino->getCurrentAngle();

    mtg.decideAction(rotateLeft, false);
    mtg.decideAction(rotateLeft, false);
    mtg.decideAction(rotateLeft, false);
    mtg.decideAction(rotateLeft, false);

    mtg.decideAction(rotateRight, false);
    mtg.decideAction(rotateRight, false);
    mtg.decideAction(rotateRight, false);
    mtg.decideAction(rotateRight, false);
    
    std::vector<Point> locationAfterRotation = mtg.currentTetromino->getCurrentLocation();
    int angleAfterRotation = mtg.currentTetromino->getCurrentAngle();

    ASSERT_EQ(angleBeforeRotation, angleAfterRotation);
    ASSERT_EQ(locationBeforeRotation, locationAfterRotation);

    // Delete tetromino to avoid memory leaks.
    delete mtg.currentTetromino;
}   

TEST(MockTetrisGamePlacement, MockTetrisGame)
{
    MockTetrisGame mtg(0, 's', 'a');
    UserInput moveDown;
    moveDown.keycode_ = 258;

    mtg.currentTetromino = new TetrominoI();
    //mtg.currentTetromino->rotate(false);

    // Move down until collide
    while(!mtg.isCurrentTetrominoPlaced)
    {
        mtg.decideAction(moveDown, false);
    }

    // By this point we should be colliding with surface.
    ASSERT_EQ(Collision::Surface, mtg.lastCollision);
    
    // Points that form our currentTetromino should be gameField and surface.
    for(auto point : mtg.currentTetromino->getCurrentLocation())
    {
        bool value = mtg.gameField[point];
        auto it = std::find(mtg.surface.begin(), mtg.surface.end(), point);

        ASSERT_TRUE(value);
        ASSERT_TRUE(it != mtg.surface.end());
    }

    // Check score
    ASSERT_EQ(19, mtg.currentPoints);

    delete mtg.currentTetromino;
}

TEST(MockTetrisGameIsGameOver, MockTetrisGame)
{
    MockTetrisGame mtg(0, 's', 'a');
    
    UserInput moveDown;
    moveDown.keycode_ = 258;
    
    mtg.currentTetromino = new TetrominoJ();
    // Place some blocks.

    for(int j = mtg.offset_col + 2; j < mtg.offset_col + mtg.cols_ - 2; j++)
    {
        mtg.gameField[Point{17, j, NamedColors::TETROMINO_I}] = true;
        mtg.surface.insert(Point{17, j, NamedColors::TETROMINO_I});

    }

    // Now the game should end because our current tetromino will connect with
    // other tetromino and at the same time it will be touching the "roof".

    mtg.decideAction(moveDown, false);
    ASSERT_TRUE(mtg.isGameOver);

    delete mtg.currentTetromino;
}
// --------------------------------------------------------------------------------------------------------------------
// MockTetrisGame tests end
// --------------------------------------------------------------------------------------------------------------------
