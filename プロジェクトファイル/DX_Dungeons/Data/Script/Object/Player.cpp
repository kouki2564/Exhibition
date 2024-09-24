#include "Player.h"
#include "Quaternion.h"
#include "Config.h"

// 一旦enum化　あとで外部ファイル化
enum Anima
{
	IDLE = 1,
	RUN = 6,
	DODGE = 15,
	ATTACK_1 = 31,
	ATTACK_2 = 30,
	ATTACK_3 = 33,
	SKILL_1 = 38

};

namespace
{
	//////////////////////////
	// アクション関連
	//////////////////////////
	/// 通常攻撃
	// 攻撃モーション全体の時間
	constexpr int kAttackTime = 20;
	// 連続攻撃の入力可能開始時間
	constexpr int kAttackPrevTime = 8;
	// 攻撃中の押し出し時間
	constexpr int kPushStartInAttack = 10;
	constexpr int kPushEndInAttack = 15;
	// フレームあたりの攻撃の押し出し距離
	// 1,2撃目
	constexpr float kAttackPow = 5.0f;
	// 3撃目
	constexpr float kAttackLastPow = 15.0f;
	/// 回避
	// 回避時間
	constexpr int kDodgeTime = 18;
	// フレームあたりの回避距離
	constexpr float kDodgePow = 15.0f;
	/// スキル攻撃
	// スキル攻撃時間
	constexpr int kSkill_1Time = 40;

	//////////////////
	// クールタイム
	//////////////////
	// 通常攻撃
	constexpr int kAttackCoolMax = 20;
	// 回避
	constexpr int kDodgeCoolMax = 60;
	// スキル技
	constexpr int kSkill_1CoolMax = 90;

	/////////////////////////
	// アニメーション速度
	/////////////////////////
	// 停止
	constexpr int kIdleSpeed = 5;
	// 通常攻撃
	constexpr int kAttackSpeed = 17;
	// 回避
	constexpr int kDodgeSpeed = 10;
	// スキル攻撃
	constexpr int kSkill_1Speed = 15;

	///////////////
	// モデル関係
	///////////////

	constexpr float kModelSize = 50;

	// 数値
	constexpr float kFirstHPBarlangth = Game::kScreenHalfWidth * (1.25f - 0.75f);
}

Player::Player() :
	m_attackCount(0),
	m_movePow(0.01f),
	m_dodgeCoolTimer(0),
	m_prevCount(0),
	m_attackCoolTimer(0),
	m_skillCoolTimer(0)
{
	m_handle = MV1LoadModel("Data/Model/Character/Knight.mv1");
	MV1SetScale(m_handle, VGet(50, 50, 50));
	m_animation.InitAnimation(m_handle, Anima::IDLE, 10, 0);
	m_dodgeVec = VGet(0, 0, 0);

	// UIハンドル
	m_handle_gauge[0] = LoadGraph("Data/Graph/gauge.png");
	m_handle_gauge[1] = LoadGraph("Data/Graph/gauge.png");
	m_hpBarHandle = LoadGraph("Data/Graph/BarV1_ProgressBar.png");
	m_handle_A = LoadGraph("Data/Graph/xbox_a_color_light.png");
	m_handle_B = LoadGraph("Data/Graph/xbox_b_color_light.png");
	m_handle_X = LoadGraph("Data/Graph/xbox_x_color_light.png");
	m_damageHandle = LoadGraph("Data/Graph/damage.png");

	m_damageAlpha = 0;

	// UI座標
	m_gaugePos[0] = {
		Game::kScreenWidth * 0.43f,
		Game::kScreenWidth * 0.6f,
		Game::kScreenHeight * 0.75f,
		Game::kScreenHeight * 1 };
	m_gaugePos[1] = {
		Game::kScreenWidth * 0.63f,
		Game::kScreenWidth * 0.8f,
		Game::kScreenHeight * 0.75f,
		Game::kScreenHeight * 1 };
	m_gaugelangth = Game::kScreenWidth * 0.17f;
	// HPバーUI
	m_HPBarPos = {
		Game::kScreenHalfWidth * 0.75f,
		Game::kScreenHalfWidth * 1.25f,
		Game::kScreenHeight * 0.8f,
		Game::kScreenHeight * 0.82f};
	m_HPBarlangth = m_HPBarPos.endX - m_HPBarPos.startX;

}

