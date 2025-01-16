//============================================================================
//
// ブーツ、ヘッダファイル [boots.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _BOOTS_H_
#define _BOOTS_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "item.h"

//****************************************************
// ブーツクラス
//****************************************************
class CBoots : public CItem
{
public:

	// <special function>
	CBoots();			// コンストラクタ
	~CBoots() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <static function>
	static CBoots* Create();	// 生成

private:

	// <function>
	void GainEffect() override;	// 取得時の効果
};

#endif // _BOOTS_H_