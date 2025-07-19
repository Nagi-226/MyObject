#include "ai.h"
#include "game.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <limits>

AI::AI() {
    // 默认难度为简单
    difficulty = GameDifficulty::EASY;
    maxDepth = 1;
}

void AI::setDifficulty(GameDifficulty diff) {
    difficulty = diff;
    
    // 根据难度设置搜索深度
    switch (difficulty) {
        case GameDifficulty::EASY:
            maxDepth = 1;
            break;
        case GameDifficulty::MEDIUM:
            maxDepth = 2;
            break;
        case GameDifficulty::HARD:
            maxDepth = 3;
            break;
        default:
            maxDepth = 1;
            break;
    }
}

std::pair<int, int> AI::getMove(Board& board, PlayerColor playerColor) {
    // 根据难度选择不同的策略
    switch (difficulty) {
        case GameDifficulty::EASY:
            // 简单难度：70%随机落子，30%最佳落子
            if (rand() % 10 < 7) {
                return getRandomMove(board);
            } else {
                return getBestMove(board, playerColor);
            }
        case GameDifficulty::MEDIUM:
            // 中等难度：30%随机落子，70%最佳落子
            if (rand() % 10 < 3) {
                return getRandomMove(board);
            } else {
                return getBestMove(board, playerColor);
            }
        case GameDifficulty::HARD:
            // 困难难度：100%最佳落子
            return getBestMove(board, playerColor);
        default:
            return getRandomMove(board);
    }
}

std::pair<int, int> AI::getRandomMove(const Board& board) {
    std::vector<std::pair<int, int>> availableMoves = getAvailableMoves(board);
    
    // 如果没有可用的移动，返回(-1, -1)
    if (availableMoves.empty()) {
        return std::make_pair(-1, -1);
    }
    
    // 随机选择一个可用的移动
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, availableMoves.size() - 1);
    
    return availableMoves[dis(gen)];
}

std::pair<int, int> AI::getBestMove(Board& board, PlayerColor playerColor) {
    std::vector<std::pair<int, int>> availableMoves = getAvailableMoves(board);
    
    // 如果没有可用的移动，返回(-1, -1)
    if (availableMoves.empty()) {
        return std::make_pair(-1, -1);
    }
    
    // 如果是第一步，选择中心位置
    bool isEmpty = true;
    for (int i = 0; i < board.getSize(); i++) {
        for (int j = 0; j < board.getSize(); j++) {
            if (board.getCell(i, j) != CellState::EMPTY) {
                isEmpty = false;
                break;
            }
        }
        if (!isEmpty) break;
    }
    
    if (isEmpty) {
        return std::make_pair(board.getSize() / 2, board.getSize() / 2);
    }
    
    // 评估每个可能的移动
    int bestScore = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = availableMoves[0];
    
    CellState aiState = (playerColor == PlayerColor::BLACK) ? CellState::BLACK : CellState::WHITE;
    
    for (const auto& move : availableMoves) {
        // 尝试这个移动
        board.placeStone(move.first, move.second, aiState);
        
        // 使用极大极小算法评估这个移动
        int score = minimax(board, maxDepth, false, std::numeric_limits<int>::min(), 
                           std::numeric_limits<int>::max(), playerColor);
        
        // 撤销这个移动
        board.placeStone(move.first, move.second, CellState::EMPTY);
        
        // 更新最佳移动
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    return bestMove;
}

int AI::minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta, PlayerColor playerColor) {
    CellState aiState = (playerColor == PlayerColor::BLACK) ? CellState::BLACK : CellState::WHITE;
    CellState opponentState = (playerColor == PlayerColor::BLACK) ? CellState::WHITE : CellState::BLACK;
    
    // 检查是否有人赢了
    if (board.checkWin(aiState)) {
        return 10000;
    }
    if (board.checkWin(opponentState)) {
        return -10000;
    }
    
    // 如果达到最大深度或棋盘已满，评估当前局面
    if (depth == 0 || board.isFull()) {
        return evaluateBoard(board, playerColor);
    }
    
    std::vector<std::pair<int, int>> availableMoves = getAvailableMoves(board);
    
    if (isMaximizing) {
        int maxEval = std::numeric_limits<int>::min();
        
        for (const auto& move : availableMoves) {
            // 尝试这个移动
            board.placeStone(move.first, move.second, aiState);
            
            // 递归评估
            int eval = minimax(board, depth - 1, false, alpha, beta, playerColor);
            
            // 撤销这个移动
            board.placeStone(move.first, move.second, CellState::EMPTY);
            
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            
            // Alpha-Beta剪枝
            if (beta <= alpha) {
                break;
            }
        }
        
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        
        for (const auto& move : availableMoves) {
            // 尝试这个移动
            board.placeStone(move.first, move.second, opponentState);
            
            // 递归评估
            int eval = minimax(board, depth - 1, true, alpha, beta, playerColor);
            
            // 撤销这个移动
            board.placeStone(move.first, move.second, CellState::EMPTY);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            // Alpha-Beta剪枝
            if (beta <= alpha) {
                break;
            }
        }
        
        return minEval;
    }
}

