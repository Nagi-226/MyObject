#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "board.h"
#include "ai.h"

enum class PlayerType {
    HUMAN,
    AI
};

enum class PlayerColor {
    BLACK,
    WHITE
};

enum class GameDifficulty {
    EASY,
    MEDIUM,
    HARD
};

class Game {
private:
    Board board;
    AI ai;
    PlayerType currentPlayerType;
    PlayerColor currentPlayerColor;
    PlayerColor humanPlayerColor;
    GameDifficulty difficulty;
    bool gameOver;
    PlayerColor winner;

    void displayMenu();
    void displayDifficultyMenu();
    void displayColorMenu();
    void initGame();
    void switchPlayer();
    void playerMove();
    void aiMove();
    void checkGameOver();
    void displayResult();

public:
    Game();
    void start();
    void playTurn();
    void restart();
};

#endif // GAME_H