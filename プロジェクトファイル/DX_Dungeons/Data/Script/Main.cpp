#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "SceneManager.h"
#include <memory>

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// アイコン変更
	SetWindowIconID(111);

	// システムバータイトル
	SetWindowTextA("Ordeal Maze");

	// ウインドウモード設定
	ChangeWindowMode(true);

	// 画面サイズ設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, 32);

	// 裏画面に描画を行う
	SetDrawScreen(DX_SCREEN_BACK);

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);


	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	SetLightEnable(FALSE);

	// フォントのロード＆変更
	LPCSTR font_path = "Data/Font/mushin.otf"; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		assert(false);
	}
	ChangeFont("無心", DX_CHARSET_DEFAULT);

	int fontHandle = CreateFontToHandle("無心", 32, 1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	ChangeFontFromHandle(fontHandle);

	// Zバッファを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetWriteZBuffer3D(TRUE);


	std::unique_ptr<SceneManager> pManager = std::make_unique<SceneManager>();
	pManager->Init();

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// フレーム開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアにする
		ClearDrawScreen();

		// ゲームの処理
		pManager->Update();


		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (pManager->GetEnd())
		{
			break;
		}

		// FPSを60に固定
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16,66ミリ秒(16667マイクロ秒)経過まで待つ
		}
	}

	DeleteFontToHandle(fontHandle);

	// フォントのアンロード
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {}
	else 
	{
		assert(false);
	}

	// Effekseerを終了する。
	Effkseer_End();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}