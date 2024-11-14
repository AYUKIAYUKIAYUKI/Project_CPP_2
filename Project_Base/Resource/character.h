//============================================================================
//
// キャラクター、ヘッダファイル [character.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _CHARACTER_H_
#define _CHARACTER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// キャラクタークラス
//****************************************************
class CCharacter : public CObject_X
{
public:

	// <special function>
	CCharacter();					// デフォルトコンストラクタ
	virtual ~CCharacter() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <getter/setter>
	const float& GetDirection() const;		// 方角を取得
	void SetDirection(float fDirection);	// 方角を設定

	// <getter/setter>
	const float& GetMoveSpeed() const;		// 移動速度を取得
	void SetMoveSpeed(float fMoveSpeed);	// 移動速度を設定

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// 目標向きを取得
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// 目標向きを設定

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// 目標座標を取得
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// 目標座標を設定

	// <getter/setter>
	const float& GetAccelY() const;	// Y軸の加速度を取得
	void SetAccelY(float fAccelY);	// Y軸の加速度を設定

	// <getter/setter>
	const int& GetLife() const;	// 体力を取得
	void SetLife(int nLife);	// 体力を設定

protected:

	// <function>
	void CorrectToTarget();		// 目標値への補正
	void AdjustLife();			// 体力の調整

	// <virtual function>
	virtual void AutoSetRotTarget();	// 目標向きを移動方向から自動で設定
	virtual void AutoSetPosTarget();	// 目標座標を方角から自動で設定

private:

	// <data>
	float		m_fCorrectionCoef;	// 補正係数
	float		m_fDirection;		// 方角
	float		m_fMoveSpeed;		// 移動速度
	D3DXVECTOR3	m_RotTarget;		// 目標向き
	D3DXVECTOR3	m_PosTarget;		// 目標座標
	float		m_fAccelY;			// Y軸の加速度
	int			m_nLife;			// 体力
};

#endif // _PLAYER_H_