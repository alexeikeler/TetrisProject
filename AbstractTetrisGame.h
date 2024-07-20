#pragma once

#include "./AbstractTetromino.h"
#include "./TerminalManager.h"
#include "./Point.h"
#include <vector>
#include <unordered_map>
#include <deque>
#include <set>
#include <functional>


enum class Collision {Roof, Wall, Block, Nothing, Surface, Floor, GameOver};


class AbstractTetrisGame{
    public:
        virtual ~AbstractTetrisGame() = default;

        // Methods for updating data.
        void updateLevelAndSpeed(int increaseLevelBy=0);
        void updateDestroyedLines();
        void updateStatistics();

        // Main game loop.
        void play();
        
        // Game logic.
        void decideAction(UserInput userInput, bool isAritificialMovement);
        void placeTetromino();
        Collision isColliding(bool downPressed, bool leftRotaion, bool rightRotation, std::vector<Point> previousLocation);
        void updateSurface();
        void reshapeGameField();
        NewAbstractTetromino* chooseTetromino(const int randomNumber);
        
        // Calculations.
        void generateRandomNumber(int a, int b);
        void generateCurrentAndNext(int a = 0, int b = 6);
        std::string intToString(int number, int maxLength);

    private:

        NewAbstractTetromino *currentTetromino;
        
        const int numberOfTetrominos = 7;
        const int rows_ = 21;
        const int cols_ = 11;
        const int offset_row = 14;
        const int offset_col = 40;
        const int maxLevel = 29;
        const int gameOverTimeoutMs = 1'500;

        int currentSpeed;
        
        int previousRandomNumber;
        int currentRandomNumber;
        int nextRandomNumber;
        
        char leftRotationKey;
        char rightRotationKey;

        int previousQuotient;
        int destroyedLines;
        int currentLevel;

        int earnedPoints = 0;
        int currentPoints = 0;

        std::deque<int> deque;
        std::unordered_map<Point, bool> gameField;
        std::set<Point> surface;
        std::unordered_map<int, int> fallingSpeed = {
                {0, 800},
                {1, 716},
                {2, 633},
                {3, 550},
                {4, 466},
                {5, 383},
                {6, 300},
                {7, 216},
                {8, 133},
                {9, 100},
                {10, 83},
                {11, 83},
                {12, 83},
                {13, 66},
                {14, 66},
                {15, 66},
                {16, 50},
                {17, 50},
                {18, 50},
                {19, 33},
                {20, 33},
                {21, 33},
                {22, 33},
                {23, 33},
                {24, 33},
                {25, 33},
                {26, 33},
                {27, 33},
                {28, 33},
                {29, 16},
        };

    std::unordered_map<int, int> statistics = {
        {0, 0},
        {1, 0},
        {2, 0},
        {3, 0},
        {4, 0},
        {5, 0},
        {6, 0}
    };

  std::unordered_map<int, int> pointsForRemovedRows = {
    {1, 40},
    {2, 100},
    {3, 300},
    {4, 1200}
  };

};  