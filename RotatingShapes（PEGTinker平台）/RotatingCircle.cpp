#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_QUICKGUI
#include "extensions/olcPGEX_QuickGUI.h"

namespace olc
{
	class Palette
	{
	public:
		enum class Stock
		{
			Empty,
			Greyscale,
			ColdHot,
			Spectrum,
		};

	public:
		Palette(const Stock stock = Stock::Empty)
		{
			switch (stock)
			{
			case Stock::Empty:
				vColours.clear();
				break;
			case Stock::Greyscale:
				vColours =
				{
					{0.0, olc::BLACK}, {1.0, olc::WHITE}
				};
				break;
			case Stock::ColdHot:
				vColours =
				{
					{0.0, olc::CYAN}, {0.5, olc::BLACK}, {1.0, olc::YELLOW}
				};
				break;
			case Stock::Spectrum:
				vColours =
				{
					{0.0 / 6.0, olc::RED},
					{1.0 / 6.0, olc::YELLOW},
					{2.0 / 6.0, olc::GREEN},
					{3.0 / 6.0, olc::CYAN},
					{4.0 / 6.0, olc::BLUE},
					{5.0 / 6.0, olc::MAGENTA},
					{6.0 / 6.0, olc::RED}
				};
				break;
			}
		}

	public:
		olc::Pixel Sample(const double t) const
		{
			// 返回显著的采样值
			if (vColours.empty())
				return olc::BLACK;

			if (vColours.size() == 1)
				return vColours.front().second;

			// 迭代颜色条目，直到找到第一个位置大于采样点的条目
			double i = std::fmod(t, 1.0); // std::clamp(t, 0.0, 1.0);
			auto it = vColours.begin();
			while (i > it->first)
				++it;

			// 如果是第一个，就返回
			if (it == std::begin(vColours))
				return it->second;
			else
			{
				// 否则获取前一个，在两者之间按比例插值
				auto it_p = std::prev(it);
				return olc::PixelLerp(it_p->second, it->second,
					float((i - it_p->first) / (it->first - it_p->first)));
			}
		}

		void SetColour(const double d, const olc::Pixel col)
		{
			double i = std::clamp(d, 0.0, 1.0);

			// 如果d已经存在，就替换
			auto it = std::find_if(vColours.begin(), vColours.end(),
				[&i](const std::pair<double, olc::Pixel>& p)
				{
					return p.first == i;
				});

			if (it != std::end(vColours))
			{
				// 替换颜色条目
				it->second = col;
			}
			else
			{
				// 排序向量
				vColours.push_back({ i, col });
				std::sort(vColours.begin(), vColours.end(),
					[](const std::pair<double, olc::Pixel>& p1, std::pair<double, olc::Pixel>& p2)
					{
						return p2.first > p1.first;
					});
			}
		}

	private:
		std::vector<std::pair<double, olc::Pixel>> vColours;
	};
}

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "旋转圆图案";
	}

	olc::QuickGUI::Manager guiManager;
	olc::QuickGUI::Slider* guiSlider1 = nullptr;
	olc::QuickGUI::Slider* guiSlider2 = nullptr;
	olc::QuickGUI::Slider* guiSlider3 = nullptr;
	olc::QuickGUI::Button* guiButton1 = nullptr;
	olc::QuickGUI::Button* guiButton2 = nullptr;
	olc::QuickGUI::CheckBox* guiCheck1 = nullptr;

	olc::vf2d vOldPenPoint;
	bool bFirst = true;

	float fAccumulatedTime = 0.0f;

	olc::Palette p;


	// PGE 默认没有 DrawCircleDecal 例程，原因有很多，所以我在这做了个用预制单位圆的例程
	std::vector<olc::vf2d> MakeUnitCircle(const size_t verts = 64)
	{
		std::vector<olc::vf2d> vOut(verts, { 0,0 });
		float anglestep = 2.0f * 3.14159f / float(verts-1);
		for (size_t i = 0; i < verts; i++)	
			vOut[i] = olc::vf2d(1.0f, anglestep * float(i)).cart();
		return vOut;
	}
	std::vector<olc::vf2d> vertsUnitCircle = MakeUnitCircle();
	void DrawCircleDecal(const olc::vf2d& vPos, const float fRadius, const olc::Pixel colour = olc::WHITE)
	{
		std::vector<olc::vf2d> vDraw(vertsUnitCircle.size(), {0,0});
		std::transform(vertsUnitCircle.begin(), vertsUnitCircle.end(), vDraw.begin(),
			[&](const olc::vf2d& vIn) { return vIn * fRadius + vPos; });
		SetDecalMode(olc::DecalMode::WIREFRAME);
		DrawPolygonDecal(nullptr, vDraw, vDraw, colour);
		SetDecalMode(olc::DecalMode::NORMAL);
	}

