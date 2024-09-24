#include "GameScene.h"
#include "DxLib.h"
#include "Player.h"
#include "Boss.h"

GameScene::GameScene()
{
	m_sceneModelHandle = MV1LoadModel("Data/Model/Stage/BossRoom.mv1");
	MV1SetPosition(m_sceneModelHandle, VGet(0, 0, 0));
	m_pPlayer = std::make_shared<Player>();
	m_pBoss = std::make_shared<Boss>();
	m_startTimer = 180;
	m_gettingPoint = 0;
	m_blindAhpha = 255;

	m_endUIHandle[0] = LoadGraph("Data/Graph/end_1.png");
	m_endUIHandle[1] = LoadGraph("Data/Graph/end_2.png");
	m_endUIHandle[2] = LoadGraph("Data/Graph/end_4.png");
	m_endUIPos[0] = { 0, Game::kScreenWidth, 0, Game::kScreenHeight };
	m_endUIPos[1] = { 0, Game::kScreenWidth, Game::kScreenHeight, Game::kScreenHeight*2 };
	m_endUIPos[2] = { 0, Game::kScreenWidth, 0, Game::kScreenHeight };
	m_endUIAlpha[0] = 0;
	m_endUIAlpha[1] = 0;
	m_endUIAlpha[2] = 0;
	m_endTimer = 0;
	m_sceneMovefrag = false;
	m_isAppResult = false;
	// フォント
	m_fontHandle[0] = CreateFontToHandle("無心", 32, 1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	m_fontHandle[1] = CreateFontToHandle("無心", 92, 1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	m_textAlpha = 0;
	m_textAlphaFrag = false;
	m_hitStopCount = 0;
}

GameScene::~GameScene()
{
	// SetUseShadowMap(0, -1);
	DeleteGraph(m_endUIHandle[0]);
	DeleteGraph(m_endUIHandle[1]);
	DeleteGraph(m_endUIHandle[2]);
	m_data.point += m_gettingPoint;
	m_pDataFile->SetData(m_data);
	MV1DeleteModel(m_sceneModelHandle);
	m_UIManager.DeleteUIPanal("Player");
	m_UIManager.DeleteUIPanal("Boss");
	m_UIManager.DeleteUIPanal("Game_end");
	DeleteFontToHandle(m_fontHandle[0]);
	DeleteFontToHandle(m_fontHandle[1]);
}

void GameScene::Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<DataFile>& dataFile)
{
	m_pDataFile = dataFile;
	m_pEffectManager = effectmanager;
	m_pSoundManager = soundManager;
	// ヒット
	m_pEffectManager->RegisterEffect("Hit", LoadEffekseerEffect("Data/Effect/Hit.efkefc", 1.0f), VGet(0, 0, 0), VGet(0, 0, 0));
	m_pEffectManager->RegisterEffect("HitEne", LoadEffekseerEffect("Data/Effect/HitEne.efkefc", 1.0f), VGet(0, 0, 0), VGet(0, 0, 0));
	m_data.playerLevel = m_pDataFile->GetStatus("Player").Level;
	m_data.bossLevel = m_pDataFile->GetStatus("Boss").Level;
	m_data.playerMaxLevel = m_pDataFile->GetStatus("Player").MaxLevel;
	m_data.bossMaxLevel = m_pDataFile->GetStatus("Boss").MaxLevel;
	m_data.point = m_pDataFile->GetPoint();
	m_pCamera->Init(VGet(0, 400.0f, -900.0f));
	m_pPlayer->Init(VGet(0, 0, -800.0f), effectmanager, soundManager, m_pDataFile->GetStatus("Player"));
	m_UIManager.SetUIPanel(m_pPlayer->GetUI(), "Player");
	m_pBoss->Init(effectmanager, soundManager, m_pDataFile->GetStatus("Boss"));
	m_UIManager.SetUIPanel(m_pBoss->GetUI(), "Boss");
	m_pLight->Init(m_pCamera->GetPos(), m_pPlayer->GetPos());
	m_pSoundManager->OnePlay("SE_GameReady");
}

void GameScene::Update()
{
	// ゲームシーン内での状態毎の処理
	// シーン移行後最初の処理
	if (m_gameState == GameState::STARTING)
	{
		m_pCamera->SetTargetPos(m_pPlayer->GetPos());
		m_pCamera->Update(false);
		if (m_startTimer > 0)
		{
			m_startTimer--;
		}
		else
		{
			m_pSoundManager->LoopPlay("BGM_Game");
			m_gameState = GameState::PLAYING;
		}
		if (m_startTimer == 60)
		{
			m_pSoundManager->OnePlay("SE_GameStart");
		}

		// フェードイン
		if (m_blindAhpha > 0)
		{
			m_blindAhpha -= 8;
			if (m_blindAhpha <= 0) m_blindAhpha = 0;
		}
	}
	// プレイヤー操作可能時の処理
	else if(m_gameState == GameState::PLAYING)
	{
		//if (m_hitStopCount == 0)
		//{
			// 通常処理
			m_pPlayer->Update(m_pCamera->GetPos());
			m_UIManager.PanelUpdate(m_pPlayer->GetUI(), "Player");
			m_pBoss->Update(m_pPlayer->GetPos());
			m_pPlayer->HitOther(m_pPlayer->GetSize(), m_pBoss->GetPos(), m_pBoss->GetSize());
			m_UIManager.PanelUpdate(m_pBoss->GetUI(), "Boss");
			m_pCamera->SetTargetPos(m_pPlayer->GetPos());
			m_pCamera->Update(true);
		//}
		// ダメージ判定処理
		if (m_pPlayer->IsHitAttack(m_pBoss->GetPos(), m_pBoss->GetSize()))
		{
			if (m_pBoss->SetDamage(m_pPlayer->GetDamage()))
			{
				m_pSoundManager->OnePlay("SE_Boss_Hit");
				m_hitStopCount = m_pPlayer->GetDamage() / 100;
				m_pEffectManager->PlayEffect("Hit", m_pBoss->GetPos(), m_pBoss->GetDir());
				
			}

		}
		if (m_pBoss->IsHitAttack(m_pPlayer->GetPos(), m_pPlayer->GetSize()))
		{
			if (m_pPlayer->SetDamage(m_pBoss->GetDamage()))
			{
				m_pSoundManager->OnePlay("SE_Player_Hit");
				m_hitStopCount = m_pBoss->GetDamage() / 100;
				m_pEffectManager->PlayEffect("HitEne", m_pPlayer->GetPos(), m_pPlayer->GetDir());
			}
		}

		if (m_hitStopCount > 0)
		{
			m_hitStopCount--;
		}

		if (m_pBoss->GetHP() <= 0 || m_pPlayer->GetHP() <= 0)
		{
			m_UIManager.DeleteUIPanal("Player");
			m_UIManager.DeleteUIPanal("Boss");
			m_pSoundManager->Stop("BGM_Game");
			m_gameState = GameState::ENDING;
   			EndingInit();
		}
	}
	// ポーズ中（ゲーム停止中）の処理
	else if (m_gameState == GameState::PAUSE)
	{

	}
	// クリアorやられた後
	else if (m_gameState == GameState::ENDING)
	{

		// フェードアウト
		if (m_whiteAlpha < 200)
		{
			m_whiteAlpha += 20;
			if (m_whiteAlpha >= 200) m_whiteAlpha = 200;
		}
		EndingUpdate();

		if (m_endTimer >= 60)
		{
			if (m_config.GetInputDown(Input::A))
			{
				m_pSoundManager->OnePlay("SE_Title");
				m_sceneMovefrag = true;
			}
		}
		if (m_sceneMovefrag)
		{
			// フェードアウト
			if (m_blindAhpha < 255)
			{
				m_blindAhpha += 8;
				if (m_blindAhpha >= 255) m_blindAhpha = 255;
			}
			else
			{
				m_nextSceneNum = Scene::HUB;
			}
		}
	}
	else
	{
		assert(false);
	}

	if (!m_textAlphaFrag)
	{
		if (m_textAlpha < 255)
		{
			m_textAlpha += 20;
			if (m_textAlpha >= 255)
			{
				m_textAlpha = 255;
				m_textAlphaFrag = true;
			}
		}
	}
	else
	{
		if (m_textAlpha > 0)
		{
			m_textAlpha -= 20;
			if (m_textAlpha < 0)
			{
				m_textAlpha = 0;
				m_textAlphaFrag = false;
			}
		}
	}

	m_pLight->Update(m_pCamera->GetPos(), m_pPlayer->GetPos());

	SetShadowMapLightDirection(shadowHandle, VSub(m_pPlayer->GetPos(), m_pCamera->GetPos()));
	auto tempPos = m_pPlayer->GetPos();
	SetShadowMapDrawArea(shadowHandle,
		VGet(tempPos.x - 1000, tempPos.y - 100, tempPos.z - 1000),
		VGet(tempPos.x + 1000, tempPos.y + 500, tempPos.z + 1000));
}

void GameScene::Draw() const
{
	// 各状態共通で表示
	MV1DrawModel(m_sceneModelHandle);
	m_pPlayer->Draw();
	m_pBoss->Draw();

	// ゲームシーン内での状態毎の処理
	// シーン移行後最初の処理
	if (m_gameState == GameState::STARTING)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blindAhpha);
		DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		ChangeFontFromHandle(m_fontHandle[1]);
		if (m_startTimer > 60)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_textAlpha);
			DrawFormatString(Game::kScreenHalfWidth * 0.75f, Game::kScreenHalfHeight * 0.8f, 0xffff00, "READY?");
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
		else
			DrawFormatString(Game::kScreenHalfWidth * 0.75f, Game::kScreenHalfHeight * 0.8f, 0xffff00, "START!!");
		ChangeFontFromHandle(m_fontHandle[0]);

		// プレイヤー
		DrawFormatString(Game::kScreenWidth * 0.4f, Game::kScreenHeight * 0.75f, 0xa8f4ff, "戦士　Lv. %d", m_data.playerLevel);
		// ボス
		DrawFormatString(Game::kScreenWidth * 0.25f, Game::kScreenHeight * 0.05f, 0xa8f4ff, "スケルドン　Lv. %d", m_data.bossLevel);

	}
	// プレイヤー操作可能時の処理
	else if (m_gameState == GameState::PLAYING)
	{
		// プレイヤー
		DrawFormatString(Game::kScreenWidth * 0.4f,Game::kScreenHeight * 0.75f, 0xa8f4ff, "戦士　Lv. %d",m_data.playerLevel);
		// ボス
		DrawFormatString(Game::kScreenWidth * 0.25f,Game::kScreenHeight * 0.05f, 0xa8f4ff, "スケルドン　Lv. %d", m_data.bossLevel);

	}
	// ポーズ中（ゲーム停止中）の処理
	else if (m_gameState == GameState::PAUSE)
	{
	}
	else if (m_gameState == GameState::ENDING)
	{
		if (m_pBoss->GetHP() <= 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_whiteAlpha);
			DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
		else if (m_pPlayer->GetHP() <= 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_whiteAlpha);
			DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0x000000, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blindAhpha);
		DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	else
	{
		assert(false);
	}

	// UI表示
	m_UIManager.Draw();
	
	if (m_isAppResult)
	{
		ChangeFontFromHandle(m_fontHandle[1]);
		DrawFormatStringF(Game::kScreenWidth * 0.6f, Game::kScreenHeight * 0.6f, 0xa0a0ff, "%d", m_gettingPoint);
		ChangeFontFromHandle(m_fontHandle[0]);
	}

	if (m_endTimer >= 60)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_textAlpha);
		DrawFormatString(Game::kScreenWidth * 0.7f, Game::kScreenHeight * 0.9f, 0xffff00, "PRESS A BUTTON");
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}

	if (m_gameState == GameState::ENDING)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blindAhpha);
		DrawBox(0, 0, Game::kScreenWidth + 1, Game::kScreenHeight + 1, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}

