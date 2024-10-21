//============================================================================
//
// プレイヤーステートマネージャー、ヘッダファイル [player_state_manager.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_STATE_MANAGER_H_
#define _PLAYER_STATE_MANAGER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "state.h"

namespace player_state_manager
{
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace abbr;

	//****************************************************
	// プレイヤーステートクラス
	//****************************************************
	class CPlayer_State_Manager
	{
	public:

		CPlayer_State_Manager();	// デフォルトコンストラクタ
		~CPlayer_State_Manager();	// デストラクタ

		void Release();	// 破棄
		void Update() ;	// 更新処理

		static CPlayer_State_Manager* Create();	// 生成

	private:

		HRESULT Init();		// 初期設定
		void	Uninit();	// 終了設定
	};
}

#endif	// _PLAYER_STATE_H_