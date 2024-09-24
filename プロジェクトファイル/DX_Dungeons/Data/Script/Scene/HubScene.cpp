#include "HubScene.h"
#include "Config.h"

namespace
{
	// トロフィーに使用するポイント
	constexpr int kTrophyPoint[6] = {99, 490, 1900, 6000, 13000, 30000};
}

HubScene::HubScene()
{
	m_backHandle = LoadGraph("Data/Graph/sora.jpg");
	m_sceneModelHandle = MV1LoadModel("Data/Model/Stage/Hub.mv1");
	MV1SetPosition(m_sceneModelHandle, VGet(0, 0, 0));
	m_cameraTargetNum = 0;
	m_cameraPos[0] = VGet(100, 200, 100);
	m_cameraPos[1] = VGet(100, 200, -100);
	m_cameraPos[2] = VGet(-100, 200, -100);
	m_cameraTargetPos[0] = VGet(-300, 150, -150);
	m_cameraTargetPos[1] = VGet(-150, 150, 300);
	m_cameraTargetPos[2] = VGet(200, 150, 300);
	m_data = { 0, 0, 0 };

	// ポイント関連
	m_restPoint = 0;

	// UI
	m_UIPos[0] = { Game::kScreenWidth * 0.1f - 30,
		Game::kScreenWidth * 0.3f + 30,
		Game::kScreenHeight * 0.4f,
		Game::kScreenHeight * 0.6f };
	m_UIPos[1] = { Game::kScreenWidth * 0.4f,
		Game::kScreenWidth * 0.6f,
		Game::kScreenHeight * 0.75f,
		Game::kScreenHeight * 0.9f };
	m_UIPos[2] = { Game::kScreenWidth * 0.7f,
		Game::kScreenWidth * 0.9f,
		Game::kScreenHeight * 0.75f,
		Game::kScreenHeight * 0.9f };

	m_uiHandle[0] = LoadGraph("Data/Graph/Hub_1.png");
	m_uiHandle[1] = LoadGraph("Data/Graph/Hub_2.png");
	m_uiHandle[2] = LoadGraph("Data/Graph/Hub_3.png");
	m_uiHandle[3] = LoadGraph("Data/Graph/Hub_4.png");
	m_uiHandle[4] = LoadGraph("Data/Graph/Hub_5.png");
	m_uiHandle[5] = LoadGraph("Data/Graph/Hub_6.png");
	m_isUI[0] = true;
	m_isUI[1] = false;
	m_isUI[2] = false;

	m_kyoukaUIHandle[0] = LoadGraph("Data/Graph/kyoukamenu.png");
	// m_kyoukaUIHandle[1] = LoadGraph("Data/Graph/rock.png");
	// m_kyoukaUIHandle[2] = LoadGraph("Data/Graph/rock.png");
	m_kyoukaUIHandle[3] = LoadGraph("Data/Graph/menu_toro_1.png");
	m_kyoukaUIHandle[4] = LoadGraph("Data/Graph/menu_toro_2.png");
	m_kyoukaUIHandle[5] = LoadGraph("Data/Graph/menu_toro_3.png");
	m_kyoukaUIHandle[6] = LoadGraph("Data/Graph/menu_toro_4.png");
	m_kyoukaUIHandle[7] = LoadGraph("Data/Graph/menu_toro_5.png");
	m_kyoukaUIHandle[8] = LoadGraph("Data/Graph/menu_toro_6.png"); 

	m_rewardUIHandle[0] = LoadGraph("Data/Graph/toro_moji1.png");
	m_rewardUIHandle[1] = LoadGraph("Data/Graph/toro_moji2.png");
	m_rewardUIHandle[2] = LoadGraph("Data/Graph/toro_moji3.png");
	m_rewardUIHandle[3] = LoadGraph("Data/Graph/toro_moji4.png");
	m_rewardUIHandle[4] = LoadGraph("Data/Graph/toro_moji5.png");
	m_rewardUIHandle[5] = LoadGraph("Data/Graph/toro_moji6.png");
	m_blindHubAlpha = 0;
	m_isMenu = false;
	m_isInputCoolTimer = 0;
	m_colum = 1;
	m_line = 1;

	// フォント
	m_fontHandle[0] = CreateFontToHandle("無心", 32, 1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	m_fontHandle[1] = CreateFontToHandle("無心", 56, 1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	m_fontHandle[2] = CreateFontToHandle("無心", 16, 1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	
	m_InputStayTimer = 0;

	m_isEnd = false;
	m_isStayKey = false;
}

HubScene::~HubScene()
{
	// m_data.point = m_restPoint;
	m_pDataFile->SetData(m_data);
	m_UIManager.DeleteUIPanal("Hub");
	DeleteGraph(m_backHandle);
	MV1DeleteModel(m_sceneModelHandle);
	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(m_uiHandle[i]);
	}
	for (int i = 0; i < 9; i++)
	{
		DeleteGraph(m_kyoukaUIHandle[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		DeleteGraph(m_rewardUIHandle[i]);
	}
	DeleteFontToHandle(m_fontHandle[0]);
	DeleteFontToHandle(m_fontHandle[1]);
	DeleteFontToHandle(m_fontHandle[2]);
}

void HubScene::Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<DataFile>& dataFile)
{
	m_pCamera->Init(m_cameraPos[0]);
	m_pLight->Init(m_pCamera->GetPos(), m_cameraTargetPos[0]);

	m_pEffectManager = effectmanager;
	m_pDataFile = dataFile;
	m_pSoundManager = soundManager;
	m_data.playerLevel = m_pDataFile->GetStatus("Player").Level;
	m_data.bossLevel = m_pDataFile->GetStatus("Boss").Level;
	m_data.playerMaxLevel = m_pDataFile->GetStatus("Player").MaxLevel;
	m_data.bossMaxLevel = m_pDataFile->GetStatus("Boss").MaxLevel;
	m_data.point = m_pDataFile->GetPoint();

	MathPoint();
	m_restPoint = m_data.point - m_playerUsedLvUpPoint - m_bossUsedLvUpPoint;
	for (int i = 0; i < 6; i++)
	{
		if (m_pDataFile->GetisTrophy(i))
		{
			m_restPoint -= kTrophyPoint[i];
		}
	}


	ChooseMenu();
	ChooseMenu();

	// UI
	InitUI();
	m_UI.SetUI(m_uiHandle[0], m_UIPos[0], true, 255);
	m_UI.SetUI(m_uiHandle[1], m_UIPos[1], true, 255);
	m_UI.SetUI(m_uiHandle[2], m_UIPos[2], true, 255);
	m_UIManager.SetUIPanel(m_UI, "Hub");

	m_pSoundManager->LoopPlay("BGM_Hub");
}

void HubScene::Update()
{

	if (m_gameState == GameState::STARTING)
	{
		// フェードイン
		if (m_blindAhpha > 0)
		{
			m_blindAhpha -= 8;
			if (m_blindAhpha <= 0) m_blindAhpha = 0;
		}
		else
		{
			m_gameState = GameState::PLAYING;
		}
	}
	else if (m_gameState == GameState::PLAYING)
	{
		ChooseMenu();
		if (m_cameraTargetNum == 0 && m_config.GetInputDown(Input::A))
		{
			m_pSoundManager->OnePlay("SE_Title");
			m_pSoundManager->Stop("BGM_Hub");
			m_gameState = GameState::ENDING;
		}
		if (m_cameraTargetNum == 1 && m_config.GetInputDown(Input::A))
		{
			m_pSoundManager->OnePlay("SE_Title");
			m_gameState = GameState::PAUSE;
			m_colum = 1;
			m_line = 1;
			m_isMenu = true;
			m_UI.SetAllIsApp(false);
			m_UIManager.PanelUpdate(m_UI, "Hub");
		}
		if (m_cameraTargetNum == 2 && m_config.GetInputDown(Input::A))
		{
			m_pSoundManager->OnePlay("SE_Title");
			m_isEnd = true;
			m_gameState = GameState::ENDING;
		}
	}
	else if (m_gameState == GameState::PAUSE)
	{
		// メニュー展開
		if (m_isMenu)
		{
			// フェードアウト
			if (m_blindHubAlpha < 200)
			{
				m_blindHubAlpha += 40;
				if (m_blindHubAlpha >= 200)
				{
					m_blindHubAlpha = 200;
					// m_kyoukaUI.SetAllIsApp(true);
					m_kyoukaUI.SetIsApp(m_kyoukaUIHandle[0], true);
					for (int i = 0; i < 6; i++)
					{
						if (m_pDataFile->GetisTrophy(i))
						{
							m_kyoukaUI.SetIsApp(m_kyoukaUIHandle[i+3], true);
						}
					}
					m_UIManager.PanelUpdate(m_kyoukaUI, "HubKyouka");
				}
			}
		}
		// メニュー閉じる
		else
		{
			m_kyoukaUI.SetAllIsApp(false); 
			m_UIManager.PanelUpdate(m_kyoukaUI, "HubKyouka");
			// フェードアウト
			if (m_blindHubAlpha > 0)
			{
				m_blindHubAlpha -= 40;
				if (m_blindHubAlpha < 0) m_blindHubAlpha = 0;
			}
			else
			{
				m_gameState = GameState::PLAYING;
				m_UI.SetAllIsApp(true);
				m_UIManager.PanelUpdate(m_UI, "Hub");
			}
		}

		if (m_blindHubAlpha == 200)
		{
			UpgradeMenu();
		}
	}
	else if (m_gameState == GameState::ENDING)
	{
		// フェードアウト
		if (m_blindAhpha < 255)
		{
			m_blindAhpha += 8;
			if (m_blindAhpha >= 255) m_blindAhpha = 255;
		}
		else
		{
			if (!m_isEnd)
			{
				m_nextSceneNum = Scene::DUNGEON;
			}
			else
			{
				m_isGameEnd = true;
			}
		}
	}

	if (m_InputStayTimer < 60)
	{
		m_InputStayTimer++;
	}

}

void HubScene::Draw() const
{
	DrawExtendGraph(-10, 100,Game::kScreenWidth,Game::kScreenHeight, m_backHandle, false);
	MV1DrawModel(m_sceneModelHandle);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blindHubAlpha);
	DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	// UI
	m_UIManager.Draw();

	if (m_gameState == GameState::PAUSE && m_isMenu)
	{
		DrawMenu();
	}

	// 全体フェード
	if (m_gameState == GameState::STARTING)
	{

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blindAhpha);
		DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	else if (m_gameState == GameState::ENDING)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blindAhpha);
		DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}


}

