//============================================================================
//
// プレイヤーダメージステート、ヘッダファイル [player_state_damage.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_STATE_DAMAGE_
#define _PLAYER_STATE_DAMAGE_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state.h"

//****************************************************
// プレイヤー	ダメージステートクラス
//****************************************************
class CPlayer_State_Damage : public CPlayer_State
{
public:

	// <special function>
	CPlayer_State_Damage();				// コンストラクタ
	~CPlayer_State_Damage() override;	// デストラクタ
	
	// <funciton>
	void Update() override;		// 更新処理
	void To_Default() override;	// 状態へ
};

#endif	// _PLAYER_STATE_DAMAGE_