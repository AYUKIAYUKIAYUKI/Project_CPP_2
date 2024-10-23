//============================================================================
//
// プレイヤーステート、ヘッダファイル [player_state.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_STATE_
#define _PLAYER_STATE_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "state.h"
#include "player.h"

//****************************************************
// プレイヤーステートクラス
//****************************************************
class CPlayer_State : public CState
{
public:

	CPlayer_State();							// デフォルトコンストラクタ
	CPlayer_State(player::CPlayer* pPlayer);	// プレイヤー取得コンストラクタ
	~CPlayer_State() override;					// デストラクタ

	virtual void Update() override = 0;	// 更新処理

	CPlayer_State* GetNextState();				// 次のステートを取得
	void SetNextState(CPlayer_State* pState);	// 次のステートを設定

protected:

	player::CPlayer*	m_pPlayer;		// プレイヤー
	CPlayer_State*		m_pNextState;	// 次のステート
};

#endif	// _PLAYER_STATE_