Player::~Player()
{
	MV1DeleteModel(m_handle);
	DeleteGraph(m_hpBarHandle);
	DeleteGraph(m_handle_gauge[0]);
	DeleteGraph(m_handle_gauge[1]);
	DeleteGraph(m_handle_A);
	DeleteGraph(m_handle_B);
	DeleteGraph(m_handle_X);
}

void Player::Init(VECTOR firstPos, std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, Status status)
{
	m_status = status;
	m_HP = m_status.HP;
	m_pos = firstPos;
	MV1SetPosition(m_handle, m_pos);
	MV1SetRotationXYZ(m_handle, VGet(0, DX_PI_F, 0));
	m_movePow += m_status.AGI * 0.00001f;
	///////////////////////
	// UI関連セットアップ
	///////////////////////
	UIInit();
	m_pEffectManager = effectmanager;
	// エフェクト
	m_pEffectManager->RegisterEffect("Attack_1", LoadEffekseerEffect("Data/Effect/Sword_1.efkefc", 1.0f), m_pos, m_dir);
	m_pEffectManager->RegisterEffect("Attack_2", LoadEffekseerEffect("Data/Effect/Sword_2.efkefc", 1.0f), m_pos, m_dir);
	m_pEffectManager->RegisterEffect("Attack_3", LoadEffekseerEffect("Data/Effect/Sword_3.efkefc", 1.0f), m_pos, m_dir);
	// スキル
	m_pEffectManager->RegisterEffect("Skill", LoadEffekseerEffect("Data/Effect/Skill.efkefc", 1.0f), m_pos, m_dir);
	// 回避
	m_pEffectManager->RegisterEffect("Dodge", LoadEffekseerEffect("Data/Effect/Dodge.efkefc", 1.0f), m_pos, m_dir);
	// ヒット
	m_pEffectManager->RegisterEffect("Hit", LoadEffekseerEffect("Data/Effect/Hit.efkefc", 1.0f), m_pos, m_dir);

	m_pSoundManager = soundManager;
}

void Player::Update(VECTOR cameraPos)
{
	// フレーム毎の移動量をリセット
	m_moveVecInFrame = VGet(0.0f, 0.0f, 0.0f);

	/////////////////
	// 入力時処理
	/////////////////
	inputUpdate(cameraPos);

	////////////////////
	// アニメーション
	////////////////////
	AnimationUpdate();


	////////////////////
	// 毎フレーム処理
	////////////////////
	StatusUpdate(cameraPos);


	// 移動量をまとめて更新
	m_pos = VAdd(m_pos, m_moveVecInFrame);
	// 壁の当たり判定
	HitWall(kModelSize);
	// モデルに移動を反映
	MV1SetPosition(m_handle, m_pos);

	// UI更新
	UIUpdate();
}

