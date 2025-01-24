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
class CField_Builder;
class CObject;
class CObject_X;
class CObject_PopUp;
class CMotion_Set;
class CPlayer;

//****************************************************
// フィールドマネージャークラス
//****************************************************
class CField_Manager final
{
public:

	/// <summary> フィールド半径 </summary>
	static constexpr float FIELD_RADIUS = 300.0f;

	/// <summary> フィールド重力 </summary>
	static constexpr float FIELD_GRAVITY = -0.15f;

	/// <summary> 最大破壊ブロック数 </summary>
	static constexpr int MAX_DESTROY_BLOCK = 300;

	// <special function>
	CField_Manager(const CField_Manager&) = delete;				// コピーコンストラクタ
	CField_Manager& operator=(const CField_Manager&) = delete;	// コピー代入演算子
	CField_Manager(CField_Manager&&) = delete;					// ムーブコンストラクタ
	CField_Manager& operator=(CField_Manager&&) = delete;		// ムーブ代入演算子

	// <function>
	void InitForTitle();						// タイトル向け追加初期設定
	void InitForGame();							// ゲーム向け追加初期設定
	void Update();								// 更新処理
	void Draw();								// 描画処理
	bool AdjustPosToFieldSize(CObject* pObj);	// 座標をフィールドサイズに調整
	void AAABBB();	// あああ

	// <getter>
	CField_Builder* const GetFieldBuilder() const;	// フィールドビルダーの取得

	// <setter>
	void SetSyncPlayer(const CPlayer* const pPlayer);	// プレイヤーをセット

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
	HRESULT Init();						// 初期設定
	void	InitEnvironment();			// 環境装飾の初期設定
	void	InitBlockSet();				// 初期ブロックのセット
	void	Uninit();					// 終了処理
	void	UpdatePhase();				// フェーズの更新
	void	UpdateEnvironment();		// 環境装飾の更新処理
	void	AppearBossEvent();			// ボス登場イベント
	void	NotifySceneTransition();	// シーン遷移を通知する
	void	PrintDebug();				// デバッグ表示

	// <data>
	CField_Builder* m_pFieldBuilder;		// フィールドビルダー
	int				m_nPhase;				// フェーズ番号
	CObject_PopUp*	m_pPopUp;				// ポップアップ表示
	CMotion_Set*	m_pStatue;				// 銅像
	int				m_nCntStatueVibration;	// 銅像振動カウント
	const CPlayer*	m_pSyncPlayer;			// プレイヤーのポインタ

	// <static data>
	static CField_Manager* m_pFieldManager;	// フィールドマネージャーの本体
	static bool			   m_bRetry;		// 周回判定
};

#endif // _FIELD_MANAGER_H_