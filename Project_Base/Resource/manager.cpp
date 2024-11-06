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

// シングルトン管理用
#include "fade.h"
#include "renderer.h"
#include "sound.h"

#include "mask_rectangle.h"

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
	// 四角形マスクの更新処理
	m_pMask_Rectangle->Update();

	// レンダラーの更新
	CRenderer::GetInstance()->Update();

	// シーンの更新
	m_pScene->Update();

	// ライトの更新
	m_pLight->Update();

	// カメラの更新
	m_pCamera->Update();

	// キーボードの更新
	m_pKeyboard->Update();

	// パッドの更新
	m_pPad->Update();
}

//============================================================================
// 描画処理
//============================================================================
void CManager::Draw()
{
	// レンダラーの描画
	CRenderer::GetInstance()->Draw();
}

//============================================================================
// 四角形マスクを取得
//============================================================================
CMask_Rectangle* CManager::GetMask_Rectangle() const
{
	return m_pMask_Rectangle;
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
// シーンを取得
//============================================================================
CScene* CManager::GetScene() const
{
	return m_pScene;
}

//============================================================================
// シーンの設定
//============================================================================
void CManager::SetScene(CScene::MODE Mode)
{
	// 現在のシーンを破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	// 新たなシーンを設定
	m_pScene = CScene::Create(Mode);

	// 生成失敗
	if (!m_pScene)
	{
		assert(false);
	}

	// 初期設定
	m_pScene->Init();
}

//============================================================================
// 生成
//============================================================================
HRESULT CManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	// マネージャーの生成
	if (m_pManager == nullptr)
	{
		m_pManager = DBG_NEW CManager();
	}

	// 生成失敗
	if (m_pManager == nullptr)
	{
		return E_FAIL;
	}

	// 初期設定
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
	if (m_pManager != nullptr)
	{
		m_pManager->Uninit();	// 終了処理
		delete m_pManager;		// メモリを解放
		m_pManager = nullptr;	// ポインタを初期化
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
// デフォルトコンストラクタ
//============================================================================
CManager::CManager() :
	m_pKeyboard{ nullptr },
	m_pPad{ nullptr },
	m_pMask_Rectangle{ nullptr },
	m_pCamera{ nullptr },
	m_pLight{ nullptr },
	m_pScene{ nullptr }
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
	if (FAILED(CRenderer::GetInstance()->Init(hWnd, TRUE)))
	{
		return E_FAIL;
	}

	// 四角形マスクの生成
	m_pMask_Rectangle = CMask_Rectangle::Create(CTexture_Manager::TYPE::MAP);

	// フェードの初期設定
	if (FAILED(CFade::GetInstance()->Init()))
	{
		return E_FAIL;
	}

	// サウンド初期設定
	if (FAILED(CSound::GetInstance()->Init(hWnd)))
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

	// パッドの生成
	m_pPad = DBG_NEW CInputPad();

	if (m_pPad == nullptr)
	{ // 生成失敗
		return E_FAIL;
	}

	// パッドの初期化
	m_pPad->Init(hInstance, hWnd);

	// 最初のシーン設定
	SetScene(CScene::MODE::TITLE);

	// BGMをかける
	CSound::GetInstance()->Play(CSound::LABEL::TEST);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CManager::Uninit()
{
	// シーンの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();	// 終了処理
		delete m_pScene;	// メモリを解放
		m_pScene = nullptr;	// ポインタを初期化
	}

	// パッドの破棄
	if (m_pPad != nullptr)
	{
		m_pPad->Uninit();	// 終了処理
		delete m_pPad;		// メモリを解放
		m_pPad = nullptr;	// ポインタを初期化
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

	// サウンド破棄
	CSound::GetInstance()->Release();

	// フェードの破棄
	CFade::GetInstance()->Release();

	// 四角形マスクの破棄
	if (m_pMask_Rectangle != nullptr)
	{
		m_pMask_Rectangle->Uninit();	// 終了処理
		delete m_pMask_Rectangle;		// メモリを解放
		m_pMask_Rectangle = nullptr;	// ポインタを初期化
	}

	// レンダラーの破棄
	CRenderer::GetInstance()->Release();
}