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

	// <special function>
	CCharacter_State();						// デフォルトコンストラクタ
	virtual ~CCharacter_State() override;	// デストラクタ

	// <virtual function>
	virtual void Update() override = 0;	// 更新処理

	// <function>
	template <typename T> T* CheckChangeState();	// ステートの変更を確認

	// <setter/getter>
	CCharacter_State* GetNextState();				// 変更予定のステートを取得
	template <typename T> void SetNextState(T* pT);	// 変更予定のステートを設定

protected:

	// <data>
	CCharacter*			m_pCharacter;	// キャラクターのポインタ
	CCharacter_State*	m_pNextState;	// 変更予定のステート
};

//============================================================================
// ステートの変更確認
//============================================================================
template <typename T> T* CCharacter_State::CheckChangeState()
{
	// 変更予定のステートは無い
	if (!m_pNextState)
	{
		return dynamic_cast<T*>(this);
	}

	// 変更先のポインタをコピー
	T* pNextState = utility::DownCast<T, CCharacter_State>(m_pNextState);

	// メンバの持つ変更先の情報を初期化
	m_pNextState = nullptr;

	// 自身を破棄
	delete this;

	return pNextState;
}

//============================================================================
// 変更予定のステートを設定
//============================================================================
template <typename T> void CCharacter_State::SetNextState(T* pT)
{
	m_pNextState = pT;
}

#endif	// _CHARACTER_STATE_