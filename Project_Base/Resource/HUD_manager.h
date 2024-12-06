//============================================================================
// 
// HUDマネージャー、ヘッダファイル [HUD_manager.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _HUD_MANAGER_H_
#define _HUD_MANAGER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "player.h"

//****************************************************
// 前方宣言
//****************************************************
class CObject_HUD;

//****************************************************
// フィールドマネージャークラス
//****************************************************
class CHUD_Manager final
{
public:

	// <special function>
	CHUD_Manager(const CHUD_Manager&) = delete;				// コピーコンストラクタ
	CHUD_Manager& operator=(const CHUD_Manager&) = delete;	// コピー代入演算子
	CHUD_Manager(CHUD_Manager&&) = delete;					// ムーブコンストラクタ
	CHUD_Manager& operator=(CHUD_Manager&&) = delete;		// ムーブ代入演算子

	// <function>
	HRESULT Init();							// 初期設定
	void	Update();						// 更新処理
	void	Draw();							// 描画処理
	void	SpecialMapSymbol(float fMove);	// マップシンボルの特殊挙動

	// <setter>
	void SetSyncPlayer(const CPlayer* const pPlayer);	// プレイヤーをセット

	// <static function>
	static HRESULT	Create();	// 生成
	static void		Release();	// 解放

	// <static getter>
	static CHUD_Manager* GetInstance();	// HUDマネージャーを取得

private:

	// <special function>
	CHUD_Manager();		// コンストラクタ
	~CHUD_Manager();	// デストラクタ

	// <function>
	void Uninit();				// 終了処理
	void ParamControl();		// 調整
	void UpdateMapSymbol();		// マップシンボルの更新
	void UpdatePlayerLife();	// プレイヤーライフの更新
	void UpdateBossGaugeBack();	// ボスゲージ背景の更新
	void UpdateBossGaugeBar();	// ボスゲージバーの更新
	bool DetectError();			// エラー検出

	// <data>
	const CPlayer* m_pSyncPlayer;						// プレイヤーのポインタ
	CObject_HUD*   m_pMapBase;							// マップ枠
	CObject_HUD*   m_pMapRing;							// マップ輪
	CObject_HUD*   m_pMapSymbol;						// マップシンボル
	CObject_HUD*   m_pPlayerGauge;						// プレイヤーゲージ
	CObject_HUD*   m_pPlayerGaugeWindow;				// プレイヤーゲージ枠
	CObject_HUD*   m_pPlayerLife[CPlayer::MAX_LIFE];	// プレイヤーライフ
	CObject_HUD*   m_pBossGaugeBack;					// ボスゲージ背景
	CObject_HUD*   m_pBossGaugeBar;						// ボスゲージバー
	JSON		   m_BossGaugeBarParam;					// ボスゲージバーパラメータ
	bool		   m_bSwitchBar;						// バー切り替え
	CObject_HUD*   m_pBossGaugeBase;					// ボスゲージ枠

	// <static data>
	static CHUD_Manager* m_pHUDManager;	// HUDマネージャーの本体 
};

#endif // _FIELD_MANAGER_H_