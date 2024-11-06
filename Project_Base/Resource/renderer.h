//============================================================================
// 
// レンダラー、ヘッダファイル [renderer.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// 二重インクルード防止

//****************************************************
// レンダラークラス
//****************************************************
class CRenderer final
{
public:

	// <special function>
	CRenderer(const CRenderer&) = delete;				// コピーコンストラクタ
	CRenderer& operator=(const CRenderer&) = delete;	// コピー代入演算子
	CRenderer(CRenderer&&) = delete;					// ムーブコンストラクタ
	CRenderer& operator=(CRenderer&&) = delete;			// ムーブ代入演算子

	// <function>
	void Update();		// 更新処理
	void Draw();		// 描画処理
	void PrintDebug();	// デバッグ表示

	// <static function>
	static HRESULT	Create(HWND hWnd, BOOL bWindiw);	// 生成
	static void		Release();							// 破棄

	// <static getter>
	static CRenderer*			GetRenderer();	// レンダラーを取得
	static LPDIRECT3DDEVICE9	GetDeviece();	// デバイスを取得

	// <static setter>
	static void	SetDebugString(std::string Str);			// デバッグ文字列に追加
	static void	SetTimeString(std::string Str, int nCnt);	// 時限式デバッグ文字列に追加

private:

	// <special function>
	CRenderer();	// コンストラクタ
	~CRenderer();	// デストラクタ

	// <function>
	HRESULT Init(HWND hWnd, BOOL bWindiw);	// 初期設定
	void	Uninit();						// 終了処理

	// <data>
	LPDIRECT3D9									m_pD3D;			// Direct3D
	LPDIRECT3DDEVICE9							m_pD3DDevice;	// デバイス
	LPD3DXFONT									m_pFont;		// フォント
	std::string									m_DebugStr;		// 表示用文字列
	std::vector<std::pair<std::string, int>>	m_TimeStr;		// 時限式文字列

	// <static data>
	static CRenderer* m_pRenderer;	// レンダラーの本体
};

#endif // _RENDERER_H_