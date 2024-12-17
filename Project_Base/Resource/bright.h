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

	// <static data>
	static WORD m_nCntGenerateSpan;	// 生成スパンカウント
	static JSON m_InitParam;		// 基礎パラメータ
};

#endif	// _BRIGHT_H_