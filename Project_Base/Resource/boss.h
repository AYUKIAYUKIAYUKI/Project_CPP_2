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

	/// <summary> ボスの最大体力 </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float CORRECT_COEF = 0.5f;

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

	// <data>
	CBounding_Cylinder* m_pBndCylinder;		// 円柱リンダー
};

#endif // _PLAYER_H_