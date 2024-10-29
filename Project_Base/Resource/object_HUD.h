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

	static CObject_HUD* Create();	// 生成

private:

	D3DXVECTOR3	m_PosTarget;	// 目標座標
	D3DXVECTOR3	m_RotTarget;	// 目標向き
	D3DXVECTOR3 m_SizeTarget;	// 目標サイズ
	D3DXCOLOR	m_ColTarget;	// 目標色
};

#endif // _OBJECT_HUD_H_