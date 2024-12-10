//============================================================================
//
// ゴースト、ヘッダファイル [ghost.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _GHOST_H_
#define _GHOST_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "enemy.h"

//****************************************************
// 前方宣言
//****************************************************
class CBounding_Cylinder;

//****************************************************
// ゴーストクラス
//****************************************************
class CGhost : public CEnemy
{
public:

	//****************************************************
	// 行動タイプを定義
	//****************************************************
	enum class ACTION : WORD
	{
		HOLD = 0,	// 漂う
		FLY,		// 飛んでくる
		DEADEND,	// 死亡
		MAX
	};

	// <special function>
	CGhost();			// コンストラクタ
	~CGhost() override;	// デストラクタ

	// <function>
	HRESULT Init() override;					// 初期設定
	void	Uninit() override;					// 終了設定
	void	Update() override;					// 更新処理
	void	Draw() override;					// 描画処理
	void	SetDamage(int nDamage) override;	// ダメージを受ける

	// <static funcion>
	static CGhost* Create();	// 生成

private:

	// <function>
	void BranchAction();	// 行動分岐
	void Hold();			// 漂う
	void Fly();				// 飛んでくる
	void DeadEnd();			// 死亡

	// <data>
	ACTION m_ActionType;		// 行動タイプ

	// <static data>
	static const JSON m_InitParam;	// 基礎パラメータ
};

#endif // _ENEMY_H_