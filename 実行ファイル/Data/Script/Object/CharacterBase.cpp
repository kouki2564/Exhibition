#include "CharacterBase.h"
#include "Game.h"
#include <math.h>

void CharacterBase::HitOther(float mySize, VECTOR pusherPos, float pusherSize)
{
	VECTOR ToPusher = VSub(pusherPos, m_pos);
	ToPusher.y = 0;
	if (VSize(ToPusher) < (mySize + pusherSize))
	{
		float pushScale = (VSize(ToPusher) - (mySize + pusherSize)) / (mySize + pusherSize) * 5;
		ToPusher = VNorm(ToPusher);
		ToPusher.y = 0;
		VECTOR pushVec = VScale(ToPusher, pushScale);
		m_pos = VAdd(m_pos, pushVec);
	}
}

void CharacterBase::HitWall(float charaSize)
{
	VECTOR Point1ToChara;
	VECTOR Point1ToPoint2;

	/*
		ai_norm = np.dot(ap, ab)/norm(ab)
        neighbor_point = a + (ab)/norm(ab)*ai_norm
        distance = norm(p - neighbor_point)
	*/

	if (m_pos.x < 0 && m_pos.z < 0)
	{
		VECTOR ap = VSub(m_pos, VGet(-800, 0, -1150));
		VECTOR ab = VSub(VGet(-1150, 0, -800), VGet(-800, 0, -1150));
		float temp = (ap.x * ab.x + ap.z * ab.z) / (VSize(ab) * VSize(ab));
		VECTOR ai = VScale(ab, temp);
		VECTOR pi = VSub(ai, ap);
		if (VSize(pi) < charaSize)
		{
			if (pi.x < 0 && pi.z < 0)
			{
				float rangeScale = (VSize(pi) - charaSize) / charaSize;
				m_pos = VAdd(m_pos, VScale(pi, rangeScale));
			}
			else
			{
				float rangeScale = ((-VSize(pi)) - charaSize) / charaSize;
				m_pos = VAdd(m_pos, VScale(VSub(VGet(0,0,0),pi), rangeScale));
			}
		}
	}
	else if (m_pos.x < 0 && m_pos.z > 0)
	{
		VECTOR ap = VSub(m_pos, VGet(-800, 0, 1150));
		VECTOR ab = VSub(VGet(-1150, 0, 800), VGet(-800, 0, 1150));
		float temp = (ap.x * ab.x + ap.z * ab.z) / (VSize(ab) * VSize(ab));
		VECTOR ai = VScale(ab, temp);
		VECTOR pi = VSub(ai, ap);
		if (VSize(pi) < charaSize)
		{
			if (pi.x < 0 && pi.z > 0)
			{
				float rangeScale = (VSize(pi) - charaSize) / charaSize;
				m_pos = VAdd(m_pos, VScale(pi, rangeScale));
			}
			else
			{
				float rangeScale = ((-VSize(pi)) - charaSize) / charaSize;
				m_pos = VAdd(m_pos, VScale(VSub(VGet(0, 0, 0), pi), rangeScale));
			}
		}
	}
	else if (m_pos.x > 0 && m_pos.z < 0)
	{
		VECTOR ap = VSub(m_pos, VGet(800, 0, -1150));
		VECTOR ab = VSub(VGet(1150, 0, -800), VGet(800, 0, -1150));
		float temp = (ap.x * ab.x + ap.z * ab.z) / (VSize(ab) * VSize(ab));
		VECTOR ai = VScale(ab, temp);
		VECTOR pi = VSub(ai, ap);
		if (VSize(pi) < charaSize)
		{
			if (pi.x > 0 && pi.z < 0)
			{
				float rangeScale = (VSize(pi) - charaSize) / charaSize;
				m_pos = VAdd(m_pos, VScale(pi, rangeScale));
			}
			else
			{
				float rangeScale = ((-VSize(pi)) - charaSize) / charaSize;
				m_pos = VAdd(m_pos, VScale(VSub(VGet(0, 0, 0), pi), rangeScale));
			}
		}
	}
	else if (m_pos.x > 0 && m_pos.z > 0)
	{
		VECTOR ap = VSub(m_pos, VGet(800, 0, 1150));
		VECTOR ab = VSub(VGet(1150, 0, 800), VGet(800, 0, 1150));
		float temp = (ap.x * ab.x + ap.z * ab.z) / (VSize(ab) * VSize(ab));
		VECTOR ai = VScale(ab, temp);
		VECTOR pi = VSub(ai, ap);
		if (VSize(pi) < charaSize)
		{
			if (pi.x > 0 && pi.z > 0)
			{
				float rangeScale = (VSize(pi) - charaSize) / charaSize;
				m_pos = VAdd(m_pos, VScale(pi, rangeScale));
			}
			else
			{
				float rangeScale = ((-VSize(pi)) - charaSize) / charaSize;
				m_pos = VAdd(m_pos, VScale(VSub(VGet(0, 0, 0), pi), rangeScale));
			}
		}
	}

	if (m_pos.x + charaSize > Game::kFloorSize)
	{
		m_pos.x -= (m_pos.x + charaSize) - Game::kFloorSize;
	}
	else if (m_pos.x - charaSize < -Game::kFloorSize)
	{
		m_pos.x -= (m_pos.x - charaSize) + Game::kFloorSize;
	}

	if (m_pos.z + charaSize > Game::kFloorSize)
	{
		m_pos.z -= (m_pos.z + charaSize) - Game::kFloorSize;
	}
	else if (m_pos.z - charaSize < -Game::kFloorSize)
	{
		m_pos.z -= (m_pos.z - charaSize) + Game::kFloorSize;
	}
}

