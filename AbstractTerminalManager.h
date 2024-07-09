// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once

// An abstract base class that defines an interface for drawing pixels
// on a screen.
class AbstractTerminalManager {
public:
  // Virtual destructor.
  virtual ~AbstractTerminalManager() = default;

  // Draw a "pixel" at the given position and with the given intensity between
  // The intensity has to be in [0.0, 1.0]
  virtual void drawPixel(int row, int col, int color) = 0;

  // Refresh the screen.
  virtual void refresh() = 0;

  // Get the dimensions of the screen.
  virtual int numRows() const = 0;
  virtual int numCols() const = 0;
};
