//============================================================================
//
// ライフ、ヘッダファイル [life.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _LIFE_H_
#define _LIFE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "item.h"

//****************************************************
// ライフクラス
//****************************************************
class CLife : public CItem
{
public:

	// <special function>
	CLife();			// コンストラクタ
	~CLife() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <static function>
	static CLife* Create();	// 生成

private:

	// <function>
	void GainEffect() override;	// 取得時の効果
};

#endif // _LIFE_H_