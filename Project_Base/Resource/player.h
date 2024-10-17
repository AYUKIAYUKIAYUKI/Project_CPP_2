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

//****************************************************
// プレイヤークラス
//****************************************************
class CPlayer : public CObject_X
{
public:

	CPlayer();	// デフォルトコンストラクタ
	~CPlayer();	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了設定
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	D3DXVECTOR3 GetPosTarget() const;			// 目標座標を取得
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// 目標座標を設定

	static CPlayer* Create();	// 生成

private:

	void Control();	// 操作

	D3DXVECTOR3 m_PosTarget;	// 目標座標
};

#endif // _PLAYER_H_