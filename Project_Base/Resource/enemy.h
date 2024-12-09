//============================================================================
//
// エネミー、ヘッダファイル [enemy.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "character.h"

//****************************************************
// 前方宣言
//****************************************************
class CBounding_Cylinder;

//****************************************************
// エネミークラス
//****************************************************
class CEnemy : public CCharacter
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

	/// <summary> エネミー最大体力 </summary>
	static constexpr int MAX_LIFE = 1;

	/// <summary> エネミーのデフォルト移動速度 </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.002f;

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float CORRECT_COEF = 0.1f;

	// <special function>
	CEnemy();	// コンストラクタ
	~CEnemy();	// デストラクタ

	// <function>
	HRESULT Init() override;					// 初期設定
	void	Uninit() override;					// 終了設定
	void	Update() override;					// 更新処理
	void	Draw() override;					// 描画処理
	void	SetDamage(int nDamage) override;	// ダメージを受ける

	// <getter>
	float GetRadius() const;								// 半径を取得
	float GetHeight() const;								// 高さを取得
	const CBounding_Cylinder* const GetBndCylinder() const;	// バウンディングシリンダーを取得

	// <static funcion>
	static CEnemy* Create();	// 生成

private:

	// <function>
	void SetNextAction();			// 次の行動を決定
	void BranchAction();			// 行動分岐
	void Hold();					// 立ち止まる
	void Coming();					// 歩いてくる
	bool StopBlockSide();			// ブロックの近くで止まる
	void GoBack();					// 引き下がる
	void DeadEnd();					// 死亡
	void AdjustHeight() override;	// 高さを補正
	bool HitCheck();				// 衝突検出

	// <data>
	ACTION				m_ActionType;		// 行動タイプ
	int					m_nCntActionCast;	// 行動キャストカウント
	CBounding_Cylinder* m_pBndCylinder;		// 円柱バウンディング
};

#endif // _PLAYER_H_