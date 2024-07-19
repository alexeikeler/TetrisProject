// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used


#include "./MockTerminalManager.h"
#include "./Point.h"


MockTerminalManager::MockTerminalManager(int numRows, int numCols)
{
    numRows_ = numRows;
    numCols_ = numCols;
}

void MockTerminalManager::drawPixel(int row, int col, int color)
{
    drawnPixels_[Point{row, col, NamedColors::BLACK}] = color;
}

void MockTerminalManager::refresh()
{
    return;
}

bool MockTerminalManager::isPixelDrawn(int row, int col) const
{
    return drawnPixels_.count(Point{row, col, NamedColors::BLACK});
}