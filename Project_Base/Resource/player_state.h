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

	// <special function>
	CPlayer_State();					// コンストラクタ
	CPlayer_State(CPlayer* pPlayer);	// プレイヤー取得コンストラクタ
	virtual ~CPlayer_State() override;	// デストラクタ

	// <function>
	virtual void Update() override = 0;	// 更新処理
	virtual void To_Default();			// 通常状態へ変更
	virtual void To_Dash();				// ダッシュ状態へ変更
	virtual void To_Jump();				// ジャンプ状態へ変更
	virtual void To_Slash();			// 斬撃状態へ変更
	virtual void To_Damage();			// ダメージ状態へ変更

private:

	// <function>
	void PrintError();	// エラー表示

	// <Getter>
	CPlayer* RetrievePlayer();	// プレイヤーポインタを取得
};

#endif	// _PLAYER_STATE_