void Player::Draw() const
{
	// 位置反映
	MV1DrawModel(m_handle);

	////////////////
	// デバック文
	////////////////
	//// 通常攻撃
	//DrawFormatString(510, 10, 0xffffff, "%d", m_attackCount);
	//DrawFormatString(500, 10, 0xffffff, "%d", m_prevCount);
	//// キャラ動作状態
	//if (m_state == State::IDLE)
	//{
	//	DrawFormatString(500, 30, 0xffffff, "STATE : IDLE");
	//}
	//else if (m_state == State::ATTACK)
	//{
	//	DrawFormatString(500, 30, 0xffffff, "STATE : ATTACK");
	//}
	//else if (m_state == State::DODGE)
	//{
	//	DrawFormatString(500, 30, 0xffffff, "STATE : DODGE");
	//}
	//else if (m_state == State::MOVE)
	//{
	//	DrawFormatString(500, 30, 0xffffff, "STATE : MOVE");
	//}
	//else if (m_state == State::SKILL)
	//{
	//	DrawFormatString(500, 30, 0xffffff, "STATE : SKILL");
	//}
	//else if (m_state == State::DOWN)
	//{
	//	DrawFormatString(500, 30, 0xffffff, "STATE : DOWN");
	//}
	//else if (m_state == State::NOCKBACK)
	//{
	//	DrawFormatString(500, 30, 0xffffff, "STATE : KNOCKBACK");
	//}
	//// 簡易操作説明
	//DrawFormatString(700, 10, 0xffffff, " カメラ移動 ： Rスティック");
	//DrawFormatString(700, 30, 0xffffff, "　　移動　　： Lスティック");
	//if (m_state == State::ATTACK || m_state == State::DODGE || m_state == State::SKILL)
	//{
	//	DrawFormatString(700, 30, 0x303030, "　　移動　　： Lスティック");
	//}
	//DrawFormatString(700, 50, 0x303030, "　　攻撃　　： A（ %d／3段攻撃 ）( クールタイム：%d/%d )", m_attackCount, m_attackCoolTimer, kAttackCoolMax);
	//if (m_attackCoolTimer >= kAttackCoolMax && m_state != State::DODGE && m_state != State::SKILL)
	//{
	//	DrawFormatString(700, 50, 0xffffff, "　　攻撃　　： A（ %d／3段攻撃 ）( クールタイム：%d/%d )", m_attackCount, m_attackCoolTimer, kAttackCoolMax);
	//}
	//DrawFormatString(700, 70, 0x303030, "ステップ回避： B　( クールタイム：%d/%d )", m_dodgeCoolTimer, kDodgeCoolMax);
	//if (m_dodgeCoolTimer >= kDodgeCoolMax && m_state != State::SKILL)
	//{
	//	DrawFormatString(700, 70, 0xffffff, "ステップ回避： B　( クールタイム：%d/%d )", m_dodgeCoolTimer, kDodgeCoolMax);
	//}
	//DrawFormatString(700, 90, 0x303030, " スキル攻撃 ： X　( クールタイム：%d/%d )", m_skillCoolTimer, kSkill_1CoolMax);
	//if (m_skillCoolTimer >= kSkill_1CoolMax && m_state != State::DODGE)
	//{
	//	DrawFormatString(700, 90, 0xffffff, " スキル攻撃 ： X　( クールタイム：%d/%d )", m_skillCoolTimer, kSkill_1CoolMax);
	//}
	//DrawFormatString(700, 130, 0xffffff, "アクションタイマー：%d", m_actionTimer);

	/*DrawSphere3D(m_pos, kModelSize, 8, 0xff0000, 0xffffff, false);
	if (m_isDealDamage)
	{
		DrawSphere3D(m_hitArea, m_hitAreaRange, 8, 0x00ff00, 0xffffff, false);
	}*/
}

