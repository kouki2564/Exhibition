#pragma once
#include <memory>
#include "Config.h"
#include "Effect.h"
#include "SoundManager.h"
#include "DataFile.h"

class SceneBase;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Init();
	void Update();

	bool GetEnd() { return m_isGameEnd; }


private:
	// ƒV[ƒ“‘JˆÚ
	void SceneUpdate();

	Config m_config;

	std::unique_ptr<SceneBase> m_pScene;
	std::shared_ptr<Effect> m_pEffectManager;
	std::shared_ptr<SoundManager> m_pSoundManager;

	std::shared_ptr<DataFile> m_pDataFile;

	bool m_isGameEnd;
};

