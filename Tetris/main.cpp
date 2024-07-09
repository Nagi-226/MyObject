#include <stdio.h>
#include <stdlib.h>
#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F " //终端禁止显示输入
#define STTY_DEF            "stty -raw echo -F " //恢复终端显示输入

//定义结构体,用来代表一个对x,y的坐标点
struct coord
{
	int x;
	int y;
};


void initMap();//初始化背景边框
void showStaticMap();//显示静态背景图
void gotoxy(int x, int y);//控制打印光标的位置
void showDynamicShape();//打印动态图形
void moveDown();//向下移动
void addStaticMap();//添加动态图形到静态背景图中
void drawDynamicShape();//画一个动态图形
static int get_char();//非阻塞输入
void moveLeft();//向左移动
void moveRight();
void clearOneLine(int row);
int isFullLine(int row);
void clearFullLine();
void addLevel();

int map[21][12];//背景图
//结构体数组,4个元素,因为有4对x,y坐标点
struct coord shape[4] = {{5, 0},{6, 0},{5, 1},{6, 1}};

int score = 0;//保存游戏得分
int level = 1;//游戏难度水平
int speed = 20;//游戏速度

int main(int argc, const char *argv[])
{ 
	int i;
	char cmd;//用来保存输入按键
	system("reset");//执行reset命令,重置终端
	system(STTY_US TTY_PATH);  
	initMap();//初始化静态背景边框
	drawDynamicShape();//画一个动态图形
	while(1)
	{
		showStaticMap();//打印静态背景图
		showDynamicShape();//显示动态图形
		fflush(stdout);//刷新缓存区
		for(i = 0; i < speed; i++)//循环20次等价于1s
		{
			//按键输入
			cmd = get_char();
			if(cmd == 'w')
			{
				printf("change\n");
			}
			else if(cmd == 'a')
			{
				moveLeft();
			}
			else if(cmd == 's') 
			{
				break;
			}
			else if(cmd == 'd')
			{
				moveRight();
			}
			else if(cmd == 3)//ctrl + c
			{
    			system(STTY_DEF TTY_PATH);	//恢复非阻塞IO设置
				return 0;
			}
			usleep(50000);
		}
		moveDown();//向下移动
	}

	return 0;
}

//初始化背景边框
void initMap()
{
	int i,j;
	printf("\033[?25l");//隐藏光标
	for(i = 0; i < 21; i++)
	{
		for(j = 0; j < 12; j++)
		{
			if(j == 0 || j == 11 || i == 20)//下标0和11列,下标20行,赋值为1,代表边框
			{
				map[i][j] = 1;
			}
		}
	}
}
//显示静态背景图
void showStaticMap()
{
	int i,j;
	//每次打印静态背景边框从0,0开始打印
	//gotoxy(0,0);
	for(i = 0; i < 21; i++)
	{
		gotoxy(0, i);//打印图形的时候,从每行的第0列开始
		for(j = 0; j < 12; j++)
		{
			if(map[i][j] == 1)//说明是边框
				printf("\033[0;31m◆\033[0m ");
			else if(map[i][j] == 2)//说明是残留物
				printf("\033[0;32m■\033[0m ");
			else if(map[i][j] == 0)//说明是空白
				printf("  ");//多敲一个空格,因为一个特殊字符占2个宽度
		}
		printf("\n");
	}
	gotoxy(0, 21);
	printf("\033[0;34m Level:%d      Score:%d\033[0m ",level,score);
}
//控制光标位置函数
void gotoxy(int x, int y)
{ 
	printf("\033[%d;%dH", y+1 , x+1);
}  

//显示动态图形dynamic
void showDynamicShape()
{
	int i;
	for(i = 0; i < 4; i++)
	{
		gotoxy(shape[i].x*2, shape[i].y);//一个特殊图形占两个宽度
		printf("\033[0;33m■\033[0m ");
	}
}
//向下移动
void moveDown()
{
	int i;
	//这个循环,专门用来判断是否能够继续向下移动
	for(i = 0; i < 4; i++)
	{
		//y+1是为了提前下一个身位是否有边框和障碍物
		if(map[shape[i].y+1][shape[i].x] != 0)//说明遇到了1或2, 边框或障碍物
		{//进入这个条件判断,就相当于落到最底部了
			addStaticMap();//添加到静态背景图
			clearFullLine();//清除满行
			drawDynamicShape();//再次画一个新的图形,重新开始从上往下落
			return;//提前结束函数
		}
	}
	//这个循环就是向下移动一个位置
	for(i = 0; i < 4; i++)
	{
		shape[i].y++;
	}
}
//添加到动态图形到静态背景图
void addStaticMap()
{
	int i;
	for(i = 0; i < 4; i++)
		map[shape[i].y][shape[i].x] = 2;
}
//画动态图形
void drawDynamicShape()
{
	//画一个田字格
	shape[0].x = 5;
	shape[0].y = 0;
	shape[1].x = 6;
	shape[1].y = 0;
	shape[2].x = 5;
	shape[2].y = 1;
	shape[3].x = 6;
	shape[3].y = 1;
}

