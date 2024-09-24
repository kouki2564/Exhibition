#pragma once
#include <vector>
#include "DxLib.h"

struct ExtendPos
{
	float startX;
	float endX;
	float startY;
	float endY;
};

struct UI
{
	int handle;
	ExtendPos pos;
	bool isApp;
	int alpha;
};

struct numberUI
{
	int number;
	ExtendPos pos;
	bool isApp;
	int alpha;
};

class UIPanel
{
public:
	UIPanel();
	~UIPanel();

	void Draw();

	void MoveUI(int GraphHandle, ExtendPos pos);
	void ChangeAlpha(int GraphHandle, int alpha);
	void SetUI(int GraphHandle, ExtendPos pos, bool isApp, int alpha);
	void SetIsApp(int GraphHandle, bool isApp);
	void SetIsAppNum(int number, bool isApp);
	void SetAllIsApp(bool isApp);

	void SetNumberUI(int number, ExtendPos pos, bool isApp, int alpha);
private:
	std::vector<UI> m_UI;
	std::vector<numberUI> m_numberUI;
	int m_lastAlpha;

	int m_numberHandle;
};

