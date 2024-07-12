#include "./AbstractTetromino.h"
#include <cmath>

void NewAbstractTetromino::moveLeft()
{
    for(Point &point : currentLocation_)
    {
        point.col -= 1;
    }
}

void NewAbstractTetromino::moveRight()
{
    for(Point &point : currentLocation_)
    {
        point.col += 1;
    }
}

void NewAbstractTetromino::moveDown()
{
    for(Point &point : currentLocation_)
    {
        point.row += 1;
    }
}

void NewAbstractTetromino::moveUp()
{
    for(Point &point : currentLocation_)
    {
        point.row -= 1;
    }
}

void NewAbstractTetromino::rotate(bool left)
{
    // Remember to shift tetromino 
    Point center = currentLocation_[centerIndex];
    int negate;
  
    if(left)
    {
        negate = -1;
    }
    else
    {
        negate = 1;
    }
    
    for(Point &point : currentLocation_)
    { 
        int translatedCol = point.col - center.col;
        int translatedRow = point.row - center.row;
        
        int rotatedCol = (int)(center.col + translatedCol * cos(rotation_angle * negate) - translatedRow * sin(rotation_angle * negate));
        int rotatedRow = (int)(center.row + translatedCol * sin(rotation_angle * negate) + translatedRow * cos(rotation_angle * negate));
        point.row = rotatedRow;
        point.col = rotatedCol;
    }

    
    currentAngle_ = (currentAngle_ + (90 * negate)) % 360;
    if( currentAngle_ < 0)
    {
        currentAngle_ += 360;
    }
    
    // if(currentAngle_  == -360 || currentAngle_ == 360)
    // {
    //     currentAngle_ = 0;
    // }
}   