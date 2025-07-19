#include "board.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

Board::Board() {
    init();
}

void Board::init() {
    grid.resize(SIZE, std::vector<CellState>(SIZE, CellState::EMPTY));
}

void Board::display() const {
    // 设置控制台颜色
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 打印列号
    std::cout << "  ";
    for (int col = 0; col < SIZE; col++) {
        std::cout << std::setw(2) << col + 1 << " ";
    }
    std::cout << std::endl;
    
    // 打印棋盘
    for (int row = 0; row < SIZE; row++) {
        // 打印行号
        std::cout << std::setw(2) << row + 1 << " ";
        
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == CellState::EMPTY) {
                // 根据位置打印不同的空白符号
                if (row == 0 && col == 0) {
                    std::cout << "┌─";
                } else if (row == 0 && col == SIZE - 1) {
                    std::cout << "┐ ";
                } else if (row == SIZE - 1 && col == 0) {
                    std::cout << "└─";
                } else if (row == SIZE - 1 && col == SIZE - 1) {
                    std::cout << "┘ ";
                } else if (row == 0) {
                    std::cout << "┬─";
                } else if (row == SIZE - 1) {
                    std::cout << "┴─";
                } else if (col == 0) {
                    std::cout << "├─";
                } else if (col == SIZE - 1) {
                    std::cout << "┤ ";
                } else {
                    std::cout << "┼─";
                }
            } else {
                // 设置棋子颜色
                if (grid[row][col] == CellState::BLACK) {
                    SetConsoleTextAttribute(hConsole, 0);
                    std::cout << "● ";
                } else { // WHITE
                    SetConsoleTextAttribute(hConsole, 7 | 8);
                    std::cout << "● ";
                }
                // 重置颜色
                SetConsoleTextAttribute(hConsole, 7);
            }
        }
        std::cout << std::endl;
    }
}

CellState Board::getCell(int row, int col) const {
    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
        return grid[row][col];
    }
    return CellState::EMPTY; // 超出边界返回空
}

void Board::placeStone(int row, int col, CellState state) {
    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && 
        grid[row][col] == CellState::EMPTY) {
        grid[row][col] = state;
    }
}

bool Board::checkDirection(int row, int col, int dRow, int dCol, CellState state) const {
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

bool Board::checkWin(CellState state) const {
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

bool Board::isFull() const {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == CellState::EMPTY) {
                return false;
            }
        }
    }
    return true;
}

int Board::getSize() const {
    return SIZE;
}

int Board::evaluatePosition(int row, int col, CellState state) const {
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