void HubScene::ChooseMenu()
{
	int stickInput = m_config.GetStickDown(Input::STICK_L, true);

	// 十字キー入力
	if (m_config.GetInputPOVDown(InputPOV::RIGHT))
	{
		stickInput = 1;
	}
	if (m_config.GetInputPOVDown(InputPOV::LEFT))
	{
		stickInput = -1;
	}

	if (stickInput > 0)
	{
		m_pSoundManager->OnePlay("SE_Hub_choose");
		m_UIPos[m_cameraTargetNum].startX += 30;
		m_UIPos[m_cameraTargetNum].endX -= 30;
		m_UIPos[m_cameraTargetNum].startY += Game::kScreenHeight * 0.35f;
		m_UIPos[m_cameraTargetNum].endY += Game::kScreenHeight * 0.3f;
		m_UI.MoveUI(m_uiHandle[m_cameraTargetNum], m_UIPos[m_cameraTargetNum]);

		if (m_cameraTargetNum < 2)
			m_cameraTargetNum++;
		else
			m_cameraTargetNum = 0;

		m_UIPos[m_cameraTargetNum].startX -= 30;
		m_UIPos[m_cameraTargetNum].endX += 30;
		m_UIPos[m_cameraTargetNum].startY -= Game::kScreenHeight * 0.35f;
		m_UIPos[m_cameraTargetNum].endY -= Game::kScreenHeight * 0.3f;
		m_UI.MoveUI(m_uiHandle[m_cameraTargetNum], m_UIPos[m_cameraTargetNum]);

	}
	else if (stickInput < 0)
	{
		m_pSoundManager->OnePlay("SE_Hub_choose");
		m_UIPos[m_cameraTargetNum].startX += 30;
		m_UIPos[m_cameraTargetNum].endX -= 30;
		m_UIPos[m_cameraTargetNum].startY += Game::kScreenHeight * 0.35f;
		m_UIPos[m_cameraTargetNum].endY += Game::kScreenHeight * 0.3f;
		m_UI.MoveUI(m_uiHandle[m_cameraTargetNum], m_UIPos[m_cameraTargetNum]);

		if (m_cameraTargetNum > 0)
			m_cameraTargetNum--;
		else
			m_cameraTargetNum = 2;

		m_UIPos[m_cameraTargetNum].startX -= 30;
		m_UIPos[m_cameraTargetNum].endX += 30;
		m_UIPos[m_cameraTargetNum].startY -= Game::kScreenHeight * 0.35f;
		m_UIPos[m_cameraTargetNum].endY -= Game::kScreenHeight * 0.3f;
		m_UI.MoveUI(m_uiHandle[m_cameraTargetNum], m_UIPos[m_cameraTargetNum]);

	}

	for (int i = 0; i < 3; i++)
	{
		if (m_cameraTargetNum != i)
		{
			m_UI.ChangeAlpha(m_uiHandle[i], 50);
		}
		else
		{
			m_UI.ChangeAlpha(m_uiHandle[i], 255);
		}
		m_isUI[i] = false;
	}
	m_isUI[m_cameraTargetNum] = true;
	m_UI.SetIsApp(m_uiHandle[3], m_isUI[0]);
	m_UI.SetIsApp(m_uiHandle[4], m_isUI[1]);
	m_UI.SetIsApp(m_uiHandle[5], m_isUI[2]);
	m_UIManager.PanelUpdate(m_UI, "Hub");

	m_pCamera->SetCameraPos(m_cameraPos[m_cameraTargetNum]);
	m_pCamera->SetTargetPos(m_cameraTargetPos[m_cameraTargetNum]);
	m_pCamera->Update(false);
	m_pLight->Update(m_pCamera->GetPos(), m_cameraTargetPos[m_cameraTargetNum]);
}

