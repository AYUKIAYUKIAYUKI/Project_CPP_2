//============================================================================
//
// メッシュドーム、ヘッダファイル [mesh_dome.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _MESH_DOME_H_
#define _MESH_DOME_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

//****************************************************
// メッシュドームクラス
//****************************************************
class CMesh_Dome : public CObject
{
public:

	/// <summary> プレイヤーの最大体力 </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> プレイヤーのデフォルト移動速度 </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.005f;

	/// <summary> 目標値への補間強度 </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	CMesh_Dome();	// デフォルトコンストラクタ
	~CMesh_Dome();	// デストラクタ

	HRESULT Init() override;		// 初期設定
	void	Uninit() override;		// 終了設定
	void	Update() override;		// 更新処理
	void	Draw() override;		// 描画処理
};

#endif // _MESH_DOME_H_