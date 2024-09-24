#include "UIPanel.h"
#include <cassert>

UIPanel::UIPanel()
{
	m_numberHandle = LoadGraph("Data/Graph/number.png");
	assert(m_numberHandle != -1);
}

UIPanel::~UIPanel()
{
	DeleteGraph(m_numberHandle);
}

void UIPanel::Draw()
{
	// âÊëúï`âÊ
	for (const auto& i : m_UI)
	{
		if (i.isApp)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, i.alpha);
			DrawExtendGraphF(i.pos.startX, i.pos.startY, i.pos.endX, i.pos.endY, i.handle, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
	}
	// êîéöï`âÊ
	for (const auto& i : m_numberUI)
	{
		if (i.isApp)
		{

			// ï`âÊÇ∑ÇÈêîílÇÃåÖêîÇ∆äeåÖÇÃílÇvectorîzóÒÇ…äiî[
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

			// åÖêîï™ÇÃïùÇämï€
			auto range = (i.pos.endX - i.pos.startX) / keta;

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, i.alpha);
			for (int k = keta; k > 0; k--)
			{
				if (k == -1)
				{
					break;
				}
				auto startX = i.pos.startX + ((keta - k) * range);
				auto endX = i.pos.startX + ((keta - k + 1) * range);

				DrawRectExtendGraphF(startX, i.pos.startY, endX, i.pos.endY,num[k-1] * 100, 0, 1000, 1000, m_numberHandle, true);
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

void UIPanel::SetNumberUI(int number, ExtendPos pos, bool isApp, int alpha)
{
	numberUI temp;
	temp.number = number;
	temp.pos = pos;
	temp.isApp = isApp;
	temp.alpha = alpha;

	m_numberUI.push_back(temp);
}
