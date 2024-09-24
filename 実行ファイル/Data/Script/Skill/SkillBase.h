#pragma once
#include "DxLib.h"
#include <memory>

class SkillBase :
	public std::enable_shared_from_this<SkillBase>
{
public:
	// ‹Z‚Ì”­¶
	virtual void Start() = 0;
	// ‹Z‚Ì“r’†’â~
	virtual void Cancel() = 0;
	// Ÿ‚Ì‹Z‚Ö‚Ì”h¶
	virtual void Conect() = 0;

protected:
	// ‹Z‚Ì“®ì
	virtual void Update() = 0;

	// ‹Z‚Ì”­¶’n“_
	VECTOR m_appPos;
	// ‹ZÄg—p‰Â”\‚Ü‚Å‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	int m_coolTimer;
};

