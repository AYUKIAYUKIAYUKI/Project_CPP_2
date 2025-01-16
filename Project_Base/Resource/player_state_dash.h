//============================================================================
//
// プレイヤーダッシュステート、ヘッダファイル [player_state_dash.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_STATE_DASH_
#define _PLAYER_STATE_DASH_	// 二重インクルード防止

//****************************************************
// 前方宣言
//****************************************************
class CObject_X;

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
	static constexpr int MAX_DASH_CAST = 60;

	// <special function>
	CPlayer_State_Dash();			// コンストラクタ
	~CPlayer_State_Dash() override;	// デストラクタ

	// <function>
	void Update() override;		// 更新処理
	void To_Default() override;	// 通常状態へ
	void To_Damage() override;	// ダメージ状態へ

private:

	// <function>
	void SetPosToFacing();	// 目標座標をダッシュ方向に増加

	// <data>
	bool					m_bDirection;	// 方向
	std::vector<CObject_X*> m_vEffect;		// エフェクト
	int						m_nDuration;	// 継続期間
};

#endif	// _PLAYER_STATE_DASH_