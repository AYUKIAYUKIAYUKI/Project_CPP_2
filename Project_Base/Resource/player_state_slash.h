//============================================================================
//
// プレイヤー斬撃ステート、ヘッダファイル [player_state_slash.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_STATE_SLASH_
#define _PLAYER_STATE_SLASH_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "player_state.h"
#include "bounding_sphere.h"

//****************************************************
// プレイヤー斬撃ステートクラス
//****************************************************
class CPlayer_State_Slash : public CPlayer_State
{
public:

	//****************************************************
	// 斬撃方向のタイプを定義
	//****************************************************
	enum class SLASH_TYPE : int
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		MAX
	};

	/// <summary> 斬撃キャスト期間 </summary>
	static constexpr int MAX_SLASH_CAST = 40;

	// <special function>
	CPlayer_State_Slash();				// コンストラクタ
	~CPlayer_State_Slash() override;	// デストラクタ

	// <function>
	void Update() override;		// 更新処理
	void To_Default() override;	// 通常状態へ
	void To_Damage() override;	// ダメージ状態へ

private:

	// <function>
	void AdjustGravity();	// 重力の補正
	bool SlashHitCheck();	// 斬撃当たり判定

	// <data>
	SLASH_TYPE							m_SlashType;	// 斬撃方向のタイプ
	std::unique_ptr<CBounding_Sphere>	m_pBndSlash;	// 斬撃のバウンディング
	
};

#endif	// _PLAYER_STATE_SLASH_