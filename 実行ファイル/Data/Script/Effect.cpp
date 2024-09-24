#include "Effect.h"
#include "EffekseerForDXLib.h"

namespace
{
	constexpr int kEffectCoolMax = 300;
}

void Effect::RegisterEffect(std::string effectName, int effectHandle, VECTOR pos, VECTOR dir)
{
	for (auto& i : m_effect)
	{
		if (i.first == effectName)
		{
			return;
		}
	}
	efc temp = { effectHandle, pos, dir, -1};
	m_effect.insert(std::make_pair(effectName, temp));

}

void Effect::UpdateEffect()
{
	UpdateEffekseer3D();
}

void Effect::DrawEffect() const
{
	DrawGraph(0, 0, m_backDrawHandle, true);
	DrawEffekseer3D();
}

void Effect::PlayEffect(std::string effectName, VECTOR pos, VECTOR dir)
{
	if (IsEffekseer3DEffectPlaying(m_effect[effectName].playinghandle) == -1)
	{
		m_effect[effectName].playinghandle = PlayEffekseer3DEffect(m_effect[effectName].handle);

		MoveEffectPos(effectName, pos, dir);
	}
	else
	{
		/*if(m_effectCoolTime < kEffectCoolMax)
			m_effectCoolTime++;*/
	}
	//assert(PlayEffekseer3DEffect(m_effect[effectName].handle) != -1);

}

void Effect::StopEffect(std::string effectName)
{
	/*for (int i = 0; i < m_playinghandle.size(); i++)
	{
		if (m_playinghandle[i] == m_effect[effectname].first)
		{
			m_playinghandle.erase(m_playinghandle.begin() + i);
		}
	}*/
}

void Effect::MoveEffectPos(std::string effectName, VECTOR pos, VECTOR dir)
{
	m_effect[effectName].pos = pos;
	m_effect[effectName].dir = dir;
	SetPosPlayingEffekseer3DEffect(m_effect[effectName].playinghandle,
		m_effect[effectName].pos.x, m_effect[effectName].pos.y, m_effect[effectName].pos.z);
	SetRotationPlayingEffekseer3DEffect(m_effect[effectName].playinghandle,
		0.0f, atan2(m_effect[effectName].dir.x, m_effect[effectName].dir.z) - (DX_PI_F / 2), 0.0f);
	UpdateEffekseer3D();
}
