#pragma once
#include <map>
#include <vector>
#include <string>
#include "DxLib.h"
#include "EffekseerForDXLib.h"


static int position_x;
static int position_y;

struct efc
{
	int handle;
	VECTOR pos;
	VECTOR dir;
	int playinghandle;
};

class Effect
{
public:
	Effect() {
		m_backDrawHandle = LoadGraph("Data/Graph/null.png");
		m_effectCoolTime = 0;
	}

	~Effect() { DeleteGraph(m_backDrawHandle); }

	void RegisterEffect(std::string effectName, int effectHandle, VECTOR pos, VECTOR dir);

	void UpdateEffect();
	void DrawEffect() const;

	void PlayEffect(std::string effectName, VECTOR pos, VECTOR dir);
	void StopEffect(std::string effectName);

	void MoveEffectPos(std::string effectName, VECTOR pos, VECTOR dir);

private:
	int m_backDrawHandle;
	std::map < std::string, efc> m_effect;

	int m_effectCoolTime;
};

