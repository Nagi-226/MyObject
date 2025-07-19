// Gobang_Simple.cpp - 五子棋游戏（简化版本）
// 这个版本使用简单ASCII字符，适合在任何环境下编译运行
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#include <random>

// 棋盘单元格状态
enum class CellState {
    EMPTY,
    BLACK,
    WHITE
};

// 玩家类型
enum class PlayerType {
    HUMAN,
    AI
};

// 玩家颜色
enum class PlayerColor {
    BLACK,
    WHITE
};

// 游戏难度
enum class GameDifficulty {
    EASY,
    MEDIUM,
    HARD
};

// 棋盘类
class Board {
private:
    static const int SIZE = 15; // 15x15的棋盘
    std::vector<std::vector<CellState>> grid;
    
    bool checkDirection(int row, int col, int dRow, int dCol, CellState state) const {
        int count = 0;
        
        // 检查一个方向
        for (int i = 0; i < 5; i++) {
            int newRow = row + i * dRow;
            int newCol = col + i * dCol;
            
            if (newRow < 0 || newRow >= SIZE || newCol < 0 || newCol >= SIZE || 
                grid[newRow][newCol] != state) {
                break;
            }
            
            count++;
        }
        
        return count >= 5;
    }

public:
    Board() {
        init();
    }
    
    void init() {
        grid.resize(SIZE, std::vector<CellState>(SIZE, CellState::EMPTY));
    }
    
    void display() const {
        // 打印列号
        std::cout << "  ";
        for (int col = 0; col < SIZE; col++) {
            std::cout << (col + 1) % 10 << " ";
        }
        std::cout << std::endl;
        
        // 打印棋盘
        for (int row = 0; row < SIZE; row++) {
            // 打印行号
            std::cout << (row + 1) % 10 << " ";
            
            for (int col = 0; col < SIZE; col++) {
                if (grid[row][col] == CellState::EMPTY) {
                    // 根据位置打印不同的空白符号
                    if (row == 0 && col == 0) {
                        std::cout << "+";
                    } else if (row == 0 && col == SIZE - 1) {
                        std::cout << "+";
                    } else if (row == SIZE - 1 && col == 0) {
                        std::cout << "+";
                    } else if (row == SIZE - 1 && col == SIZE - 1) {
                        std::cout << "+";
                    } else if (row == 0) {
                        std::cout << "+";
                    } else if (row == SIZE - 1) {
                        std::cout << "+";
                    } else if (col == 0) {
                        std::cout << "+";
                    } else if (col == SIZE - 1) {
                        std::cout << "+";
                    } else {
                        std::cout << "+";
                    }
                } else if (grid[row][col] == CellState::BLACK) {
                    std::cout << "X";
                } else { // WHITE
                    std::cout << "O";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
    
    CellState getCell(int row, int col) const {
        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
            return grid[row][col];
        }
        return CellState::EMPTY; // 超出边界返回空
    }
    
    void placeStone(int row, int col, CellState state) {
        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && 
            grid[row][col] == CellState::EMPTY) {
            grid[row][col] = state;
        }
    }
    
    bool checkWin(CellState state) const {
        // 检查所有可能的五子连线
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (grid[row][col] == state) {
                    // 检查水平方向
                    if (checkDirection(row, col, 0, 1, state)) return true;
                    
                    // 检查垂直方向
                    if (checkDirection(row, col, 1, 0, state)) return true;
                    
                    // 检查右下对角线
                    if (checkDirection(row, col, 1, 1, state)) return true;
                    
                    // 检查左下对角线
                    if (checkDirection(row, col, 1, -1, state)) return true;
                }
            }
        }
        
        return false;
    }
    
    bool isFull() const {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (grid[row][col] == CellState::EMPTY) {
                    return false;
                }
            }
        }
        return true;
    }
    
    int getSize() const {
        return SIZE;
    }
    
    int evaluatePosition(int row, int col, CellState state) const {
        // 如果该位置已经有棋子，返回最低分
        if (grid[row][col] != CellState::EMPTY) {
            return -1000;
        }
        
        int score = 0;
        
        // 定义8个方向
        int directions[8][2] = {
            {-1, 0}, {1, 0},   // 垂直
            {0, -1}, {0, 1},   // 水平
            {-1, -1}, {1, 1},  // 左上-右下对角线
            {-1, 1}, {1, -1}   // 右上-左下对角线
        };
        
        // 检查每个方向
        for (int d = 0; d < 8; d += 2) {
            int count = 1;  // 当前位置算一个
            int empty = 0;  // 空位计数
            int blocked = 0; // 被阻挡计数
            
            // 检查两个相反的方向
            for (int i = d; i < d + 2; i++) {
                int dRow = directions[i][0];
                int dCol = directions[i][1];
                
                // 向该方向检查4个位置
                for (int step = 1; step <= 4; step++) {
                    int newRow = row + dRow * step;
                    int newCol = col + dCol * step;
                    
                    // 检查边界
                    if (newRow < 0 || newRow >= SIZE || newCol < 0 || newCol >= SIZE) {
                        blocked++;
                        break;
                    }
                    
                    if (grid[newRow][newCol] == state) {
                        count++;
                    } else if (grid[newRow][newCol] == CellState::EMPTY) {
                        empty++;
                        break;
                    } else {
                        blocked++;
                        break;
                    }
                }
            }
            
            // 根据连子数和阻挡情况评分
            if (count >= 5) {
                score += 100000; // 五连珠，最高分
            } else if (count == 4) {
                if (blocked == 0) score += 10000;  // 活四
                else if (blocked == 1) score += 1000;  // 冲四
            } else if (count == 3) {
                if (blocked == 0) score += 1000;  // 活三
                else if (blocked == 1) score += 100;  // 冲三
            } else if (count == 2) {
                if (blocked == 0) score += 100;  // 活二
                else if (blocked == 1) score += 10;  // 冲二
            }
        }
        
        return score;
    }
};

