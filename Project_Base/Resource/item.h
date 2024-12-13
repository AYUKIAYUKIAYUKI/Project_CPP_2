//============================================================================
//
// アイテム、ヘッダファイル [item.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _ITEM_H_
#define _ITEM_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// 前方宣言
//****************************************************
class CBounding_Sphere;

//****************************************************
// アイテムクラス
//****************************************************
class CItem : public CObject_X
{
public:

	// <special function>
	CItem();			// コンストラクタ
	~CItem() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <getter/setter>
	float GetRadius() const override;	// 半径を取得
	void SetRadius(float fRadius);		// 半径を設定

	// <setter>
	void SetCorrectCoef(float fCoef);	// 補正強度を設定

	// <getter/setter>
	const float& GetDirection() const;		// 方角を取得
	void SetDirection(float fDirection);	// 方角を設定

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// 目標向きを取得
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// 目標向きを設定

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// 目標座標を取得
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// 目標座標を設定

	// <setter>
	void SetPosY(float fPosY);	// Y座標を設定

private:

	// <function>
	void HitChecklPlayer();	// プレイヤーとの接触を検出
	void AppealRotate();	// くるくる回る
	void CorrectToTarget();	// 目標値への補間

	// <virtual function>
	virtual void GainEffect() = 0;	// 取得時の効果

	// <data>
	CBounding_Sphere* m_pBndSphere;		// 球バウンディング
	float			  m_fCorrectCoef;	// 補間強度
	float			  m_fDirection;		// 方角
	D3DXVECTOR3		  m_RotTarget;		// 目標向き
	D3DXVECTOR3		  m_PosTarget;		// 目標座標
	CObject_X*		  m_pSummoning;		// 魔法陣
};

#endif // _ITEM_H_