void Player::inputUpdate(VECTOR cameraPos)
{
	////////////////////////////
	// 入力時処理
	////////////////////////////

	// 移動処理
	if ((m_config.GetStick(STICK_L).x != 0 || m_config.GetStick(STICK_L).y != 0) && m_state == State::IDLE)
	{
		m_state = State::MOVE;
	}
	else if (m_config.GetStick(STICK_L).x == 0 && m_config.GetStick(STICK_L).y == 0 && m_state == State::MOVE)
	{
		m_state = State::IDLE;
	}

	// 通常攻撃
	if (m_attackCoolTimer >= kAttackCoolMax)
	{
		if (m_config.GetInputStay(Input::A) && m_prevCount < Game::kAttackCountMax)
		{
			if (m_state == State::IDLE || m_state == State::MOVE)
			{
				m_state = State::ATTACK;
				SetPushVec(m_dir);
			}
			if (m_state == State::ATTACK)
				NormalAttack();
		}
	}
	if (m_attackCoolTimer < kAttackCoolMax)
	{
		m_attackCoolTimer++;
	}
	if ((m_attackCount != 0 || m_prevCount != 0) && m_state != State::ATTACK)
	{
		m_attackCount = 0;
		m_prevCount = 0;
	}

	// 回避
	if (m_config.GetInputDown(Input::B) && m_dodgeCoolTimer >= kDodgeCoolMax && m_state != State::SKILL)
	{
		m_state = State::DODGE;
		m_actionTimer = 0;
		m_attackCoolTimer = 0;

		/// 入力状況から方向を設定
		auto moveVec = VGet(0, 0, 0);
		//カメラから見た前方へのベクトル
		auto dirFront = VSub(m_pos, cameraPos);
		// yを０にした上で正規化することで、xz平面での向きを決める
		dirFront.y = 0;
		dirFront = VNorm(dirFront);
		// 正規化した前方ベクトルをもとに
		// 右向きベクトルの作成
		auto dirRight = VGet(-dirFront.z, 0, dirFront.x);

		// コントローラのスティック入力値を獲得
		auto stickInput = m_config.GetStick(STICK_L);

		// 入力に応じて、移動量を確定
		moveVec = VGet((dirFront.x * (-stickInput.y) + dirRight.x * (-stickInput.x)),
			0,
			(dirFront.z * (-stickInput.y) + dirRight.z * (-stickInput.x)));
		if (moveVec.x != 0 || moveVec.y != 0)
		{
			m_dir = VNorm(moveVec);
			m_dir.y = 0;
		}
		/// 押し出し方向を設定
		SetPushVec(m_dir);
		/// その方向にモデル回転
		Rotation();

		m_dodgeCoolTimer = 0;
	}
	if (m_dodgeCoolTimer < kDodgeCoolMax)
	{
		m_dodgeCoolTimer++;
		if ((m_attackCount != 0 || m_prevCount != 0) && m_state != State::ATTACK)
		{
			m_attackCount = 0;
			m_prevCount = 0;
		}
	}

	// スキル
	if (m_state != State::DODGE)
	{
		if (m_config.GetInputDown(Input::X) && m_skillCoolTimer >= kSkill_1CoolMax)
		{
			m_state = State::SKILL;
			m_actionTimer = 0;
			m_skillCoolTimer = 0;
			m_attackCoolTimer = 0;

		}
	}
	if (m_skillCoolTimer < kSkill_1CoolMax)
	{
		m_skillCoolTimer++;
	}
}

void Player::StatusUpdate(VECTOR cameraPos)
{
	/////////////////////
	// 毎フレーム処理
	/////////////////////

	// 停止,移動以外の動作時間更新
	if (m_state != State::IDLE && m_state != State::MOVE)
	{
		m_actionTimer++;
	}
	else
	{
		m_actionTimer = 0;
	}

	// 動作状態毎の処理
	if (m_state == State::IDLE)				// 停止
	{

	}
	else if (m_state == State::MOVE)		// 移動
	{
		Move(cameraPos, m_movePow);
		Rotation();
	}
	else if (m_state == State::ATTACK)		// 通常攻撃
	{
		// 通常攻撃の更新
		if (m_actionTimer == 10)
		{
			m_pSoundManager->OnePlay("SE_Player_Attack");
			// ダメージ関係処理
			m_hitArea = VAdd(m_pos, VScale(m_dir, kModelSize * 2.5f));
			m_hitAreaRange = kModelSize * 1.5f;
			m_damagePoint = m_status.POW * (m_attackCount * 0.2f + 1);
			m_isDealDamage = true;
		}
		if (m_actionTimer >= kAttackTime)
		{
			// 継続
			if (m_prevCount != m_attackCount)
			{
				m_attackCount = m_prevCount;
			}
			// 終了
			else
			{
				m_isDealDamage = false;
				m_attackCount = 0;
				m_prevCount = 0;
				m_attackCoolTimer = 0;
				m_state = State::IDLE;
			}
			m_actionTimer = 0;
		}
		else if (m_actionTimer >= kPushStartInAttack && m_actionTimer <= kPushEndInAttack)
		{

			// 攻撃中の押し出し
			if (m_attackCount != 3)
				m_moveVecInFrame = VAdd(m_moveVecInFrame, VScale(m_pushVec, kAttackPow));
			else
				m_moveVecInFrame = VAdd(m_moveVecInFrame, VScale(m_pushVec, kAttackLastPow));
		}
	}
	else if (m_state == State::DODGE)		// 回避
	{
		Dodge(cameraPos, m_movePow);
	}
	else if (m_state == State::SKILL)		// スキル攻撃
	{
		if (m_actionTimer == 5)
		{
			// ダメージ関係処理
			m_hitArea = m_pos;
			m_hitAreaRange = kModelSize * 5.5f;
			m_damagePoint = m_status.POW * 3;
			m_isDealDamage = true;
			m_pSoundManager->OnePlay("SE_Player_Skill");
		}
		if (m_actionTimer >= kSkill_1Time)
		{
			m_state = State::IDLE;
			m_isDealDamage = false;
			m_actionTimer = 0;
		}
	}
}




