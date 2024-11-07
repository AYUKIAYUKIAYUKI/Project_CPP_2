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
	CPlayer_State_Jump();			// デフォルトコンストラクタ
	~CPlayer_State_Jump() override;	// デストラクタ

	// <function>
	void Update() override;	// 更新処理

private:

	// <function>
	void Control();		// 操作
	void To_Default();	// ステート - 通常状態へ
};

#endif	// _PLAYER_STATE_JUMP_