bool CharacterBase::IsHitAttack(VECTOR pos, float size)
{
	if ((VSize(VSub(m_hitArea, pos)) < (m_hitAreaRange + size)) && m_isDealDamage)
	{
		m_isDealDamage = false;
		return true;
	}
	else
	{
		return false;
	}
}

int CharacterBase::GetDamage()
{
	auto temp = m_damagePoint;
	// m_damagePoint = 0;
	return temp;
}

bool CharacterBase::SetDamage(int damage)
{
	if (m_state != State::DODGE)
	{
		m_HP -= damage;
		if (m_HP < 0)
		{
			m_HP = 0;
		}
		return true;
	}
	else
	{
		return false;
	}
}

void CharacterBase::Rotation()
{
	// モデル回転
	MV1SetRotationXYZ(m_handle, VGet(0, atan2(-m_dir.x, -m_dir.z), 0));
}

void CharacterBase::WeponMove(int attachFrame)
{
	if (m_handle != -1 && m_weponHandle != -1)
	{
		VECTOR AttachSuruFrame1Position;
		MATRIX TransMat;
		MATRIX AttachSareruFrame10Mat;
		MATRIX MixMatrix;
		
		// アタッチするモデルの MV1SetMatrix の設定を無効化する
		MV1SetMatrix(m_weponHandle, MGetIdent());

		// アタッチするモデルのフレームの座標を取得する
		AttachSuruFrame1Position = MV1GetFramePosition(m_weponHandle, 0);

		// アタッチするモデルをフレームの座標を原点にするための平行移動行列を作成
		TransMat = MGetTranslate(VScale(AttachSuruFrame1Position, -1.0f));

		// アタッチされるモデルのフレームの行列を取得
		AttachSareruFrame10Mat = MV1GetFrameLocalWorldMatrix(m_handle, attachFrame);

		// アタッチするモデルをフレームの座標を原点にするための平行移動行列と、
		// アタッチされるモデルのフレームの行列を合成
		MixMatrix = MMult(MGetRotAxis(VGet(0, 1, 0), 1.0f * DX_PI_F),	// 角度調整
					MMult(MGetScale(VGet(0.005f, 0.005f, 0.005f)),		// スケール値調整
					MMult(TransMat, AttachSareruFrame10Mat)));			// 移動量設定
		// 合成した行列をアタッチするモデルにセット
		MV1SetMatrix(m_weponHandle, MixMatrix);
	}
}
