#include "AnimationManager.h"
#include <cassert>

AnimationManager::AnimationManager():
	m_modelHandle(-1),
	m_animaFrame(0),
	m_animaRate(0),
	m_animaSpeed(10),
	m_nowIndex(-1),
	m_nextIndex(-1)
{
}

AnimationManager::~AnimationManager()
{
	MV1DetachAnim(m_modelHandle, m_nowIndex);
	MV1DetachAnim(m_modelHandle, m_nextIndex);
	MV1DeleteModel(m_modelHandle);
}

void AnimationManager::InitAnimation(int modelHandle, int animaNum, float speed, float frame)
{
	m_modelHandle = modelHandle;
	m_nextIndex = MV1AttachAnim(m_modelHandle, animaNum);
	if (m_nowIndex == -1)
	{
		m_nowIndex = m_nextIndex;
	}
	m_animaSpeed = speed;
	m_animaFrame = frame;
}

void AnimationManager::SetAnimation(int animaNum, float speed)
{
	if (MV1GetAttachAnim(m_modelHandle, m_nextIndex) != animaNum)
	{
		if(m_nowIndex == m_nextIndex)
			m_nextIndex = MV1AttachAnim(m_modelHandle, animaNum);
	}
	m_animaSpeed = speed;
}

void AnimationManager::Play()
{
	assert(m_modelHandle != -1);
	// アニメーション変更時
	if (m_nowIndex != m_nextIndex)
	{
		AnimaRate();
	}
	// アニメーション更新
	else
	{
		Animation();
	}
}

void AnimationManager::Animation()
{
	// m_nextIndex = MV1AttachAnim(modelHandle, AnimaFlag, -1);;
	// アニメーション継続時
	if (m_nowIndex == m_nextIndex)
	{
		// 現在使用中のアニメーションの終了フレーム
		float TotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_nowIndex);

		// アニメフレーム進行
		m_animaFrame += m_animaSpeed * 0.1f;

		// アニメフレーム更新
		MV1SetAttachAnimTime(m_modelHandle, m_nowIndex, m_animaFrame);

		// アニメーションが1周したとき
		if (m_animaFrame > TotalTime)
		{
			m_animaFrame = 0.0f;
		}
	}
	// アニメーション遷移時
	else
	{
	}
}

void AnimationManager::AnimaRate()
{
	if (m_animaRate < 1.0f)
	{
		// アニメーション遷移
		m_animaRate += 0.2f;
		MV1SetAttachAnimBlendRate(m_modelHandle, m_nowIndex, 1.0f - m_animaRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_nextIndex, m_animaRate);
		MV1DrawModel(m_modelHandle);
	}
	else
	{
		// 直前のアニメーションインデックスを消去
		MV1DetachAnim(m_modelHandle, m_nowIndex);
		// 次のインデックスを付与
		m_nowIndex = m_nextIndex;
		m_animaRate = 0;
		m_animaFrame = 0;
	}
	
}