//非阻塞输入
static int get_char()
{
    fd_set rfds;
    struct timeval tv;
    int ch = 0;
	
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 10; //设置等待超时时间

    //检测键盘是否有输入
    if (select(1, &rfds, NULL, NULL, &tv) > 0)
    {
        ch = getchar(); 
    }
	
    return ch;
}
//向左移动
void moveLeft()
{
	int i;
	//在移动之前加容错判断,是否碰到残留物或边框
	for(i = 0; i < 4; i++)
	{
		if(map[shape[i].y][shape[i].x-1] != 0)//说明遇到边框或残留物
		{
			return;//提前结束函数
		}
	}
	//向左移动
	for(i = 0; i < 4; i++)
		shape[i].x--;
}
//向右移动 
void moveRight()
{
	int i;
	//在移动之前加容错判断,是否碰到残留物或边框
	for(i = 0; i < 4; i++)
	{
		if(map[shape[i].y][shape[i].x+1] != 0)//说明遇到边框或残留物
		{
			return;//提前结束函数
		}
	}
	//向右移动
	for(i = 0; i < 4; i++)
		shape[i].x++;
}
//清除满行
void clearFullLine()
{
	int i;
	for(i = 19; i > 0; i--)
	{
		//i代表的是每一行的下标
		if(isFullLine(i))//对每一行进行判断是否是满行,如果是满行返回值是1
		{
			//清除这个第i行这个满行
			clearOneLine(i);
			score += 100;
			addLevel();//是否需要提升游戏难度
			i++;//为了解决连续出现两个满行的bug
		}
	}
}
 
//判断第i行是否是满行
//如果是满行,返回值是1
//如果不是满行,返回值是0 
int isFullLine(int row)
{
	int j;
	for(j = 1; j <= 10; j++)
	{
		if(map[row][j] != 2)//说明当前的row行不是满行
			return 0;
	}
	//如果程序能走到这,说明没有执行过return 0;,肯定是满行
	return 1;
}

//清除第i行这个满行
void clearOneLine(int row)
{
	int i,j;
	//逐行向下移动,覆盖删除
	for(i = row; i > 0; i--)
	{
		for(j = 1; j <= 10; j++)
		{
			map[i][j] = map[i-1][j];
		}
	}
}

//增加游戏难度水平
void addLevel()
{
	if(score >= 500 && level == 1)
	{
		level = 2;
		speed -= 5;
	}
	else if(score >= 1000 && level == 2)
	{
		level == 3;
		speed -= 5;
	}
}
```

![image-20230114153608290](C基础笔记.assets/image-20230114153608290.png)



# 6. 产生新的图形+变形

```C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F " //终端禁止显示输入
#define STTY_DEF            "stty -raw echo -F " //恢复终端显示输入

//定义结构体,用来代表一个对x,y的坐标点
struct coord
{
	int x;
	int y;
};


void initMap();//初始化背景边框
void showStaticMap();//显示静态背景图
void gotoxy(int x, int y);//控制打印光标的位置
void showDynamicShape();//打印动态图形
void moveDown();//向下移动
void addStaticMap();//添加动态图形到静态背景图中
void drawDynamicShape();//画一个动态图形
static int get_char();//非阻塞输入
void moveLeft();//向左移动
void moveRight();//向右移动
void clearOneLine(int row);//清除一行
int isFullLine(int row);//是否是满行
void clearFullLine();//清除满行
void addLevel();//添加游戏难度水平
void changeShape();//变换图形

int map[21][12];//背景图
//结构体数组,4个元素,因为有4对x,y坐标点
struct coord shape[4] = {{5, 0},{6, 0},{5, 1},{6, 1}};

int score = 0;//保存游戏得分
int level = 1;//游戏难度水平
int speed = 20;//游戏速度
int currentShape;//用来保存当前的图形

int main(int argc, const char *argv[])
{ 
	int i;
	char cmd;//用来保存输入按键
	srand(time(NULL));//初始化随机种子
	system("reset");//执行reset命令,重置终端
	system(STTY_US TTY_PATH);  
	initMap();//初始化静态背景边框
	drawDynamicShape();//画一个动态图形
	while(1)
	{
		showStaticMap();//打印静态背景图
		showDynamicShape();//显示动态图形
		fflush(stdout);//刷新缓存区
		for(i = 0; i < speed; i++)//循环20次等价于1s
		{
			//按键输入
			cmd = get_char();
			if(cmd == 'w')
			{
				changeShape();
				showStaticMap();
				showDynamicShape();
				fflush(stdout);
			}
			else if(cmd == 'a')
			{
				moveLeft();
				showStaticMap();
				showDynamicShape();
				fflush(stdout);
			}
			else if(cmd == 's') 
			{
				break;
			}
			else if(cmd == 'd')
			{
				moveRight();
				showStaticMap();
				showDynamicShape();
				fflush(stdout);
			}
			else if(cmd == 3)//ctrl + c
			{
    			system(STTY_DEF TTY_PATH);	//恢复非阻塞IO设置
				return 0;
			}
			usleep(50000);
		}
		moveDown();//向下移动
	}

	return 0;
}

