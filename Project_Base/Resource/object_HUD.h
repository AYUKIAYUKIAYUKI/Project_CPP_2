//============================================================================
// 
// HUDオブジェクト、ヘッダファイル [object_HUD.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_HUD_H_
#define _OBJECT_HUD_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_2D.h"

//****************************************************
// HUDオブジェクトクラス
//****************************************************
class CObject_HUD : public CObject_2D
{
public:

	CObject_HUD(int nPriority = static_cast<int>(LAYER::UI));	// 描画優先度指定コンストラクタ
	~CObject_HUD() override;									// デストラクタ

	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	const D3DXVECTOR3& GetPosTarget() const;	// 目標座標取得
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// 目標座標設定

	const D3DXVECTOR3& GetRotTarget() const;	// 目標向き取得
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// 目標向き設定

	const D3DXVECTOR3& GetSizeTarget() const;	// 目標サイズ取得
	void SetSizeTarget(D3DXVECTOR3 SizeTarget);	// 目標サイズ設定

	const D3DXCOLOR& GetColTarget() const;	// 目標色取得
	void SetColTarget(D3DXCOLOR ColTarget);	// 目標色設定

	static CObject_HUD* Create();						// 生成
	static CObject_HUD* Create(std::string FilePath);	// 生成

private:

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	void AdjustToTarget();	// 目標値への補正

	D3DXVECTOR3	m_PosTarget;	// 目標座標
	D3DXVECTOR3	m_RotTarget;	// 目標向き
	D3DXVECTOR3 m_SizeTarget;	// 目標サイズ
	D3DXCOLOR	m_ColTarget;	// 目標色
};

#endif // _OBJECT_HUD_H_