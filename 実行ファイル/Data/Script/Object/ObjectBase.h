#pragma once
#include "Game.h"
#include "DxLib.h"
#include <memory>

class ObjectBase :
	public std::enable_shared_from_this<ObjectBase>
{
public:
	ObjectBase()
	{
		m_pos = VGet(0.0f, 0.0f, 0.0f);
		m_rot = VGet(0.0f, 0.0f, 0.0f);
		m_scale = VGet(1.0f, 1.0f, 1.0f);
		m_handle = 0;
	}
	virtual ~ObjectBase() {}

	// オブジェクト描画
	virtual void Draw() const = 0;

	// 座標取得
	const VECTOR GetPos() const { return m_pos; }
	// ハンドル取得
	const int GetHandle() const { return m_handle; }

protected:
	// 座標
	VECTOR m_pos;
	// モデルの回転角
	VECTOR m_rot;
	// モデルの大きさの倍率
	VECTOR m_scale;
	// デュプリケイトハンドル
	int m_handle;

};

