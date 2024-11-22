//============================================================================
// 
// フィールドマネージャー、ヘッダファイル [field_manager.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _FIELD_MANAGER_H_
#define _FIELD_MANAGER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"
#include "player.h"

//****************************************************
// 前方宣言
//****************************************************
class CObject_HUD;
class CFan;

//****************************************************
// フィールドマネージャークラス
//****************************************************
class CField_Manager final
{
public:

	/// <summary> フィールド半径 </summary>
	static constexpr float FIELD_RADIUS = 300.0f;

	/// <summary> 最大ブロック数 </summary>
	static constexpr int MAX_BLOCK = 5;

	/// <summary> 重力 </summary>
	static constexpr float FIELD_GRAVITY = -0.5f;

	// <function>
	HRESULT Init();	// 初期設定
	void Release();	// 解放
	void Update();	// 更新処理
	void Draw();	// 描画処理

	// <static function>
	static CField_Manager* GetInstance();	// 自インスタンスを取得

private:

	// <special function>
	CField_Manager();	// コンストラクタ
	~CField_Manager();	// デストラクタ

	// <function>
	void Create();										// 生成
	void Uninit();										// 終了処理
	void GenerateBlock();								// ブロックの自動生成
	bool DetectAdjacentBlock(const D3DXVECTOR3& Pos);	// 隣接し合うブロックを検出
	void DestroyBlock();								// ブロックの自動削除
	void DestroyAllBlock();								// 全ブロックの削除
	void UpdateHUD();									// HUDの更新処理

	/* けします */
	void DEBUG_CIRCLE();	// デバッグサークル

	// <data>
	CObject_HUD*	m_pMap;								// マップ
	CPlayer*		m_pPlayer;							// プレイヤー	
	CObject_HUD*	m_pPlayerLife[CPlayer::MAX_LIFE];	// プレイヤーの体力
	CObject_HUD*	m_pPlayerGauge;						// プレイヤーのゲージ
	CObject_HUD*	m_pPlayerGaugeWindow;				// プレイヤーのゲージウィンドウ
	CFan*			m_pFan;								// 扇形

	// <static data>
	static CField_Manager* m_pField_Manager;	// フィールドマネージャーの本体 
};

#endif // _FIELD_MANAGER_H_