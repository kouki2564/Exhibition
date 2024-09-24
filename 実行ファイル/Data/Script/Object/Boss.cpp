#include "Boss.h"
#include <cassert>

namespace
{
	// 行動待機時間
	constexpr int kMoveCoolTimeMax = 180;
	// 行動速度
	constexpr float kMoveSpeed = 10.0;
	// タックル速度
	constexpr float kRunSpeed = 30.0 / 240;
	// 接近最短距離
	constexpr float kNearFar = 300;
	constexpr float kLongFar = 750;
	// モデルサイズ
	constexpr float kModelSize = 150;

	constexpr float kFirstHPBarlangth = Game::kScreenHalfWidth * (1.5f - 0.5f);
		
	constexpr int kHitStartFrame = 30;
}

enum Anima
{
	ENTRY = 72,
	IDLE = 94,
	MOVE = 93,
	PRELIMINARY = 42,
	ATTACK = 2,
	TACLE = 56,
	SRASH = 4
};

Boss::Boss():
	m_moveCoolTimer(0),
	m_isAction(false),
	m_attackCount(0)
	{
		m_handle = MV1LoadModel("Data/Model/Character/Golem.mv1");
		m_hpBarHandle = LoadGraph("Data/Graph/BarV6_ProgressBar.png");
		COLOR_F eyeDef = { 1.0f,0.2f,0.2f,0.8f };
		COLOR_F eyeSpc = { 1.0f,0.0f,0.0f,1.0f };
		COLOR_F eyeEmi = { 1.0f,0.0f,0.0f,1.0f };
		COLOR_F eyeAmb = { 0.0f,0.0f,0.0f,1.0f };

		MV1SetFrameDifColorScale(m_handle, 49, eyeDef);
		MV1SetFrameSpcColorScale(m_handle, 49, eyeSpc);
		MV1SetFrameEmiColorScale(m_handle, 49, eyeEmi);
		MV1SetFrameAmbColorScale(m_handle, 49, eyeAmb);

		m_animation.InitAnimation(m_handle, Anima::PRELIMINARY, 2, 0);

		/////////////////
		// 所持武器関連
		/////////////////
		m_weponHandle = MV1LoadModel("Data/Model/Wepon/GolemAxe.mv1");

		m_state = State::IDLE;
		m_runDir = m_dir;

		// UI
		// HPバーUI
		m_HPBarPos = {
			Game::kScreenHalfWidth * 0.5f,
			Game::kScreenHalfWidth * 1.5f,
			Game::kScreenHeight * 0.1f,
			Game::kScreenHeight * 0.12f };
		m_HPBarlangth = kFirstHPBarlangth;
		m_UI.SetUI(LoadGraph("Data/Graph/BarV6_Bar.png"),
			{ Game::kScreenHalfWidth * 0.5f,
			Game::kScreenHalfWidth * 1.5f,
			Game::kScreenHeight * 0.1f,
			Game::kScreenHeight * 0.12f },
			true, 255);
		m_UI.SetUI(m_hpBarHandle,
			m_HPBarPos,
			true, 255);
		m_UI.SetUI(LoadGraph("Data/Graph/BarV1_ProgressBarBorder.png"),
			{ Game::kScreenHalfWidth * 0.5f,
			Game::kScreenHalfWidth * 1.5f,
			Game::kScreenHeight * 0.1f,
			Game::kScreenHeight * 0.12f },
			true, 255);
}

	Boss::~Boss()
	{
		MV1DeleteModel(m_handle);
		MV1DeleteModel(m_weponHandle);
		DeleteGraph(m_hpBarHandle);
	}

	void Boss::Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, Status status)
	{
		m_status = status;
		m_HP = m_status.HP;
		m_pos = VGet(0, 0, 900.0f);
		MV1SetScale(m_handle, VGet(80, 80, 80));
		MV1SetPosition(m_handle, m_pos);
		m_weponAttachFrameNum = MV1SearchFrame(m_handle, "handslot.r");
		WeponMove(m_weponAttachFrameNum);
		m_pEffectManager = effectmanager;
		m_pEffectManager->RegisterEffect("furiorosi", LoadEffekseerEffect("Data/Effect/BossAttack.efkefc", 1.0f), m_pos, m_dir);
		m_pEffectManager->RegisterEffect("yokofuri", LoadEffekseerEffect("Data/Effect/BossNormalAttack.efkefc", 1.0f), m_pos, m_dir);
		m_pEffectManager->RegisterEffect("takkuru", LoadEffekseerEffect("Data/Effect/tacckle.efkefc", 1.0f), m_pos, m_dir);

		m_pSoundManager = soundManager;
	}

	void Boss::Update(VECTOR playerPos)
	{
		WeponMove(m_weponAttachFrameNum);

		// 行動
		VECTOR toPlayer = VSub(playerPos, m_pos);
		if (m_moveCoolTimer >= 120 - (m_status.AGI * 0.53f))
		{
			m_isAction = true;
			// 方向の確定
			toPlayer.y = 0;
			if (m_state == State::IDLE || m_state == State::MOVE)
				m_dir = VNorm(toPlayer);
			// 回転
			Rotation();
			// 行動範囲内にプレイヤーがいた場合
			if (VSize(toPlayer) < kNearFar)
			{
				if (m_isAction)
				{
					NearAttack();
					if (m_attackCount < 2)
					{
						m_attackCount++;
					}
					else
					{
						m_attackCount = 0;
					}
				}
				m_moveCoolTimer = 0;
			}
			// 一定以上遠かった場合
			else if (VSize(toPlayer) > kLongFar)
			{
				if (m_isAction)
				{
					m_state = State::SKILL;
				}
				m_moveCoolTimer = 0;
			}
			// その中間の位置にいる場合
			else
			{
				// 追跡
				Move();
			}
		}
		else if (m_state == State::ATTACK && m_moveCoolTimer == kHitStartFrame)
		{
		}


		// 状態更新
		if (m_state == State::IDLE)
		{
			m_isDealDamage = false;
		}
		if (m_state != State::IDLE && m_state != State::MOVE)
		{
			m_actionTimer++;
			if (m_state == State::ATTACK)
			{
				if (m_actionTimer > 40)
				{
					m_actionTimer = 0;
					m_state = State::IDLE;
					m_isAction = false;
					m_moveCoolTimer = 0;
				}

				// ダメージ関連処理
				if (m_attackCount == 2)	// 強攻撃
				{
					if (m_actionTimer == 40)
					{
						m_pSoundManager->OnePlay("SE_Boss_AttackBig");
						m_hitArea = VAdd(m_pos, VScale(m_dir, kModelSize * 2.0f));
						m_hitAreaRange = kModelSize * 1.8f;
						m_damagePoint = m_status.POW * 2.0f;
						m_isDealDamage = true;
					}
					else if (m_actionTimer > 60)
					{
						m_isDealDamage = false;
					}
				}
				else 					// 弱攻撃
				{
					if (m_actionTimer == 30)
					{
						m_pSoundManager->OnePlay("SE_Boss_AttackRight");
						m_hitArea = VAdd(m_pos, VScale(m_dir, kModelSize * 2.0f));
						m_hitAreaRange = kModelSize * 1.2f;
						m_damagePoint = m_status.POW * 1.0f;
						m_isDealDamage = true;
					}
					else if (m_actionTimer > 30)
					{
						m_isDealDamage = false;
					}
				}
				
				
			}
			else if (m_state == State::SKILL)
			{
				LongAttack(toPlayer);
				
				if (m_actionTimer > 110)
					// || (m_actionTimer > 80 && VSize(toPlayer) < kNearFar))
				{
					m_actionTimer = 0;
					m_state = State::IDLE;
					m_isAction = false;
					m_moveCoolTimer = 0;
				}
		}
	}

		HitWall(kModelSize);

	// 行動クールタイム更新
	if (m_moveCoolTimer < 120 - (m_status.AGI * 0.53f) && m_state == State::IDLE)
	{
		m_moveCoolTimer++;
	}

	m_HPBarlangth = kFirstHPBarlangth * (static_cast<float>(m_HP) / static_cast<float>(m_status.HP));

	if (m_HPBarlangth < (m_HPBarPos.endX - m_HPBarPos.startX))
	{
		int temp = (m_HPBarPos.endX - m_HPBarPos.startX) - m_HPBarlangth;
		m_HPBarPos.endX -= temp * 0.5f;
		if (m_HPBarPos.endX - m_HPBarPos.startX < 0)
		{
			m_HPBarPos.endX = m_HPBarPos.startX;
		}
		m_UI.MoveUI(m_hpBarHandle, m_HPBarPos);
	}

	// アニメーション更新
	AnimationUpdate();
}

