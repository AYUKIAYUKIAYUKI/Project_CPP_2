//============================================================================
// 
// Xモデルマネージャー [model_X_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "X_manager.h"
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバの初期化
//****************************************************
CX_Manager* CX_Manager::m_pXModelManager = nullptr;	// Xモデルマネージャーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 生成
//============================================================================
HRESULT CX_Manager::Create()
{
	// 既に生成されていたら
	if (m_pXModelManager != nullptr)
	{ 
#ifdef _DEBUG
		CRenderer::SetTimeString("Xモデルマネージャーは既に生成されています", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// インスタンスを生成
	m_pXModelManager = DBG_NEW CX_Manager();

	// 生成失敗
	if (m_pXModelManager == nullptr)
	{
		return E_FAIL;
	}

	// Xモデルマネージャーの初期設定
	if (FAILED(m_pXModelManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CX_Manager::Release()
{
	if (m_pXModelManager != nullptr)
	{
		// Xモデルマネージャーの終了処理
		m_pXModelManager->Uninit();

		// メモリの解放
		delete m_pXModelManager;

		// ポインタの初期化
		m_pXModelManager = nullptr;
	}
}

//============================================================================
// モデルを取得
//============================================================================
CX_Manager::MODEL* CX_Manager::GetModel(TYPE Type)
{
	// モデル取得不能
	if (m_apModelTemp[static_cast<WORD>(Type)].pMesh == nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("【警告】モデル取得時にエラー発生", 300);
#endif	// _DEBUG

		return nullptr;
	}

	return &m_apModelTemp[static_cast<WORD>(Type)];
}

//============================================================================
// Xモデルマネージャーを取得
//============================================================================
CX_Manager* CX_Manager::GetInstance()
{
	// 本体が存在しなければ
	if (m_pXModelManager == nullptr)
	{
		// 生成
		if (FAILED(Create()))
		{
			assert(false && "Xモデルマネージャーの取得に失敗");
		}
	}

	return m_pXModelManager;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CX_Manager::CX_Manager()
{
	for (WORD wCntModel = 0; wCntModel < static_cast<WORD>(TYPE::MAX); ++wCntModel)
	{
		// モデル情報の初期化
		m_apModelTemp[wCntModel].pMesh = nullptr;		// メッシュのポインタ
		m_apModelTemp[wCntModel].Size = { VEC3_INIT };	// サイズ
		m_apModelTemp[wCntModel].pBuffMat = nullptr;	// マテリアルのポインタ
		m_apModelTemp[wCntModel].dwNumMat = 0;			// マテリアル数
		m_apModelTemp[wCntModel].apColMat = nullptr;	// マテリアルの色のポインタ
		m_apModelTemp[wCntModel].apTex = nullptr;		// テクスチャのポインタ
	}
}

//============================================================================
// デストラクタ
//============================================================================
CX_Manager::~CX_Manager()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CX_Manager::Init()
{
	// モデルリストを取得
	JSON Json = utility::OpenJsonFile("Data\\JSON\\model_list.json");

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	for (WORD wCntModel = 0; wCntModel < static_cast<WORD>(TYPE::MAX); ++wCntModel)
	{
		// モデルファイルのパスを作成する
		const std::string& ModelFilePath = Json["ModelList"][wCntModel];

		// モデルファイルの取得
		HRESULT hr = D3DXLoadMeshFromX(ModelFilePath.c_str(),
			D3DXMESH_SYSTEMMEM,
			pDev,
			nullptr,
			&m_apModelTemp[wCntModel].pBuffMat,
			nullptr,
			&m_apModelTemp[wCntModel].dwNumMat,
			&m_apModelTemp[wCntModel].pMesh);

		if (FAILED(hr))
		{ // 取得失敗
#ifdef _DEBUG
			CRenderer::SetTimeString("【警告】モデル[" + ModelFilePath + "]は読み込みに失敗しました", 300);
#endif	// _DEBUG

			continue;
		}

		// モデルのサイズを取得する
		m_apModelTemp[wCntModel].Size = LoadSize(ModelFilePath);

		// マテリアルデータへのポインタを取得
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_apModelTemp[wCntModel].pBuffMat->GetBufferPointer();

		// マテリアル数分、マテリアル色情報のポインタ配列を確保
		m_apModelTemp[wCntModel].apColMat = DBG_NEW D3DXCOLOR[m_apModelTemp[wCntModel].dwNumMat];

		// マテリアル数分、テクスチャ情報のポインタ配列を確保
		m_apModelTemp[wCntModel].apTex = DBG_NEW LPDIRECT3DTEXTURE9[m_apModelTemp[wCntModel].dwNumMat];

		// マテリアルごとにテクスチャの有無を確認
		for (WORD wCntMat = 0; wCntMat < m_apModelTemp[wCntModel].dwNumMat; ++wCntMat)
		{
			// マテリアルの色をモデル情報内に保存しておく
			m_apModelTemp[wCntModel].apColMat[wCntMat] = pMat[wCntMat].MatD3D.Diffuse;

			// テクスチャ名を取得失敗
			if (pMat[wCntMat].pTextureFilename == nullptr)
			{
				// テクスチャのポインタを初期化しておく
				m_apModelTemp[wCntModel].apTex[wCntMat] = nullptr;
			
				continue;
			}

			// テクスチャ名を読み取れたら生成
			hr = D3DXCreateTextureFromFileA(pDev,
				pMat[wCntMat].pTextureFilename,
				&m_apModelTemp[wCntModel].apTex[wCntMat]);

			if (FAILED(hr))
			{ // 生成失敗
#ifdef _DEBUG
				CRenderer::SetTimeString("【警告】モデル[" + ModelFilePath + "]におけるテクスチャの[" + pMat[wCntMat].pTextureFilename + "]は生成に失敗しました", 600);
#endif	// _DEBUG

				// テクスチャのポインタを初期化しておく
				m_apModelTemp[wCntModel].apTex[wCntMat] = nullptr;
			}
		}
	}

	return S_OK;
}

//============================================================================
// サイズ読み込み
//============================================================================
D3DXVECTOR3 CX_Manager::LoadSize(std::string filename)
{
	// 該当モデルファイルを展開
	std::ifstream ModelFile(filename);

	// 展開に失敗
	if (!ModelFile.good())
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("【警告】モデル[" + filename + "]はサイズ読み込みに失敗しました", 600);
#endif	// _DEBUG

		return VEC3_INIT;
	}

	// 汎用文字列格納用
	std::string Str;

	// テキストから頂点情報の行を検索
	while (std::getline(ModelFile, Str))
	{
		// 頂点情報を発見したので検索終了
		if (Str == "Mesh {")
		{
			// 不要な行を一度読み込む
			std::getline(ModelFile, Str);

			break;
		}
	}

	// 比較処理用に数値を入れておく
	Vec3 ResultMin = { FLT_MAX, FLT_MAX, FLT_MAX };
	Vec3 ResultMax = { FLT_MIN, FLT_MIN, FLT_MIN };

	// 取得予定の軸方向数
	const WORD wAxis = 3;

	// 数値を比較していく
	while (std::getline(ModelFile, Str))
	{
		// 終了条件
		if (Str == " ")
		{
			break;
		}

		// 数値格納用
		float fNum[wAxis] = { 0.0f, 0.0f, 0.0f };

		// 読みとった数値を一旦格納する
		for (WORD wCntAxis = 0; wCntAxis < wAxis; ++wCntAxis)
		{
			fNum[wCntAxis] = std::stof(Str);
			Str = Str.substr(Str.find(";") + 1, Str.back());
		}

		// 各軸の最大・最小地点を更新
		if (ResultMin.x > fNum[0])
		{
			ResultMin.x = fNum[0];
		}

		if (ResultMin.y > fNum[1])
		{
			ResultMin.y = fNum[1];
		}

		if (ResultMin.z > fNum[2])
		{
			ResultMin.z = fNum[2];
		}

		if (ResultMax.x < fNum[0])
		{
			ResultMax.x = fNum[0];
		}

		if (ResultMax.y < fNum[1])
		{
			ResultMax.y = fNum[1];
		}

		if (ResultMax.z < fNum[2])
		{
			ResultMax.z = fNum[2];
		}
	}

	// ファイルを閉じる
	ModelFile.close();

	return ResultMax;
}

//============================================================================
// 終了処理
//============================================================================
void CX_Manager::Uninit()
{
	for (WORD wCntModel = 0; wCntModel < static_cast<WORD>(TYPE::MAX); ++wCntModel)
	{
		// マテリアル用のテクスチャを破棄
		for (WORD wCntMat = 0; wCntMat < m_apModelTemp[wCntModel].dwNumMat; ++wCntMat)
		{
			if (m_apModelTemp[wCntModel].apTex[wCntMat] != nullptr)
			{
				m_apModelTemp[wCntModel].apTex[wCntMat]->Release();
				m_apModelTemp[wCntModel].apTex[wCntMat] = nullptr;
			}
		}

		// テクスチャポインタ配列の破棄
		if (m_apModelTemp[wCntModel].apTex != nullptr)
		{
			delete[] m_apModelTemp[wCntModel].apTex;
			m_apModelTemp[wCntModel].apTex = nullptr;
		}

		// マテリアル色ポインタ配列の破棄
		if (m_apModelTemp[wCntModel].apColMat != nullptr)
		{
			delete[] m_apModelTemp[wCntModel].apColMat;
			m_apModelTemp[wCntModel].apColMat = nullptr;
		}

		// マテリアルバッファの破棄
		if (m_apModelTemp[wCntModel].pBuffMat != nullptr)
		{
			m_apModelTemp[wCntModel].pBuffMat->Release();
			m_apModelTemp[wCntModel].pBuffMat = nullptr;
		}

		// メッシュバッファの破棄
		if (m_apModelTemp[wCntModel].pMesh != nullptr)
		{
			m_apModelTemp[wCntModel].pMesh->Release();
			m_apModelTemp[wCntModel].pMesh = nullptr;
		}
	}
}