//============================================================================
//
// ストーン、ヘッダファイル [stone.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _STONE_H_
#define _STONE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// 前方宣言
//****************************************************
class CBounding_Sphere;

//****************************************************
// ストーンクラス
//****************************************************
class CStone : public CObject_X
{
public:

	// <special function>
	CStone();			// コンストラクタ
	~CStone() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <getter/setter>
	float GetRadius() const override;	// 半径を取得
	void SetRadius(float fRadius);		// 半径を設定

	static CStone* Create(D3DXVECTOR3 InitPos, D3DXVECTOR3 Accel);	// 生成

private:

	// 滞空時間
	static const int WAIT_DURATION = 35;

	// <function>
	void HitCheckEnemy();	// 敵との接触を検出

	// <data>
	CBounding_Sphere* m_pBndSphere;	// 球バウンディング
	D3DXVECTOR3		  m_Accel;		// 推進力
	int				  m_nDuration;	// 期間
};

#endif // _STONE_H_