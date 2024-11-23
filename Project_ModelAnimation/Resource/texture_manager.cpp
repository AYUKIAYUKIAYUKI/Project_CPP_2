//============================================================================
// 
// テクスチャマネージャー [texture_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "texture_manager.h"
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CTexture_Manager* CTexture_Manager::m_pTextureManager = nullptr;	// テクスチャマネージャーの本体

//============================================================================
//
// publicメンバ
//
//============================================================================

//============================================================================
// 生成
//============================================================================
HRESULT CTexture_Manager::Create()
{
	// 既に生成されていたら
	if (m_pTextureManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("テクスチャマネージャーは既に生成されています", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// インスタンスを生成
	m_pTextureManager = DBG_NEW CTexture_Manager();

	// 生成失敗
	if (m_pTextureManager == nullptr)
	{
		return E_FAIL;
	}

	// テクスチャマネージャーの初期設定
	if (FAILED(m_pTextureManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CTexture_Manager::Release()
{
	if (m_pTextureManager != nullptr)
	{
		// テクスチャマネージャーの終了処理
		m_pTextureManager->Uninit();

		// メモリの解放
		delete m_pTextureManager;

		// ポインタの初期化
		m_pTextureManager = nullptr;
	}
}

//============================================================================
// テクスチャを取得
//============================================================================
LPDIRECT3DTEXTURE9 CTexture_Manager::GetTexture(TYPE Type)
{
	// モデル取得不能
	if (m_apTexture[static_cast<WORD>(Type)] == nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("【警告】テクスチャ取得時にエラー発生", 300);
#endif	// _DEBUG

		return nullptr;
	}

	return m_apTexture[static_cast<WORD>(Type)];
}

//============================================================================
// テクスチャマネージャーを取得
//============================================================================
CTexture_Manager* CTexture_Manager::GetInstance()
{
	// 本体が存在しなければ
	if (m_pTextureManager == nullptr)
	{
		// 生成
		if (FAILED(Create()))
		{
			assert(false && "テクスチャマネージャーの取得に失敗");
		}
	}

	return m_pTextureManager;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CTexture_Manager::CTexture_Manager()
{
	for (WORD wCntTex = 0; wCntTex < static_cast<int>(TYPE::MAX); ++wCntTex)
	{
		m_apTexture[wCntTex] = nullptr;	// テクスチャ情報の初期化
	}
}

//============================================================================
// デストラクタ
//============================================================================
CTexture_Manager::~CTexture_Manager()
{

}

//============================================================================
// 終了処理
//============================================================================
void CTexture_Manager::Uninit()
{
	for (WORD wCntTex = 0; wCntTex < static_cast<int>(TYPE::MAX); ++wCntTex)
	{
		// テクスチャの破棄
		if (m_apTexture[wCntTex] != nullptr)
		{
			m_apTexture[wCntTex]->Release();
			m_apTexture[wCntTex] = nullptr;
		}
	}
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CTexture_Manager::Init()
{
	// テクスチャリストを取得
	JSON Json = utility::OpenJsonFile("Data\\JSON\\texture_list.json");

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();

	for (WORD wCntTex = 0; wCntTex < static_cast<int>(TYPE::MAX); ++wCntTex)
	{
		// テクスチャファイルのパスを作成する
		const std::string& TextureFilePath = Json["ModelList"][wCntTex];

		// テクスチャの生成
		HRESULT hr = D3DXCreateTextureFromFileA(pDev,
			TextureFilePath.c_str(),
			&m_apTexture[wCntTex]);

		if (FAILED(hr))
		{ // テクスチャ生成失敗
#ifdef _DEBUG
			CRenderer::SetTimeString("【警告】テクスチャ[" + TextureFilePath + "]は生成に失敗しました", 300);
#endif	// _DEBUG
		}
	}

	return S_OK;
}