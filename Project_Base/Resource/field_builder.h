//============================================================================
// 
// フィールドビルダー、ヘッダファイル [field_builder.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _FIELD_BUILDER_H_
#define _FIELD_BUILDER_H_	// 二重インクルード防止

//****************************************************
// 前方宣言
//****************************************************
class CField_Type;
class CPlayer;
class CFan;

//****************************************************
// フィールドビルダークラス
//****************************************************
class CField_Builder
{
public:

	// <special function>
	CField_Builder(const CField_Builder&) = delete;				// コピーコンストラクタ
	CField_Builder& operator=(const CField_Builder&) = delete;	// コピー代入演算子
	CField_Builder(CField_Builder&&) = delete;					// ムーブコンストラクタ
	CField_Builder& operator=(CField_Builder&&) = delete;		// ムーブ代入演算子

	// <function>
	void Release();						// 解放
	void Update();						// 更新処理
	void Draw();						// 描画処理
	void IncrementCntJump();			// ジャンプした回数のインクリメント
	void IncrementCntDash();			// ダッシュした回数のインクリメント
	void IncrementCntSlash();			// 攻撃した回数のインクリメント
	bool DetectInFanRange(D3DXVECTOR3);	// 扇形範囲内に存在しているか判定

	// <getter>
	int GetCntDestroyBlock();	// ブロックの破壊数を取得

	// <setter>
	void SetSyncPlayer(const CPlayer* const pPlayer);	// プレイヤーをセット

	// <static function>
	static CField_Builder* Create();	// 生成

private:

	//****************************************************
	// アクションデータ構造体の定義
	//****************************************************
	struct ActionData
	{
		int nCntJump;	// ジャンプした回数
		int nCntDash;	// ダッシュした回数
		int nCntSlash;	// 攻撃した回数
	};

	// <special function>
	CField_Builder();	// コンストラクタ
	~CField_Builder();	// デストラクタ

	// <function>
	HRESULT Init();								// 初期設定
	void	Uninit();							// 終了処理
	void	UpdateFan();						// 扇形の更新
	void	UpdateBuilder();					// ビルダーの更新
	void	BranchFieldType();					// フィールドタイプ分岐
	void	GenerateItem();						// アイテムの自動生成
	bool	DetectNearBlock(D3DXVECTOR3 Pos);	// 隣接し合うブロックを検出
	void	DestroyCheck();						// 破壊判定
	void	DestroyAllBlock();					// 全ブロックの削除
	void	PrintDebug();						// デバッグ表示

	// <data>
	std::unique_ptr<CField_Type> m_upFieldType;			// フィールドタイプ
	ActionData					 m_ActionData;			// アクションデータ
	int							 m_nCntDestroyBlock;	// ブロックの破壊数
	const CPlayer*				 m_pSyncPlayer;			// プレイヤーのポインタ
	CFan*						 m_pFan;				// 扇形範囲
};

#endif // _FIELD_BUILEDER_H_