public:
	bool OnUserCreate() override
	{
		// 外齿轮半径
		guiSlider1 = new olc::QuickGUI::Slider(guiManager,
			{ 550.0f, 10.0f }, { 750.0f, 10.0f }, 0, 256.0f, 200.0f);

		// 内齿轮半径
		guiSlider2 = new olc::QuickGUI::Slider(guiManager,
			{ 550.0f, 30.0f }, { 750.0f, 30.0f }, -256.0f, 256.0f, 77.0f);

		// 笔尖半径
		guiSlider3 = new olc::QuickGUI::Slider(guiManager,
			{ 550.0f, 50.0f }, { 750.0f, 50.0f }, 0, 256.0f, 65.0f);

		guiButton1 = new olc::QuickGUI::Button(guiManager,
			"清除所有", { 550.0f, 80.0f }, { 100.0f, 16.0f });

		guiCheck1 = new olc::QuickGUI::CheckBox(guiManager,
			"显示齿轮", true, { 660.0f, 80.0f }, { 90.0f, 16.0f });

		guiButton2 = new olc::QuickGUI::Button(guiManager,
			"画！",{ 550.0f, 110.0f }, { 200.0f, 20.0f });

		p = olc::Palette(olc::Palette::Stock::Spectrum);

		Reset();
		return true;
	}

	// 恢复到已知状态
	void Reset()
	{		
		bFirst = true;
		fAccumulatedTime = 0.0f;
		Clear(olc::BLACK);
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		guiManager.Update(this);

		// 按"R"重置图像
		if (GetKey(olc::Key::R).bPressed || guiButton1->bPressed)
			Reset();

		// 推进“时间”（根据需求）
		if (GetKey(olc::Key::SPACE).bHeld || guiButton2->bHeld)
			fAccumulatedTime += fElapsedTime * 5.0f;


		float fFixedGearRadius = guiSlider1->fValue;  // 200.0f;
		float fMovingGearRadius = guiSlider2->fValue; // 77.0f;
		float fPenOffsetRadius = guiSlider3->fValue;  // 65.0f;

		// 内齿轮的中心点
		olc::vf2d vMovingGearPos =
		{
			(fFixedGearRadius - fMovingGearRadius) * cos(fAccumulatedTime),
			(fFixedGearRadius - fMovingGearRadius) * sin(fAccumulatedTime)
		};

		// 确定齿轮之间的齿轮比
		float ratio = fFixedGearRadius / fMovingGearRadius;

		// 笔尖从内齿轮中心的偏移量，方向是反向
		olc::vf2d vPenOffset =
		{
			(fPenOffsetRadius) * cos(-fAccumulatedTime * ratio),
			(fPenOffsetRadius) * sin(-fAccumulatedTime * ratio)
		};

		olc::vf2d vFixedGearPos = { 256.0f, 256.0f };
		olc::vf2d vPenPoint = vFixedGearPos + vMovingGearPos + vPenOffset;
		
		// 检查有没有正在绘制第一个点，避免从某个旧位置开始破坏图案
		if (bFirst)
		{
			vOldPenPoint = vPenPoint;
			bFirst = false;
		}

		// 画“齿轮”
		if (guiCheck1->bChecked)
		{
			// 作“Decals”绘制，让它们出现在图案上
			DrawCircleDecal(vFixedGearPos, fFixedGearRadius, olc::WHITE);
			DrawCircleDecal(vFixedGearPos + vMovingGearPos, std::abs(fMovingGearRadius), olc::WHITE);
			DrawCircleDecal(vPenPoint, 4, olc::WHITE);
			DrawLineDecal(
				vFixedGearPos + vMovingGearPos + vPenOffset.norm() * fMovingGearRadius, 
				vFixedGearPos + vMovingGearPos - vPenOffset.norm() * fMovingGearRadius, olc::WHITE);
		}

		// 绘制 GUI
		guiManager.DrawDecal(this);
		
		// 从上一个笔点到新笔点绘制一条线
		if(GetKey(olc::Key::SPACE).bHeld || guiButton2->bHeld)
			DrawLine(vOldPenPoint, vPenPoint, p.Sample(fAccumulatedTime / 300.0f));

		// 存储旧笔点
		vOldPenPoint = vPenPoint;
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(768, 512, 1, 1))
		demo.Start();
	return 0;
}