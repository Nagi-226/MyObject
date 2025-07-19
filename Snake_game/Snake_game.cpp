#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <random>

using namespace std;

// 游戏区域大小
const int WIDTH = 40;
const int HEIGHT = 20;

// 方向枚举
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// 游戏状态枚举
enum GameState { RUNNING, PAUSED, GAME_OVER };

// 点类，用于表示坐标
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// 食物类
class Food {
private:
    Point position;
    char symbol;
    int score;
    bool isSpecial;
    int lifespan;
    int timer;

public:
    Food() : symbol('F'), score(10), isSpecial(false), lifespan(0), timer(0) {
        Respawn();
    }

    void Respawn() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> disX(1, WIDTH - 2);
        uniform_int_distribution<> disY(1, HEIGHT - 2);
        uniform_int_distribution<> disSpecial(0, 9); // 10%的概率生成特殊食物

        position.x = disX(gen);
        position.y = disY(gen);

        // 随机决定是否为特殊食物
        isSpecial = (disSpecial(gen) == 0);
        if (isSpecial) {
            symbol = '$';
            score = 30;
            lifespan = 30; // 特殊食物存在30个游戏周期
            timer = 0;
        } else {
            symbol = 'F';
            score = 10;
            lifespan = 0;
            timer = 0;
        }
    }

    void Update() {
        if (isSpecial && lifespan > 0) {
            timer++;
            if (timer >= lifespan) {
                Respawn();
            }
        }
    }

    Point GetPosition() const { return position; }
    char GetSymbol() const { return symbol; }
    int GetScore() const { return score; }
    bool IsSpecial() const { return isSpecial; }
};

// 蛇类
class Snake {
private:
    vector<Point> body;
    Direction direction;
    bool hasGrown;
    int speed;
    int speedBoostTimer;

public:
    Snake() : direction(STOP), hasGrown(false), speed(1), speedBoostTimer(0) {
        // 初始化蛇身，长度为3
        body.push_back(Point(WIDTH / 2, HEIGHT / 2));
        body.push_back(Point(WIDTH / 2 - 1, HEIGHT / 2));
        body.push_back(Point(WIDTH / 2 - 2, HEIGHT / 2));
    }

    void ChangeDirection(Direction newDir) {
        // 防止反向移动
        if (newDir == LEFT && direction != RIGHT)
            direction = LEFT;
        else if (newDir == RIGHT && direction != LEFT)
            direction = RIGHT;
        else if (newDir == UP && direction != DOWN)
            direction = UP;
        else if (newDir == DOWN && direction != UP)
            direction = DOWN;
    }

    void Move() {
        if (direction == STOP)
            return;

        // 如果蛇没有生长，移除尾部
        if (!hasGrown) {
            body.pop_back();
        } else {
            hasGrown = false;
        }

        // 根据方向移动头部
        Point newHead = body.front();
        switch (direction) {
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

        // 穿墙效果
        if (newHead.x <= 0) newHead.x = WIDTH - 2;
        else if (newHead.x >= WIDTH - 1) newHead.x = 1;
        if (newHead.y <= 0) newHead.y = HEIGHT - 2;
        else if (newHead.y >= HEIGHT - 1) newHead.y = 1;

        // 将新头部添加到身体前面
        body.insert(body.begin(), newHead);

        // 更新速度加成计时器
        if (speedBoostTimer > 0) {
            speedBoostTimer--;
            if (speedBoostTimer == 0) {
                speed = 1; // 恢复正常速度
            }
        }
    }

    void Grow() {
        hasGrown = true;
    }

    void BoostSpeed() {
        speed = 2; // 加速
        speedBoostTimer = 20; // 持续20个游戏周期
    }

    bool CheckCollision() const {
        // 检查头部是否与身体碰撞
        for (size_t i = 1; i < body.size(); i++) {
            if (body[0] == body[i])
                return true;
        }
        return false;
    }

    bool EatFood(const Food& food) {
        if (body[0] == food.GetPosition()) {
            Grow();
            if (food.IsSpecial()) {
                BoostSpeed();
            }
            return true;
        }
        return false;
    }

    vector<Point> GetBody() const { return body; }
    int GetSpeed() const { return speed; }
};

// 游戏类
class Game {
private:
    Snake snake;
    Food food;
    GameState state;
    int score;
    int level;
    int frameCount;