//初始化背景边框
void initMap()
{
	int i,j;
	printf("\033[?25l");//隐藏光标
	for(i = 0; i < 21; i++)
	{
		for(j = 0; j < 12; j++)
		{
			if(j == 0 || j == 11 || i == 20)//下标0和11列,下标20行,赋值为1,代表边框
			{
				map[i][j] = 1;
			}
		}
	}
}
//显示静态背景图
void showStaticMap()
{
	int i,j;
	//每次打印静态背景边框从0,0开始打印
	//gotoxy(0,0);
	for(i = 0; i < 21; i++)
	{
		gotoxy(0, i);//打印图形的时候,从每行的第0列开始
		for(j = 0; j < 12; j++)
		{
			if(map[i][j] == 1)//说明是边框
				printf("\033[0;31m◆\033[0m ");
			else if(map[i][j] == 2)//说明是残留物
				printf("\033[0;32m■\033[0m ");
			else if(map[i][j] == 0)//说明是空白
				printf("  ");//多敲一个空格,因为一个特殊字符占2个宽度
		}
		printf("\n");
	}
	gotoxy(0, 21);
	printf("\033[0;34m Level:%d      Score:%d\033[0m ",level,score);
}
//控制光标位置函数
void gotoxy(int x, int y)
{ 
	printf("\033[%d;%dH", y+1 , x+1);
}  

//显示动态图形dynamic
void showDynamicShape()
{
	int i;
	for(i = 0; i < 4; i++)
	{
		gotoxy(shape[i].x*2, shape[i].y);//一个特殊图形占两个宽度
		printf("\033[0;33m■\033[0m ");
	}
}
//向下移动
void moveDown()
{
	int i;
	//这个循环,专门用来判断是否能够继续向下移动
	for(i = 0; i < 4; i++)
	{
		//y+1是为了提前下一个身位是否有边框和障碍物
		if(map[shape[i].y+1][shape[i].x] != 0)//说明遇到了1或2, 边框或障碍物
		{//进入这个条件判断,就相当于落到最底部了
			addStaticMap();//添加到静态背景图
			clearFullLine();//清除满行
			drawDynamicShape();//再次画一个新的图形,重新开始从上往下落
			return;//提前结束函数
		}
	}
	//这个循环就是向下移动一个位置
	for(i = 0; i < 4; i++)
	{
		shape[i].y++;
	}
}
//添加到动态图形到静态背景图
void addStaticMap()
{
	int i;
	for(i = 0; i < 4; i++)
		map[shape[i].y][shape[i].x] = 2;
}
//画动态图形
void drawDynamicShape()
{
	currentShape = rand() % 3;
	switch(currentShape)
	{
	case 0://田字格
		//画一个田字格
		shape[0].x = 5;
		shape[0].y = 0;
		shape[1].x = 6;
		shape[1].y = 0;
		shape[2].x = 5;
		shape[2].y = 1;
		shape[3].x = 6;
		shape[3].y = 1;
		break;
	case 1:
		//画一个横条
		shape[0].x = 4;
		shape[0].y = 0;
		shape[1].x = 5;
		shape[1].y = 0;
		shape[2].x = 6;
		shape[2].y = 0;
		shape[3].x = 7;
		shape[3].y = 0;
		break;
	case 2:
		//画一个竖条
		shape[0].x = 5;
		shape[0].y = 0;
		shape[1].x = 5;
		shape[1].y = 1;
		shape[2].x = 5;
		shape[2].y = 2;
		shape[3].x = 5;
		shape[3].y = 3;
		break;
	}
}

