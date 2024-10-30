//============================================================================
//
// プレイヤーダッシュステート、ヘッダファイル [player_state_dash.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_STATE_DASH_
#define _PLAYER_STATE_DASH_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state.h"

//****************************************************
// プレイヤーダッシュステートクラス
//****************************************************
class CPlayer_State_Dash : public CPlayer_State
{
public:

	CPlayer_State_Dash();					// デフォルトコンストラクタ
	CPlayer_State_Dash(bool bDirection);	// 方向指定コンストラクタ
	~CPlayer_State_Dash() override;			// デストラクタ

	void Update() override;	// 更新処理

private:

	/// <summary> ダッシュ継続期間 </summary>
	static constexpr int MAX_DASH_DURATION = 45;

	bool	m_bDirection;		// 方向
	int		m_nDashDuration;	// ダッシュ継続期間

	void SetRotTargetToMoveDirection();	// 目標向きを移動方向に設定
	void SetPosTarget_Unnamed();		// 目標座標をダッシュ方向に増加
	void To_Default();					// 元に戻る
};

#endif	// _PLAYER_STATE_DASH_