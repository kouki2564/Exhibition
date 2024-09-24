#include "Config.h"

StickInput Config::GetStick(int num)
{
	// LRスティック以外ならば止める
	assert(num == STICK_L || num == STICK_R);

	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_pad);
	if (num == STICK_L)			// Lスティック
	{
		return { m_pad.X, m_pad.Y };
	}
	else if (num == STICK_R)	// Rスティック
	{
		return { m_pad.Rx, m_pad.Ry };
	}
	else
	{
		assert(false);
	}
}

int Config::GetStickDown(int num, bool isX)
{
	// LRスティック以外ならば止める
	assert(num == STICK_L || num == STICK_R);
	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_pad);

	if (!m_isLastStick)
	{
		if (num == STICK_L)			// Lスティック
		{
			if (isX == true)
			{
				if (m_pad.X != 0)
				{
					m_isLastStick = true;
				}
				return  m_pad.X;
			}
			else
			{
				if (m_pad.Y != 0)
				{
					m_isLastStick = true;
				}
				return m_pad.Y;
			}
		}
		else if (num == STICK_R)	// Rスティック
		{
			if (isX == true)
			{
				if (m_pad.Rx != 0)
				{
					m_isLastStick = true;
				}
				return  m_pad.Rx;
			}
			else
			{
				if (m_pad.Ry != 0)
				{
					m_isLastStick = true;
				}
				return m_pad.Ry;
			}
		}
		else
		{
			assert(false);
		}
	}
	else
	{
		if (num == STICK_L)			// Lスティック
		{
			if (isX == true && m_pad.X == 0)
			{
				m_isLastStick = false;
			}
			else if(isX = false && m_pad.Y == 0)
			{
				m_isLastStick = false;
			}
		}
		else if (num == STICK_R)	// Rスティック
		{
			if (isX == true && m_pad.Rx == 0)
			{
				m_isLastStick = false;
			}
			else if (isX = false && m_pad.Ry == 0)
			{
				m_isLastStick = false;
			}
		}
		else
		{
			assert(false);
		}
	}

	return 0;
}

bool Config::GetInputPOVStay(int inputPOV)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_pad);
	if (m_pad.POV[0] == inputPOV * 4500)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Config::GetInputPOVDown(int inputPOV)
{

	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_pad);
	auto isHit = (m_pad.POV[0] == inputPOV * 4500);
	// 直前動作で押されていないとき
	if (!m_isLastHit[Game::kButtonNum + inputPOV])
	{

		// 押し状況をそのままboolにして返す
		m_isLastHit[Game::kButtonNum + inputPOV] = isHit;

		return m_isLastHit[Game::kButtonNum + inputPOV];
	}
	// 直前動作で押されていた時
	else
	{

		// 現在フレームで押されていなければfalseに
		m_isLastHit[Game::kButtonNum + inputPOV] = isHit;

		// 返り値は関係なくfalse
		return false;
	}
}

bool Config::GetInputStay(int num)
{
	// 指定しているボタン以外ならば止める
	assert(num >= 0 && num < Game::kButtonNum);


	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_pad);
	// ボタン入力をそのままboolとして返す
	return m_pad.Buttons[num];
}

bool Config::GetInputDown(int num)
{
	// 指定しているボタン以外ならば止める
	assert(num >= 0 && num < Game::kButtonNum);


	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_pad);
	// 直前動作で押されていないとき
	if (!m_isLastHit[num])
	{
		// 押し状況をそのままboolにして返す
		m_isLastHit[num] = m_pad.Buttons[num];
		return m_isLastHit[num];
	}
	// 直前動作で押されていた時
	else
	{
		// 現在フレームで押されていなければfalseに
		m_isLastHit[num] = m_pad.Buttons[num];
		// 返り値は関係なくfalse
		return false;
	}
}

bool Config::GetInputUp(int num)
{
	// 指定しているボタン以外ならば止める
	assert(num >= 0 && num < Game::kButtonNum);

	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_pad);
	// 直前動作で押されていたとき
	if (m_isLastHit[num])
	{
		// 押し状況を逆にして返す
		m_isLastHit[num] = m_pad.Buttons[num];
		return !m_isLastHit[num];
	}
	// 直前動作で押されていない時
	else
	{
		// 現在フレームで押されていなければfalseに
		m_isLastHit[num] = m_pad.Buttons[num];
		// 返り値は関係なくfalse
		return false;
	}
}
