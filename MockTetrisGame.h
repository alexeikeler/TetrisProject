// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once

#include "./AbstractTetrisGame.h"

class MockTetrisGame : public AbstractTetrisGame{
    public:
        // Instead of writing tons of getters I've decided to use friend test.
        friend class MockTetrisGameTests_MockTetrisGame_Test;

        MockTetrisGame(int level, char rrk, char lrk);
        ~MockTetrisGame() {};
        void play() override;
        void gameOver() override;
        void decideAction(UserInput userInput, bool isAritificialMovement) override;
        void placeTetromino() override;
        void reshapeGameField() override;





    private:
        bool isGameOver = false;

};