    void DrawBorder() {
        // 绘制上边界
        for (int i = 0; i < WIDTH; i++) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(i), 0});
            cout << '#';
        }

        // 绘制下边界
        for (int i = 0; i < WIDTH; i++) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(i), static_cast<SHORT>(HEIGHT - 1)});
            cout << '#';
        }

        // 绘制左边界
        for (int i = 0; i < HEIGHT; i++) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, static_cast<SHORT>(i)});
            cout << '#';
        }

        // 绘制右边界
        for (int i = 0; i < HEIGHT; i++) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH - 1), static_cast<SHORT>(i)});
            cout << '#';
        }
    }

    void DrawInfo() {
        // 显示分数和等级
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH + 2), 1});
        cout << "分数: " << score;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH + 2), 2});
        cout << "等级: " << level;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH + 2), 4});
        cout << "控制:";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH + 2), 5});
        cout << "W - 上";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH + 2), 6});
        cout << "S - 下";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH + 2), 7});
        cout << "A - 左";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH + 2), 8});
        cout << "D - 右";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH + 2), 9});
        cout << "P - 暂停";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH + 2), 10});
        cout << "ESC - 退出";

        // 如果游戏暂停，显示暂停信息
        if (state == PAUSED) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH / 2 - 5), static_cast<SHORT>(HEIGHT / 2)});
            cout << "游戏已暂停";
        }

        // 如果游戏结束，显示游戏结束信息
        if (state == GAME_OVER) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH / 2 - 5), static_cast<SHORT>(HEIGHT / 2)});
            cout << "游戏结束!";
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(WIDTH / 2 - 10), static_cast<SHORT>(HEIGHT / 2 + 1)});
            cout << "按R键重新开始游戏";
        }
    }

public:
    Game() : state(RUNNING), score(0), level(1), frameCount(0) {
        // 初始化随机数生成器
        srand(static_cast<unsigned int>(time(nullptr)));

        // 隐藏控制台光标
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 100;
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

        // 设置控制台窗口大小
        COORD bufferSize = {static_cast<SHORT>(WIDTH + 20), static_cast<SHORT>(HEIGHT + 5)};
        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
        SMALL_RECT windowSize = {0, 0, static_cast<SHORT>(WIDTH + 19), static_cast<SHORT>(HEIGHT + 4)};
        SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
    }

    void Reset() {
        // 重置游戏状态
        snake = Snake();
        food.Respawn();
        state = RUNNING;
        score = 0;
        level = 1;
        frameCount = 0;
    }

    void Input() {
        if (_kbhit()) {
            char key = _getch();
            switch (tolower(key)) {
            case 'w':
                snake.ChangeDirection(UP);
                break;
            case 's':
                snake.ChangeDirection(DOWN);
                break;
            case 'a':
                snake.ChangeDirection(LEFT);
                break;
            case 'd':
                snake.ChangeDirection(RIGHT);
                break;
            case 'p':
                if (state == RUNNING)
                    state = PAUSED;
                else if (state == PAUSED)
                    state = RUNNING;
                break;
            case 'r':
                if (state == GAME_OVER)
                    Reset();
                break;
            case 27: // ESC键
                state = GAME_OVER;
                break;
            }
        }
    }

    void Update() {
        if (state != RUNNING)
            return;

        frameCount++;

        // 根据蛇的速度决定移动频率
        if (frameCount % (10 / snake.GetSpeed()) == 0) {
            snake.Move();

            // 检查蛇是否吃到食物
            if (snake.EatFood(food)) {
                score += food.GetScore();
                food.Respawn();

                // 每得到100分升一级
                level = score / 100 + 1;
            }

            // 检查蛇是否碰撞到自己
            if (snake.CheckCollision()) {
                state = GAME_OVER;
            }
        }

        // 更新食物状态
        food.Update();
    }

    void Render() {
        // 清屏
        system("cls");

        // 绘制边界
        DrawBorder();

        // 绘制蛇
        vector<Point> snakeBody = snake.GetBody();
        for (size_t i = 0; i < snakeBody.size(); i++) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(snakeBody[i].x), static_cast<SHORT>(snakeBody[i].y)});
            if (i == 0)
                cout << 'O'; // 蛇头
            else
                cout << 'o'; // 蛇身
        }

        // 绘制食物
        Point foodPos = food.GetPosition();
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(foodPos.x), static_cast<SHORT>(foodPos.y)});
        cout << food.GetSymbol();

        // 绘制游戏信息
        DrawInfo();
    }

    bool IsRunning() const {
        return state != GAME_OVER;
    }

    void Run() {
        while (IsRunning()) {
            Input();
            Update();
            Render();
            Sleep(50); // 控制游戏速度
        }

        // 游戏结束后再渲染一次，显示游戏结束信息
        Render();

        // 等待玩家按R键重新开始或按其他键退出
        while (true) {
            if (_kbhit()) {
                char key = _getch();
                if (tolower(key) == 'r') {
                    Reset();
                    Run();
                    return;
                } else if (key == 27) { // ESC键
                    return;
                }
            }
        }
    }
};

int main() {
    // 设置控制台标题
    SetConsoleTitle(TEXT("贪吃蛇游戏"));

    // 创建游戏实例并运行
    Game game;
    game.Run();

    return 0;
}