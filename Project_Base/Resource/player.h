//============================================================================
//
// プレイヤー、ヘッダファイル [player.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"
#include "player_state_manager.h"

//****************************************************
// 名前空間を定義
//****************************************************
namespace player
{
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace abbr;

	//****************************************************
	// プレイヤークラス
	//****************************************************
	class CPlayer : public CObject_X
	{
	public:

		/// <summary> デフォルトのプレイヤー移動速度 </summary>
		static constexpr float DEFAULT_MOVE_SPEED = 0.01f;

		CPlayer();	// デフォルトコンストラクタ
		~CPlayer();	// デストラクタ

		HRESULT Init() override;	// 初期設定
		void	Uninit() override;	// 終了設定
		void	Update() override;	// 更新処理
		void	Draw() override;	// 描画処理

		const Vec3& GetPosTarget() const;	// 目標座標を取得
		void SetPosTarget(Vec3 PosTarget);	// 目標座標を設定

		const float& GetDirection() const;		// 方角を取得
		void SetDirection(float fDirection);	// 方角を取得

		static CPlayer* Create();	// 生成

	private:

		void Control();	// 操作

		player_state_manager::CPlayer_State_Manager* m_pPlayerStateManager;	// プレイヤーステートマネージャー

		Vec3	m_PosTarget;	// 目標座標
		float	m_fMoveSpeed;	// 移動速度
		float	m_fDirection;	// 方角
	};
}

#endif // _PLAYER_H_