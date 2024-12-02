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

	/// <summary> ダッシュキャスト期間 </summary>
	static constexpr int MAX_DASH_CAST = 30;

	// <special function>
	CPlayer_State_Dash();			// コンストラクタ
	~CPlayer_State_Dash() override;	// デストラクタ

	// <function>
	void Update() override;		// 更新処理
	void To_Default() override;	// 通常状態へ
	void To_Damage() override;	// ダメージ状態へ

private:

	/// <summary> ダッシュ継続期間 </summary>
	static constexpr int MAX_DASH_DURATION = 10;

	// <function>
	void SetPosTarget_Unnamed();	// 目標座標をダッシュ方向に増加

	// <data>
	bool	m_bDirection;		// 方向
	int		m_nDashDuration;	// ダッシュ継続期間
};

#endif	// _PLAYER_STATE_DASH_