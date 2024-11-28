//============================================================================
// 
// フィールドマネージャー、ヘッダファイル [field_manager.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _FIELD_MANAGER_H_
#define _FIELD_MANAGER_H_	// 二重インクルード防止

//****************************************************
// 前方宣言
//****************************************************
class CObject_X;
class CMotion_Set;
class CObject_HUD;
class CPlayer;
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
	static constexpr int MAX_BLOCK = 1;

	/// <summary> 重力 </summary>
	static constexpr float FIELD_GRAVITY = -0.5f;

	// <special function>
	CField_Manager(const CField_Manager&) = delete;				// コピーコンストラクタ
	CField_Manager& operator=(const CField_Manager&) = delete;	// コピー代入演算子
	CField_Manager(CField_Manager&&) = delete;					// ムーブコンストラクタ
	CField_Manager& operator=(CField_Manager&&) = delete;		// ムーブ代入演算子

	// <function>
	void Update();		// 更新処理
	void Draw();		// 描画処理
	void AppearBoss();	// ボス登場

	// <setter>
	/* 方法を考えてください*/
	void SetPlayer(CPlayer* p) { m_pPlayer = p; };

	// <static function>
	static HRESULT	Create();	// 生成
	static void		Release();	// 解放

	// <static getter>
	static CField_Manager* GetInstance();	// フィールドマネージャーを取得

private:

	/// <summary> 最大銅像振動カウント </summary>
	static constexpr int MAX_CNT_STATUEVIBERATION = 300;

	// <special function>
	CField_Manager();	// コンストラクタ
	~CField_Manager();	// デストラクタ

	// <function>
	HRESULT Init();											// 初期設定
	void	InitEnvironment();								// 環境装飾の初期設定
	void	Uninit();										// 終了処理
	void	UpdateEnvironment();							// 環境装飾の更新処理
	void	AppearBossEvent();								// ボス登場イベント
	void	GenerateBlock();								// ブロックの自動生成
	bool	DetectAdjacentBlock(const D3DXVECTOR3& Pos);	// 隣接し合うブロックを検出
	void	DestroyBlock();									// ブロックの自動削除
	void	DestroyAllBlock();								// 全ブロックの削除
	void	UpdateHUD();									// HUDの更新処理

	/* けします */
	void DEBUG_CIRCLE();	// デバッグサークル

	// <data>
	CObject_X*	 m_pDome;				// ドーム
	CMotion_Set* m_pStatue;				// 銅像
	int			 m_nCntStatueVibration;	// 銅像振動カウント
	CPlayer*	 m_pPlayer;				// プレイヤー情報
	CObject_HUD* m_pMap;				// マップ表示
	CObject_HUD* m_pPlayerLife[5];		// プレイヤーの体力表示
	CObject_HUD* m_pPlayerGauge;		// プレイヤーのゲージ表示
	CObject_HUD* m_pPlayerGaugeWindow;	// プレイヤーのゲージウィンドウ表示
	CFan*		 m_pRenderFan;			// 扇形表示

	// <static data>
	static CField_Manager* m_pFieldManager;	// フィールドマネージャーの本体 
};

#endif // _FIELD_MANAGER_H_