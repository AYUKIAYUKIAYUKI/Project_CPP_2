//============================================================================
//
// ボス、ヘッダファイル [boss.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _BOSS_H_
#define _BOSS_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "character.h"

//****************************************************
// 前方宣言
//****************************************************
class CBounding_Cylinder;
class CPlayer_State;

//****************************************************
// ボスクラス
//****************************************************
class CBoss : public CCharacter
{
public:

	//****************************************************
	// 行動タイプを定義
	//****************************************************
	enum class ACTION : WORD
	{
		HOLDCENTER = 0,	// 中央で待機
		DIRECTATTACK,	// 体当たり
		WALLATTACK,		// 壁作り
		SUMMONENEMY,	// 敵召喚
		DAMAGEBACK,		// ダメージ喰らい
		DEADEND,		// 死亡
		MAX
	};
	
	/// <summary> ボスの最大体力 </summary>
	static constexpr int MAX_LIFE = 7;

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float CORRECT_COEF = 0.1f;

	// <special function>
	CBoss();	// コンストラクタ
	~CBoss();	// デストラクタ

	// <function>
	HRESULT Init() override;		// 初期設定
	void	Uninit() override;		// 終了設定
	void	Update() override;		// 更新処理
	void	Draw() override;		// 描画処理
	void	SetDamage(int nDamage);	// ダメージを受ける

	// <getter>
	float GetRadius() const;								// 半径を取得
	float GetHeight() const;								// 高さを取得
	const CBounding_Cylinder* const GetBndCylinder() const;	// 円柱シリンダーを取得

	// <static function>
	static CBoss* Create();	// 生成

private:

	// <function>
	void SetNextAction();	// 次の行動を決定
	void BranchAction();	// 行動分岐
	void HoldCenter();		// 中心で待機
	void DirectAttack();	// 体当たり
	void WallAttack();		// 壁作り
	void SummonEnemy();		// 敵召喚
	void DamageBack();		// ダメージ喰らい
	void DeadEnd();			// 死亡
	bool HitCheck();		// 衝突検出

	// <data>
	ACTION				m_ActionType;		// 行動タイプ
	int					m_nCntActionCast;	// 行動キャストカウント
	int					m_nDuration;		// 汎用期間
	CBounding_Cylinder* m_pBndCylinder;		// 円柱バウンディング
};

#endif // _PLAYER_H_