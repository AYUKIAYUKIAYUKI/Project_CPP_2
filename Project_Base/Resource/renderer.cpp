//============================================================================
// 
// レンダラー [renderer.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "renderer.h"
#include "scene.h"

// 描画処理用
#include "manager.h"

// オブジェクト取得
#include "object.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define ENABLE_STENCIL_BUFFER 1	// ステンシルバッファの有効化

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CRenderer* CRenderer::m_pRenderer = nullptr;	// レンダラーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CRenderer::Update()
{
	// 文字列クリア
	m_DebugStr = {};

	// 全オブジェクト更新処理
	CObject::UpdateAll();

	// 全オブジェクト後更新処理
	CObject::LateUpdateAll();
}

//============================================================================
// 描画処理
//============================================================================
void CRenderer::Draw()
{
#if ENABLE_STENCIL_BUFFER

	// 画面バッファクリア
	m_pD3DDevice->Clear(0,											// クリアしたい四角形の数を設定 (ビューポート全体の場合は0)
		nullptr,													// 四角形構造体のポインタを設定 (nullptrを渡すことでビューポート全体の範囲)
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// クリアするサーフェスを指定する
		D3DCOLOR_RGBA(0, 0, 0, 0),									// このカラーでターゲットをクリア
		1.0f,														// この値に大してデプスバッファをクリア
		0);															// この値でステンシルバッファをクリア

	// ステンシルバッファへの書き込みを有効化
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

#else

	// 画面バッファクリア
	m_pD3DDevice->Clear(0,
		nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

#endif

	// 疑似スクリーンのテクスチャ内へ描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// カメラをセット
		CManager::GetManager()->GetCamera()->SetCamera();

		// 全オブジェクトの描画
		CObject::DrawAll();

		// シーンの専用描画
		CScene_Manager::GetInstance()->GetScene()->Draw();

		// ImGuiの描画
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// デバッグ表示
		PrintDebug();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//============================================================================
// デバッグ表示
//============================================================================
void CRenderer::PrintDebug()
{
	// 表示範囲調整用
	static WORD wAdjustHeight = 0;

	// 表示範囲変動
	if (CManager::GetKeyboard()->GetPress(DIK_NUMPAD2))
	{
		wAdjustHeight > 0 ? wAdjustHeight -= 5 : wAdjustHeight = 0;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_NUMPAD8))
	{
		wAdjustHeight += 5;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_NUMPAD0))
	{
		wAdjustHeight = 0;
	}

	// 表示位置
	RECT Rect = { 0, 0 - wAdjustHeight, WSCREEN_WIDTH, WSCREEN_HEIGHT - wAdjustHeight };

	for (WORD i = 0; i < m_TimeStr.size(); ++i)
	{
		if (m_TimeStr[i].second > 0)
		{
			// 表示時間をカウントダウン
			m_TimeStr[i].second--;

			// 文章をつなげる
			m_DebugStr += m_TimeStr[i].first + "\n";
		}
		else
		{
			// 文章を消去
			m_TimeStr.erase(m_TimeStr.begin() + i);
		}
	}

	// テキストの描画
	m_pFont->DrawText(NULL, m_DebugStr.c_str(), -1, &Rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//============================================================================
// 生成
//============================================================================
HRESULT CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer != nullptr)
	{
		assert(false && "レンダラーは既に作成されているか、ダングリングしています");
	}

	// レンダラーを生成
	m_pRenderer = DBG_NEW CRenderer();

	if (m_pRenderer == nullptr)
	{
		assert(false && "レンダラーの生成に失敗");
	}

	// レンダラーの初期設定
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CRenderer::Release()
{
	// レンダラーの破棄
	if (m_pRenderer != nullptr)
	{
		// レンダラー終了処理
		m_pRenderer->Uninit();

		// メモリを解放
		delete m_pRenderer;

		// ポインタを初期化
		m_pRenderer = nullptr;
	}
}

//============================================================================
// レンダラーの取得
//============================================================================
CRenderer* CRenderer::GetRenderer()
{
	return m_pRenderer;
}

//============================================================================
// デバイスの取得
//============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDeviece()
{
	return m_pRenderer->m_pD3DDevice;
}

//============================================================================
// デバッグ用文字列に追加
//============================================================================
void CRenderer::SetDebugString(std::string Str)
{
	m_pRenderer->m_DebugStr += Str + "\n";
}

//============================================================================
// 時限式デバッグ文字列に追加
//============================================================================
void CRenderer::SetTimeString(std::string Str, int nCnt)
{
	m_pRenderer->m_TimeStr.push_back({ Str, nCnt });
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CRenderer::CRenderer() :
	m_pD3D{ nullptr },
	m_pD3DDevice{ nullptr },
	m_pFont{ nullptr },
	m_DebugStr{},
	m_TimeStr{}
{

}

//============================================================================
// デストラクタ
//============================================================================
CRenderer::~CRenderer()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindiw)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{ // 生成に失敗した場合
		return E_FAIL;
	}

	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{ // 現在のディスプレイモードを取得
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// パラメータのゼロクリア
#if 0
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// ゲームサイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		// ゲームサイズ(高さ)
#else
	d3dpp.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);	// ゲームサイズ(幅)
	d3dpp.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);	// ゲームサイズ(高さ)
#endif
	d3dpp.BackBufferFormat = d3ddm.Format;		// バックバッファの形式
	d3dpp.BackBufferCount = 1;					// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;		// デプスバッファとステンシルバッファを作成
#if ENABLE_STENCIL_BUFFER
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// デプスバッファとして24bit、ステンシルバッファとして8bitを使用する
#else
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// デプスバッファとして16bitを使う
#endif
	d3dpp.Windowed = bWindiw;									// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成行程①
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		// Direct3Dデバイスの生成行程②
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// Direct3Dデバイスの生成行程③
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの初期設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// テクスチャステージステートの初期設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ImGuiのコンテキストを作成
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ImGuiの入出力設定
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.Fonts->AddFontFromFileTTF("Data\\FONT\\meiryo.ttc", 18.0f, nullptr);

	// ImGuiの表示スタイルを設定
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// バックエンドの初期設定
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(CRenderer::GetDeviece());

	// フォントを生成
	D3DXCreateFont(m_pD3DDevice,
		18,
		0,
		FW_HEAVY,
		1,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pFont);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CRenderer::Uninit()
{
	// 全オブジェクト解放処理
	CObject::ReleaseAll();

	// フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}

	// ImGUiの終了処理
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}