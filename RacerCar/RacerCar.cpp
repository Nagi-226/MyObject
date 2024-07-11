#include <iostream>
#include <string>
using namespace std;

#include "olcConsoleGameEngine.h"


class OneLoneCoder_FormulaOLC : public olcConsoleGameEngine
{
public:
	OneLoneCoder_FormulaOLC()
	{
		m_sAppName = L"Formula OLC";
	}

private:

	float fDistance = 0.0f;			// 赛车在赛道上行驶的距离
	float fCurvature = 0.0f;		// 当前赛道曲率，在赛道段之间插值
	float fTrackCurvature = 0.0f;	// 赛道曲率的累计
	float fTrackDistance = 0.0f;	// 赛道总里程

	float fCarPos = 0.0f;			// 当前赛车位置
	float fPlayerCurvature = 0.0f;	// 玩家曲率的累计
	float fSpeed = 0.0f;			// 当前玩家速度

	vector<pair<float, float>> vecTrack; // 赛道段，弯曲程度，赛道段的长度

	list<float> listLapTimes;		// 之前圈速的列表
	float fCurrentLapTime;			// 当前圈速

protected:
	// 调用olcConsoleGameEngine 
	virtual bool OnUserCreate()
	{
		// 定义赛道
		vecTrack.push_back(make_pair(0.0f, 10.0f));		// 起点/终点线的短段
		vecTrack.push_back(make_pair(0.0f, 200.0f));
		vecTrack.push_back(make_pair(1.0f, 200.0f));
		vecTrack.push_back(make_pair(0.0f, 400.0f));
		vecTrack.push_back(make_pair(-1.0f, 100.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));
		vecTrack.push_back(make_pair(-1.0f, 200.0f));
		vecTrack.push_back(make_pair(1.0f, 200.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));
		vecTrack.push_back(make_pair(0.2f, 500.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));

		// 计算赛道的总距离，以方便设置圈速
		for (auto t : vecTrack)
			fTrackDistance += t.second;

		listLapTimes = { 0,0,0,0,0 };
		fCurrentLapTime = 0.0f;

		return true;
	}


	// 调用olcConsoleGameEngine 
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// 处理控制输入
		int nCarDirection = 0;

		if (m_keys[VK_UP].bHeld)
			fSpeed += 2.0f * fElapsedTime;
		else
			fSpeed -= 1.0f * fElapsedTime;

		// 赛车曲率是左右输入的累计，和速度成反比
		if (m_keys[VK_LEFT].bHeld)
		{
			fPlayerCurvature -= 0.7f * fElapsedTime * (1.0f - fSpeed / 2.0f);
			nCarDirection = -1;
		}

		if (m_keys[VK_RIGHT].bHeld)
		{
			fPlayerCurvature += 0.7f * fElapsedTime * (1.0f - fSpeed / 2.0f);
			nCarDirection = +1;
		}

		// 如果赛车曲率和赛道曲率相差太大，就减速

		if (fabs(fPlayerCurvature - fTrackCurvature) >= 0.8f)
			fSpeed -= 5.0f * fElapsedTime;

		// 限制速度
		if (fSpeed < 0.0f)	fSpeed = 0.0f;
		if (fSpeed > 1.0f)	fSpeed = 1.0f;

		// 根据汽车速度沿赛道移动
		fDistance += (70.0f * fSpeed) * fElapsedTime;

		// 获取赛道上的点
		float fOffset = 0;
		int nTrackSection = 0;

		// 圈速计时和计数
		fCurrentLapTime += fElapsedTime;
		if (fDistance >= fTrackDistance)
		{
			fDistance -= fTrackDistance;
			listLapTimes.push_front(fCurrentLapTime);
			listLapTimes.pop_back();
			fCurrentLapTime = 0.0f;
		}

		// 查找赛道上的位置
		while (nTrackSection < vecTrack.size() && fOffset <= fDistance)
		{
			fOffset += vecTrack[nTrackSection].second;
			nTrackSection++;
		}

		// 向目标赛道曲率插值
		float fTargetCurvature = vecTrack[nTrackSection - 1].first;
		float fTrackCurveDiff = (fTargetCurvature - fCurvature) * fElapsedTime * fSpeed;

		// 累计玩家曲率
		fCurvature += fTrackCurveDiff;

		// 累计赛道曲率
		fTrackCurvature += (fCurvature)*fElapsedTime * fSpeed;

		// 绘制天空 - 浅蓝色和深蓝色
		for (int y = 0; y < ScreenHeight() / 2; y++)
			for (int x = 0; x < ScreenWidth(); x++)
				Draw(x, y, y < ScreenHeight() / 4 ? PIXEL_HALF : PIXEL_SOLID, FG_DARK_BLUE);

		// 绘制风景 - 我们的山丘是一个修正的正弦波，其中相位由累计的赛道曲率调整
		for (int x = 0; x < ScreenWidth(); x++)
		{
			int nHillHeight = (int)(fabs(sinf(x * 0.01f + fTrackCurvature) * 16.0f));
			for (int y = (ScreenHeight() / 2) - nHillHeight; y < ScreenHeight() / 2; y++)
				Draw(x, y, PIXEL_SOLID, FG_DARK_YELLOW);
		}


