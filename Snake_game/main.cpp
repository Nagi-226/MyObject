#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

// 游戏区域大小
const int WIDTH = 40;
const int HEIGHT = 20;

// 方向枚举
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// 坐标结构体
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }
};

// 游戏类
class Game {
private:
    bool gameOver;
    bool win;
    int score;
    Point food;
    vector<Point> snake;
    Direction dir;
    
    // 绘制游戏界面
    void Draw() {
        system("cls"); // 清屏
        
        // 绘制上边界
        for (int i = 0; i < WIDTH + 2; i++)
            cout << "#";
        cout << endl;
        
        // 绘制游戏区域
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0)
                    cout << "#"; // 左边界
                
                // 绘制蛇头
                if (j == snake[0].x && i == snake[0].y)
                    cout << "O";
                // 绘制食物
                else if (j == food.x && i == food.y)
                    cout << "F";
                else {
                    bool isBodyPart = false;
                    // 绘制蛇身
                    for (size_t k = 1; k < snake.size(); k++) {
                        if (j == snake[k].x && i == snake[k].y) {
                            cout << "o";
                            isBodyPart = true;
                            break;
                        }
                    }
                    
                    if (!isBodyPart)
                        cout << " ";
                }
                
                if (j == WIDTH - 1)
                    cout << "#"; // 右边界
            }
            cout << endl;
        }
        
        // 绘制下边界
        for (int i = 0; i < WIDTH + 2; i++)
            cout << "#";
        cout << endl;
        
        // 显示得分
        cout << "得分: " << score << endl;
    }
    
    // 处理用户输入
    void Input() {
        if (_kbhit()) { // 检测键盘输入
            switch (_getch()) {
                case 'a':
                    if (dir != RIGHT) // 防止直接掉头
                        dir = LEFT;
                    break;
                case 'd':
                    if (dir != LEFT)
                        dir = RIGHT;
                    break;
                case 'w':
                    if (dir != DOWN)
                        dir = UP;
                    break;
                case 's':
                    if (dir != UP)
                        dir = DOWN;
                    break;
                case 'x':
                    gameOver = true;
                    break;
            }
        }
    }
    
    // 游戏逻辑更新
    void Logic() {
        // 根据蛇头位置和方向，计算新的蛇头位置
        Point newHead = snake[0];
        
        switch (dir) {
            case LEFT:
                newHead.x--;
                break;
            case RIGHT:
                newHead.x++;
                break;
            case UP:
                newHead.y--;
                break;
            case DOWN:
                newHead.y++;
                break;
            default:
                break;
        }
        
        // 检测碰撞墙壁
        if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT) {
            gameOver = true;
            return;
        }
        
        // 检测碰撞自身
        for (size_t i = 1; i < snake.size(); i++) {
            if (newHead == snake[i]) {
                gameOver = true;
                return;
            }
        }
        
        // 在蛇头位置插入新的头部
        snake.insert(snake.begin(), newHead);
        
        // 检测是否吃到食物
        if (newHead == food) {
            score += 10;
            SpawnFood(); // 生成新的食物
        } else {
            // 如果没有吃到食物，移除尾部
            snake.pop_back();
        }
        
        // 胜利条件：达到一定长度
        if (snake.size() >= 50) {
            win = true;
            gameOver = true;
        }
    }
    
    // 生成食物
    void SpawnFood() {
        bool validPosition;
        do {
            validPosition = true;
            food.x = rand() % WIDTH;
            food.y = rand() % HEIGHT;
            
            // 确保食物不会生成在蛇身上
            for (const auto& segment : snake) {
                if (food == segment) {
                    validPosition = false;
                    break;
                }
            }
        } while (!validPosition);
    }
    
public:
    // 构造函数
    Game() {
        Reset();
    }
    
    // 重置游戏
    void Reset() {
        gameOver = false;
        win = false;
        dir = STOP;
        score = 0;
        
        // 清空蛇
        snake.clear();
        
        // 初始化蛇的位置（中间）
        snake.push_back(Point(WIDTH / 2, HEIGHT / 2));
        
        // 生成第一个食物
        srand(static_cast<unsigned>(time(nullptr)));
        SpawnFood();
    }
    
    // 运行游戏
    void Run() {
        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(100); // 控制游戏速度
        }
        
        // 游戏结束显示
        system("cls");
        if (win) {
            cout << "恭喜你赢了！" << endl;
        } else {
            cout << "游戏结束！" << endl;
        }
        cout << "最终得分: " << score << endl;
        cout << "按任意键退出..." << endl;
        _getch();
    }
};

// 主函数
int main() {
    // 设置控制台标题
    SetConsoleTitle(TEXT("贪吃蛇游戏"));
    
    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    // 显示游戏说明
    cout << "贪吃蛇游戏" << endl;
    cout << "控制：" << endl;
    cout << "W - 上移" << endl;
    cout << "S - 下移" << endl;
    cout << "A - 左移" << endl;
    cout << "D - 右移" << endl;
    cout << "X - 退出游戏" << endl;
    cout << "按任意键开始游戏..." << endl;
    _getch();
    
    // 创建并运行游戏
    Game game;
    game.Run();
    
    return 0;
}