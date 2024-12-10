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
class CPlayer;
class CBounding_Cylinder;

//****************************************************
// エネミークラス
//****************************************************
class CEnemy : public CCharacter
{
public:

	// <special function>
	CEnemy();			// コンストラクタ
	~CEnemy() override;	// デストラクタ

	// <function>
	HRESULT Init() override;					// 初期設定
	void	Uninit() override;					// 終了設定
	void	Update() override;					// 更新処理
	void	Draw() override;					// 描画処理
	void	SetDamage(int nDamage) override;	// ダメージを受ける

	// <getter>
	float GetRadius() const;								// 半径を取得
	float GetHeight() const;								// 高さを取得
	const CBounding_Cylinder* const GetBndCylinder() const;	// バウンディングシリンダーを取得

protected:

	// <function>
	virtual bool HitCheck(); // 衝突検出
	
	// <setter>
	void SetBndSize(float fRadius, float fHeight);	// バウンディングサイズの設定

	// <getter>
	CPlayer* const FindPlayer() const;	// プレイヤー検索

private:

	// <function>
	void AdjustHeight() override;	// 高さを補正

	// <data>
	CBounding_Cylinder* m_pBndCylinder;	// 円柱バウンディング
};

#endif // _ENEMY_H_