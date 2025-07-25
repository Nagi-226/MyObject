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
			if (vColours.empty())
				return olc::BLACK;

			if (vColours.size() == 1)
				return vColours.front().second;

			double i = std::fmod(t, 1.0);
			auto it = vColours.begin();
			while (i > it->first)
				++it;

			if (it == std::begin(vColours))
				return it->second;
			else
			{
				auto it_p = std::prev(it);
				return olc::PixelLerp(it_p->second, it->second,
					float((i - it_p->first) / (it->first - it_p->first)));
			}
		}

		void SetColour(const double d, const olc::Pixel col)
		{
			double i = std::clamp(d, 0.0, 1.0);

			auto it = std::find_if(vColours.begin(), vColours.end(),
				[&i](const std::pair<double, olc::Pixel>& p)
				{
					return p.first == i;
				});

			if (it != std::end(vColours))
			{
				it->second = col;
			}
			else
			{
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
		sAppName = "旋转星形图案";
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

	// 替换圆形生成函数，生成星形图案
	std::vector<olc::vf2d> MakeStarShape(const size_t verts = 10, const float inner_radius = 0.5f)
	{
		std::vector<olc::vf2d> vOut(verts * 2, { 0,0 });
		float anglestep = 3.14159f / float(verts);
		for (size_t i = 0; i < verts * 2; i++)
		{
			float radius = (i % 2 == 0) ? 1.0f : inner_radius;
			vOut[i] = olc::vf2d(radius * cos(anglestep * float(i)), radius * sin(anglestep * float(i)));
		}
		return vOut;
	}

	std::vector<olc::vf2d> vertsStarShape = MakeStarShape();

	void DrawStarDecal(const olc::vf2d& vPos, const float fRadius, const olc::Pixel colour = olc::WHITE)
	{
		std::vector<olc::vf2d> vDraw(vertsStarShape.size(), { 0,0 });
		std::transform(vertsStarShape.begin(), vertsStarShape.end(), vDraw.begin(),
			[&](const olc::vf2d& vIn) { return vIn * fRadius + vPos; });
		SetDecalMode(olc::DecalMode::WIREFRAME);
		DrawPolygonDecal(nullptr, vDraw, vDraw, colour);
		SetDecalMode(olc::DecalMode::NORMAL);
	}

public:
	bool OnUserCreate() override
	{
		guiSlider1 = new olc::QuickGUI::Slider(guiManager,
			{ 550.0f, 10.0f }, { 750.0f, 10.0f }, 0, 256.0f, 200.0f);

		guiSlider2 = new olc::QuickGUI::Slider(guiManager,
			{ 550.0f, 30.0f }, { 750.0f, 30.0f }, -256.0f, 256.0f, 77.0f);

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

	void Reset()
	{
		bFirst = true;
		fAccumulatedTime = 0.0f;
		Clear(olc::BLACK);
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		guiManager.Update(this);

		if (GetKey(olc::Key::R).bPressed || guiButton1->bPressed)
			Reset();

		if (GetKey(olc::Key::SPACE).bHeld || guiButton2->bHeld)
			fAccumulatedTime += fElapsedTime * 5.0f;

		float fFixedGearRadius = guiSlider1->fValue;
		float fMovingGearRadius = guiSlider2->fValue;
		float fPenOffsetRadius = guiSlider3->fValue;

		olc::vf2d vMovingGearPos =
		{
			(fFixedGearRadius - fMovingGearRadius) * cos(fAccumulatedTime),
			(fFixedGearRadius - fMovingGearRadius) * sin(fAccumulatedTime)
		};

		float ratio = fFixedGearRadius / fMovingGearRadius;

		olc::vf2d vPenOffset =
		{
			(fPenOffsetRadius) * cos(-fAccumulatedTime * ratio),
			(fPenOffsetRadius) * sin(-fAccumulatedTime * ratio)
		};

		olc::vf2d vFixedGearPos = { 256.0f, 256.0f };
		olc::vf2d vPenPoint = vFixedGearPos + vMovingGearPos + vPenOffset;

		if (bFirst)
		{
			vOldPenPoint = vPenPoint;
			bFirst = false;
		}

		if (guiCheck1->bChecked)
		{
			DrawStarDecal(vFixedGearPos, fFixedGearRadius, olc::WHITE);
			DrawStarDecal(vFixedGearPos + vMovingGearPos, std::abs(fMovingGearRadius), olc::WHITE);
			DrawStarDecal(vPenPoint, 4, olc::WHITE);
			DrawLineDecal(
				vFixedGearPos + vMovingGearPos + vPenOffset.norm() * fMovingGearRadius, 
				vFixedGearPos + vMovingGearPos - vPenOffset.norm() * fMovingGearRadius, olc::WHITE);
		}

		guiManager.DrawDecal(this);

		if (GetKey(olc::Key::SPACE).bHeld || guiButton2->bHeld)
			DrawLine(vOldPenPoint, vPenPoint, p.Sample(fAccumulatedTime / 300.0f));

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