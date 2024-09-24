#include "DebugScene.h"
#include "DxLib.h"
#include "Player.h"
#include "Boss.h"

DebugScene::DebugScene()
{
	m_sceneModelHandle = MV1LoadModel("Data/Model/Stage/DebugFloor.mv1");
	MV1SetPosition(m_sceneModelHandle, VGet(0, 0, 0));
	m_pPlayer = std::make_shared<Player>();
	m_pBoss = std::make_shared<Boss>();
	// shadowHandle = MakeShadowMap(Game::kScreenHeight, Game::kScreenWidth);
	// SetShadowMapLightDirection(shadowHandle, VGet(0.0f, -1.0f, 0.0f));
}

DebugScene::~DebugScene()
{
	MV1DeleteModel(m_sceneModelHandle);
	// DeleteShadowMap(shadowHandle);
}

void DebugScene::Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<DataFile>& dataFile)
{
	/*m_pCamera->Init(VGet(0, 400.0f, -450.0f));
	m_pPlayer->Init(VGet(0, 0, 0));
	m_pBoss->Init();
	m_pLight->Init(m_pCamera->GetPos(), m_pPlayer->GetPos());*/
}

void DebugScene::Update()
{
	//m_pPlayer->Update(m_pCamera->GetPos());
	//m_pBoss->Update(m_pPlayer->GetPos());
	//m_pCamera->SetTargetPos(m_pPlayer->GetPos());
	//m_pCamera->Update(true);
	//m_pLight->Update(m_pCamera->GetPos(), m_pPlayer->GetPos());

	//// SetShadowMapLightDirection(shadowHandle, VSub(m_pPlayer->GetPos(), m_pCamera->GetPos()));
	//auto tempPos = m_pPlayer->GetPos();
	///*SetShadowMapDrawArea(shadowHandle,
	//	VGet(tempPos.x - 1000, tempPos.y - 100, tempPos.z - 1000),
	//	VGet(tempPos.x + 1000, tempPos.y + 500, tempPos.z + 1000));*/
}

void DebugScene::Draw() const
{
	//// ShadowMap_DrawSetup(shadowHandle);
	//MV1DrawModel(m_sceneModelHandle);
	//m_pPlayer->Draw();
	//m_pBoss->Draw();
	//// ShadowMap_DrawEnd();

	//// SetUseShadowMap(0, shadowHandle);
	///*MV1DrawModel(m_sceneModelHandle);
	//m_pPlayer->Draw();*/
	//// SetUseShadowMap(-1, shadowHandle);

	//// ゲームパッドからの回転入力
	//DINPUT_JOYSTATE pad;
	//GetJoypadDirectInputState(DX_INPUT_PAD1, &pad);
	//for (int i = 0; i < 32; i++)
	//{
	//	DrawFormatString(10, 10*i + 10, 0xffffff, "%d", pad.Buttons[i]);
	//}
}
