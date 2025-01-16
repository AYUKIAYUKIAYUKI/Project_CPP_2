//============================================================================
//
// 羽、ヘッダファイル [wings.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _WINGS_H_
#define _WINGS_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "item.h"

//****************************************************
// 羽クラス
//****************************************************
class CWings : public CItem
{
public:

	// <special function>
	CWings();			// コンストラクタ
	~CWings() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <static function>
	static CWings* Create();	// 生成

private:

	// <function>
	void GainEffect() override;	// 取得時の効果
};

#endif // _WINGS_H_