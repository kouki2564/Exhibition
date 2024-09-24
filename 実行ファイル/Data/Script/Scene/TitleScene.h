#pragma once
#include "SceneBase.h"

class TitleScene :
    public SceneBase
{
public:
	TitleScene();
	~TitleScene();

	void Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<DataFile>& dataFile);
	void Update();
	void Draw() const;

private:
	UIPanel m_ui;
	int m_titleHandle;
	int m_timer;
};

