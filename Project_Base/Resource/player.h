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

	CPlayer();	// デフォルトコンストラクタ
	~CPlayer();	// デストラクタ

	HRESULT Init() override;		// 初期設定
	void	Uninit() override;		// 終了設定
	void	Update() override;		// 更新処理
	void	Draw() override;		// 描画処理
	void	To_Damage(int nDamage);	// ダメージ状態へ

	static CPlayer* Create();	// 生成

private:

	CCharacter_State_Manager* m_pStateManager;	// ステートマネージャー
};

#endif // _PLAYER_H_