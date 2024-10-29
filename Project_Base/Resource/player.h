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

		/// <summary> プレイヤーの最大体力 </summary>
		static constexpr int MAX_LIFE = 5;

		/// <summary> プレイヤーのデフォルト移動速度 </summary>
		static constexpr float DEFAULT_MOVE_SPEED = 0.005f;

		CPlayer();	// デフォルトコンストラクタ
		~CPlayer();	// デストラクタ

		HRESULT Init() override;	// 初期設定
		void	Uninit() override;	// 終了設定
		void	Update() override;	// 更新処理
		void	Draw() override;	// 描画処理

		const int& GetLife() const;	// 体力を取得
		void SetLife(int nLife);	// 体力を設定

		const Vec3& GetPosTarget() const;	// 目標座標を取得
		void SetPosTarget(Vec3 PosTarget);	// 目標座標を設定

		const Vec3& GetRotTarget() const;	// 目標向きを取得
		void SetRotTarget(Vec3 RotTarget);	// 目標向きを設定

		const float& GetMoveSpeed() const;		// 移動速度を取得
		void SetMoveSpeed(float fMoveSpeed);	// 移動速度を設定

		const float& GetDirection() const;		// 方角を取得
		void SetDirection(float fDirection);	// 方角を設定

		static CPlayer* Create();	// 生成

	private:

		/// <summary> 目標値への補間強度 </summary>
		static constexpr float COEF_ADJUST = 0.1f;

		void AdjustToTarget();	// 目標値への補正

		player_state_manager::CPlayer_State_Manager* m_pPlayerStateManager;	// プレイヤーステートマネージャー

		float	m_fDirection;	// 方角
		Vec3	m_PosTarget;	// 目標座標
		float	m_fMoveSpeed;	// 移動速度
		Vec3	m_RotTarget;	// 目標向き
		int		m_nLife;		// 体力
	};
}

#endif // _PLAYER_H_