void Boss::Draw() const
{
	MV1DrawModel(m_weponHandle);
	MV1SetPosition(m_handle, m_pos);
	MV1DrawModel(m_handle);
	
	// debug
	// DrawSphere3D(m_pos, kModelSize, 8, 0xff0000, 0xffffff, false);
	//// キャラ動作状態
	//if (m_state == State::IDLE)
	//{
	//	DrawFormatString(500, 60, 0xffffff, "BSTATE : IDLE");
	//}
	//else if (m_state == State::ATTACK)
	//{
	//	DrawFormatString(500, 60, 0xffffff, "BSTATE : ATTACK");
	//}
	//else if (m_state == State::DODGE)
	//{
	//	DrawFormatString(500, 60, 0xffffff, "BSTATE : DODGE");
	//}
	//else if (m_state == State::MOVE)
	//{
	//	DrawFormatString(500, 60, 0xffffff, "BSTATE : MOVE");
	//}
	//else if (m_state == State::SKILL)
	//{
	//	DrawFormatString(500, 60, 0xffffff, "BSTATE : SKILL");
	//}
	//else if (m_state == State::DOWN)
	//{
	//	DrawFormatString(500, 60, 0xffffff, "BSTATE : DOWN");
	//}
	//else if (m_state == State::NOCKBACK)
	//{
	//	DrawFormatString(500, 60, 0xffffff, "BSTATE : KNOCKBACK");
	//}

	/*if (m_isDealDamage)
	{
		DrawSphere3D(m_hitArea, m_hitAreaRange, 8, 0x00ff00, 0xffffff, false);
	}*/
}