		// 绘制赛道 - 每一行分为草地、夹板和赛道
		for (int y = 0; y < ScreenHeight() / 2; y++)
			for (int x = 0; x < ScreenWidth(); x++)
			{
				// 透视用于修改赛道行段的宽度
				float fPerspective = (float)y / (ScreenHeight() / 2.0f);
				float fRoadWidth = 0.1f + fPerspective * 0.8f; // 最小 10% 最大 90%
				float fClipWidth = fRoadWidth * 0.15f;
				fRoadWidth *= 0.5f;	// 减半，因为赛道在赛道中心对称

				// 取决于中点的位置，通过当前赛道曲率定义。
				float fMiddlePoint = 0.5f + fCurvature * powf((1.0f - fPerspective), 3);

				// 计算段边界
				int nLeftGrass = (fMiddlePoint - fRoadWidth - fClipWidth) * ScreenWidth();
				int nLeftClip = (fMiddlePoint - fRoadWidth) * ScreenWidth();
				int nRightClip = (fMiddlePoint + fRoadWidth) * ScreenWidth();
				int nRightGrass = (fMiddlePoint + fRoadWidth + fClipWidth) * ScreenWidth();

				int nRow = ScreenHeight() / 2 + y;

				// 用周期性振荡函数给出线条，其中相位由赛道周围的距离控制
				int nGrassColour = sinf(20.0f * powf(1.0f - fPerspective, 3) + fDistance * 0.1f) > 0.0f ? FG_GREEN : FG_DARK_GREEN;
				int nClipColour = sinf(80.0f * powf(1.0f - fPerspective, 2) + fDistance) > 0.0f ? FG_RED : FG_WHITE;

				// 起点直道改变赛道颜色，通知玩家圈速重置
				int nRoadColour = (nTrackSection - 1) == 0 ? FG_WHITE : FG_GREY;

				// 绘制行段
				if (x >= 0 && x < nLeftGrass)
					Draw(x, nRow, PIXEL_SOLID, nGrassColour);
				if (x >= nLeftGrass && x < nLeftClip)
					Draw(x, nRow, PIXEL_SOLID, nClipColour);
				if (x >= nLeftClip && x < nRightClip)
					Draw(x, nRow, PIXEL_SOLID, nRoadColour);
				if (x >= nRightClip && x < nRightGrass)
					Draw(x, nRow, PIXEL_SOLID, nClipColour);
				if (x >= nRightGrass && x < ScreenWidth())
					Draw(x, nRow, PIXEL_SOLID, nGrassColour);
			}

		// 绘制汽车 - 赛车在赛道上的位置和当前累计的赛道曲率和当前累计的玩家曲率之间的差异成正比，如果它们相似，赛车在赛道中间
		fCarPos = fPlayerCurvature - fTrackCurvature;
		int nCarPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * fCarPos) / 2.0) - 7; 

		// 绘制赛车图案。
		switch (nCarDirection)
		{
		case 0:
			DrawStringAlpha(nCarPos, 80, L"   ||####||   ");
			DrawStringAlpha(nCarPos, 81, L"      ##      ");
			DrawStringAlpha(nCarPos, 82, L"     ####     ");
			DrawStringAlpha(nCarPos, 83, L"     ####     ");
			DrawStringAlpha(nCarPos, 84, L"|||  ####  |||");
			DrawStringAlpha(nCarPos, 85, L"|||########|||");
			DrawStringAlpha(nCarPos, 86, L"|||  ####  |||");
			break;

		case +1:
			DrawStringAlpha(nCarPos, 80, L"      //####//");
			DrawStringAlpha(nCarPos, 81, L"         ##   ");
			DrawStringAlpha(nCarPos, 82, L"       ####   ");
			DrawStringAlpha(nCarPos, 83, L"      ####    ");
			DrawStringAlpha(nCarPos, 84, L"///  ####//// ");
			DrawStringAlpha(nCarPos, 85, L"//#######///O ");
			DrawStringAlpha(nCarPos, 86, L"/// #### //// ");
			break;

		case -1:
			DrawStringAlpha(nCarPos, 80, L"\\\\####\\\\      ");
			DrawStringAlpha(nCarPos, 81, L"   ##         ");
			DrawStringAlpha(nCarPos, 82, L"   ####       ");
			DrawStringAlpha(nCarPos, 83, L"    ####      ");
			DrawStringAlpha(nCarPos, 84, L" \\\\\\\\####  \\\\\\");
			DrawStringAlpha(nCarPos, 85, L" O\\\\\\#######\\\\");
			DrawStringAlpha(nCarPos, 86, L" \\\\\\\\ #### \\\\\\");
			break;
		}

		// 绘制统计信息
		DrawString(0, 0, L"Distance: " + to_wstring(fDistance));
		DrawString(0, 1, L"Target Curvature: " + to_wstring(fCurvature));
		DrawString(0, 2, L"Player Curvature: " + to_wstring(fPlayerCurvature));
		DrawString(0, 3, L"Player Speed    : " + to_wstring(fSpeed));
		DrawString(0, 4, L"Track Curvature : " + to_wstring(fTrackCurvature));

		auto disp_time = [](float t) // 小的 lambda，将浮点秒数转换为分钟:秒:毫秒字符串
		{
			int nMinutes = t / 60.0f;
			int nSeconds = t - (nMinutes * 60.0f);
			int nMilliSeconds = (t - (float)nSeconds) * 1000.0f;
			return to_wstring(nMinutes) + L"." + to_wstring(nSeconds) + L":" + to_wstring(nMilliSeconds);
		};

		// 显示当前圈速
		DrawString(10, 8, disp_time(fCurrentLapTime));

		// 显示最后 5 圈的时间
		int j = 10;
		for (auto l : listLapTimes)
		{
			DrawString(10, j, disp_time(l));
			j++;
		}

		return true;
	}
};


int main()
{
	// 使用从 olcConsoleGameEngine 派生的应用
	OneLoneCoder_FormulaOLC game;
	game.ConstructConsole(120, 100, 16, 16);
	game.Start();

	return 0;
}