#pragma once
#include "ObjectBase.h"
#include "AnimationManager.h"
#include "UIPanel.h"
#include "Effect.h"
#include "SoundManager.h"

enum class State
{
	IDLE,
	MOVE,
	DODGE,
	ATTACK,
	SKILL,
	NOCKBACK,
	DOWN
};

struct Status
{
	int Level = 1;
	int HP = 1;
	int POW = 1;
	int AGI = 1;
	int MaxLevel = 1;
};

class CharacterBase :
    public ObjectBase
{
public:
    CharacterBase(){
		m_weponHandle = -1;
		m_weponAttachFrameNum = -1;
		m_HPBarPos = {0,0,0,0};
		m_hpBarHandle = -1;
		m_state = State::IDLE;
		m_dir = VGet(0.0f, 0.0f, 0.0f);
		m_pushVec = VGet(0.0f, 0.0f, 0.0f);
		m_actionTimer = 0;
		m_moveVecInFrame = VGet(0.0f, 0.0f, 0.0f);

		// ダメージ関係
		// ダメージエリア(球形)
		m_hitArea = VGet(0.0f, 0.0f, 0.0f);
		m_hitAreaRange = 0;
		// ダメージフラグ
		m_isDealDamage = false;
		// 敵に与えるダメージ値
		m_damagePoint = 0;
		m_HP = 0;
		m_HPBarlangth = 0;
	}
	virtual ~CharacterBase() { DeleteGraph(m_hpBarHandle); }

	virtual void Draw() const = 0;

	virtual float GetSize() const = 0;

	void HitOther(float mySize, VECTOR pusherPos, float pusherSize);

	void HitWall(float charaSize);

	// 押し出すベクトルを設定
	void SetPushVec(VECTOR vec) { m_pushVec = vec; }

	// ダメージ関連
	// ダメージを与える側
	bool IsHitAttack(VECTOR pos, float size);
	int GetDamage();
	VECTOR GetDir() { return m_dir; }
	// ダメージを受ける側
	bool SetDamage(int damage);

	int GetHP() { return m_HP; }


	UIPanel& GetUI() { return m_UI; }

private:
	virtual void AnimationUpdate() = 0;

protected:
	/// <summary>
	/// モデルの回転
	/// </summary>
	/// <param name="moveVec">回転先の正面方向</param>
	void Rotation();

	void WeponMove(int attachFrame);


	int m_weponHandle;
	int m_weponAttachFrameNum;

	State m_state;

	// 向き
	VECTOR m_dir;

	VECTOR m_moveVecInFrame;

	AnimationManager m_animation;

	int m_actionTimer;
	
	// ダメージ関係
	// ダメージエリア(球形)
	VECTOR m_hitArea;
	int m_hitAreaRange;
	// ダメージフラグ
	bool m_isDealDamage;
	// 敵に与えるダメージ値
	int m_damagePoint;

	// 一定フレーム間決まった方向に押し出す為のベクトル
	VECTOR m_pushVec;

	UIPanel m_UI;
	ExtendPos m_HPBarPos;
	int m_hpBarHandle;
	float m_HPBarlangth;

	std::shared_ptr<Effect> m_pEffectManager;
	std::shared_ptr<SoundManager> m_pSoundManager;

	Status m_status;
	int m_HP;
};