void Player::Move(VECTOR cameraPos, float movePow)
{
	auto moveVec = VGet(0,0,0);

	//カメラから見た前方へのベクトル
	auto dirFront = VSub(m_pos, cameraPos);
	// yを０にした上で正規化することで、xz平面での向きを決める
	dirFront.y = 0;
	dirFront = VNorm(dirFront);
	// 正規化した前方ベクトルをもとに
	// 右向きベクトルの作成
	auto dirRight = VGet(-dirFront.z, 0, dirFront.x);

	// コントローラのスティック入力値を獲得
	auto stickInput = m_config.GetStick(STICK_L);

	// 入力に応じて、移動量を確定
	moveVec = VGet((dirFront.x * (-stickInput.y) + dirRight.x * (-stickInput.x)),
				  0, 
				 (dirFront.z * (-stickInput.y) + dirRight.z * (-stickInput.x)));
	m_dir = VNorm(moveVec);
	m_dir.y = 0;
	moveVec = VScale(moveVec, movePow);
	// 移動反映
	m_moveVecInFrame = VAdd(m_moveVecInFrame, moveVec);
}


void Player::Dodge(VECTOR cameraPos, float movePow)
{
	// 移動反映
	if (m_dodgeCoolTimer <= kDodgeTime)
	{
		if (m_dodgeCoolTimer == 5)
		{
			m_pSoundManager->OnePlay("SE_Player_Dodge");
		}
		m_moveVecInFrame = VAdd(m_moveVecInFrame, VScale(m_pushVec, kDodgePow));
	}
	else
	{
		m_state = State::IDLE;
	}
}

void Player::NormalAttack()
{
	if (m_attackCount == m_prevCount)
	{
		if (m_attackCount == 0)
		{
			m_prevCount++;
			m_attackCount++;
		}
		else if ((m_animation.GetAnimationNum() == Anima::ATTACK_1 && m_attackCount == 1) ||
			(m_animation.GetAnimationNum() == Anima::ATTACK_2 && m_attackCount == 2))
		{
			if(m_actionTimer > kAttackPrevTime)
				m_prevCount++;
		}
	}

}

