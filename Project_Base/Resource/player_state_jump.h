//============================================================================
//
// プレイヤージャンプステート、ヘッダファイル [player_state_jump.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_STATE_JUMP_
#define _PLAYER_STATE_JUMP_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state.h"

//****************************************************
// プレイヤージャンプステートクラス
//****************************************************
class CPlayer_State_Jump : public CPlayer_State
{
public:

	// <special function>
	CPlayer_State_Jump();			// コンストラクタ
	~CPlayer_State_Jump() override;	// デストラクタ

	// <function>
	void Update() override;		// 更新処理
	void To_Default() override;	// 通常状態へ
	void To_Dash() override;	// ダッシュ状態へ
	void To_Wall() override;	// 壁ずり状態へ
	void To_Slash() override;	// 斬撃状態へ
	void To_Damage() override;	// ダメージ状態へ

	// <static data>
	static float AMOUNT_JUMPACCEL;	// ジャンプ一回当たりの増加加速度

private:

	// <function>
	void Control();			// 操作
	void AdjustGravity();	// 重力の補正

	// <data>
	int		m_nJumpRemainDuration;	// 入力延長期間
	bool	m_bEndRemain;			// 延長判定

	// <static data>
	static JSON m_JumpParam;	// ジャンプ用パラメータ
};

#endif	// _PLAYER_STATE_JUMP_