void HubScene::UpgradeMenu()
{

	if (m_config.GetInputDown(Input::A))
	{
		m_isStayKey = true;
	}

	// スティック入力
	auto stickInput = m_config.GetStick(Input::STICK_L);
	// 十字キー入力
	if (m_config.GetInputPOVStay(InputPOV::RIGHT))
	{
		stickInput.x = 1;
	}
	else if (m_config.GetInputPOVStay(InputPOV::LEFT))
	{
		stickInput.x = -1;
	}

	if (m_config.GetInputPOVStay(InputPOV::UP))
	{
		stickInput.y = -1;
	}
	else if (m_config.GetInputPOVStay(InputPOV::DOWN))
	{
		stickInput.y = 1;
	}


	if (stickInput.x > 0 && m_isInputCoolTimer >= 10)
	{
		if (m_colum < 3)
		{
			if (m_line < 4)
			{
				m_line = 2;
			}
		}
		else
		{
			if (m_line < 6)
			{
				m_line++;
			}
		}
		m_pSoundManager->OnePlay("SE_Hub_choose");
		m_isInputCoolTimer = 0;
	}
	else if (stickInput.x < 0 && m_isInputCoolTimer >= 10)
	{
		if (m_colum < 3)
		{
			m_line = 1;
		}
		else
		{
			if (m_line > 1)
			{
				m_line--;
			}
		}
		m_pSoundManager->OnePlay("SE_Hub_choose");
		m_isInputCoolTimer = 0;
	}

	if (stickInput.y < 0 && m_isInputCoolTimer >= 10)
	{
		if (m_colum > 1)
		{
			m_colum--;
			if (m_colum == 2)
			{
				m_line = 1;
			}
		}
		m_pSoundManager->OnePlay("SE_Hub_choose");
		m_isInputCoolTimer = 0;
	}
	else if (stickInput.y > 0 && m_isInputCoolTimer >= 10)
	{
		if (m_colum < 3)
		{
			m_colum++;
			if (m_colum == 3)
			{
				m_line = 1;
			}
		}
		m_pSoundManager->OnePlay("SE_Hub_choose");
		m_isInputCoolTimer = 0;
	}

	if (m_isInputCoolTimer < 10)
	{
		m_isInputCoolTimer++;
	}

	// プレイヤーレベル設定
	if (m_isStayKey)
	{
		if (m_colum == 1)
		{
			if (m_line == 1)
			{
				if (m_config.GetInputStay(Input::A) && m_InputStayTimer >= 15)
				{
					m_pSoundManager->OnePlay("SE_Hub_LvDown");
					if (m_data.playerLevel > 1)
					{
						m_data.playerLevel--;
					}
					m_InputStayTimer = 0;
				}
			}
			else
			{
				if (m_data.playerLevel < m_data.playerMaxLevel)
				{
					if (m_config.GetInputStay(Input::A) && m_InputStayTimer >= 15)
					{
						m_pSoundManager->OnePlay("SE_Hub_LvUp");
						m_data.playerLevel++;
						m_InputStayTimer = 0;
					}
				}
				else
				{
					if (m_restPoint >= m_playerNeedLvUpPoint)
					{
						if (m_config.GetInputStay(Input::A) && m_data.playerMaxLevel < 99 && m_InputStayTimer >= 60)
						{
							m_pSoundManager->OnePlay("SE_Hub_LvUp");
							m_pSoundManager->OnePlay("SE_Hub_Reward");
							m_data.playerMaxLevel++;
							m_data.playerLevel++;
							m_restPoint -= m_playerNeedLvUpPoint;
							m_playerNeedLvUpPoint += 5;
							m_InputStayTimer = 0;
						}
					}
				}
			}
		}
		// ボスレベル設定
		if (m_colum == 2)
		{
			if (m_line == 1)
			{
				if (m_config.GetInputStay(Input::A) && m_InputStayTimer >= 15)
				{
					m_pSoundManager->OnePlay("SE_Hub_LvDown");
					if (m_data.bossLevel > 1)
					{
						m_data.bossLevel--;
					}
					m_InputStayTimer = 0;
				}
			}
			else
			{
				if (m_data.bossLevel < m_data.bossMaxLevel)
				{
					if (m_config.GetInputStay(Input::A) && m_InputStayTimer >= 15)
					{
						m_pSoundManager->OnePlay("SE_Hub_LvUp");
						m_data.bossLevel++;
						m_InputStayTimer = 0;
					}
				}
				else
				{
					if (m_restPoint >= m_bossNeedLvUpPoint)
					{
						if (m_config.GetInputStay(Input::A) && m_data.bossMaxLevel < 99 && m_InputStayTimer >= 60)
						{
							m_pSoundManager->OnePlay("SE_Hub_LvUp");
							m_pSoundManager->OnePlay("SE_Hub_Reward");
							m_data.bossMaxLevel++;
							m_data.bossLevel++;
							m_restPoint -= m_bossNeedLvUpPoint;
							m_bossNeedLvUpPoint += 5;
							m_InputStayTimer = 0;
						}
					}
				}
			}
		}
		// トロフィー
		if (m_colum == 3)
		{
			if (m_config.GetInputStay(Input::A) && m_InputStayTimer >= 60)
			{
				if (m_pDataFile->GetisTrophy(m_line - 1))
				{
				}
				else
				{
					if (m_restPoint >= kTrophyPoint[m_line - 1])
					{
						m_pSoundManager->OnePlay("SE_Hub_Reward");
						m_restPoint -= kTrophyPoint[m_line - 1];
						m_pDataFile->SetIsTorophy(m_line - 1, true);
						m_kyoukaUI.SetIsApp(m_kyoukaUIHandle[m_line + 2], true);
						m_UIManager.PanelUpdate(m_kyoukaUI, "HubKyouka");
					}
				}
			}
		}
	}

	if (!m_config.GetInputStay(Input::A))
	{
		m_InputStayTimer = 60;
	}

	if (m_config.GetInputDown(Input::B))
	{
		m_pSoundManager->OnePlay("SE_Hub_LvDown");
		m_isStayKey = false;
		m_isMenu = false;
	}
}

