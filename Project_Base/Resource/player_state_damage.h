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

	CPlayer_State_Damage();				// デフォルトコンストラクタ
	~CPlayer_State_Damage() override;	// デストラクタ

	void Update() override;	// 更新処理

private:

	/// <summary> ダメージ継続期間 </summary>
	static constexpr int MAX_DASH_DURATION = 60;

	int m_nDamageDuration;	// ダメージ継続期間

	void To_Default();	// 通常状態へ
};

#endif	// _PLAYER_STATE_DAMAGE_