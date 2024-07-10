#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#include "olcConsoleGameEngine.h"

class OneLoneCoder_UltimateFPS : public olcConsoleGameEngine
{
public:
	OneLoneCoder_UltimateFPS()
	{
		m_sAppName = L"Ultimate First Person Shooter";
	}

private:
	int nMapWidth = 32;				// 世界尺寸
	int nMapHeight = 32;

	float fPlayerX = 14.7f;			// 起始位置
	float fPlayerY = 8;// 5.09f;
	float fPlayerA = -3.14159f / 2.0f;			// 起始旋转角度
	float fFOV = 3.14159f / 4.0f;	// 视野
	float fDepth = 16.0f;			// 最大渲染距离
	float fSpeed = 5.0f;			// 行走速度
	wstring map;

	olcSprite	*spriteWall;
	olcSprite	*spriteLamp;
	olcSprite	*spriteFireBall;

	float *fDepthBuffer = nullptr;

	struct sObject
	{
		float x;
		float y;
		float vx;
		float vy;
		bool bRemove;
		olcSprite *sprite;
	};

	list<sObject> listObjects;

protected:
	virtual bool OnUserCreate()
	{
		map += L"#########.......#########.......";
		map += L"#...............#...............";
		map += L"#.......#########.......########";
		map += L"#..............##..............#";
		map += L"#......##......##......##......#";
		map += L"#......##..............##......#";
		map += L"#..............##..............#";
		map += L"###............####............#";
		map += L"##.............###.............#";
		map += L"#............####............###";
		map += L"#..............................#";
		map += L"#..............##..............#";
		map += L"#..............##..............#";
		map += L"#...........#####...........####";
		map += L"#..............................#";
		map += L"###..####....########....#######";
		map += L"####.####.......######..........";
		map += L"#...............#...............";
		map += L"#.......#########.......##..####";
		map += L"#..............##..............#";
		map += L"#......##......##.......#......#";
		map += L"#......##......##......##......#";
		map += L"#..............##..............#";
		map += L"###............####............#";
		map += L"##.............###.............#";
		map += L"#............####............###";
		map += L"#..............................#";
		map += L"#..............................#";
		map += L"#..............##..............#";
		map += L"#...........##..............####";
		map += L"#..............##..............#";
		map += L"################################";


		spriteWall = new olcSprite(L"FPSSprites/fps_wall1.spr");
		spriteLamp = new olcSprite(L"FPSSprites/fps_lamp1.spr");
		spriteFireBall = new olcSprite(L"FPSSprites/fps_fireball1.spr");
		fDepthBuffer = new float[ScreenWidth()];

		listObjects = { 
			{ 8.5f, 8.5f, 0.0f, 0.0f, false, spriteLamp },
			{ 7.5f, 7.5f, 0.0f, 0.0f, false, spriteLamp },
			{ 10.5f, 3.5f, 0.0f, 0.0f, false, spriteLamp },			
		};
		return true;
	}


	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// 处理逆时针旋转
		if (m_keys[L'A'].bHeld)
			fPlayerA -= (fSpeed * 0.5f) * fElapsedTime;

		// 处理顺时针旋转
		if (m_keys[L'D'].bHeld)
			fPlayerA += (fSpeed * 0.5f) * fElapsedTime;

