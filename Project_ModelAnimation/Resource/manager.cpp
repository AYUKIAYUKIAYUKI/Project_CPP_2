//============================================================================
// 
// マネージャー [manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "manager.h"
#include "renderer.h"
#include "object_X.h"

// シングルトンクラス用
#include "motion_manager.h"
#include "renderer.h"
#include "texture_manager.h"
#include "X_manager.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CManager* CManager::m_pManager = nullptr;	// マネージャーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CManager::Update()
{
	// レンダラーの更新
	CRenderer::GetRenderer()->Update();

	// ライトの更新
	m_pLight->Update();

	// カメラの更新
	m_pCamera->Update();

	// キーボードの更新
	m_pKeyboard->Update();

	// マウスの更新
	m_pMouse->Update();

	// パッドの更新
	m_pPad->Update();
}

//============================================================================
// 描画処理
//============================================================================
void CManager::Draw()
{
	// レンダラーの描画
	CRenderer::GetRenderer()->Draw();
}

//============================================================================
// カメラを取得
//============================================================================
CCamera* CManager::GetCamera() const
{
	return m_pCamera;
}

//============================================================================
// ライトを取得
//============================================================================
CLight* CManager::GetLight() const
{
	return m_pLight;
}

//============================================================================
// 生成
//============================================================================
HRESULT CManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pManager != nullptr)
	{
		assert(false && "マネージャーは既に作成されているか、ダングリングしています");
	}

	// マネージャーを生成
	m_pManager = DBG_NEW CManager();

	if (m_pManager == nullptr)
	{
		assert(false && "マネージャーの生成に失敗");
	}

	// マネージャーの初期設定
	if (FAILED(m_pManager->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CManager::Release()
{
	// マネージャーの破棄
	if (m_pManager != nullptr)
	{
		// マネージャーの終了処理
		m_pManager->Uninit();

		// メモリを解放
		delete m_pManager;

		// ポインタを初期化
		m_pManager = nullptr;
	}
}

//============================================================================
// マネージャーを取得
//============================================================================
CManager* CManager::GetManager()
{
	return m_pManager;
}

//============================================================================
// キーボードを取得
//============================================================================
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pManager->m_pKeyboard;
}

//============================================================================
// マウスを取得
//============================================================================
CInputMouse* CManager::GetMouse()
{
	return m_pManager->m_pMouse;
}

//============================================================================
// パッドを取得
//============================================================================
CInputPad* CManager::GetPad()
{
	return m_pManager->m_pPad;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CManager::CManager() :
	m_pKeyboard{ nullptr },
	m_pMouse{ nullptr },
	m_pPad{ nullptr },
	m_pCamera{ nullptr },
	m_pLight{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CManager::~CManager()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	// レンダラーの生成
	if (FAILED(CRenderer::Create(hWnd, TRUE)))
	{
		return E_FAIL;
	}

	// テクスチャマネージャーを生成
	if (FAILED(CTexture_Manager::Create()))
	{
		return E_FAIL;
	}

	// Xモデルマネージャーを生成
	if (FAILED(CX_Manager::Create()))
	{
		return E_FAIL;
	}

	// モーションマネージャーを生成 (Xモデルマネージャー生成後)
	if (FAILED(CMotion_Manager::Create()))
	{
		return E_FAIL;
	}

	// カメラの生成
	m_pCamera = DBG_NEW CCamera();

	if (m_pCamera == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// カメラの初期化
	m_pCamera->Init();

	// ライトの生成
	m_pLight = DBG_NEW CLight();

	if (m_pLight == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// ライトの初期化
	m_pLight->Init();

	// キーボードの生成
	m_pKeyboard = DBG_NEW CInputKeyboard();

	if (m_pKeyboard == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// キーボードの初期化
	m_pKeyboard->Init(hInstance, hWnd);

	// マウスの生成
	m_pMouse = DBG_NEW CInputMouse();

	if (m_pMouse == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// マウスの初期化
	m_pMouse->Init(hInstance, hWnd);

	// パッドの生成
	m_pPad = DBG_NEW CInputPad();

	if (m_pPad == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// パッドの初期化
	m_pPad->Init(hInstance, hWnd);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CManager::Uninit()
{
	// パッドの破棄
	if (m_pPad != nullptr)
	{
		m_pPad->Uninit();	// 終了処理
		delete m_pPad;		// メモリを解放
		m_pPad = nullptr;	// ポインタを初期化
	}

	// マウスの破棄
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();	// 終了処理
		delete m_pMouse;	// メモリを解放
		m_pMouse = nullptr;	// ポインタを初期化
	}

	// キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();	// 終了処理
		delete m_pKeyboard;		// メモリを解放
		m_pKeyboard = nullptr;	// ポインタを初期化
	}

	// ライトの破棄
	if (m_pLight != nullptr)
	{
		delete m_pLight;		// メモリを解放
		m_pLight = nullptr;		// ポインタを初期化
	}

	// カメラの破棄
	if (m_pCamera != nullptr)
	{
		delete m_pCamera;		// メモリを解放
		m_pCamera = nullptr;	// ポインタを初期化
	}

	// モーションマネージャーの破棄
	CMotion_Manager::Release();

	// Xモデルマネージャーの破棄
	CX_Manager::Release();

	// テクスチャマネージャーの破棄
	CTexture_Manager::Release();

	// レンダラーの破棄
	CRenderer::Release();
}