// AI类
class AI {
private:
    GameDifficulty difficulty;
    int maxDepth; // 搜索深度，根据难度调整
    
    // 极大极小算法
    int minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta, PlayerColor playerColor) {
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
    
    int evaluateBoard(const Board& board, PlayerColor playerColor) {
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
    
    std::vector<std::pair<int, int>> getAvailableMoves(const Board& board) {
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
    
    std::pair<int, int> getBestMove(Board& board, PlayerColor playerColor) {
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
    
    std::pair<int, int> getRandomMove(const Board& board) {
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
    
public:
    AI() {
        // 默认难度为简单
        difficulty = GameDifficulty::EASY;
        maxDepth = 1;
    }
    
    void setDifficulty(GameDifficulty diff) {
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
    
    std::pair<int, int> getMove(Board& board, PlayerColor playerColor) {
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
};

// 游戏类
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

    void displayMenu() {
        std::cout << "\n===== 游戏模式 =====" << std::endl;
        std::cout << "1. 人机对战" << std::endl;
        std::cout << "请选择游戏模式 (1): ";
        
        int choice = 1;
        std::cin >> choice;
        
        // 目前只支持人机对战
        currentPlayerType = PlayerType::HUMAN;
    }
    
    void displayDifficultyMenu() {
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
    
    void displayColorMenu() {
        std::cout << "\n===== 选择棋子颜色 =====" << std::endl;
        std::cout << "1. 黑棋 (先手) - 显示为X" << std::endl;
        std::cout << "2. 白棋 (后手) - 显示为O" << std::endl;
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
    
    void initGame() {
        board.init();
        gameOver = false;
        winner = PlayerColor::BLACK; // 默认值，实际获胜者会在游戏结束时确定
        currentPlayerColor = PlayerColor::BLACK; // 黑棋先行
    }
    
    void switchPlayer() {
        // 切换玩家颜色
        currentPlayerColor = (currentPlayerColor == PlayerColor::BLACK) ? 
                             PlayerColor::WHITE : PlayerColor::BLACK;
        
        // 切换玩家类型
        currentPlayerType = (currentPlayerColor == humanPlayerColor) ? 
                            PlayerType::HUMAN : PlayerType::AI;
    }
    
    void playerMove() {
        std::string colorName = (currentPlayerColor == PlayerColor::BLACK) ? "黑棋(X)" : "白棋(O)";
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
        
        // 清屏
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
        board.display();
    }
    
    void aiMove() {
        std::string colorName = (currentPlayerColor == PlayerColor::BLACK) ? "黑棋(X)" : "白棋(O)";
        std::cout << "\nAI思考中..." << std::endl;
        
        // 让AI思考一会儿
        std::cout << "请稍等..." << std::endl;
        
        // 获取AI的落子位置
        std::pair<int, int> move = ai.getMove(board, currentPlayerColor);
        
        board.placeStone(move.first, move.second, (currentPlayerColor == PlayerColor::BLACK) ? 
                         CellState::BLACK : CellState::WHITE);
        
        std::cout << "AI落子位置: " << move.first + 1 << " " << move.second + 1 << std::endl;
        
        // 清屏
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
        board.display();
    }
    
    void checkGameOver() {
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
    
    void displayResult() {
        if (board.isFull() && !board.checkWin(CellState::BLACK) && !board.checkWin(CellState::WHITE)) {
            std::cout << "\n游戏结束，平局！" << std::endl;
        } else {
            std::string winnerName = (winner == PlayerColor::BLACK) ? "黑棋(X)" : "白棋(O)";
            std::string resultText = (winner == humanPlayerColor) ? "恭喜，你赢了！" : "AI赢了，再接再厉！";
            std::cout << "\n游戏结束，" << winnerName << "获胜！" << std::endl;
            std::cout << resultText << std::endl;
        }
    }

public:
    Game() {
        initGame();
    }
    
    void start() {
        // 清屏
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
        std::cout << "欢迎来到五子棋游戏！" << std::endl;
        
        displayMenu();
        displayDifficultyMenu();
        displayColorMenu();
        
        // 清屏
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
        board.display();
        
        while (!gameOver) {
            playTurn();
            checkGameOver();
            if (!gameOver) {
                switchPlayer();
            }
        }
        
        displayResult();
        
        std::cout << "\n按1重新开始，按0退出游戏: ";
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            restart();
        } else {
            exit(0);
        }
    }
    
    void playTurn() {
        if (currentPlayerType == PlayerType::HUMAN) {
            playerMove();
        } else {
            aiMove();
        }
    }
    
    void restart() {
        initGame();
        start();
    }
};

// 主函数
int main() {
    // 设置随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));
    
    Game game;
    game.start();
    
    return 0;
}