//非阻塞输入
static int get_char()
{
    fd_set rfds;
    struct timeval tv;
    int ch = 0;
	
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 10; //设置等待超时时间

    //检测键盘是否有输入
    if (select(1, &rfds, NULL, NULL, &tv) > 0)
    {
        ch = getchar(); 
    }
	
    return ch;
}
//向左移动
void moveLeft()
{
	int i;
	//在移动之前加容错判断,是否碰到残留物或边框
	for(i = 0; i < 4; i++)
	{
		if(map[shape[i].y][shape[i].x-1] != 0)//说明遇到边框或残留物
		{
			return;//提前结束函数
		}
	}
	//向左移动
	for(i = 0; i < 4; i++)
		shape[i].x--;
}
//向右移动 
void moveRight()
{
	int i;
	//在移动之前加容错判断,是否碰到残留物或边框
	for(i = 0; i < 4; i++)
	{
		if(map[shape[i].y][shape[i].x+1] != 0)//说明遇到边框或残留物
		{
			return;//提前结束函数
		}
	}
	//向右移动
	for(i = 0; i < 4; i++)
		shape[i].x++;
}
//清除满行
void clearFullLine()
{
	int i;
	for(i = 19; i > 0; i--)
	{
		//i代表的是每一行的下标
		if(isFullLine(i))//对每一行进行判断是否是满行,如果是满行返回值是1
		{
			//清除这个第i行这个满行
			clearOneLine(i);
			score += 100;
			addLevel();//是否需要提升游戏难度
			i++;//为了解决连续出现两个满行的bug
		}
	}
}
 
//判断第i行是否是满行
//如果是满行,返回值是1
//如果不是满行,返回值是0 
int isFullLine(int row)
{
	int j;
	for(j = 1; j <= 10; j++)
	{
		if(map[row][j] != 2)//说明当前的row行不是满行
			return 0;
	}
	//如果程序能走到这,说明没有执行过return 0;,肯定是满行
	return 1;
}

//清除第i行这个满行
void clearOneLine(int row)
{
	int i,j;
	//逐行向下移动,覆盖删除
	for(i = row; i > 0; i--)
	{
		for(j = 1; j <= 10; j++)
		{
			map[i][j] = map[i-1][j];
		}
	}
}

//增加游戏难度水平
void addLevel()
{
	if(score >= 500 && level == 1)
	{
		level = 2;
		speed -= 5;
	}
	else if(score >= 1000 && level == 2)
	{
		level == 3;
		speed -= 5;
	}
}

//变换图形
void changeShape()
{
	//变换图形之前,先判断当前是什么图形
	switch(currentShape)
	{
	case 0://说明是田字格
		break;
	case 1://说明是横条
		shape[0].x = shape[1].x;
		shape[0].y = shape[1].y-1;
		shape[2].x = shape[1].x;
		shape[2].y = shape[1].y+1;
		shape[3].x = shape[1].x;
		shape[3].y = shape[1].y+2;
		//旋转之后,变为竖条
		currentShape = 2;//代表状态已经切换到竖条
		break;
	case 2://说明是竖条
		shape[0].x = shape[1].x-1;
		shape[0].y = shape[1].y;
		shape[2].x = shape[1].x+1;
		shape[2].y = shape[1].y;
		shape[3].x = shape[1].x+2;
		shape[3].y = shape[1].y;
		//旋转之后,已经变为了横条
		currentShape = 1;
		break;

	}

}
```

# 7. 游戏结束

```C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F " //终端禁止显示输入
#define STTY_DEF            "stty -raw echo -F " //恢复终端显示输入

//定义结构体,用来代表一个对x,y的坐标点
struct coord
{
	int x;
	int y;
};


void initMap();//初始化背景边框
void showStaticMap();//显示静态背景图
void gotoxy(int x, int y);//控制打印光标的位置
void showDynamicShape();//打印动态图形
void moveDown();//向下移动
void addStaticMap();//添加动态图形到静态背景图中
void drawDynamicShape();//画一个动态图形
static int get_char();//非阻塞输入
void moveLeft();//向左移动
void moveRight();//向右移动
void clearOneLine(int row);//清除一行
int isFullLine(int row);//是否是满行
void clearFullLine();//清除满行
void addLevel();//添加游戏难度水平
void changeShape();//变换图形
int isGameOver();//是否游戏结束

int map[21][12];//背景图
//结构体数组,4个元素,因为有4对x,y坐标点
struct coord shape[4] = {{5, 0},{6, 0},{5, 1},{6, 1}};

int score = 0;//保存游戏得分
int level = 1;//游戏难度水平
int speed = 20;//游戏速度
int currentShape;//用来保存当前的图形

int main(int argc, const char *argv[])
{ 
	int i;
	char cmd;//用来保存输入按键
	srand(time(NULL));//初始化随机种子
	system("reset");//执行reset命令,重置终端
	system(STTY_US TTY_PATH);  
	initMap();//初始化静态背景边框
	drawDynamicShape();//画一个动态图形
	while(1)
	{
		//判断游戏是否结束
		if(isGameOver())
		{
			gotoxy(5, 10);
			printf("\033[44;31mGame Over!!\033[0m ");
			fflush(stdout);
    		system(STTY_DEF TTY_PATH);	//恢复非阻塞IO设置
			gotoxy(0, 22);
			sleep(3);
			return 0;
		}

		showStaticMap();//打印静态背景图
		showDynamicShape();//显示动态图形
		fflush(stdout);//刷新缓存区
		for(i = 0; i < speed; i++)//循环20次等价于1s
		{
			//按键输入
			cmd = get_char();
			if(cmd == 'w')
			{
				changeShape();
				showStaticMap();
				showDynamicShape();
				fflush(stdout);
			}
			else if(cmd == 'a')
			{
				moveLeft();
				showStaticMap();
				showDynamicShape();
				fflush(stdout);
			}
			else if(cmd == 's') 
			{
				break;
			}
			else if(cmd == 'd')
			{
				moveRight();
				showStaticMap();
				showDynamicShape();
				fflush(stdout);
			}
			else if(cmd == 3)//ctrl + c
			{
    			system(STTY_DEF TTY_PATH);	//恢复非阻塞IO设置
				return 0;
			}
			usleep(50000);
		}
		moveDown();//向下移动
	}

	return 0;
}

