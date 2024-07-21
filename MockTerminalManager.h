// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

// Currently empty, but will be added later

#pragma once
#include <unordered_map>
#include "./AbstractTerminalManager.h"
#include "./Point.h"

// Mostly the same as in the previous exercise.
class MockTerminalManager : public AbstractTerminalManager{
    public:
        MockTerminalManager(int numRows, int numCols);
        ~MockTerminalManager() = default;

        void drawPixel(int row, int col, int color) override;
        void refresh() override;
        int numRows() const override {return numRows_;}
        int numCols() const override {return numCols_;}

        bool isPixelDrawn(int row, int col) const;
        std::unordered_map<Point, int> getDrawnPixels() const {return drawnPixels_;}

    private:
        // Point & color
        std::unordered_map<Point, int> drawnPixels_;
        int numCols_;
        int numRows_;
};