		// 处理前进移动和碰撞
		if (m_keys[L'W'].bHeld)
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
		if (m_keys[L'S'].bHeld)
		{
			fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
				fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
			}
		}

		// 处理向右平移移动和碰撞
		if (m_keys[L'E'].bHeld)
		{
			fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;
			fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;
				fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
			}
		}

		// 处理向左平移移动和碰撞
		if (m_keys[L'Q'].bHeld)
		{
			fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;
			fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;
				fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
			}
		}

		// 发射子弹
		if (m_keys[VK_SPACE].bReleased)
		{
			sObject o;
			o.x = fPlayerX;
			o.y = fPlayerY;
			float fNoise = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.1f;
			o.vx = sinf(fPlayerA + fNoise) * 8.0f;
			o.vy = cosf(fPlayerA + fNoise) * 8.0f;
			o.sprite = spriteFireBall;
			o.bRemove = false;
			listObjects.push_back(o);
		}

		for (int x = 0; x < ScreenWidth(); x++)
		{
			// 对于每一列，计算投射到世界空间的射线角度
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth()) * fFOV;

			// 找到到墙壁的距离
			float fStepSize = 0.01f;	  // 射线投射的增量大小，减少以增加分辨	
			float fDistanceToWall = 0.0f; // 墙壁的距离

			bool bHitWall = false;		// 当射线击中墙壁块时设置
			bool bBoundary = false;		// 当射线击中两个墙壁块的边界时设置

			float fEyeX = sinf(fRayAngle); // 玩家空间中射线的单位向量
			float fEyeY = cosf(fRayAngle);

			float fSampleX = 0.0f;

			bool bLit = false;

			// 从玩家位置逐步投射射线，沿射线角度，测试是否和块相交 
			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				// 测试射线是否出界
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;			// 把距离设置成最大深度
					fDistanceToWall = fDepth;
				}
				else
				{
					// 射线在界内，所以测试射线单元是否是墙壁块
					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#')
					{
						// 射线命中墙壁
						bHitWall = true;

						// 确定射线击中墙壁的位置，把块边界分成4段
						float fBlockMidX = (float)nTestX + 0.5f;
						float fBlockMidY = (float)nTestY + 0.5f;

						float fTestPointX = fPlayerX + fEyeX * fDistanceToWall;
						float fTestPointY = fPlayerY + fEyeY * fDistanceToWall;

						float fTestAngle = atan2f((fTestPointY - fBlockMidY), (fTestPointX - fBlockMidX));

						if (fTestAngle >= -3.14159f * 0.25f && fTestAngle < 3.14159f * 0.25f)
							fSampleX = fTestPointY - (float)nTestY;
						if (fTestAngle >= 3.14159f * 0.25f && fTestAngle < 3.14159f * 0.75f)
							fSampleX = fTestPointX - (float)nTestX;
						if (fTestAngle < -3.14159f * 0.25f && fTestAngle >= -3.14159f * 0.75f)
							fSampleX = fTestPointX - (float)nTestX;
						if (fTestAngle >= 3.14159f * 0.75f || fTestAngle < -3.14159f * 0.75f)
							fSampleX = fTestPointY - (float)nTestY;
					}
				}
			}

			// 计算到天花板和地板的距离
			int nCeiling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceToWall);
			int nFloor = ScreenHeight() - nCeiling;

			// 更新深度缓冲区
			fDepthBuffer[x] = fDistanceToWall;

			for (int y = 0; y < ScreenHeight(); y++)
			{
				// 每一行
				if (y <= nCeiling)
					Draw(x, y, L' ');
				else if (y > nCeiling && y <= nFloor)
				{
					// 画墙
					if (fDistanceToWall < fDepth)
					{
						float fSampleY = ((float)y - (float)nCeiling) / ((float)nFloor - (float)nCeiling);
						Draw(x, y, spriteWall->SampleGlyph(fSampleX, fSampleY), spriteWall->SampleColour(fSampleX, fSampleY));
					}
					else
						Draw(x, y, PIXEL_SOLID, 0);
				}
				else // 地板
				{
					Draw(x, y, PIXEL_SOLID, FG_DARK_GREEN);
				}
			}
		}

		// 更新并绘制对象		
		for (auto &object : listObjects)
		{
			// 更新对象物理状态
			object.x += object.vx * fElapsedTime;
			object.y += object.vy * fElapsedTime;

			// 检查对象是否在墙内 - 设置移除标志
			if (map.c_str()[(int)object.x * nMapWidth + (int)object.y] == '#')
				object.bRemove = true;
						
			// 对象能否被看到？
			float fVecX = object.x - fPlayerX;
			float fVecY = object.y - fPlayerY;
			float fDistanceFromPlayer = sqrtf(fVecX*fVecX + fVecY*fVecY);

			float fEyeX = sinf(fPlayerA);
			float fEyeY = cosf(fPlayerA);

			// 计算灯和玩家脚之间的角度，以及玩家的视线方向，确定灯是否在玩家的视野内
			float fObjectAngle = atan2f(fEyeY, fEyeX) - atan2f(fVecY, fVecX);
			if (fObjectAngle < -3.14159f)
				fObjectAngle += 2.0f * 3.14159f;
			if (fObjectAngle > 3.14159f)
				fObjectAngle -= 2.0f * 3.14159f;

			bool bInPlayerFOV = fabs(fObjectAngle) < fFOV / 2.0f;

			if (bInPlayerFOV && fDistanceFromPlayer >= 0.5f && fDistanceFromPlayer < fDepth && !object.bRemove)
			{
				float fObjectCeiling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceFromPlayer);
				float fObjectFloor = ScreenHeight() - fObjectCeiling;
				float fObjectHeight = fObjectFloor - fObjectCeiling;
				float fObjectAspectRatio = (float)object.sprite->nHeight / (float)object.sprite->nWidth;
				float fObjectWidth = fObjectHeight / fObjectAspectRatio;
				float fMiddleOfObject = (0.5f * (fObjectAngle / (fFOV / 2.0f)) + 0.5f) * (float)ScreenWidth();

				// 绘制灯
				for (float lx = 0; lx < fObjectWidth; lx++)
				{
					for (float ly = 0; ly < fObjectHeight; ly++)
					{
						float fSampleX = lx / fObjectWidth;
						float fSampleY = ly / fObjectHeight;
						wchar_t c = object.sprite->SampleGlyph(fSampleX, fSampleY);
						int nObjectColumn = (int)(fMiddleOfObject + lx - (fObjectWidth / 2.0f));
						if (nObjectColumn >= 0 && nObjectColumn < ScreenWidth())
							if (c != L' ' && fDepthBuffer[nObjectColumn] >= fDistanceFromPlayer)
							{							
								Draw(nObjectColumn, fObjectCeiling + ly, c, object.sprite->SampleColour(fSampleX, fSampleY));
								fDepthBuffer[nObjectColumn] = fDistanceFromPlayer;
							}
					}
				}
			}
		}

		// 从对象列表中移除死亡对象
		listObjects.remove_if([](sObject &o) {return o.bRemove; });

		// 显示地图和玩家
		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++)
				Draw(nx+1, ny+1, map[ny * nMapWidth + nx]);
		Draw(1 + (int)fPlayerY, 1 + (int)fPlayerX, L'P');
		
		return true;
	}

};

int main()
{
	OneLoneCoder_UltimateFPS game;
	game.ConstructConsole(320, 240,4,4);
	game.Start();
	return 0;
}