#include "TitleScene.h"

TitleScene::TitleScene()
{
	// タイトルロゴ
	m_titleHandle = LoadGraph("Data/Graph/titleRogo.png");
	m_ui.SetUI(m_titleHandle,
		{ Game::kScreenWidth * 0.15f,
		  Game::kScreenWidth * 0.85f,
		  Game::kScreenHeight * 0.1f,
		  Game::kScreenHeight * 0.7f },
		true, 255);
	m_timer = 0;
}


TitleScene::~TitleScene()
{
	m_UIManager.DeleteUIPanal("Title");
	DeleteGraph(m_titleHandle);
}

void TitleScene::Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<DataFile>& dataFile)
{
	m_pCamera->Init(VGet(0,-400,-400));
	m_pCamera->SetTargetPos(VGet(0, 0, 0));
	m_UIManager.SetUIPanel(m_ui, "Title");
	m_pDataFile = dataFile;

	m_pEffectManager = effectmanager;
	m_pEffectManager->RegisterEffect("FieldEffect", LoadEffekseerEffect("Data/Effect/Field.efkefc", 1.0f), VGet(0, 0, 0), VGet(0, 0, 0));
	m_pEffectManager->PlayEffect("FieldEffect", VGet(0, 0, 0), VGet(0, 0, 0));

	m_pSoundManager = soundManager;
	m_pSoundManager->LoopPlay("BGM_Title");
}

void TitleScene::Update()
{
	m_pCamera->Update(false);
	if (m_gameState == GameState::STARTING)
	{
		// フェードイン
		if (m_blindAhpha > 0)
		{
			m_blindAhpha -= 10;
			if (m_blindAhpha <= 0) m_blindAhpha = 0;
		}
		else
		{
			m_gameState = GameState::PLAYING;
		}
	}
	else if (m_gameState == GameState::PLAYING)
	{
		if (m_timer < 40)
		{
			m_timer++;
		}
		else
		{
			m_timer = 0;
		}
		if (m_config.GetInputDown(Input::A))
		{
			m_pSoundManager->OnePlay("SE_Title");
			m_nextSceneNum = Scene::HUB;
				m_pSoundManager->Stop("BGM_Title");
		}
	}
	else if (m_gameState == GameState::ENDING)
	{
		// フェードアウト
		if (m_blindAhpha < 255)
		{
			m_blindAhpha += 10;
			if (m_blindAhpha >= 255)
			{
				m_blindAhpha = 255;
			}
		}
		else
		{
			m_nextSceneNum = Scene::DUNGEON;
		}
	}
}

void TitleScene::Draw() const
{
	m_UIManager.Draw();
	if (m_gameState == GameState::STARTING)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blindAhpha);
		DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	else if (m_gameState == GameState::PLAYING)
	{
		if(m_timer < 20)
			DrawFormatString(Game::kScreenWidth * 0.4, Game::kScreenHeight * 0.65f, 0xe0e000, "PRESS A BUTTON");
	}
	else if (m_gameState == GameState::ENDING)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blindAhpha);
		DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}
