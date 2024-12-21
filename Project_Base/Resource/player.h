//============================================================================
//
// プレイヤー、ヘッダファイル [player.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// 二重インクルード防止

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
// プレイヤークラス
//****************************************************
class CPlayer : public CCharacter
{
public:

	/// <summary> プレイヤーの最大体力 </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> プレイヤーのデフォルト移動速度 </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.0035f;

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float CORRECT_COEF = 0.5f;

	// <special function>
	CPlayer();	// コンストラクタ
	~CPlayer();	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理
	void	SetDamage(int nDamage, float fImpact);	// ダメージを受ける

	// <getter>
	float GetRadius() const;								// 半径を取得
	float GetHeight() const;								// 高さを取得
	const CBounding_Cylinder* const GetBndCylinder() const;	// 円柱バウンディングを取得
	const CPlayer_State* const GetNowState() const;			// 現在のステートを取得

	// <static function>
	static CPlayer* Create();	// 生成

private:

	// <function>
	void AdjustHeight() override;	// 高さを補正
	void HitCheck();				// 当たり判定

	// <data>
	CBounding_Cylinder*	m_pBndCylinder;	// 円柱バウンディング
	CPlayer_State*		m_pState;		// ステート
};

#endif // _PLAYER_H_