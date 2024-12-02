//============================================================================
// 
// ゲーム、ヘッダファイル [game.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _GAME_H_
#define _GAME_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "scene.h"

//****************************************************
// 前方宣言
//****************************************************
class CObject_HUD;

//****************************************************
// ゲームクラス
//****************************************************
class CGame : public CScene
{
public:

	// <function>
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理
	void SetTransition();		// 遷移開始
	void To_Next() override;	// 次のシーンへ

	// <static function>
	static CGame* Create();	// ゲームを生成

private:

	// <special function>
	CGame();			// コンストラクタ
	~CGame() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理

	// <data>
	bool		 m_bTransition;	// 遷移フラグ
	CObject_HUD* m_pRenderFade;	// フェード表示
};

#endif // _GAME_H_