//初始化背景边框
void initMap()
{
	int i,j;
	printf("\033[?25l");//隐藏光标
	for(i = 0; i < 21; i++)
	{
		for(j = 0; j < 12; j++)
		{
			if(j == 0 || j == 11 || i == 20)//下标0和11列,下标20行,赋值为1,代表边框
			{
				map[i][j] = 1;
			}
		}
	}
}
//显示静态背景图
void showStaticMap()
{
	int i,j;
	//每次打印静态背景边框从0,0开始打印
	//gotoxy(0,0);
	for(i = 0; i < 21; i++)
	{
		gotoxy(0, i);//打印图形的时候,从每行的第0列开始
		for(j = 0; j < 12; j++)
		{
			if(map[i][j] == 1)//说明是边框
				printf("\033[0;31m◆\033[0m ");
			else if(map[i][j] == 2)//说明是残留物
				printf("\033[0;32m■\033[0m ");
			else if(map[i][j] == 0)//说明是空白
				printf("  ");//多敲一个空格,因为一个特殊字符占2个宽度
		}
		printf("\n");
	}
	gotoxy(0, 21);
	printf("\033[0;34m Level:%d      Score:%d\033[0m ",level,score);
}
//控制光标位置函数
void gotoxy(int x, int y)
{ 
	printf("\033[%d;%dH", y+1 , x+1);
}  

//显示动态图形dynamic
void showDynamicShape()
{
	int i;
	for(i = 0; i < 4; i++)
	{
		gotoxy(shape[i].x*2, shape[i].y);//一个特殊图形占两个宽度
		printf("\033[0;33m■\033[0m ");
	}
}
//向下移动
void moveDown()
{
	int i;
	//这个循环,专门用来判断是否能够继续向下移动
	for(i = 0; i < 4; i++)
	{
		//y+1是为了提前下一个身位是否有边框和障碍物
		if(map[shape[i].y+1][shape[i].x] != 0)//说明遇到了1或2, 边框或障碍物
		{//进入这个条件判断,就相当于落到最底部了
			addStaticMap();//添加到静态背景图
			clearFullLine();//清除满行
			drawDynamicShape();//再次画一个新的图形,重新开始从上往下落
			return;//提前结束函数
		}
	}
	//这个循环就是向下移动一个位置
	for(i = 0; i < 4; i++)
	{
		shape[i].y++;
	}
}
//添加到动态图形到静态背景图
void addStaticMap()
{
	int i;
	for(i = 0; i < 4; i++)
		map[shape[i].y][shape[i].x] = 2;
}
//画动态图形
void drawDynamicShape()
{
	currentShape = rand() % 3;
	switch(currentShape)
	{
	case 0://田字格
		//画一个田字格
		shape[0].x = 5;
		shape[0].y = 0;
		shape[1].x = 6;
		shape[1].y = 0;
		shape[2].x = 5;
		shape[2].y = 1;
		shape[3].x = 6;
		shape[3].y = 1;
		break;
	case 1:
		//画一个横条
		shape[0].x = 4;
		shape[0].y = 0;
		shape[1].x = 5;
		shape[1].y = 0;
		shape[2].x = 6;
		shape[2].y = 0;
		shape[3].x = 7;
		shape[3].y = 0;
		break;
	case 2:
		//画一个竖条
		shape[0].x = 5;
		shape[0].y = 0;
		shape[1].x = 5;
		shape[1].y = 1;
		shape[2].x = 5;
		shape[2].y = 2;
		shape[3].x = 5;
		shape[3].y = 3;
		break;
	}
}

