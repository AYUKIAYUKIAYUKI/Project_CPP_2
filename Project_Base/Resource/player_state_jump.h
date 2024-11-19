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

	/// <summary> ジャンプ一回当たりの増加加速度 </summary>
	static constexpr float AMOUNT_JUMPACCEL = 5.0f;

	// <special function>
	CPlayer_State_Jump();			// コンストラクタ
	~CPlayer_State_Jump() override;	// デストラクタ

	// <function>
	void Update() override;		// 更新処理
	void To_Default() override;	// 通常状態へ
	void To_Dash() override;	// ダッシュ状態へ

private:

	/// <summary> 最大延長猶予 </summary>
	static constexpr int MAX_JUMPREMAIN_DURATION = 8;

	// <function>
	void Control();			// 操作
	void AdjustGravity();	// 重力の補正

	// <data>
	int		m_nJumpRemainDuration;	// 入力延長期間
	bool	m_bEndRemain;			// 延長判定
};

#endif	// _PLAYER_STATE_JUMP_