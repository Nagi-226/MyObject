#ifndef BOARD_H
#define BOARD_H

#include <vector>

enum class CellState {
    EMPTY,
    BLACK,
    WHITE
};

class Board {
private:
    static const int SIZE = 15; // 15x15的棋盘
    std::vector<std::vector<CellState>> grid;
    
    bool checkDirection(int row, int col, int dRow, int dCol, CellState state) const;

public:
    Board();
    void init();
    void display() const;
    CellState getCell(int row, int col) const;
    void placeStone(int row, int col, CellState state);
    bool checkWin(CellState state) const;
    bool isFull() const;
    int getSize() const;
    
    // 用于AI评估的方法
    int evaluatePosition(int row, int col, CellState state) const;
};

#endif // BOARD_H