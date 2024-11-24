//============================================================================
//
// プレイヤー通常ステート、ヘッダファイル [player_state_default.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_STATE_DEFAULT_
#define _PLAYER_STATE_DEFAULT_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state.h"

//****************************************************
// プレイヤー通常ステートクラス
//****************************************************
class CPlayer_State_Default : public CPlayer_State
{
public:

	// <special function>
	CPlayer_State_Default();			// コンストラクタ
	~CPlayer_State_Default() override;	// デストラクタ

	// <function>
	void Update() override;		// 更新処理
	void To_Dash() override;	// ダッシュ状態へ
	void To_Jump() override;	// ジャンプ状態へ
	void To_Slash() override;	// 斬撃状態へ
	void To_Damage() override;	// ダメージ状態へ

private:

	// <function>
	void Control();	// 操作
};

#endif	// _PLAYER_STATE_DEFAULT_