#pragma once
#include "DxLib.h"

//ゲーム全体で使う定数を定義

// データファイルとScene間の受け渡し用
struct SendData
{
	int playerLevel;
	int playerMaxLevel;
	int bossLevel;
	int bossMaxLevel;
	int point;
};

namespace Game
{
	//////////////
	// 基本設定 //
	//////////////

	// 画面幅
	constexpr int kScreenWidth = 1280;      //画面の横幅
	constexpr int kScreenHeight = 720;     //画面の縦幅
	// 半分サイズ
	constexpr int kScreenHalfWidth = kScreenWidth / 2;      //画面の横幅
	constexpr int kScreenHalfHeight = kScreenHeight / 2;     //画面の縦幅

	// ゲーム色深度
	constexpr int kColorDepth = 32;        //16 or 32

	//////////////////////
	// ゲームごとの設定 //
	//////////////////////

	// フィールドのグラフィック数
	constexpr int kFloorLine = 12;
	constexpr int kFloorRow = 12;
	// １つ当たりのサイズ
	constexpr float kModelSize = 160.0f;

	constexpr int kFloorSize = 1150;
	constexpr int kFloorSize2 = 800;

	// 移動座標範囲
	constexpr float kPosXMax = 900.0f;
	constexpr float kPosXMin = -900.0f;
	constexpr float kPosZMax = 900.0f;
	constexpr float kPosZMin = -900.0f;

	constexpr int kTarfgetNumMax = 25;

	// 全体シーン数
	constexpr int kSceneNum = 3;

	// 使用ボタン数
	constexpr int kButtonNum = 8;

	// サウンド数
	constexpr int kBGMNum = 0;
	constexpr int kSENum = 0;

	// プレイヤー通常攻撃回数
	constexpr int kAttackCountMax = 3;
}