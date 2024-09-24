#include "UIPanel.h"
#include <cassert>

UIPanel::UIPanel()
{
}

UIPanel::~UIPanel()
{
}

void UIPanel::Draw()
{
	// ‰æ‘œ•`‰æ
	for (const auto& i : m_UI)
	{
		if (i.isApp)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, i.alpha);
			DrawExtendGraphF(i.pos.startX, i.pos.startY, i.pos.endX, i.pos.endY, i.handle, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
	}
	// ”š•`‰æ
	for (const auto& i : m_numberUI)
	{
		if (i.isApp)
		{

			// •`‰æ‚·‚é”’l‚ÌŒ…”‚ÆŠeŒ…‚Ì’l‚ğvector”z—ñ‚ÉŠi”[
			auto keta = 0;
			std::vector<int> num;
			for (int t = 1; t <= 10000; t *= 10)
			{
				if ( i.number == t || i.number / t != 0)
				{
					keta++;
					int pushNum = (i.number % (t * 10)) / t;
					num.push_back(pushNum);
				}
				else
				{
					break;
				}
			}

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
	}
}

void UIPanel::MoveUI(int GraphHandle, ExtendPos pos)
{
	for (auto& i : m_UI)
	{
		if (i.handle == GraphHandle)
		{
			i.pos = pos;
		}
	}
}

void UIPanel::ChangeAlpha(int GraphHandle, int alpha)
{

	for (auto& i : m_UI)
	{
		if (i.handle == GraphHandle)
		{
			i.alpha = alpha;

		}
	}
}

void UIPanel::SetUI(int GraphHandle, ExtendPos pos, bool isApp, int alpha)
{
	UI temp;
	temp.handle = GraphHandle;
	temp.pos = pos;
	temp.isApp = isApp;
	temp.alpha = alpha;

	m_UI.push_back(temp);
}

void UIPanel::SetIsApp(int GraphHandle, bool isApp)
{
	for (auto& i : m_UI)
	{
		if (i.handle == GraphHandle)
		{
			i.isApp = isApp;
		}
	}
}

void UIPanel::SetIsAppNum(int number, bool isApp)
{

	for (auto& i : m_numberUI)
	{
		if (i.number== number)
		{
			i.isApp = isApp;
		}
	}
}

void UIPanel::SetAllIsApp(bool isApp)
{
	for (auto& i : m_UI)
	{
		i.isApp = isApp;
	}
}
