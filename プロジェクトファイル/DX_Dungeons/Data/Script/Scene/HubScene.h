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

    // �c�|�C���g
    int m_restPoint;
    // �v���C���[���x���A�b�v�K�v�|�C���g
    int m_playerNeedLvUpPoint;
    // �{�X���x���A�b�v�K�v�|�C���g
    int m_bossNeedLvUpPoint;
    // �v���C���[���x���A�b�v�K�v�|�C���g
    int m_playerUsedLvUpPoint;
    // �{�X���x���A�b�v�K�v�|�C���g
    int m_bossUsedLvUpPoint;
    // �c�|�C���g�Z�o�p
    void MathPoint();

    // UI�֘A
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

    // �t�H���g
    int m_fontHandle[3];

    int m_InputStayTimer;

    bool m_isEnd;

    bool m_isStayKey;
};



// memo
//
// �v���C���[�ƃ{�X���X�X���x���܂łɁu49500 - 990�v�K�v
// �����������u99999�v�Ƃ��āA���܂�u50499�v
// 
// �g���t�B�[��
// �u�X�X�v�u�S�X�O�v�u�P�X�O�O�v�u�U�O�O�O�v�u�P�R�O�O�O�v�u�R�O�O�O�O�v
// �ł�������g���؂��
//
