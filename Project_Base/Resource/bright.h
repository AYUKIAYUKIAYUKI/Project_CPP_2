//============================================================================
// 
// 閃光、ヘッダファイル [bright.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _BRIGHT_H_
#define _BRIGHT_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "motion_set.h"

//****************************************************
// 閃光クラス
//****************************************************
class CBright : public CMotion_Set
{
public:

	// <special fuction>
	CBright();	// コンストラクタ
	~CBright();	// デストラクタ

	// <function fuction>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <static function>
	static void Generate(D3DXVECTOR3 Pos);	// 発生

private:

	// <static function>
	static void Create(D3DXVECTOR3 Pos);	// 生成

	// <function>
	bool Disappear();	// 消滅

	// <data>
	int m_nCntDuration;	// 継続期間
	int m_nMaxDuration;	// 最大継続期間

	// <static data>
	static const JSON m_MotionData;	// モーションデータ
	static const JSON m_InitParam;	// 基礎パラメータ
};

#endif	// _BRIGHT_H_