//非阻塞输入
static int get_char()
{
    fd_set rfds;
    struct timeval tv;
    int ch = 0;
	
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 10; //设置等待超时时间

    //检测键盘是否有输入
    if (select(1, &rfds, NULL, NULL, &tv) > 0)
    {
        ch = getchar(); 
    }
	
    return ch;
}
//向左移动
void moveLeft()
{
	int i;
	//在移动之前加容错判断,是否碰到残留物或边框
	for(i = 0; i < 4; i++)
	{
		if(map[shape[i].y][shape[i].x-1] != 0)//说明遇到边框或残留物
		{
			return;//提前结束函数
		}
	}
	//向左移动
	for(i = 0; i < 4; i++)
		shape[i].x--;
}
//向右移动 
void moveRight()
{
	int i;
	//在移动之前加容错判断,是否碰到残留物或边框
	for(i = 0; i < 4; i++)
	{
		if(map[shape[i].y][shape[i].x+1] != 0)//说明遇到边框或残留物
		{
			return;//提前结束函数
		}
	}
	//向右移动
	for(i = 0; i < 4; i++)
		shape[i].x++;
}
//清除满行
void clearFullLine()
{
	int i;
	for(i = 19; i > 0; i--)
	{
		//i代表的是每一行的下标
		if(isFullLine(i))//对每一行进行判断是否是满行,如果是满行返回值是1
		{
			//清除这个第i行这个满行
			clearOneLine(i);
			score += 100;
			addLevel();//是否需要提升游戏难度
			i++;//为了解决连续出现两个满行的bug
		}
	}
}
 
//判断第i行是否是满行
//如果是满行,返回值是1
//如果不是满行,返回值是0 
int isFullLine(int row)
{
	int j;
	for(j = 1; j <= 10; j++)
	{
		if(map[row][j] != 2)//说明当前的row行不是满行
			return 0;
	}
	//如果程序能走到这,说明没有执行过return 0;,肯定是满行
	return 1;
}

//清除第i行这个满行
void clearOneLine(int row)
{
	int i,j;
	//逐行向下移动,覆盖删除
	for(i = row; i > 0; i--)
	{
		for(j = 1; j <= 10; j++)
		{
			map[i][j] = map[i-1][j];
		}
	}
}

//增加游戏难度水平
void addLevel()
{
	if(score >= 500 && level == 1)
	{
		level = 2;
		speed -= 5;
	}
	else if(score >= 1000 && level == 2)
	{
		level == 3;
		speed -= 5;
	}
}

//变换图形
void changeShape()
{
	//变换图形之前,先判断当前是什么图形
	switch(currentShape)
	{
	case 0://说明是田字格
		break;
	case 1://说明是横条
		shape[0].x = shape[1].x;
		shape[0].y = shape[1].y-1;
		shape[2].x = shape[1].x;
		shape[2].y = shape[1].y+1;
		shape[3].x = shape[1].x;
		shape[3].y = shape[1].y+2;
		//旋转之后,变为竖条
		currentShape = 2;//代表状态已经切换到竖条
		break;
	case 2://说明是竖条
		shape[0].x = shape[1].x-1;
		shape[0].y = shape[1].y;
		shape[2].x = shape[1].x+1;
		shape[2].y = shape[1].y;
		shape[3].x = shape[1].x+2;
		shape[3].y = shape[1].y;
		//旋转之后,已经变为了横条
		currentShape = 1;
		break;

	}

}


//判断游戏是否结束,第0行是否有残留物
//游戏结束函数返回值是1,未结束返回0
int isGameOver()
{
	int j;
	for(j = 1; j <= 10; j++)
	{
		if(map[0][j] != 0)
			return 1;//游戏结束,第0行,有残留物
	}

	return 0;//游戏未结束
}
```



![image-20230114165033125](C基础笔记.assets/image-20230114165033125.png)

# 9. 旋转碰撞bug

```C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F " //终端禁止显示输入
#define STTY_DEF            "stty -raw echo -F " //恢复终端显示输入

//定义结构体,用来代表一个对x,y的坐标点
struct coord
{
	int x;
	int y;
};


void initMap();//初始化背景边框
void showStaticMap();//显示静态背景图
void gotoxy(int x, int y);//控制打印光标的位置
void showDynamicShape();//打印动态图形
void moveDown();//向下移动
void addStaticMap();//添加动态图形到静态背景图中
void drawDynamicShape();//画一个动态图形
static int get_char();//非阻塞输入
void moveLeft();//向左移动
void moveRight();//向右移动
void clearOneLine(int row);//清除一行
int isFullLine(int row);//是否是满行
void clearFullLine();//清除满行
void addLevel();//添加游戏难度水平
void changeShape();//变换图形
int isGameOver();//是否游戏结束

int map[21][12];//背景图
//结构体数组,4个元素,因为有4对x,y坐标点
struct coord shape[4] = {{5, 0},{6, 0},{5, 1},{6, 1}};

int score = 0;//保存游戏得分
int level = 1;//游戏难度水平
int speed = 20;//游戏速度
int currentShape;//用来保存当前的图形

