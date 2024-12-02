//============================================================================
//
// プレイヤー壁ずりステート、ヘッダファイル [player_state_wall.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_STATE_WALL_
#define _PLAYER_STATE_WALL_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state.h"

//****************************************************
// プレイヤー壁ずりステートクラス
//****************************************************
class CPlayer_State_Wall : public CPlayer_State
{
public:

	// <special function>
	CPlayer_State_Wall();			// コンストラクタ
	~CPlayer_State_Wall() override;	// デストラクタ

	// <function>
	void Update() override;		// 更新処理
	void To_Default() override;	// 通常状態へ
	void To_Jump() override;	// ジャンプ状態へ
	void To_Damage() override;	// ダメージ状態へ

private:

	// <function>
	void AdjustGravity();	// 重力の補正

	// <data>
	int	m_nWallDuration;	// 継続期間
};

#endif	// _PLAYER_STATE_SLASH_