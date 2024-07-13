// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#include "./Tetromino.h"
#include "./TerminalManager.h"
#include <iostream>
#include <string>

// ------------------------------------------------------------------------
TetrominoT::TetrominoT()
{
  color_ = NamedColors::TETROMINO_T;
  startRow_ = 15;
  startCol_ = 45;
  
  currentLocation_.push_back(Point{startRow_, startCol_, color_});
  currentLocation_.push_back(Point{startRow_, startCol_ + 1, color_});
  currentLocation_.push_back(Point{startRow_ + 1, startCol_ + 1, color_});
  currentLocation_.push_back(Point{startRow_, startCol_ + 2, color_ });
  centerIndex = 1;

}
// ------------------------------------------------------------------------
TetrominoL::TetrominoL()
{
  color_ = NamedColors::TETROMINO_L;
  startRow_ = 15;
  startCol_ = 45;
  
  currentLocation_.push_back(Point{startRow_, startCol_, color_});
  currentLocation_.push_back(Point{startRow_, startCol_ + 1, color_});
  currentLocation_.push_back(Point{startRow_, startCol_ + 2, color_});
  currentLocation_.push_back(Point{startRow_ + 1, startCol_ + 2, color_});
  
  centerIndex = 1;

}
// ------------------------------------------------------------------------
TetrominoJ::TetrominoJ()
{
  color_ = NamedColors::TETROMINO_J;
  startRow_ = 15;
  startCol_ = 45;
  
  currentLocation_.push_back(Point{startRow_ + 1, startCol_, color_});
  currentLocation_.push_back(Point{startRow_, startCol_, color_});
  currentLocation_.push_back(Point{startRow_, startCol_+1, color_});
  currentLocation_.push_back(Point{startRow_, startCol_+2, color_});
    
  centerIndex = 2;
}
// ------------------------------------------------------------------------
TetrominoO::TetrominoO()
{
  color_ = NamedColors::TETROMINO_O;
  startRow_ = 15;
  startCol_ = 45;

  currentLocation_.push_back(Point{startRow_, startCol_, color_});
  currentLocation_.push_back(Point{startRow_, startCol_+1, color_});
  currentLocation_.push_back(Point{startRow_+1, startCol_, color_});
  currentLocation_.push_back(Point{startRow_+1, startCol_+1, color_});
  
  // We don't need to rotate the cube, hence we don't need centerIndex
}

// We don't nned to rotate the cube
void TetrominoO::rotate([[maybe_unused]] bool left)
{
  return;
}
// ------------------------------------------------------------------------

TetrominoI::TetrominoI()
{
  color_ = NamedColors::TETROMINO_I;
  startRow_ = 15;
  startCol_ = 44;
  
  currentLocation_.push_back(Point{startRow_, startCol_, color_});
  currentLocation_.push_back(Point{startRow_, startCol_+1, color_});
  currentLocation_.push_back(Point{startRow_, startCol_+2, color_});
  currentLocation_.push_back(Point{startRow_, startCol_+3, color_});

  centerIndex = 2;

}

// We need to modify standart rotation to be able to fold and unfold
// this tetromino.
void TetrominoI::rotate(bool left)
{
  // I to cube
  if(currentAngle_ == 90 && !left)
  {
    currentLocation_[0].col -= 1;
    currentLocation_[0].row += 1;
    currentLocation_[2].col -= 1;
    currentLocation_[3].row -= 1;
    currentAngle_ += 90;
  }
  else if(currentAngle_ == 180 && !left)
  {

    // cube to ____
    currentLocation_[2].col += 2;
    currentLocation_[2].row -= 1;
    currentLocation_[3].col += 2;
    currentLocation_[3].row -= 1;

    // now we have form x1 x2 x3 x4
    // but because of the previouse rotations
    // it's shifted to the right by 1, so
    // we need to shift current tetromino coordinates to the left (means xi.col
    // -= 1) and move it down by 1 row (means xi.row += 1) to come back to the
    // initial coordinates by xi I mean currentLocation_[i]

    for (int i = 0; i < size_; i++) {
      currentLocation_[i].col -= 1;
      currentLocation_[i].row += 1;
    }
    currentAngle_ = 0;
  }
  else if(currentAngle_ == 0 && left)
  {

    //   // From ---- to cube
    currentLocation_[0].row -= 1;
    currentLocation_[0].col += 1;

    currentLocation_[1].row -= 1;
    currentLocation_[1].col += 1;

    currentLocation_[2].col -= 1;

    currentLocation_[3].col -= 1;

    currentAngle_  = 180;
  }
  else if(currentAngle_ == 180 && left)
  {
    // From cube to I
    currentLocation_[0].col += 1;
    currentLocation_[0].row -= 1;
    currentLocation_[2].col += 1;
    currentLocation_[3].row += 1;
    currentAngle_ -= 90;
  }
  else
  {
    NewAbstractTetromino::rotate(left);
  }
}
// ------------------------------------------------------------------------

TetrominoZ::TetrominoZ()
{
  color_ = NamedColors::TETROMINO_Z;
  startRow_ = 15;
  startCol_ = 45;

  currentLocation_.push_back(Point{startRow_, startCol_, color_});
  currentLocation_.push_back(Point{startRow_, startCol_+1, color_});
  currentLocation_.push_back(Point{startRow_+1, startCol_+1, color_});
  currentLocation_.push_back(Point{startRow_+1, startCol_+2, color_});

  centerIndex = 2;
  
}

void TetrominoZ::rotate(bool left)
{
  return;
}

// ------------------------------------------------------------------------
