//============================================================================
// 
// リザルト、ヘッダファイル [result.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "scene.h"
#include "spline_test.h"

//****************************************************
// リザルトクラス
//****************************************************
class CResult : public CScene
{
public:

	// <function>
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理
	void To_Next() override;	// 次のシーンへ

	// <static function>
	static CResult* Create();	// リザルトを生成

private:

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理

	// <special function>
	CResult();			// コンストラクタ
	~CResult() override;	// デストラクタ

	// <Data>
	CSpline_Test* m_pSpline;	// スプラインテスト
};

#endif // _RESULT_H_