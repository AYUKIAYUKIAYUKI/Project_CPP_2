//============================================================================
//
// パワーストーン、ヘッダファイル [powerstone.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _POWERSTONE_H_
#define _POWERSTONE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "item.h"

//****************************************************
// パワーストーンクラス
//****************************************************
class CPowerStone : public CItem
{
public:

	// <special function>
	CPowerStone();			// コンストラクタ
	~CPowerStone() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <static function>
	static CPowerStone* Create();	// 生成

private:

	// <function>
	void GainEffect() override;	// 取得時の効果
};

#endif // _POWERSTONE_H_