int main(int argc, const char *argv[])
{ 
	int i;
	char cmd;//用来保存输入按键
	srand(time(NULL));//初始化随机种子
	system("reset");//执行reset命令,重置终端
	system(STTY_US TTY_PATH);  
	initMap();//初始化静态背景边框
	drawDynamicShape();//画一个动态图形
	while(1)
	{
		//判断游戏是否结束
		if(isGameOver())
		{
			gotoxy(5, 10);
			printf("\033[44;31mGame Over!!\033[0m ");
			fflush(stdout);
    		system(STTY_DEF TTY_PATH);	//恢复非阻塞IO设置
			gotoxy(0, 22);
			sleep(3);
			return 0;
		}

		showStaticMap();//打印静态背景图
		showDynamicShape();//显示动态图形
		fflush(stdout);//刷新缓存区
		for(i = 0; i < speed; i++)//循环20次等价于1s
		{
			//按键输入
			cmd = get_char();
			if(cmd == 'w')
			{
				changeShape();
				showStaticMap();
				showDynamicShape();
				fflush(stdout);
			}
			else if(cmd == 'a')
			{
				moveLeft();
				showStaticMap();
				showDynamicShape();
				fflush(stdout);
			}
			else if(cmd == 's') 
			{
				break;
			}
			else if(cmd == 'd')
			{
				moveRight();
				showStaticMap();
				showDynamicShape();
				fflush(stdout);
			}
			else if(cmd == 3)//ctrl + c
			{
    			system(STTY_DEF TTY_PATH);	//恢复非阻塞IO设置
				return 0;
			}
			usleep(50000);
		}
		moveDown();//向下移动
	}

	return 0;
}

//初始化背景边框
void initMap()
{
	int i,j;
	printf("\033[?25l");//隐藏光标
	for(i = 0; i < 21; i++)
	{
		for(j = 0; j < 12; j++)
		{
			if(j == 0 || j == 11 || i == 20)//下标0和11列,下标20行,赋值为1,代表边框
			{
				map[i][j] = 1;
			}
		}
	}
}
//显示静态背景图
void showStaticMap()
{
	int i,j;
	//每次打印静态背景边框从0,0开始打印
	//gotoxy(0,0);
	for(i = 0; i < 21; i++)
	{
		gotoxy(0, i);//打印图形的时候,从每行的第0列开始
		for(j = 0; j < 12; j++)
		{
			if(map[i][j] == 1)//说明是边框
				printf("\033[0;31m◆\033[0m ");
			else if(map[i][j] == 2)//说明是残留物
				printf("\033[0;32m■\033[0m ");
			else if(map[i][j] == 0)//说明是空白
				printf("  ");//多敲一个空格,因为一个特殊字符占2个宽度
		}
		printf("\n");
	}
	gotoxy(0, 21);
	printf("\033[0;34m Level:%d      Score:%d\033[0m ",level,score);
}
//控制光标位置函数
void gotoxy(int x, int y)
{ 
	printf("\033[%d;%dH", y+1 , x+1);
}  

//显示动态图形dynamic
void showDynamicShape()
{
	int i;
	for(i = 0; i < 4; i++)
	{
		gotoxy(shape[i].x*2, shape[i].y);//一个特殊图形占两个宽度
		printf("\033[0;33m■\033[0m ");
	}
}
//向下移动
void moveDown()
{
	int i;
	//这个循环,专门用来判断是否能够继续向下移动
	for(i = 0; i < 4; i++)
	{
		//y+1是为了提前下一个身位是否有边框和障碍物
		if(map[shape[i].y+1][shape[i].x] != 0)//说明遇到了1或2, 边框或障碍物
		{//进入这个条件判断,就相当于落到最底部了
			addStaticMap();//添加到静态背景图
			clearFullLine();//清除满行
			drawDynamicShape();//再次画一个新的图形,重新开始从上往下落
			return;//提前结束函数
		}
	}
	//这个循环就是向下移动一个位置
	for(i = 0; i < 4; i++)
	{
		shape[i].y++;
	}
}
//添加到动态图形到静态背景图
void addStaticMap()
{
	int i;
	for(i = 0; i < 4; i++)
		map[shape[i].y][shape[i].x] = 2;
}
//画动态图形
void drawDynamicShape()
{
	currentShape = rand() % 3;
	switch(currentShape)
	{
	case 0://田字格
		//画一个田字格
		shape[0].x = 5;
		shape[0].y = 0;
		shape[1].x = 6;
		shape[1].y = 0;
		shape[2].x = 5;
		shape[2].y = 1;
		shape[3].x = 6;
		shape[3].y = 1;
		break;
	case 1:
		//画一个横条
		shape[0].x = 4;
		shape[0].y = 0;
		shape[1].x = 5;
		shape[1].y = 0;
		shape[2].x = 6;
		shape[2].y = 0;
		shape[3].x = 7;
		shape[3].y = 0;
		break;
	case 2:
		//画一个竖条
		shape[0].x = 5;
		shape[0].y = 0;
		shape[1].x = 5;
		shape[1].y = 1;
		shape[2].x = 5;
		shape[2].y = 2;
		shape[3].x = 5;
		shape[3].y = 3;
		break;
	}
}

