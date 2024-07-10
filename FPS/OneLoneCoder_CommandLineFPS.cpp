#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>

int nScreenWidth = 120;			// 控制台屏幕宽度（列）
int nScreenHeight = 40;			// 控制台屏幕高度（行）
int nMapWidth = 16;				// 世界尺寸
int nMapHeight = 16;

float fPlayerX = 14.7f;			// 玩家起始位置
float fPlayerY = 5.09f;
float fPlayerA = 0.0f;			// 玩家起始旋转角度
float fFOV = 3.14159f / 4.0f;	// 视野
float fDepth = 16.0f;			// 最大渲染距离
float fSpeed = 5.0f;			// 行走速度

int main()
{
	// 创建屏幕缓冲区
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// 创建世界地图 # = 墙壁块, . = 空间
	wstring map;
	map += L"#########.......";
	map += L"#...............";
	map += L"#.......########";
	map += L"#..............#";
	map += L"#......##......#";
	map += L"#......##......#";
	map += L"#..............#";
	map += L"###............#";
	map += L"##.............#";
	map += L"#......####..###";
	map += L"#......#.......#";
	map += L"#......#.......#";
	map += L"#..............#";
	map += L"#......#########";
	map += L"#..............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();
	
	while (1)
	{
		// 需要每帧的时间差来计算移动速度的修改，确保一致的移动，因为光线追踪是非确定性的
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();


		// 处理逆时针旋转
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			fPlayerA -= (fSpeed * 0.75f) * fElapsedTime;

		// 处理顺时针旋转
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			fPlayerA += (fSpeed * 0.75f) * fElapsedTime;
		
		// 处理前进移动和碰撞
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;;
				fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;;
			}			
		}

		// 处理后退移动和碰撞
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
				fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
			}
		}

		for (int x = 0; x < nScreenWidth; x++)
		{
			// 对于每一列，计算投射到世界空间的射线角度
			float fRayAngle = (fPlayerA - fFOV/2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			// 找到到墙壁的距离
			float fStepSize = 0.1f;		  // 射线投射的增量大小，减少以增加分辨										
			float fDistanceToWall = 0.0f; // 墙壁的距离

			bool bHitWall = false;		// 当射线击中墙壁块时设置
			bool bBoundary = false;		// 当射线击中两个墙壁块的边界时设置

			float fEyeX = sinf(fRayAngle); // 玩家空间中射线的单位向量
			float fEyeY = cosf(fRayAngle);

			// 从玩家位置逐步投射射线，沿射线角度，测试是不是和块相交
			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);
				
				// 测试射线有没有出界
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;			// 把距离设置成最大深度
					fDistanceToWall = fDepth;
				}
				else
				{
					// 射线在界内，所以测试射线单元是不是墙壁块
					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#')
					{
						// 射线击中了墙壁
						bHitWall = true;

						// 为了突出瓷砖边界，从瓷砖的每个角落投射一条射线到玩家。
						// 这条射线与渲染射线的重合度越高，我们就越接近瓷砖边界，
						// 进行阴影处理以增加墙壁的细节
						vector<pair<float, float>> p;

						// 测试击中瓷砖的每个角，存储从玩家的距离和两个射线的计算点积
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								// 从角到视线的角度
								float vy = (float)nTestY + ty - fPlayerY;
								float vx = (float)nTestX + tx - fPlayerX;
								float d = sqrt(vx*vx + vy*vy); 
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}

						// 将对从近到远排序
						sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) {return left.first < right.first; });
						
						// 前两个/三个是最近的（不会看见全部四个）
						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) bBoundary = true;
						if (acos(p.at(1).second) < fBound) bBoundary = true;
						if (acos(p.at(2).second) < fBound) bBoundary = true;
					}
				}
			}
		
			// 计算到天花板和地板的距离
			int nCeiling = (float)(nScreenHeight/2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			// 基于距离的墙壁着色
			short nShade = ' ';
			if (fDistanceToWall <= fDepth / 4.0f)			nShade = 0x2588;	// 非常近	
			else if (fDistanceToWall < fDepth / 3.0f)		nShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f)		nShade = 0x2592;
			else if (fDistanceToWall < fDepth)				nShade = 0x2591;
			else											nShade = ' ';		// 非常远

			if (bBoundary)		nShade = ' '; // 黑掉它
			
			for (int y = 0; y < nScreenHeight; y++)
			{
				// 每一行
				if(y <= nCeiling)
					screen[y*nScreenWidth + x] = ' ';
				else if(y > nCeiling && y <= nFloor)
					screen[y*nScreenWidth + x] = nShade;
				else // 地板
				{				
					// 基于距离的地板着色
					float b = 1.0f - (((float)y -nScreenHeight/2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)		nShade = '#';
					else if (b < 0.5)	nShade = 'x';
					else if (b < 0.75)	nShade = '.';
					else if (b < 0.9)	nShade = '-';
					else				nShade = ' ';
					screen[y*nScreenWidth + x] = nShade;
				}
			}
		}

		// 显示统计信息
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, 1.0f/fElapsedTime);

		// 显示地图
		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++)
			{
				screen[(ny+1)*nScreenWidth + nx] = map[ny * nMapWidth + nx];
			}
		screen[((int)fPlayerX+1) * nScreenWidth + (int)fPlayerY] = 'P';

		// 显示帧
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}