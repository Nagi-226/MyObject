#include "game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

Game::Game() {
    initGame();
}

void Game::initGame() {
    board.init();
    gameOver = false;
    winner = PlayerColor::BLACK; // 默认值，实际获胜者会在游戏结束时确定
    currentPlayerColor = PlayerColor::BLACK; // 黑棋先行
}

void Game::start() {
    system("cls");
    std::cout << "欢迎来到五子棋游戏！" << std::endl;
    
    displayMenu();
    displayDifficultyMenu();
    displayColorMenu();
    
    system("cls");
    board.display();
    
    while (!gameOver) {
        playTurn();
        checkGameOver();
        if (!gameOver) {
            switchPlayer();
        }
    }
    
    displayResult();
    
    std::cout << "\n按任意键重新开始，按ESC退出游戏..." << std::endl;
    int key = _getch();
    if (key == 27) { // ESC键
        exit(0);
    } else {
        restart();
    }
}

void Game::displayMenu() {
    std::cout << "\n===== 游戏模式 =====" << std::endl;
    std::cout << "1. 人机对战" << std::endl;
    std::cout << "请选择游戏模式 (1): ";
    
    int choice = 1;
    std::cin >> choice;
    
    // 目前只支持人机对战
    currentPlayerType = PlayerType::HUMAN;
}

void Game::displayDifficultyMenu() {
    std::cout << "\n===== 难度选择 =====" << std::endl;
    std::cout << "1. 简单" << std::endl;
    std::cout << "2. 中等" << std::endl;
    std::cout << "3. 困难" << std::endl;
    std::cout << "请选择难度 (1-3): ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            difficulty = GameDifficulty::EASY;
            break;
        case 2:
            difficulty = GameDifficulty::MEDIUM;
            break;
        case 3:
            difficulty = GameDifficulty::HARD;
            break;
        default:
            difficulty = GameDifficulty::EASY;
            break;
    }
    
    ai.setDifficulty(difficulty);
}

void Game::displayColorMenu() {
    std::cout << "\n===== 选择棋子颜色 =====" << std::endl;
    std::cout << "1. 黑棋 (先手)" << std::endl;
    std::cout << "2. 白棋 (后手)" << std::endl;
    std::cout << "请选择棋子颜色 (1-2): ";
    
    int choice;
    std::cin >> choice;
    
    if (choice == 2) {
        humanPlayerColor = PlayerColor::WHITE;
        currentPlayerType = PlayerType::AI; // 如果玩家选择白棋，AI先行
    } else {
        humanPlayerColor = PlayerColor::BLACK;
        currentPlayerType = PlayerType::HUMAN; // 如果玩家选择黑棋，玩家先行
    }
}

void Game::playTurn() {
    if (currentPlayerType == PlayerType::HUMAN) {
        playerMove();
    } else {
        aiMove();
    }
}

void Game::playerMove() {
    std::string colorName = (currentPlayerColor == PlayerColor::BLACK) ? "黑棋" : "白棋";
    std::cout << "\n轮到" << colorName << "，请输入落子位置 (行 列): ";
    
    int row, col;
    std::cin >> row >> col;
    
    // 检查输入是否有效
    while (row < 1 || row > board.getSize() || col < 1 || col > board.getSize() || 
           board.getCell(row-1, col-1) != CellState::EMPTY) {
        std::cout << "无效的位置，请重新输入 (行 列): ";
        std::cin >> row >> col;
    }
    
    // 转换为0-based索引
    board.placeStone(row-1, col-1, (currentPlayerColor == PlayerColor::BLACK) ? 
                     CellState::BLACK : CellState::WHITE);
    
    system("cls");
    board.display();
}

void Game::aiMove() {
    std::string colorName = (currentPlayerColor == PlayerColor::BLACK) ? "黑棋" : "白棋";
    std::cout << "\nAI思考中..." << std::endl;
    
    // 让AI思考一会儿
    Sleep(1000);
    
    // 获取AI的落子位置
    std::pair<int, int> move = ai.getMove(board, currentPlayerColor);
    
    board.placeStone(move.first, move.second, (currentPlayerColor == PlayerColor::BLACK) ? 
                     CellState::BLACK : CellState::WHITE);
    
    std::cout << "AI落子位置: " << move.first + 1 << " " << move.second + 1 << std::endl;
    
    system("cls");
    board.display();
}

void Game::switchPlayer() {
    // 切换玩家颜色
    currentPlayerColor = (currentPlayerColor == PlayerColor::BLACK) ? 
                         PlayerColor::WHITE : PlayerColor::BLACK;
    
    // 切换玩家类型
    currentPlayerType = (currentPlayerColor == humanPlayerColor) ? 
                        PlayerType::HUMAN : PlayerType::AI;
}

void Game::checkGameOver() {
    CellState winState = (currentPlayerColor == PlayerColor::BLACK) ? 
                        CellState::BLACK : CellState::WHITE;
    
    if (board.checkWin(winState)) {
        gameOver = true;
        winner = currentPlayerColor;
    } else if (board.isFull()) {
        gameOver = true;
        // 平局情况下，winner的值不重要
    }
}

void Game::displayResult() {
    if (board.isFull() && !board.checkWin(CellState::BLACK) && !board.checkWin(CellState::WHITE)) {
        std::cout << "\n游戏结束，平局！" << std::endl;
    } else {
        std::string winnerName = (winner == PlayerColor::BLACK) ? "黑棋" : "白棋";
        std::string resultText = (winner == humanPlayerColor) ? "恭喜，你赢了！" : "AI赢了，再接再厉！";
        std::cout << "\n游戏结束，" << winnerName << "获胜！" << std::endl;
        std::cout << resultText << std::endl;
    }
}

void Game::restart() {
    initGame();
    start();
}