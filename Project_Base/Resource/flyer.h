//============================================================================
//
// フライヤー、ヘッダファイル [flyer.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _FLYER_H_
#define _FLYER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "enemy.h"

//****************************************************
// 前方宣言
//****************************************************
class CBounding_Cylinder;

//****************************************************
// フライヤークラス
//****************************************************
class CFlyer : public CEnemy
{
public:

	//****************************************************
	// 行動タイプを定義
	//****************************************************
	enum class ACTION : WORD
	{
		UPDOWN = 0,	// 昇降
		SLASH,		// 斬撃
		DEADEND,	// 死亡
		MAX
	};

	// <special function>
	CFlyer();			// コンストラクタ
	~CFlyer() override;	// デストラクタ

	// <function>
	HRESULT Init() override;					// 初期設定
	void	Uninit() override;					// 終了設定
	void	Update() override;					// 更新処理
	void	Draw() override;					// 描画処理
	void	SetDamage(int nDamage) override;	// ダメージを受ける

	// <static funcion>
	static CFlyer* Create();	// 生成

private:

	// <function>
	void BranchAction();	// 行動分岐
	void UpDown();			// 昇降
	void DeadEnd();			// 死亡

	// <data>
	ACTION m_ActionType;		// 行動タイプ
	bool   m_bAdder;			// 増加判定
	float  m_fDiffFromInitY;	// 高さの変化量
	float  m_fMinRangeDiff;		// 変化量の最低量
	float  m_fMaxRangeDiff;		// 変化量の最大量

	// <static data>
	static JSON m_InitParam;	// 基礎パラメータ
};

#endif // _FLYER_H_