#include "SceneManager.h"
#include "TitleScene.h"
#include "HubScene.h"
#include "GameScene.h"
#include "DebugScene.h"

SceneManager::SceneManager()
{
	m_pScene = std::make_unique<TitleScene>();
	m_pEffectManager = std::make_shared<Effect>();
	m_pSoundManager = std::make_shared<SoundManager>();
	m_pDataFile = std::make_shared<DataFile>();
	m_isGameEnd = false;
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	m_pScene->Init(m_pEffectManager, m_pSoundManager, m_pDataFile);
}

void SceneManager::Update()
{
	
	m_pScene->Update();
	m_pEffectManager->UpdateEffect();

	m_pScene->Draw();
	m_pEffectManager->DrawEffect();

	m_pScene->SwitchSceneMoveFrag();
	SceneUpdate();
	m_isGameEnd = m_pScene->GetEnd();
}

void SceneManager::SceneUpdate()
{

	if (m_pScene->GetIsMoveScene())
	{

		if (m_pScene->GetNextSceneNum() == Scene::TITLE)
		{
			m_pScene = std::make_unique<TitleScene>();
		}
		else if (m_pScene->GetNextSceneNum() == Scene::HUB)
		{
			m_pScene = std::make_unique<HubScene>();
		}
		else if (m_pScene->GetNextSceneNum() == Scene::DUNGEON)
		{
			m_pScene = std::make_unique<GameScene>();
		}
		else if (m_pScene->GetNextSceneNum() == Scene::DEBUG)
		{
			m_pScene = std::make_unique<DebugScene>();
		}
		else
		{
			assert(false);
		}
		m_pScene->Init(m_pEffectManager, m_pSoundManager, m_pDataFile);
	}
}
