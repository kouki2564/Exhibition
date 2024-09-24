#include "UIManager.h"

UIManager::UIManager()
{

}

UIManager::~UIManager()
{
}

void UIManager::Draw()const
{
	for (auto i : m_UIs)
	{
		i.second.Draw();
	}
}

void UIManager::SetUIPanel(UIPanel& ui, std::string name)
{
	m_UIs.insert(std::make_pair(name, ui));
}

void UIManager::PanelUpdate(UIPanel& ui, std::string name)
{
	m_UIs[name] = ui;
}

void UIManager::DeleteUIPanal(std::string name)
{
	m_UIs.erase(name);
}