int AI::evaluateBoard(const Board& board, PlayerColor playerColor) {
    int score = 0;
    CellState aiState = (playerColor == PlayerColor::BLACK) ? CellState::BLACK : CellState::WHITE;
    CellState opponentState = (playerColor == PlayerColor::BLACK) ? CellState::WHITE : CellState::BLACK;
    
    // 评估每个空位置的分数
    for (int row = 0; row < board.getSize(); row++) {
        for (int col = 0; col < board.getSize(); col++) {
            if (board.getCell(row, col) == CellState::EMPTY) {
                // 评估AI放置在这个位置的分数
                int aiScore = board.evaluatePosition(row, col, aiState);
                
                // 评估对手放置在这个位置的分数
                int opponentScore = board.evaluatePosition(row, col, opponentState);
                
                // 总分 = AI得分 - 对手得分
                score += aiScore - opponentScore;
            }
        }
    }
    
    return score;
}

std::vector<std::pair<int, int>> AI::getAvailableMoves(const Board& board) {
    std::vector<std::pair<int, int>> moves;
    
    // 获取所有空位置
    for (int row = 0; row < board.getSize(); row++) {
        for (int col = 0; col < board.getSize(); col++) {
            if (board.getCell(row, col) == CellState::EMPTY) {
                // 只考虑已有棋子周围的空位置（提高效率）
                bool hasNeighbor = false;
                
                // 检查周围8个方向是否有棋子
                for (int dRow = -1; dRow <= 1; dRow++) {
                    for (int dCol = -1; dCol <= 1; dCol++) {
                        if (dRow == 0 && dCol == 0) continue;
                        
                        int newRow = row + dRow;
                        int newCol = col + dCol;
                        
                        if (newRow >= 0 && newRow < board.getSize() && 
                            newCol >= 0 && newCol < board.getSize() && 
                            board.getCell(newRow, newCol) != CellState::EMPTY) {
                            hasNeighbor = true;
                            break;
                        }
                    }
                    if (hasNeighbor) break;
                }
                
                // 如果周围有棋子，或者棋盘几乎是空的，添加这个位置
                if (hasNeighbor || moves.size() < 5) {
                    moves.push_back(std::make_pair(row, col));
                }
            }
        }
    }
    
    // 如果没有找到有邻居的空位置，返回所有空位置
    if (moves.empty()) {
        for (int row = 0; row < board.getSize(); row++) {
            for (int col = 0; col < board.getSize(); col++) {
                if (board.getCell(row, col) == CellState::EMPTY) {
                    moves.push_back(std::make_pair(row, col));
                }
            }
        }
    }
    
    return moves;
}