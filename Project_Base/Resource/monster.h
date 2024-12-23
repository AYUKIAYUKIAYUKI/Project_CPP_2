//============================================================================
//
// モンスター、ヘッダファイル [monster.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _MONSTER_H_
#define _MONSTER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "enemy.h"

//****************************************************
// 前方宣言
//****************************************************
class CBounding_Cylinder;

//****************************************************
// モンスタークラス
//****************************************************
class CMonster : public CEnemy
{
public:

	//****************************************************
	// 行動タイプを定義
	//****************************************************
	enum class ACTION : WORD
	{
		HOLD = 0,	// 立ち止まる
		COMING,		// 歩いてくる
		GOBACK,		// 引き下がる
		DEADEND,	// 死亡
		MAX
	};

	// <special function>
	CMonster();				// コンストラクタ
	~CMonster() override;	// デストラクタ

	// <function>
	HRESULT Init() override;					// 初期設定
	void	Uninit() override;					// 終了設定
	void	Update() override;					// 更新処理
	void	Draw() override;					// 描画処理
	void	SetDamage(int nDamage) override;	// ダメージを受ける

	// <static funcion>
	static CMonster* Create();	// 生成

private:

	// <function>
	void FakeGravity();		// 疑似重力
	void BranchAction();	// 行動分岐
	void Hold();			// 立ち止まる
	void Coming();			// 歩いてくる
	bool StopBlockSide();	// ブロックの近くで止まる
	void GoBack();			// 引き下がる
	void DeadEnd();			// 死亡

	// <data>
	ACTION m_ActionType;		// 行動タイプ
	int	   m_nCntActionCast;	// 行動キャストカウント
	float  m_fFakeVelY;			// 疑似加速度

	// <static data>
	static const JSON m_InitParam;	// 基礎パラメータ
};

#endif // _MONSTER_H_