//非阻塞输入
static int get_char()
{
    fd_set rfds;
    struct timeval tv;
    int ch = 0;
	
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 10; //设置等待超时时间

    //检测键盘是否有输入
    if (select(1, &rfds, NULL, NULL, &tv) > 0)
    {
        ch = getchar(); 
    }
	
    return ch;
}
//向左移动
void moveLeft()
{
	int i;
	//在移动之前加容错判断,是否碰到残留物或边框
	for(i = 0; i < 4; i++)
	{
		if(map[shape[i].y][shape[i].x-1] != 0)//说明遇到边框或残留物
		{
			return;//提前结束函数
		}
	}
	//向左移动
	for(i = 0; i < 4; i++)
		shape[i].x--;
}
//向右移动 
void moveRight()
{
	int i;
	//在移动之前加容错判断,是否碰到残留物或边框
	for(i = 0; i < 4; i++)
	{
		if(map[shape[i].y][shape[i].x+1] != 0)//说明遇到边框或残留物
		{
			return;//提前结束函数
		}
	}
	//向右移动
	for(i = 0; i < 4; i++)
		shape[i].x++;
}
//清除满行
void clearFullLine()
{
	int i;
	for(i = 19; i > 0; i--)
	{
		//i代表的是每一行的下标
		if(isFullLine(i))//对每一行进行判断是否是满行,如果是满行返回值是1
		{
			//清除这个第i行这个满行
			clearOneLine(i);
			score += 100;
			addLevel();//是否需要提升游戏难度
			i++;//为了解决连续出现两个满行的bug
		}
	}
}
 
//判断第i行是否是满行
//如果是满行,返回值是1
//如果不是满行,返回值是0 
int isFullLine(int row)
{
	int j;
	for(j = 1; j <= 10; j++)
	{
		if(map[row][j] != 2)//说明当前的row行不是满行
			return 0;
	}
	//如果程序能走到这,说明没有执行过return 0;,肯定是满行
	return 1;
}

//清除第i行这个满行
void clearOneLine(int row)
{
	int i,j;
	//逐行向下移动,覆盖删除
	for(i = row; i > 0; i--)
	{
		for(j = 1; j <= 10; j++)
		{
			map[i][j] = map[i-1][j];
		}
	}
}

//增加游戏难度水平
void addLevel()
{
	if(score >= 500 && level == 1)
	{
		level = 2;
		speed -= 5;
	}
	else if(score >= 1000 && level == 2)
	{
		level == 3;
		speed -= 5;
	}
}

//变换图形
void changeShape()
{
	//变换图形之前,先判断当前是什么图形
	switch(currentShape)
	{
	case 0://说明是田字格
		break;
	case 1://说明是横条
		//在变形之前进行判断,变形之后的位置,是否碰到障碍物或边框
		if(shape[1].y-1 < 0 || shape[1].y+1 >= 20 || shape[1].y+2 >= 20)//变形后,数组越界
		{
			return;
		}
		//对变形之后,3个点,判断是否碰到障碍物或墙
		else if(map[shape[1].y-1][shape[1].x] != 0 || map[shape[1].y+1][shape[1].x] != 0 || map[shape[1].y+2][shape[1].x] != 0 )
		{
			return;
		}

		shape[0].x = shape[1].x;
		shape[0].y = shape[1].y-1;
		shape[2].x = shape[1].x;
		shape[2].y = shape[1].y+1;
		shape[3].x = shape[1].x;
		shape[3].y = shape[1].y+2;
		//旋转之后,变为竖条
		currentShape = 2;//代表状态已经切换到竖条
		break;
	case 2://说明是竖条
		//在变形之前进行判断,变形之后的位置,是否碰到障碍物或边框
		if(shape[1].x-1 <= 0 || shape[1].x+1 >= 11 || shape[1].x+2 >= 11)//变形后,数组越界
		{
			return;
		}
		//对变形之后,3个点,判断是否碰到障碍物或墙
		else if(map[shape[1].y][shape[1].x-1] != 0 || map[shape[1].y][shape[1].x+1] != 0 || map[shape[1].y][shape[1].x+2] != 0 )
		{
			return;
		}
		shape[0].x = shape[1].x-1;
		shape[0].y = shape[1].y;
		shape[2].x = shape[1].x+1;
		shape[2].y = shape[1].y;
		shape[3].x = shape[1].x+2;
		shape[3].y = shape[1].y;
		//旋转之后,已经变为了横条
		currentShape = 1;
		break;

	}

}


//判断游戏是否结束,第0行是否有残留物
//游戏结束函数返回值是1,未结束返回0
int isGameOver()
{
	int j;
	for(j = 1; j <= 10; j++)
	{
		if(map[0][j] != 0)
			return 1;//游戏结束,第0行,有残留物
	}

	return 0;//游戏未结束
}