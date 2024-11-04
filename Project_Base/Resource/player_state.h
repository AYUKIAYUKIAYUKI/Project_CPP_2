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
#include "character_state.h"
#include "player.h"

//****************************************************
// プレイヤーステートクラス
//****************************************************
class CPlayer_State : public CCharacter_State
{
public:

	CPlayer_State();					// デフォルトコンストラクタ
	CPlayer_State(CPlayer* pPlayer);	// プレイヤー取得コンストラクタ
	virtual ~CPlayer_State() override;	// デストラクタ

	virtual void Update() override = 0;	// 更新処理

private:

	CPlayer* RetrievePlayer();	// プレイヤーポインタを取得
};

#endif	// _PLAYER_STATE_