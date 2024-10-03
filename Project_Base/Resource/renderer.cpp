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

// 描画処理用
#include "manager.h"

// オブジェクト取得
#include "object.h"

// シングルトン管理
#include "texture_manager.h"
#include "model_X_manager.h"

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CRenderer* CRenderer::m_pRenderer = nullptr;	// レンダラー

//============================================================================
// コンストラクタ
//============================================================================
CRenderer::CRenderer() :
	m_pD3D{ nullptr },				// Direct3D
	m_pD3DDevice{ nullptr },		// デバイス
	m_pFont{ nullptr },				// フォント
	m_debugStr{},					// 表示用文字列
	m_timeStr{}						// 時限式文字列
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
	ZeroMemory(&d3dpp, sizeof(d3dpp));	// パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲームサイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲームサイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
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

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ?
	//ID3DXAllocateHierarchy;

	// フォントを生成
	D3DXCreateFont(m_pD3DDevice, 22, 0, FW_HEAVY, 1,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &m_pFont);

	// テクスチャマネージャー初期設定
	if (FAILED(CTexture_Manager::GetInstance()->Load()))
	{
		return E_FAIL;
	}

	// Xモデルマネージャー初期設定
	if (FAILED(CModel_X_Manager::GetInstance()->Load()))
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
	if (m_pRenderer != nullptr)
	{
		// 終了処理
		m_pRenderer->Uninit();

		// メモリを解放
		delete m_pRenderer;

		// ポインタを初期化
		m_pRenderer = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CRenderer::Update()
{
	// 文字列クリア
	m_debugStr = {};

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
	//// 画面バッファクリア
	//m_pD3DDevice->Clear(0, nullptr,
	//	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
	//	D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// 疑似スクリーンのテクスチャ内へ描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// カメラをセット
		CManager::GetCamera()->SetCamera();

		// 全オブジェクトの描画
		CObject::DrawAll();

		// シーンの専用描画
		CManager::GetScene()->Draw();

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
	// 表示位置
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	for (unsigned int i = 0; i < m_timeStr.size(); i++)
	{
		if (m_timeStr[i].second > 0)
		{
			// 表示時間をカウントダウン
			m_timeStr[i].second--;

			// 文章をつなげる
			m_debugStr += m_timeStr[i].first + "\n";
		}
		else
		{
			// 文章を消去
			m_timeStr.erase(m_timeStr.begin() + i);
		}
	}

	//テキストの描画
	m_pFont->DrawText(NULL, m_debugStr.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//============================================================================
// レンダラーの取得
//============================================================================
CRenderer* CRenderer::GetInstance()
{
	if (m_pRenderer == nullptr)
	{
		// 生成
		m_pRenderer->Create();
	}

	return m_pRenderer;
}

//============================================================================
// デバイスの取得
//============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDeviece()
{
	return m_pD3DDevice;
}

//============================================================================
// デバッグ用文字列に追加
//============================================================================
void CRenderer::SetDebugString(std::string str)
{
	m_debugStr += str + "\n";
}

//============================================================================
// 時限式デバッグ文字列に追加
//============================================================================
void CRenderer::SetTimeString(std::string str, int nCnt)
{
	m_timeStr.push_back({ str, nCnt });
}

//============================================================================
// 生成
//============================================================================
void CRenderer::Create()
{
	if (m_pRenderer != nullptr)
	{ // 二重生成禁止
		assert(false);
	}

	// インスタンスを生成
	m_pRenderer = DBG_NEW CRenderer{};
}

//============================================================================
// 終了処理
//============================================================================
void CRenderer::Uninit()
{
	// 全オブジェクト解放処理
	CObject::ReleaseAll();

	// Xモデルマネージャー破棄
	CModel_X_Manager::GetInstance()->Release();

	// テクスチャマネージャー破棄
	CTexture_Manager::GetInstance()->Release();

	// フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}

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