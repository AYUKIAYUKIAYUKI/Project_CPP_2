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
#include "character_state_manager.h"

//****************************************************
// 前方宣言
//****************************************************
class CBounding_Cylinder;

//****************************************************
// プレイヤークラス
//****************************************************
class CPlayer : public CCharacter
{
public:

	/// <summary> プレイヤーの最大体力 </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> プレイヤーのデフォルト移動速度 </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.005f;

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	// <special function>
	CPlayer();	// デフォルトコンストラクタ
	~CPlayer();	// デストラクタ

	// <function>
	HRESULT Init() override;		// 初期設定
	void	Uninit() override;		// 終了設定
	void	Update() override;		// 更新処理
	void	Draw() override;		// 描画処理
	void	To_Damage(int nDamage);	// ステート - ダメージ状態へ

	// <getter>
	float GetRadius() const override;	// 半径を取得
	float GetHeight() const override;	// 高さを取得
	CCharacter_State* GetNowState();	// 現在のステートを取得

	// <static function>
	static CPlayer* Create();	// 生成

private:

	// <function>
	void HitCheck();		// 当たり判定
	void AdjustHeight();	// 高さを補正

	// <data>
	CBounding_Cylinder*			m_pBndCylinder;		// バウンディングシリンダー
	CCharacter_State_Manager*	m_pStateManager;	// ステートマネージャー
};

#endif // _PLAYER_H_