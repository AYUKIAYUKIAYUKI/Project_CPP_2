////============================================================================
////
//// プレイヤー斬撃ステート、ヘッダファイル [player_state_slash.h]
//// Author : 福田歩希
////
////============================================================================
//
//#ifndef _PLAYER_STATE_SLASH_
//#define _PLAYER_STATE_SLASH_	// 二重インクルード防止
//
////****************************************************
//// インクルードファイル
////****************************************************
//#include "player_state.h"
//
////****************************************************
//// プレイヤージャンプステートクラス
////****************************************************
//class CPlayer_State_Slash : public CPlayer_State
//{
//public:
//
//	//****************************************************
//	// 斬撃方向のタイプを定義
//	//****************************************************
//	enum class SLASH_TYPE : int
//	{
//		LEFT = 0,
//		RIGHT,
//		UP,
//		DOWN,
//		MAX
//	};
//
//	// <special function>
//	CPlayer_State_Slash();			// コンストラクタ
//	~CPlayer_State_Slash() override;	// デストラクタ
//
//	// <function>
//	void Update() override;		// 更新処理
//	void To_Default() override;	// 通常状態へ
//	void To_Dash() override;	// ダメージ状態へ
//
//private:
//
//	/// <summary> 斬撃継続期間 </summary>
//	static constexpr int MAX_DASH_DURATION = 10;
//
//	// <function>
//	void AdjustGravity();	// 重力の補正
//
//	// <data>
//	int			m_nDuration;	// 継続期間
//	SLASH_TYPE	m_SlashType;	// 斬撃方向のタイプ
//};
//
//#endif	// _PLAYER_STATE_SLASH_