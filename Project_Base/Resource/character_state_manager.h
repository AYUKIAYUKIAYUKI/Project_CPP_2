//============================================================================
//
// キャラクターステートマネージャー、ヘッダファイル [character_state_manager.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _CHARACTER_STATE_MANAGER_H_
#define _CHARACTER_STATE_MANAGER_H_	// 二重インクルード防止

//****************************************************
// 前方宣言
//****************************************************
class CCharacter_State;

//****************************************************
// プレイヤーステートマネージャークラス
//****************************************************
class CCharacter_State_Manager
{
public:

	CCharacter_State_Manager();		// デフォルトコンストラクタ
	~CCharacter_State_Manager();	// デストラクタ

	void Release();				// 破棄
	void Update();				// 更新処理
	void CheckChangeState();	// ステートの変更を確認

	static CCharacter_State_Manager* Create(CCharacter_State* pState);	// 生成

private:

	HRESULT Init(CCharacter_State* pState);	// 初期設定
	void	Uninit();						// 終了設定

	CCharacter_State* m_pState;	// ステートのポインタ
};

#endif	// _CHARACTER_STATE_MANAGER_H_