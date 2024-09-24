#pragma once
#include "DxLib.h"
#include "Game.h"
#include <cassert>

 enum Input
{
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	LB = 4,
	RB = 5,
	BACK = 6,
	START = 7,
	STICK_L = 8,
	STICK_R = 9
};

 enum InputPOV
 {
	 UP = 0,
	 UP_RIGHT = 1,
	 RIGHT = 2,
	 DOWN_RIGHT = 3,
	 DOWN = 4,
	 DOWN_LEFT = 5,
	 LEFT = 6,
	 UP_LEFT = 7,
 };

/// <summary>
/// �X�e�B�b�N�̓��͒l�iint x, int y�j
/// </summary>
struct StickInput
{
	int x = 0;
	int y = 0;
};

class Config
{
public:
	Config()
	{
		GetJoypadDirectInputState(DX_INPUT_PAD1, &m_pad);
		for (int i = 0; i < Game::kButtonNum; i++)
		{
			m_isLastHit[i] = true;
		}
		m_isLastStick = true;
	}

	/// <summary>
	/// �X�e�B�b�N����
	/// </summary>
	/// <param name="num">�X�e�B�b�N(enum Input)</param>
	/// <returns>int x (-1000�`1000), int y (-1000�`1000)</returns>
	StickInput GetStick(int num);

	/// <summary>
	/// �X�e�B�b�N���́iXY�����w��̒P�����j
	/// </summary>
	/// <param name="num"></param>
	/// <param name="isX"></param>
	/// <returns></returns>
	int GetStickDown(int num, bool isX);

	/// <summary>
	/// �\���L�[�̉������ςȂ�
	/// </summary>
	/// <param name="inputPOV"> enum InputPOV ���Q�� </param>
	/// <returns></returns>
	bool GetInputPOVStay(int inputPOV);

	bool GetInputPOVDown(int inputPOV);

	/// <summary>
	/// �{�^���̉������ςȂ�����i�X�e�B�b�N�͎g���܂���j
	/// </summary>
	/// <param name="num">�{�^��(enum Input)</param>
	/// <returns>true : �����Ă���, false : �����Ă��Ȃ�</returns>
	bool GetInputStay(int num);
	

	/// <summary>
	/// �{�^���̒P�����i�X�e�B�b�N�͎g���܂���j
	/// </summary>
	/// <param name="num">�{�^��(enum Input)</param>
	/// <returns>true : �����Ă���, false : �������ςȂ� or �����Ă��Ȃ�</returns>
	bool GetInputDown(int num);

	/// <summary>
	/// �{�^��������ԁ������i�X�e�B�b�N�͎g���܂���j
	/// </summary>
	/// <param name="num">�{�^��(enum Input)</param>
	/// <returns>true : ������, false : �������ςȂ� or �����Ă��Ȃ�</returns>
	bool GetInputUp(int num);

private:
	bool m_isLastHit[Game::kButtonNum + 8];
	bool m_isLastStick;
	DINPUT_JOYSTATE m_pad;
};

