#pragma once
#include "DxLib.h"
#include <map>
#include <string>
#include "UIPanel.h"

class UIManager
{
public:
	UIManager();
	~UIManager();

	void Draw() const;

	void SetUIPanel(UIPanel& ui, std::string name);
	void PanelUpdate(UIPanel& ui, std::string name);
	void DeleteUIPanal(std::string name);

private:
	std::map<std::string, UIPanel> m_UIs;
};

