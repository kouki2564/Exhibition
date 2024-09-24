#pragma once
#include "ObjectBase.h"
#include "Config.h"

class Camera :
    public ObjectBase
{
public:
	Camera();
	~Camera();

	void Init(VECTOR firstPos);
	void Update(bool isRotate);
	void Draw() const{}

	void SetCameraPos(VECTOR cameraPos) { m_pos = cameraPos; }
	void SetTargetPos(VECTOR targetPos) { m_targetPos = targetPos; }
	float Rotate();

private:
	// カメラ初期位置
	VECTOR m_firstPos;
	// カメラのターゲット位置
	VECTOR m_targetPos;
	// 直前フレームのターゲット位置
	VECTOR m_lastObjPos;
	// カメラとターゲットのXZ平面上の距離
	float m_distanceXZ;

	float m_debugMove;

	Config m_config;
};

