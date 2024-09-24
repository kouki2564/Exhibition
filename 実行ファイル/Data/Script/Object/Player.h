#pragma once
#include "CharacterBase.h"
#include "Config.h"

class Player :
    public CharacterBase
{
public:
    Player();
    ~Player();

    void Init(VECTOR firstPos, std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, Status status);
    void Update(VECTOR cameraPos);
    void Draw() const;

    float GetSize() const;


private:
    // ���͏󋵂ɂ�閈�t���[���̏���
    void inputUpdate(VECTOR cameraPos);
    // ���͏󋵂�����炸���t���[���s���鏈��
    void StatusUpdate(VECTOR cameraPos);

    // �v���C���[�ړ�
    void Move(VECTOR cameraPos, float movePow);
    // ���
    void Dodge(VECTOR cameraPos, float movePow);
    // �ʏ�U��
    void NormalAttack();
    // �A�j���[�V�����X�V
    void AnimationUpdate();
    // UI�o�^
    void UIInit();
    // UI�X�V
    void UIUpdate();


private:
    //  �ʏ�U���֘A
    int m_prevCount;
    int m_attackCount;
    int m_attackCoolTimer;

    // ����֘A
    int m_dodgeCoolTimer;
    VECTOR m_dodgeVec;
    float m_movePow;

    // �X�L���Z�֘A
    int m_skillCoolTimer;

    Config m_config;

    // �Q�[�WUI
    int m_handle_gauge[2];
    ExtendPos m_gaugePos[2];
    float m_gaugelangth;

    // �{�^��UI
    int m_handle_A;
    int m_handle_B;
    int m_handle_X;

    // �_���[�WUI
    int m_damageHandle;
    int m_damageAlpha;
};

