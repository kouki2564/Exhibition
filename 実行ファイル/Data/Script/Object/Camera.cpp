#include "Camera.h"
#include "DxLib.h"
#include "Quaternion.h"
#include "EffekseerForDXLib.h"

namespace
{
	// カメラ移動量の倍率（ 0 ～ 1.0f ）
	constexpr float kMoveScale = 0.3f;
	constexpr float kTempAngle = 0.000025f;
}

Camera::Camera():
	m_distanceXZ(0.0f),
	m_firstPos(VGet(0, 0, 0))
{
	m_pos = m_firstPos;
	m_targetPos = VGet(0, 0, 0);
	//奥行0.1～10000までをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 10000.0f);
}

Camera::~Camera()
{
}

void Camera::Init(VECTOR firstPos)
{
	m_firstPos = firstPos;
	m_pos = m_firstPos;
	m_distanceXZ = sqrtf(m_pos.x * m_pos.x + m_pos.z * m_pos.z);
	m_lastObjPos = VGet(0, 0, 0);
	m_debugMove = 1.0f;
}

void Camera::Update(bool isRotate)
{
	Quaternion moving;
	float angle = 0;
	if (isRotate)
	{
		angle = Rotate();
	}
	// カメラの回転軸設定
	VECTOR axis = VGet(0, 1, 0);

	// カメラの移動値設定
	VECTOR targetMove = VSub(m_targetPos, m_lastObjPos);

	// Quaternionの設定
	moving.SetMove(angle, axis, targetMove);


	// 実移動
	m_pos = moving.Move(m_targetPos, m_pos);

	// カメラが中心にとらえる座標
	VECTOR adjustTargetPos = VGet(m_targetPos.x, m_targetPos.y + 10.0f, m_targetPos.z);


	// カメラ位置が徐々にずれていく対策
	// XZ平面の位置補正
	VECTOR localPos = VSub(m_pos, adjustTargetPos);
	float tempDisXZ = sqrtf(localPos.x * localPos.x + localPos.z * localPos.z);
	if (tempDisXZ != m_distanceXZ)
	{
		localPos.x *= m_distanceXZ / tempDisXZ;
		localPos.z *= m_distanceXZ / tempDisXZ;
		m_pos = VAdd(localPos, adjustTargetPos);
	}
	// 高さは固定の為初期位置に
	m_pos.y = m_firstPos.y;


	// カメラ反映
	DxLib::SetCameraPositionAndTarget_UpVecY(m_pos, adjustTargetPos);

	// 次フレームで扱う直前フレームのカメラターゲット位置
	m_lastObjPos = m_targetPos;

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

float Camera::Rotate()
{
	// カメラの回転値設定
	// ゲームパッドからの回転入力
	StickInput inputState = m_config.GetStick(STICK_R);
	// 回転角の確定
	return kTempAngle * inputState.x;
}
