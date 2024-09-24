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
    // 入力状況による毎フレームの処理
    void inputUpdate(VECTOR cameraPos);
    // 入力状況かかわらず毎フレーム行われる処理
    void StatusUpdate(VECTOR cameraPos);

    // プレイヤー移動
    void Move(VECTOR cameraPos, float movePow);
    // 回避
    void Dodge(VECTOR cameraPos, float movePow);
    // 通常攻撃
    void NormalAttack();
    // アニメーション更新
    void AnimationUpdate();
    // UI登録
    void UIInit();
    // UI更新
    void UIUpdate();


private:
    //  通常攻撃関連
    int m_prevCount;
    int m_attackCount;
    int m_attackCoolTimer;

    // 回避関連
    int m_dodgeCoolTimer;
    VECTOR m_dodgeVec;
    float m_movePow;

    // スキル技関連
    int m_skillCoolTimer;

    Config m_config;

    // ゲージUI
    int m_handle_gauge[2];
    ExtendPos m_gaugePos[2];
    float m_gaugelangth;

    // ボタンUI
    int m_handle_A;
    int m_handle_B;
    int m_handle_X;

    // ダメージUI
    int m_damageHandle;
    int m_damageAlpha;
};

