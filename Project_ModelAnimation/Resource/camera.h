﻿//============================================================================
//
// カメラ、ヘッダファイル [camera.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_	// 二重インクルード防止

//****************************************************
// カメラクラス
//****************************************************
class CCamera
{
public:

	// <special function>
	CCamera();	// デフォルトコンストラクタ
	~CCamera();	// デストラクタ

	// <funciton>
	HRESULT	Init();			// 初期設定
	void	Update();		// 更新処理
	void	SetCamera();	// カメラをセット

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// 座標を取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標を設定

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// 目標座標を取得
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// 目標座標を設定

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// 向きを取得
	void SetRot(D3DXVECTOR3 Rot);		// 向きを設定

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// 目標向きを取得
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// 目標向きを設定

	// <getter/setter>
	const float& GetDistance() const;	// 間距離を取得
	void SetDistance(float fDistance);	// 間距離を設定

private:

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float COEF_ADJUST = 0.25f;

	// <function>
	void BranchMode();			// ビューモード分岐
	void Control();				// 操作
	void Rotation();			// 回転
	void RestrictPitch();		// ピッチ角の範囲制限
	void Translation();			// 移動
	void CalcPosV();			// 視点座標を計算
	void CalcPosR();			// 注視点座標を計算
	void CalcMtxProjection();	// プロジェクション行列計算
	void CalcMtxView();			// ビュー行列計算

	// <data>
	D3DXVECTOR3	m_Pos;				// 座標
	D3DXVECTOR3	m_PosTarget;		// 目標座標
	D3DXVECTOR3	m_PosV;				// 視点座標
	D3DXVECTOR3	m_PosTargetV;		// 目標視点座標
	D3DXVECTOR3	m_PosR;				// 注視点座標
	D3DXVECTOR3	m_PosTargetR;		// 目標注視点座標
	D3DXVECTOR3	m_Rot;				// 向き
	D3DXVECTOR3	m_RotTarget;		// 目標向き
	float		m_fDistance;		// 視点 -> 注視点間の距離
	D3DXVECTOR3	m_VecU;				// 上方向ベクトル
	D3DXMATRIX	m_MtxProjection;	// プロジェクション行列
	D3DXMATRIX	m_MtxView;			// ビュー行列
	float		m_fAdjust;			// 俯瞰度合い
};

#endif // _CAMERA_H_