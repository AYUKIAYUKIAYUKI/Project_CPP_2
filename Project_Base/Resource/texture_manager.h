//============================================================================
// 
// テクスチャマネージャー、ヘッダファイル [texture_manager.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_	// 二重インクルード防止

//****************************************************
// テクスチャマネージャークラス
//****************************************************
class CTexture_Manager final
{
public:

	//****************************************************
	// テクスチャ識別
	//****************************************************
	enum class TYPE : WORD
	{
		TITLE = 0,			// タイトル
		START,				// スタート表示
		RESULT,				// リザルト表示
		CONSTELLATION0,		// 星座0
		CONSTELLATION1,		// 星座1
		CONSTELLATION2,		// 星座2
		CONSTELLATION3,		// 星座3
		MAPBASE,			// マップ枠
		MAPRING,			// マップ輪
		CIRCLE,				// サークル
		PLAYERGAUGE,		// プレイヤーのゲージ
		PLAYERGAUGEWINDOW,	// プレイヤーのゲージウィンドウ
		BOSSGAUBASE,		// ゲージ枠
		BOSSGAUBAR,			// ゲージバー
		BOSSGAUBACK,		// ゲージ背景
		MAX,
	};

	// +演算子のオーバーロード
	friend TYPE operator+(TYPE Type, WORD wNum)
	{
		return static_cast<TYPE>(static_cast<WORD>(Type) + wNum);
	}

	// <special function>
	CTexture_Manager(const CTexture_Manager&) = delete;				// コピーコンストラクタ
	CTexture_Manager& operator=(const CTexture_Manager&) = delete;	// コピー代入演算子
	CTexture_Manager(CTexture_Manager&&) = delete;					// ムーブコンストラクタ
	CTexture_Manager& operator=(CTexture_Manager&&) = delete;		// ムーブ代入演算子

	// <getter>
	LPDIRECT3DTEXTURE9 GetTexture(TYPE Type);	// テクスチャを取得

	// <static function>
	static HRESULT	Create();	// 生成
	static void		Release();	// 解放

	// <static getter>
	static CTexture_Manager* GetInstance();	// テクスチャマネージャーを取得

private:

	// <special function>
	CTexture_Manager();		// コンストラクタ
	~CTexture_Manager();	// デストラクタ

	// <function>
	HRESULT	Init();		// 初期設定
	void	Uninit();	// 終了処理

	// <data>
	LPDIRECT3DTEXTURE9 m_apTexture[static_cast<WORD>(TYPE::MAX)];	// テクスチャ情報

	// <static data>
	static CTexture_Manager* m_pTextureManager;	// テクスチャマネージャーの本体
};

#endif // _TEXTURE_MANAGER_H_