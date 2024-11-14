//============================================================================
//
// エネミー、ヘッダファイル [enemy.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "character.h"

//****************************************************
// 前方宣言
//****************************************************
class CBounding_Cylinder;

//****************************************************
// エネミークラス
//****************************************************
class CEnemy : public CCharacter
{
public:

	/// <summary> エネミー最大体力 </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> エネミーのデフォルト移動速度 </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.003f;

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	// <special function>
	CEnemy();	// コンストラクタ
	~CEnemy();	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <getter>
	float GetRadius() const override;	// 半径を取得
	float GetHeight() const override;	// 高さを取得

	// <static funcion>
	static CEnemy* Create();	// 生成

private:

	// <function>
	void AttackOnPlayer();	// プレイヤーへ攻撃

	// <data>
	CBounding_Cylinder* m_pBndCylinder;		// バウンディングシリンダー
};

#endif // _PLAYER_H_