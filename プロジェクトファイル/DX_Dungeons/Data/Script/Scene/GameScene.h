#pragma once
#include "SceneBase.h"
#include "Effect.h"


class GameScene :
    public SceneBase
{
public:
	GameScene();
	~GameScene();
	void Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<DataFile>& dataFile);
	void Update();
	void Draw() const;

	void EndingInit();
	void EndingUpdate();

	void CalculateReward();

private:

	int m_startTimer;
	
	int m_gettingPoint;

	int m_blindAhpha;

	Effect m_effect;

	int m_whiteAlpha;
	UIPanel m_startUI;
	UIPanel m_endUI;
	int m_endUIHandle[3];
	ExtendPos m_endUIPos[3];
	int m_endUIAlpha[3];

	int m_endTimer;
	bool m_sceneMovefrag;

	bool m_isAppResult;

	// ƒtƒHƒ“ƒg
	int m_fontHandle[2];

	int m_textAlpha;
	bool m_textAlphaFrag;

	int m_hitStopCount;

	int shadowHandle;
};

