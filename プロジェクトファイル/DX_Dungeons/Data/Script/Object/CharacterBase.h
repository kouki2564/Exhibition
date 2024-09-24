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

		// �_���[�W�֌W
		// �_���[�W�G���A(���`)
		m_hitArea = VGet(0.0f, 0.0f, 0.0f);
		m_hitAreaRange = 0;
		// �_���[�W�t���O
		m_isDealDamage = false;
		// �G�ɗ^����_���[�W�l
		m_damagePoint = 0;
		m_HP = 0;
		m_HPBarlangth = 0;
	}
	virtual ~CharacterBase() { DeleteGraph(m_hpBarHandle); }

	virtual void Draw() const = 0;

	virtual float GetSize() const = 0;

	void HitOther(float mySize, VECTOR pusherPos, float pusherSize);

	void HitWall(float charaSize);

	// �����o���x�N�g����ݒ�
	void SetPushVec(VECTOR vec) { m_pushVec = vec; }

	// �_���[�W�֘A
	// �_���[�W��^���鑤
	bool IsHitAttack(VECTOR pos, float size);
	int GetDamage();
	VECTOR GetDir() { return m_dir; }
	// �_���[�W���󂯂鑤
	bool SetDamage(int damage);

	int GetHP() { return m_HP; }


	UIPanel& GetUI() { return m_UI; }

private:
	virtual void AnimationUpdate() = 0;

protected:
	/// <summary>
	/// ���f���̉�]
	/// </summary>
	/// <param name="moveVec">��]��̐��ʕ���</param>
	void Rotation();

	void WeponMove(int attachFrame);


	int m_weponHandle;
	int m_weponAttachFrameNum;

	State m_state;

	// ����
	VECTOR m_dir;

	VECTOR m_moveVecInFrame;

	AnimationManager m_animation;

	int m_actionTimer;
	
	// �_���[�W�֌W
	// �_���[�W�G���A(���`)
	VECTOR m_hitArea;
	int m_hitAreaRange;
	// �_���[�W�t���O
	bool m_isDealDamage;
	// �G�ɗ^����_���[�W�l
	int m_damagePoint;

	// ���t���[���Ԍ��܂��������ɉ����o���ׂ̃x�N�g��
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