float Boss::GetSize() const
{
	return kModelSize;
}

void Boss::Move()
{
	m_state = State::MOVE;
	// 方向ベクトルに移動量分スケールする
	VECTOR moveVec = VScale(m_dir, kMoveSpeed);

	m_pos = VAdd(m_pos, moveVec);
}

void Boss::NearAttack()
{
	m_state = State::ATTACK;
}

void Boss::LongAttack(VECTOR toPlayer)
{
	m_state = State::SKILL;
	if (m_actionTimer < 60)
	{
		m_runDir = VScale(toPlayer, 0.15f);
		m_dir = VNorm(toPlayer);
		Rotation();
	}
	else if (m_actionTimer == 60)
	{
		// ダメージ関連処理
		m_hitArea = m_pos;
		m_hitAreaRange = kModelSize * 1.1f;
		m_damagePoint = m_status.POW * (VSize(toPlayer) / 1000);
		m_isDealDamage = true;
		m_pSoundManager->OnePlay("SE_Boss_taccle");
	}
	else
	{
		// ダメージ関連処理
		m_hitArea = m_pos;
		// 方向ベクトルに移動量分スケールする
		m_runDir = VScale(m_runDir, 0.9f);
		VECTOR moveVec = m_runDir;
		// 反映
		m_pos = VAdd(m_pos, moveVec);
	}
}

void Boss::AnimationUpdate()
{
	if (m_state == State::IDLE)
	{
		m_animation.SetAnimation(Anima::IDLE, 2);
	}
	else if (m_state == State::MOVE)
	{
		m_animation.SetAnimation(Anima::MOVE, 10);
	}
	else if (m_state == State::ATTACK)
	{
		if (m_attackCount < 2)
		{
			m_animation.SetAnimation(Anima::SRASH, 6 );
			if(m_actionTimer < 10)
				m_pEffectManager->PlayEffect("yokofuri", m_pos, m_dir);
		}
		else
		{
			m_animation.SetAnimation(Anima::ATTACK, 8);
			if (m_actionTimer < 10)
				m_pEffectManager->PlayEffect("furiorosi", m_pos, m_dir);
		}
	}
	else if (m_state == State::SKILL)
	{
		if (m_actionTimer < 60)
		{
			m_animation.SetAnimation(Anima::PRELIMINARY, 8);
			m_pEffectManager->MoveEffectPos("takkuru", m_pos, m_dir);
			m_pEffectManager->PlayEffect("takkuru", m_pos, m_dir);
		}
		else
		{
			m_animation.SetAnimation(Anima::TACLE, 8);
		}
	}
	else if (m_actionTimer < 270)
	{

	}
	else if (m_actionTimer < 300)
	{

	}
	// アニメーション再生
	m_animation.Play();
}
