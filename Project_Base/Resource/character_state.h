//============================================================================
//
// キャラクターステート、ヘッダファイル [character_state.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _CHARACTER_STATE_
#define _CHARACTER_STATE_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "state.h"
#include "character.h"

//****************************************************
// キャラクターステートクラス
//****************************************************
class CCharacter_State : public CState
{
public:

	CCharacter_State();						// デフォルトコンストラクタ
	virtual ~CCharacter_State() override;	// デストラクタ

	virtual void Update() override = 0;	// 更新処理

	CCharacter_State* GetNextState();				// 変更予定のステートを取得
	template <typename T> void SetNextState(T* pT);	// 変更予定のステートを設定

protected:

	CCharacter*			m_pCharacter;	// キャラクターのポインタ
	CCharacter_State*	m_pNextState;	// 変更予定のステート
};

//============================================================================
// 変更予定のステートを設定
//============================================================================
template <typename T> void CCharacter_State::SetNextState(T* pT)
{
	m_pNextState = pT;
}

#endif	// _CHARACTER_STATE_