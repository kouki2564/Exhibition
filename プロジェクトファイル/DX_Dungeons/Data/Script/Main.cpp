#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "SceneManager.h"
#include <memory>

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �A�C�R���ύX
	SetWindowIconID(111);

	// �V�X�e���o�[�^�C�g��
	SetWindowTextA("Ordeal Maze");

	// �E�C���h�E���[�h�ݒ�
	ChangeWindowMode(true);

	// ��ʃT�C�Y�ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, 32);

	// ����ʂɕ`����s��
	SetDrawScreen(DX_SCREEN_BACK);

	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);


	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	SetLightEnable(FALSE);

	// �t�H���g�̃��[�h���ύX
	LPCSTR font_path = "Data/Font/mushin.otf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		assert(false);
	}
	ChangeFont("���S", DX_CHARSET_DEFAULT);

	int fontHandle = CreateFontToHandle("���S", 32, 1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	ChangeFontFromHandle(fontHandle);

	// Z�o�b�t�@��L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetUseZBuffer3D(TRUE);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetWriteZBuffer3D(TRUE);


	std::unique_ptr<SceneManager> pManager = std::make_unique<SceneManager>();
	pManager->Init();

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// �t���[���J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A�ɂ���
		ClearDrawScreen();

		// �Q�[���̏���
		pManager->Update();


		// ��ʂ��؂�ւ��̂�҂�
		ScreenFlip();

		// esc�L�[�ŃQ�[���I��
		if (pManager->GetEnd())
		{
			break;
		}

		// FPS��60�ɌŒ�
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16,66�~���b(16667�}�C�N���b)�o�߂܂ő҂�
		}
	}

	DeleteFontToHandle(fontHandle);

	// �t�H���g�̃A�����[�h
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {}
	else 
	{
		assert(false);
	}

	// Effekseer���I������B
	Effkseer_End();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}