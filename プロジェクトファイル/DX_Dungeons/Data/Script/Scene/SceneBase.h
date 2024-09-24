#pragma once
#include <memory>
#include <map>
#include <string>
#include "Camera.h"
#include "Light.h"
#include "Config.h"
#include "UIManager.h"
#include "Effect.h"
#include "SoundManager.h"
#include "DataFile.h"

class Player;
class Boss;


enum GameState
{
	STARTING,
	PLAYING,
	PAUSE,
	ENDING
};

enum Scene
{
	TITLE,		// タイトル
	HUB,		// 拠点
	DUNGEON,	// ダンジョン
	DEBUG = 99	// デバック
};

class SceneBase :
	public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase()
	{
		m_pCamera = std::make_shared<Camera>();
		m_pLight = std::make_shared<Light>();
		m_nextSceneNum = -1;
		m_isMoveScene = false;
		m_sceneModelHandle = -1;
		m_blindAhpha = 255;
		m_gameState = GameState::STARTING;
		m_isGameEnd = false;
	}
	virtual ~SceneBase() { MV1DeleteModel(m_sceneModelHandle); }

	virtual void Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<DataFile>& dataFile) = 0;
	virtual void Update() = 0;
	virtual void Draw() const = 0;


	//	シーン移動フラグ取得
	bool GetIsMoveScene() { return m_isMoveScene; }

	// シーン移動先取得
	int GetNextSceneNum() 
	{
		assert(m_nextSceneNum != -1);
		return m_nextSceneNum; 
	}

	// 次のシーン確定時、シーン変更フラグをtrueに
	void SwitchSceneMoveFrag() 
	{
		if (m_nextSceneNum != -1)
		{
			m_isMoveScene = true;
		}
	}

	SendData GetData() { return m_data; }


	bool GetEnd() { return m_isGameEnd; }

protected:
	// オブジェクト関連をすべてmap配列としてまとめようか考えたが、
	// 視認性とメモリ使用量を懸念して、
	// 一旦コンポーネントベースに...
	// std::map<std::string, std::shared_ptr<ObjectBase>> m_pObjects;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Light> m_pLight;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Boss> m_pBoss;
	UIManager m_UIManager;
	Config m_config;

	std::shared_ptr<Effect> m_pEffectManager;
	std::shared_ptr<SoundManager> m_pSoundManager;
	std::shared_ptr<DataFile> m_pDataFile;

	SendData m_data;

	int m_nextSceneNum;
	bool m_isMoveScene;

	int m_gameState;

	// フェード用
	int m_blindAhpha;

	int m_sceneModelHandle;

	bool m_isGameEnd;
};

