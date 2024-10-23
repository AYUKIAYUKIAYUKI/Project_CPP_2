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

	CPlayer_State_Dash();			// デフォルトコンストラクタ
	~CPlayer_State_Dash() override;	// デストラクタ

	void Update() override;	// 更新処理

private:

	/// <summary> ダッシュ継続期間 </summary>
	static constexpr int MAX_DASH_DURATION = 45;

	int m_nDashDuration;	// ダッシュ継続期間

	void To_Default();	// 元に戻る
};

#endif	// _PLAYER_STATE_DASH_