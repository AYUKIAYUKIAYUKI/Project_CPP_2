//============================================================================
// 
// オブジェクト管理、ヘッダファイル [object.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_	// 二重インクルード防止

//****************************************************
// オブジェクトクラス
//****************************************************
class CObject
{
public:

	/// <summary> 許容するオブジェクトの最大数 </summary>
	static constexpr int MAX_OBJECT = 2048;

	//****************************************************
	// オブジェクトのレイヤー識別
	//****************************************************
	enum class LAYER : WORD
	{
		BG = 0,		// 背景
		DEFAULT,	// 通常
		FRONT,		// 前面
		STENCIL,	// ステンシル
		UI,			// UI
		MAX
	};

	//****************************************************
	// オブジェクトの種類識別
	//****************************************************
	enum class TYPE : WORD
	{
		NONE = 0,	// 無し
		TEXTMESH,	// テキストメッシュ
		ONLYTEXT,	// テキストのみ
		PLAYER,		// プレイヤー
		BOSS,		// ボス
		ITEM,		// アイテム
		BLOCK,		// ブロック
		BRIGHT,		// 閃光
		ENEMY,		// エネミー
		LATEDRAW,	// 後描画対応
		MAX
	};

	// <special function>
	CObject();									// コンストラクタ
	CObject(LAYER Priority = LAYER::DEFAULT);	// 描画優先度指定コンストラクタ
	virtual ~CObject() = 0;						// デストラクタ

	// <function>
	void SetRelease();	// 解放予約設定
	void Release();		// 個別解放処理

	// <virtual function>
	virtual HRESULT	Init() = 0;		// 初期設定
	virtual void	Uninit() = 0;	// 終了処理
	virtual void	Update() = 0;	// 更新処理
	virtual void	Draw() = 0;		// 描画処理

	// <getter/setter>
	const TYPE& GetType() const;	// タイプを取得
	void SetType(TYPE Type);		// タイプを設定

	// <getter>
	CObject* GetPrev();	// 前のオブジェクトのポインタを取得
	CObject* GetNext();	// 次のオブジェクトのポインタを取得

	// <static function>
	static void ReleaseAll();		// 全オブジェクト解放処理
	static void UpdateAll();		// 全オブジェクト更新処理
	static void LateUpdateAll();	// 全オブジェクト後更新処理
	static void DrawAll();			// 全オブジェクト描画処理
	static void LateDrawAll();		// 全オブジェクト描画処理

	// <static getter>
	static CObject* GetTopObject();					// 先頭オブジェクトのポインタ取得
	static CObject* GetTopObject(WORD wPriority);	// 先頭オブジェクトのポインタ取得
	static CObject* GetTopObject(LAYER Priority);	// 先頭オブジェクトのポインタ取得
	static CObject* FindSpecificObject(TYPE Type);	// 特定タイプのオブジェクト探す
	static int		CountSpecificObject(TYPE Type);	// 特定タイプのオブジェクト数を取得

	// <仮>
	template <typename T> static T* DownCast(CObject* pBase);	// オブジェクト派生専用ダウンキャスト

private:

	// <data>
	WORD	 m_wPriority;	// 描画優先度
	CObject* m_pPrev;		// 前のオブジェクトのポインタ
	CObject* m_pNext;		// 次のオブジェクトのポインタ
	TYPE	 m_Type;		// タイプ識別 
	bool	 m_bDeath;		// 死亡フラグ

	// <static data>
	static int		m_nNumAll;								// オブジェクト総数
	static CObject* m_pTop[static_cast<WORD>(LAYER::MAX)];	// 先頭オブジェクトのポインタ
	static CObject* m_pCur[static_cast<WORD>(LAYER::MAX)];	// 終端オブジェクトのポインタ
};

//============================================================================
// オブジェクト派生専用ダウンキャスト
//============================================================================
template <typename T> T* CObject::DownCast(CObject* pBase)
{
	// 指定されたクラスにダウンキャスト
	T* pNew = dynamic_cast<T*>(pBase);

	// ダウンキャスト失敗
	if (!pNew)
	{
		return nullptr;
	}
	
	return pNew;
}

#endif // _OBJECT_H_