void HubScene::DrawMenu() const
{
	// フォント切り替え
	ChangeFontFromHandle(m_fontHandle[1]);

	// プレイヤーレベル
	DrawFormatString2F(Game::kScreenWidth * 0.5f, Game::kScreenHeight * 0.35f, 0xa0a0ff, 0x00000a, "%d", m_data.playerLevel);
	// ボスレベル
	DrawFormatString2F(Game::kScreenWidth * 0.5f, Game::kScreenHeight * 0.55f, 0xa0a0ff, 0x00000a, "%d", m_data.bossLevel);
	
	// 使用可能アイテム
	DrawFormatString2F(Game::kScreenWidth * 0.3f, Game::kScreenHeight * 0.1f, 0xa0a0ff, 0x00000a, "%d", m_restPoint);

	// レベルアップ必要アイテム
	// プレイヤー
	if (m_data.playerMaxLevel == 99)
	{
		DrawFormatString2F(Game::kScreenWidth * 0.75f, Game::kScreenHeight * 0.35f, 0xa0a0ff, 0x00000a, "ー");
	}
	else if (m_restPoint >= m_playerNeedLvUpPoint)
	{
		DrawFormatString2F(Game::kScreenWidth * 0.75f, Game::kScreenHeight * 0.35f, 0xa0a0ff, 0x00000a, "%d", m_playerNeedLvUpPoint);
	}
	else
	{
		DrawFormatString2F(Game::kScreenWidth * 0.75f, Game::kScreenHeight * 0.35f, 0xff0a0a, 0xe0e0e0, "%d", m_playerNeedLvUpPoint);
	}
	// ボス
	if (m_data.bossMaxLevel == 99)
	{
		DrawFormatString2F(Game::kScreenWidth * 0.75f, Game::kScreenHeight * 0.55f, 0xa0a0ff, 0x00000a, "ー");
	}
	else if (m_restPoint >= m_bossNeedLvUpPoint)
	{
		DrawFormatString2F(Game::kScreenWidth * 0.75f, Game::kScreenHeight * 0.55f, 0xa0a0ff, 0x00000a, "%d", m_bossNeedLvUpPoint);
	}
	else
	{
		DrawFormatString2F(Game::kScreenWidth * 0.75f, Game::kScreenHeight * 0.55f, 0xff0a0a, 0xe0e0e0, "%d", m_bossNeedLvUpPoint);
	}

	// フォント小
	ChangeFontFromHandle(m_fontHandle[2]);

	DrawFormatString2F(Game::kScreenWidth * 0.56f, Game::kScreenHeight * 0.26f, 0x8080a0, 0x00000a, "/ 最大レベル");
	DrawFormatString2F(Game::kScreenWidth * 0.45f, Game::kScreenHeight * 0.08f, 0x8080a0, 0x00000a, "/ 総計獲得数");

	// フォント戻し
	ChangeFontFromHandle(m_fontHandle[0]);

	// レベル表記文
	DrawFormatString2F(Game::kScreenWidth * 0.37f, Game::kScreenHeight * 0.245f, 0xa0a0ff, 0x00000a, "ー　現在レベル　　 　＋");
	DrawFormatString2F(Game::kScreenWidth * 0.31f, Game::kScreenHeight * 0.06f, 0xa0a0ff, 0x00000a, "現在所持数");

	// プレイヤー最大レベル
	DrawFormatString2F(Game::kScreenWidth * 0.55f, Game::kScreenHeight * 0.38f, 0x8080a0, 0x00000a, "/ %d", m_data.playerMaxLevel);
	// ボスレベル
	DrawFormatString2F(Game::kScreenWidth * 0.55f, Game::kScreenHeight * 0.58f, 0x8080a0, 0x00000a, "/ %d", m_data.bossMaxLevel);

	// 最大所持アイテム
	DrawFormatString2F(Game::kScreenWidth * 0.43f, Game::kScreenHeight * 0.14f, 0x8080a0, 0x00000a, "/ %d", m_data.point);


	// トロフィー解放必要アイテム
	for (int i = 0; i < 6; i++)
	{
		if (!m_pDataFile->GetisTrophy(i))
		{
			if (m_restPoint >= kTrophyPoint[i])
			{
				DrawFormatString2F(Game::kScreenWidth * 0.35f + (i * (90 + 20)), Game::kScreenHeight * 0.85f,
					0xa0a0ff, 0x00000a, "%d", kTrophyPoint[i]);
			}
			else
			{
				DrawFormatString2F(Game::kScreenWidth * 0.35f + (i * (90 + 20)), Game::kScreenHeight * 0.85f,
					0xff0a0a, 0xe0e0e0, "%d", kTrophyPoint[i]);
			}
		}
	}


	// プレイヤー左▲
	DrawTriangle( (Game::kScreenWidth * 0.37f),  (Game::kScreenHeight * 0.375f),
		 (Game::kScreenWidth * 0.42f),  (Game::kScreenHeight * 0.32f),
		 (Game::kScreenWidth * 0.42f),  (Game::kScreenHeight * 0.43f),
		0xa0a0ff, true);
	// プレイヤー右▲
	DrawTriangle( (Game::kScreenWidth * 0.63f),  (Game::kScreenHeight * 0.32f),
		 (Game::kScreenWidth * 0.63f),  (Game::kScreenHeight * 0.43f),
		 (Game::kScreenWidth * 0.68f),  (Game::kScreenHeight * 0.375f),
		0xa0a0ff, true);

	// ボス左▲
	DrawTriangle( (Game::kScreenWidth * 0.37f),  (Game::kScreenHeight * 0.575f),
		 (Game::kScreenWidth * 0.42f),  (Game::kScreenHeight * 0.52f),
		 (Game::kScreenWidth * 0.42f),  (Game::kScreenHeight * 0.63f),
		0xa0a0ff, true);
	// ボス右▲
	DrawTriangle( (Game::kScreenWidth * 0.63f),  (Game::kScreenHeight * 0.52f),
		 (Game::kScreenWidth * 0.63f),  (Game::kScreenHeight * 0.63f),
		 (Game::kScreenWidth * 0.68f),  (Game::kScreenHeight * 0.575f),
		0xa0a0ff, true);

	// 選択状況ごとの表示
	if (m_colum == 1)
	{
		if (m_line == 1)
		{
			// プレイヤー左▲
			DrawTriangle( (Game::kScreenWidth * 0.37f),  (Game::kScreenHeight * 0.375f),
				 (Game::kScreenWidth * 0.42f),  (Game::kScreenHeight * 0.32f),
				 (Game::kScreenWidth * 0.42f),  (Game::kScreenHeight * 0.43f),
				0xe0e000, true);
		}
		else
		{
			// プレイヤー右▲
			DrawTriangle( (Game::kScreenWidth * 0.63f),  (Game::kScreenHeight * 0.32f),
				 (Game::kScreenWidth * 0.63f),  (Game::kScreenHeight * 0.43f),
				 (Game::kScreenWidth * 0.68f),  (Game::kScreenHeight * 0.375f),
				0xe0e000, true);
		}
	}
	else if (m_colum == 2)
	{
		if (m_line == 1)
		{
			// ボス左▲
			DrawTriangle( (Game::kScreenWidth * 0.37f),  (Game::kScreenHeight * 0.575f),
				 (Game::kScreenWidth * 0.42f),  (Game::kScreenHeight * 0.52f),
				 (Game::kScreenWidth * 0.42f),  (Game::kScreenHeight * 0.63f),
				0xe0e000, true);
		}
		else
		{
			// ボス右▲
			DrawTriangleAA( (Game::kScreenWidth * 0.63f),  (Game::kScreenHeight * 0.52f),
				 (Game::kScreenWidth * 0.63f),  (Game::kScreenHeight * 0.63f),
				 (Game::kScreenWidth * 0.68f),  (Game::kScreenHeight * 0.575f),
				0xe0e000, true);
		}
	}
	else
	{
		DrawBoxAA(Game::kScreenWidth * 0.345f + ((m_line-1) * (90+20.5f)), Game::kScreenHeight * 0.705f,
			Game::kScreenWidth * 0.345f + ((m_line - 1) * (90 + 20.5f)) + 90, Game::kScreenHeight * 0.705f + 90,
			0xe0e000, false, 3.0f);
	}

	if (m_colum == 3)
	{
		if (m_pDataFile->GetisTrophy(m_line - 1))
		{
			DrawGraph(0, 0, m_rewardUIHandle[m_line - 1], true);
		}
	}

}

