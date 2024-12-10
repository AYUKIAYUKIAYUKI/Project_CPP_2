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

	/// <summary> フィールド重力 </summary>
	static constexpr float FIELD_GRAVITY = -0.35f;

	/// <summary> 最大破壊ブロック数 </summary>
	static constexpr int MAX_DESTROY_BLOCK = 50;

	// <special function>
	CField_Manager(const CField_Manager&) = delete;				// コピーコンストラクタ
	CField_Manager& operator=(const CField_Manager&) = delete;	// コピー代入演算子
	CField_Manager(CField_Manager&&) = delete;					// ムーブコンストラクタ
	CField_Manager& operator=(CField_Manager&&) = delete;		// ムーブ代入演算子

	// <function>
	void InitForTitle();		// タイトルでの追加初期設定
	void Update();				// 更新処理
	void Draw();				// 描画処理
	void IncrementCntJump();	// ジャンプした回数のインクリメント
	void IncrementCntDash();	// ダッシュした回数のインクリメント

	// <getter>
	int GetCntDestroyBlock();	// ブロックの破壊数を取得

	// <setter>
	void SetSyncPlayer(const CPlayer* const pPlayer);	// プレイヤーをセット

	// <static function>
	static HRESULT	Create();	// 生成
	static void		Release();	// 解放

	// <static getter>
	static CField_Manager* GetInstance();	// フィールドマネージャーを取得

private:

	//****************************************************
	// アクションデータ構造体の定義
	//****************************************************
	struct ActionData
	{
		int nCntJump;	// ジャンプした回数
		int nCntDash;	// ダッシュした回数
	};

	//****************************************************
	// フィールドタイプの定義
	//****************************************************
	enum class FIELD_TYPE : WORD
	{
		NORMAL, // 普通
		JUMP,	// ジャンプ
		DASH,	// ダッシュ
		MAX
	};

	/// <summary> 最大銅像振動カウント </summary>
	static constexpr int MAX_CNT_STATUEVIBERATION = 300;

	// <special function>
	CField_Manager();	// コンストラクタ
	~CField_Manager();	// デストラクタ

	// <function>
	HRESULT Init();								// 初期設定
	void	InitEnvironment();					// 環境装飾の初期設定
	void	Uninit();							// 終了処理
	void	UpdateEnvironment();				// 環境装飾の更新処理
	void	UpdateFan();						// 扇形の更新
	void	UpdateField();						// フィールド更新
	void	BranchFieldType();					// フィールドタイプ分岐
	void	AutoCreateItem();					// アイテムの自動生成
	void	AutoCreateBlockDash();				// ダッシュタイプのブロック自動生成
	bool	DetectNearBlock(D3DXVECTOR3 Pos);	// 隣接し合うブロックを検出
	void	AutoDestroyBlock();					// ブロックの自動削除
	void	DestroyAllBlock();					// 全ブロックの削除
	void	AppearBossEvent();					// ボス登場イベント
	void	NotifyTransition();					// 遷移を通知する
	void	PrintDebug();						// デバッグ表示

	// <data>
	ActionData		m_ActionData;			// アクションデータ
	FIELD_TYPE		m_FiledType;			// フィールドタイプ
	int				m_nCntDestroyBlock;		// ブロックの破壊数
	const CPlayer*	m_pSyncPlayer;			// プレイヤーのポインタ
	CMotion_Set*	m_pStatue;				// 銅像
	int				m_nCntStatueVibration;	// 銅像振動カウント
	CFan*			m_pRenderFan;			// 扇形表示

	// <static data>
	static CField_Manager* m_pFieldManager;	// フィールドマネージャーの本体 
};

#endif // _FIELD_MANAGER_H_