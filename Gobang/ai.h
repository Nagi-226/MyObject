#ifndef AI_H
#define AI_H

#include "board.h"
#include <utility>

enum class GameDifficulty;

class AI {
private:
    GameDifficulty difficulty;
    int maxDepth; // 搜索深度，根据难度调整
    
    // 极大极小算法相关方法
    int minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta, PlayerColor playerColor);
    int evaluateBoard(const Board& board, PlayerColor playerColor);
    std::vector<std::pair<int, int>> getAvailableMoves(const Board& board);
    
    // 根据难度选择最佳移动或随机移动
    std::pair<int, int> getBestMove(Board& board, PlayerColor playerColor);
    std::pair<int, int> getRandomMove(const Board& board);
    
 public:
    AI();
    void setDifficulty(GameDifficulty diff);
    std::pair<int, int> getMove(Board& board, PlayerColor playerColor);
};

#endif // AI_H