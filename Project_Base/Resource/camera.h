//============================================================================
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
	CCamera();	// コンストラクタ
	~CCamera();	// デストラクタ

	// <funciton>
	HRESULT	Init();						// 初期設定
	void	Update();					// 更新処理
	void	SetVibration(float fCoef);	// カメラ振動
	void	SetCamera();				// カメラをセット
	void	SetAppearBoss();			// ボス登場用のカメラセット

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// 向きを取得
	void SetRot(D3DXVECTOR3 Rot);		// 向きを設定

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// 目標向きを取得
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// 目標向きを設定

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// 座標を取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標を設定

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// 目標座標を取得
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// 目標座標を設定

	// <getter/setter>
	const float& GetDistance() const;	// 間距離を取得
	void SetDistance(float fDistance);	// 間距離を設定

	// <getter/setter>
	const float& GetUpAdjust() const;	// 俯瞰度合いを取得
	void SetUpAdjust(float fAdjust);	// 俯瞰度合いを設定

	// <setter>
	void ChangeTrackPlayer(bool bTrack);	// プレイヤー追従切り替え

private:

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	// <function>
	void BranchViewMode();		// ビューモード分岐
	void Control();				// 操作
	void CorrectToTarget();		// 目標値への補間
	void AutoSetRot();			// 自動で目標向きへ補間
	void AutoRestrictPitch();	// ピッチ角の範囲制限
	void AutoSetPos();			// 自動で目標座標へ補間
	void CalcPosV();			// 視点座標を計算
	void CalcPosR();			// 注視点座標を計算
	void CalcMtxProjection();	// プロジェクション行列計算
	void CalcMtxView();			// ビュー行列計算
	void PrintDebug();			// デバッグ表示

	// <data>
	D3DXVECTOR3	m_Rot;				// 向き
	D3DXVECTOR3	m_RotTarget;		// 目標向き
	D3DXVECTOR3	m_Pos;				// 座標
	D3DXVECTOR3	m_PosTarget;		// 目標座標
	D3DXVECTOR3	m_PosV;				// 視点座標
	D3DXVECTOR3	m_PosTargetV;		// 目標視点座標
	D3DXVECTOR3	m_PosR;				// 注視点座標
	D3DXVECTOR3	m_PosTargetR;		// 目標注視点座標
	float		m_fDistance;		// 視点 -> 注視点間の距離
	D3DXVECTOR3	m_VecU;				// 上方向ベクトル
	D3DXMATRIX	m_MtxProjection;	// プロジェクション行列
	D3DXMATRIX	m_MtxView;			// ビュー行列
	float		m_fAdjust;			// 俯瞰度合い
	bool		m_bTrackPlayer;		// プレイヤーを追従
	bool		m_bVertical;		// 垂直ビュー
	bool		m_bHorizon;			// 水平ビュー
};

#endif // _CAMERA_H_