void HubScene::InitUI()
{
	m_UI.SetUI(m_uiHandle[0], m_UIPos[0], true, 255);
	m_UI.SetUI(m_uiHandle[1], m_UIPos[1], true, 255);
	m_UI.SetUI(m_uiHandle[2], m_UIPos[2], true, 255);
	m_UI.SetUI(m_uiHandle[3], {0,Game::kScreenWidth, 0,Game::kScreenHeight}, m_isUI[0], 255);
	m_UI.SetUI(m_uiHandle[4], { 0,Game::kScreenWidth, 0,Game::kScreenHeight }, m_isUI[1], 255);
	m_UI.SetUI(m_uiHandle[5], { 0,Game::kScreenWidth, 0,Game::kScreenHeight }, m_isUI[2], 255);
	


	for (int i = 0; i < 9; i++)
	{
		if (i != 1 && i != 2)
		{
			m_kyoukaUI.SetUI(m_kyoukaUIHandle[i],
				{ 0,Game::kScreenWidth,0,Game::kScreenHeight },
				false, 255);
		}
		if (i == 1)
		{
			m_kyoukaUI.SetUI(m_kyoukaUIHandle[i],
				{ Game::kScreenWidth * 0.64f,Game::kScreenWidth * 0.67f,Game::kScreenHeight * 0.33f,Game::kScreenHeight * 0.42f },
				false, 255);
		}
		else if (i == 2)
		{
			m_kyoukaUI.SetUI(m_kyoukaUIHandle[i],
				{ Game::kScreenWidth * 0.64f,Game::kScreenWidth * 0.67f,Game::kScreenHeight * 0.53f,Game::kScreenHeight * 0.62f },
				false, 255);
		}
	}

	m_UIManager.SetUIPanel(m_UI, "Hub");
	m_UIManager.SetUIPanel(m_kyoukaUI, "HubKyouka");
}

void HubScene::MathPoint()
{
	m_playerNeedLvUpPoint = 0;
	m_playerUsedLvUpPoint = 0;
	for (int i = 0; i < m_data.playerMaxLevel; i++)
	{
		m_playerNeedLvUpPoint += 5;
		m_playerUsedLvUpPoint += 5 * i;
	}

	m_bossNeedLvUpPoint = 0;
	m_bossUsedLvUpPoint = 0;
	for (int i = 0; i < m_data.bossMaxLevel; i++)
	{
		m_bossNeedLvUpPoint += 5;
		m_bossUsedLvUpPoint += 5 * i;
	}

}
