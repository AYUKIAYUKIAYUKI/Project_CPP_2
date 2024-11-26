//============================================================================
// 
// タイトル、ヘッダファイル [title.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "scene.h"

//****************************************************
// 前方宣言
//****************************************************
class CMotion_Set;
class CObject_X;

//****************************************************
// タイトルクラス
//****************************************************
class CTitle : public CScene
{
public:

	// <function>
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理
	void To_Next() override;	// 次のシーンへ

	// <static function>
	static CTitle* Create();	// タイトルを生成

private:

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理

	// <special function>
	CTitle();			// コンストラクタ
	~CTitle() override;	// デストラクタ

	// <data>
	CMotion_Set*			 m_pButterfly;	// 蝶
	std::vector<D3DXVECTOR3> m_Path;		// パス
	int						 m_nNowFrame;	// フレーム
	int						 m_nMaxFrame;	// 最大フレーム
	CMotion_Set*			 m_pTree;		// 樹
	CObject_X*				 m_pHole;		// 穴
};

#endif // _TITLE_H_