void GameScene::EndingInit()
{
	m_endUI.SetUI(m_endUIHandle[0], m_endUIPos[0], true, 0);
	m_endUI.SetUI(m_endUIHandle[1], m_endUIPos[1], true, 0);
	m_endUI.SetUI(m_endUIHandle[2], m_endUIPos[2], true, 0);
	m_UIManager.SetUIPanel(m_endUI, "Game_end");
	// 獲得アイテム個数の確定
	CalculateReward();
	m_endUI.SetNumberUI(m_gettingPoint,
		{ Game::kScreenWidth * 0.6f,
		Game::kScreenWidth * 0.7f,
		Game::kScreenHeight * 0.6f,
		Game::kScreenHeight * 0.8f },
		false, 255);

	if (m_pBoss->GetHP() <= 0)
	{
		m_pSoundManager->OnePlay("SE_Game_Win");
	}
	else if (m_pPlayer->GetHP() <= 0)
	{
		m_pSoundManager->OnePlay("SE_Game_Lose");
	}
}

void GameScene::EndingUpdate()
{
	// ごり押しです()
	if (m_pBoss->GetHP() <= 0)
	{
		// 白画面化
		if (m_whiteAlpha >= 200)
		{
			// くりあ位置
			if (m_endUIPos[0].startY > -Game::kScreenHalfHeight * 0.625f)
			{
				m_endUIPos[0].startY -= 10;
				m_endUIPos[0].endY -= 10;
				m_endUI.MoveUI(m_endUIHandle[0], m_endUIPos[0]);
				if (m_config.GetInputDown(Input::A))
				{
					m_endUIPos[0].startY = -Game::kScreenHalfHeight * 0.625f;
					m_endUIPos[0].endY = Game::kScreenHalfHeight * 1.375f;
					m_endUI.MoveUI(m_endUIHandle[0], m_endUIPos[0]);
				}
			}
			else
			{
				// 結果メニューあるふぁ
				if (m_endUIAlpha[1] < 255)
				{
					m_endUIAlpha[1] += 5;
					m_endUI.ChangeAlpha(m_endUIHandle[1], m_endUIAlpha[1]);
				}
				// 結果メニュー位置
				if (m_endUIPos[1].startY > 0)
				{
					m_endUIPos[1].startY -= 10;
					m_endUIPos[1].endY -= 10;
					m_endUI.MoveUI(m_endUIHandle[1], m_endUIPos[1]);
				}
				if ((m_endUIAlpha[1] < 255 || m_endUIPos[1].startY > 0) && m_config.GetInputDown(Input::A))
				{
					m_endUIAlpha[1] = 255;
					m_endUI.ChangeAlpha(m_endUIHandle[1], m_endUIAlpha[1]);

					m_endUIPos[1].startY = 0;
					m_endUIPos[1].endY = Game::kScreenHeight;
					m_endUI.MoveUI(m_endUIHandle[1], m_endUIPos[1]);
				}
				// 獲得アイテム数の表示
				if (m_endUIAlpha[1] >= 255 && m_endUIPos[1].startY <= 0)
				{
					// m_endUI.SetIsAppNum(m_gettingPoint, true);
					m_isAppResult = true;
					if (m_endTimer < 60)
					{
						m_endTimer++;
					}
				}
			}
		}
		else
		{
			// くりああるふぁ
			if (m_endUIAlpha[0] < 255)
			{
				m_endUIAlpha[0] += 30;
				if (m_endUIAlpha[0] >= 255) m_endUIAlpha[0] = 255;
				m_endUI.ChangeAlpha(m_endUIHandle[0], m_endUIAlpha[0]);
			}
		}
	}
	else
	{
		// くりああるふぁ
		if (m_endUIAlpha[2] < 255)
		{
			m_endUIAlpha[2] += 30;
			if (m_endUIAlpha[2] >= 255) m_endUIAlpha[2] = 255;
			m_endUI.ChangeAlpha(m_endUIHandle[2], m_endUIAlpha[2]);
		}
		else
		{
			if (m_endTimer < 60)
			{
				m_endTimer++;
			}
		}
	}
	m_UIManager.PanelUpdate(m_endUI, "Game_end");
}

void GameScene::CalculateReward()
{
	if (m_pBoss->GetHP() <= 0)
	{
		int temp = m_data.bossLevel - m_data.playerLevel;
		if (temp < 0) temp = 0;
		m_gettingPoint = ((m_data.bossLevel) * 2 + 8) *			// 基本数
			((static_cast<float>(((temp) * 0.1f + 1) +			// レベル差倍率
				static_cast<int>((temp) * 0.1f))));				// レベル差ボーナス倍率（10差単位）
	}
	else
	{
		m_gettingPoint = 0;
	}
}