void Player::AnimationUpdate()
{
	auto stickInput = m_config.GetStick(Input::STICK_L);
	auto stickSize = sqrtf(stickInput.x * stickInput.x + stickInput.y * stickInput.y);

	// プレイヤーの状態に合わせたアニメーションを設定
	if (m_state == State::SKILL)				// スキル攻撃
	{
		m_animation.SetAnimation(Anima::SKILL_1, kSkill_1Speed);
		m_pEffectManager->PlayEffect("Skill", m_pos, m_dir);
		m_pEffectManager->MoveEffectPos("Skill", m_pos, m_dir);

	}
	else if (m_state == State::DODGE)			// 回避
	{
		m_animation.SetAnimation(Anima::DODGE, kDodgeSpeed); 
		m_pEffectManager->PlayEffect("Dodge", m_pos, m_dir);
	}
	else if (m_state == State::ATTACK)			// 通常攻撃
	{
		if (m_attackCount == 1)					// 1段目
		{
			m_animation.SetAnimation(Anima::ATTACK_1, kAttackSpeed);
			m_pEffectManager->PlayEffect("Attack_1", m_pos, m_dir);
			m_pEffectManager->MoveEffectPos("Attack_1", m_pos, m_dir);
		}
		else if (m_attackCount == 2)			// 2段目
		{
			m_animation.SetAnimation(Anima::ATTACK_2, kAttackSpeed);
			m_pEffectManager->PlayEffect("Attack_2", m_pos, m_dir);
			m_pEffectManager->MoveEffectPos("Attack_2", m_pos, m_dir);
		}
		else if (m_attackCount == 3)			// 3段目
		{
			m_animation.SetAnimation(Anima::ATTACK_3, kAttackSpeed);
			m_pEffectManager->PlayEffect("Attack_3", m_pos, m_dir);
			m_pEffectManager->MoveEffectPos("Attack_3", m_pos, m_dir);
		}
		
	}
	else if (m_state == State::MOVE)			// 移動
	{
		m_animation.SetAnimation(Anima::RUN, stickSize * 0.01f);
	}
	else if (m_state == State::IDLE)			// 停止
	{
		m_animation.SetAnimation(Anima::IDLE, kIdleSpeed);
	}


	// アニメーション再生
	m_animation.Play();
}

void Player::UIInit()
{
	// HPバー
	m_UI.SetUI(LoadGraph("Data/Graph/BarV1_Bar.png"),
		{ Game::kScreenHalfWidth * 0.75f,
		Game::kScreenHalfWidth * 1.25f,
		Game::kScreenHeight * 0.8f,
		Game::kScreenHeight * 0.82f },
		true, 255);
	m_UI.SetUI(m_hpBarHandle,
		m_HPBarPos,
		true, 255);
	m_UI.SetUI(LoadGraph("Data/Graph/BarV1_ProgressBarBorder.png"),
		{ Game::kScreenHalfWidth * 0.75f,
		Game::kScreenHalfWidth * 1.25f,
		Game::kScreenHeight * 0.8f,
		Game::kScreenHeight * 0.82f },
		true, 255);

	// 操作系統
	// ゲージ（back）
	m_UI.SetUI(LoadGraph("Data/Graph/gauge_back.png"),
		{ Game::kScreenWidth * 0.23f,
		Game::kScreenWidth * 0.4f,
		Game::kScreenHeight * 0.75f,
		Game::kScreenHeight * 1 },
		true, 255);
	m_UI.SetUI(LoadGraph("Data/Graph/gauge_back.png"),
		m_gaugePos[0],
		true, 255);
	m_UI.SetUI(LoadGraph("Data/Graph/gauge_back.png"),
		m_gaugePos[1],
		true, 255);
	// ゲージ（本体）
	// A
	m_UI.SetUI(LoadGraph("Data/Graph/gauge.png"),
		{ Game::kScreenWidth * 0.23f,
		Game::kScreenWidth * 0.4f,
		Game::kScreenHeight * 0.75f,
		Game::kScreenHeight * 1 },
		true, 255);
	// B
	m_UI.SetUI(m_handle_gauge[0],
		m_gaugePos[0],
		true, 255);
	// X
	m_UI.SetUI(m_handle_gauge[1],
		m_gaugePos[1],
		true, 255);
	// 文字
	m_UI.SetUI(LoadGraph("Data/Graph/moji_attack.png"),
		{ Game::kScreenWidth * 0.23f,
		Game::kScreenWidth * 0.4f,
		Game::kScreenHeight * 0.75f,
		Game::kScreenHeight * 1 },
		true, 255);
	m_UI.SetUI(LoadGraph("Data/Graph/moji_dodge.png"),
		m_gaugePos[0],
		true, 255);
	m_UI.SetUI(LoadGraph("Data/Graph/moji_skill.png"),
		m_gaugePos[1],
		true, 255);



	// Aボタン
	m_UI.SetUI(LoadGraph("Data/Graph/xbox_a_color_dark.png"),
		{ Game::kScreenWidth * 0.2f,
		Game::kScreenWidth * 0.23f,
		Game::kScreenHeight * 0.85f,
		Game::kScreenHeight * 0.90f },
		true, 255);
	m_UI.SetUI(m_handle_A,
		{Game::kScreenWidth * 0.2f,
		Game::kScreenWidth * 0.23f,
		Game::kScreenHeight * 0.85f,
		Game::kScreenHeight * 0.90f},
		true, 255);
	// Bボタン
	m_UI.SetUI(LoadGraph("Data/Graph/xbox_b_color_dark.png"),
		{ Game::kScreenWidth * 0.4f,
		Game::kScreenWidth * 0.43f,
		Game::kScreenHeight * 0.85f,
		Game::kScreenHeight * 0.90f },
		true, 255);
	m_UI.SetUI(m_handle_B,
		{ Game::kScreenWidth * 0.4f,
		Game::kScreenWidth * 0.43f,
		Game::kScreenHeight * 0.85f,
		Game::kScreenHeight * 0.90f },
		true, 255);
	// Xボタン
	m_UI.SetUI(LoadGraph("Data/Graph/xbox_x_color_dark.png"),
		{ Game::kScreenWidth * 0.6f,
		Game::kScreenWidth * 0.63f,
		Game::kScreenHeight * 0.85f,
		Game::kScreenHeight * 0.90f },
		true, 255);
	m_UI.SetUI(m_handle_X,
		{ Game::kScreenWidth * 0.6f,
		Game::kScreenWidth * 0.63f,
		Game::kScreenHeight * 0.85f,
		Game::kScreenHeight * 0.90f },
		true, 255);

	m_UI.SetUI(m_damageHandle,
		{ 0,
		Game::kScreenWidth,
		0,
		Game::kScreenHeight },
		true, m_damageAlpha);
}

