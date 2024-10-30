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

	CPlayer_State_Default();			// デフォルトコンストラクタ
	~CPlayer_State_Default() override;	// デストラクタ

	void Update() override;	// 更新処理

private:

	void Control();						// 操作
	void SetRotTargetToMoveDirection();	// 目標向きを移動方向に設定
	void SetPosTargetByDirection();		// 方角から目標座標を設定
	void To_Dash(bool bDirection);		// ダッシュをする
};

#endif	// _PLAYER_STATE_DEFAULT_