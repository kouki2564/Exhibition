#pragma once
#include "SceneBase.h"

class HubScene :
    public SceneBase
{
public:
    HubScene();
    ~HubScene();

    void Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<DataFile>& dataFile);
    void Update();
    void Draw() const;

    bool GetEnd() { return m_isGameEnd; }

private:
    void ChooseMenu();

    void UpgradeMenu();
    void DrawMenu() const;

    void InitUI();


    int m_backHandle;
    int m_cameraTargetNum;
    VECTOR m_cameraPos[3];
    VECTOR m_cameraTargetPos[3];

    // 残ポイント
    int m_restPoint;
    // プレイヤーレベルアップ必要ポイント
    int m_playerNeedLvUpPoint;
    // ボスレベルアップ必要ポイント
    int m_bossNeedLvUpPoint;
    // プレイヤーレベルアップ必要ポイント
    int m_playerUsedLvUpPoint;
    // ボスレベルアップ必要ポイント
    int m_bossUsedLvUpPoint;
    // 残ポイント算出用
    void MathPoint();

    // UI関連
    UIPanel m_UI;
    int m_uiHandle[6];
    ExtendPos m_UIPos[3];
    bool m_isUI[3];

    UIPanel m_kyoukaUI;
    int m_kyoukaUIHandle[9];
    int m_rewardUIHandle[6];
    int m_blindHubAlpha;
    bool m_isMenu;
    int m_isInputCoolTimer;
    int m_line;
    int m_colum;

    // フォント
    int m_fontHandle[3];

    int m_InputStayTimer;

    bool m_isEnd;

    bool m_isStayKey;
};



// memo
//
// プレイヤーとボスを９９レベルまでに「49500 - 990」必要
// 所持制限を「99999」として、あまり「50499」
// 
// トロフィーを
// 「９９」「４９０」「１９００」「６０００」「１３０００」「３００００」
// できっちり使い切れる
//