void Player::UIUpdate()
{
	if (m_config.GetInputStay(Input::A))
	{
		m_UI.SetIsApp(m_handle_A, false);
	}
	else
	{
		m_UI.SetIsApp(m_handle_A, true);
	}

	if (m_dodgeCoolTimer < kDodgeCoolMax)
	{
		m_UI.SetIsApp(m_handle_B, false);
	}
	else
	{
		m_UI.SetIsApp(m_handle_B, true);
	}

	if (m_skillCoolTimer < kSkill_1CoolMax)
	{
		m_UI.SetIsApp(m_handle_X, false);
	}
	else
	{
		m_UI.SetIsApp(m_handle_X, true);
	}


	m_gaugePos[0].endX = (static_cast<float>(m_dodgeCoolTimer) / static_cast<float>(kDodgeCoolMax)) * m_gaugelangth	// クールタイムでの変動値
							+ (Game::kScreenWidth * 0.43f); // 開始点（startX）
	m_UI.MoveUI(m_handle_gauge[0], m_gaugePos[0]);

	m_gaugePos[1].endX = static_cast<float>(m_skillCoolTimer) / static_cast<float>(kSkill_1CoolMax) * m_gaugelangth	// クールタイムでの変動値
							+ (Game::kScreenWidth * 0.63f);	// 開始点（startX）
	m_UI.MoveUI(m_handle_gauge[1], m_gaugePos[1]);


	m_HPBarlangth = kFirstHPBarlangth * (static_cast<float>(m_HP) / static_cast<float>(m_status.HP));
	if (m_HPBarlangth < (m_HPBarPos.endX - m_HPBarPos.startX))
	{
		int temp = (m_HPBarPos.endX - m_HPBarPos.startX) - m_HPBarlangth;
		m_HPBarPos.endX -= temp * 0.5f;
		float damage = (kFirstHPBarlangth * temp) / kFirstHPBarlangth;
		// m_damageAlpha = 255 - (255 * damage);
		m_damageAlpha = 0;
		m_UI.ChangeAlpha(m_damageHandle, m_damageAlpha);
		if (m_HPBarPos.endX - m_HPBarPos.startX < 0)
		{
			m_HPBarPos.endX = m_HPBarPos.startX;
		}
		m_UI.MoveUI(m_hpBarHandle, m_HPBarPos);
	}

}

float Player::GetSize() const
{
	return kModelSize;
}
