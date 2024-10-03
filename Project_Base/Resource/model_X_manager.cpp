//============================================================================
// 
// Xモデルマネージャー [model_X_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "model_X_manager.h"

// デバイス取得用
#include "renderer.h"

//****************************************************
// 静的メンバの初期化
//****************************************************
CModel_X_Manager* CModel_X_Manager::m_pInstnce;	// Xモデルマネージャー

//============================================================================
// モデル読み込み
//============================================================================
HRESULT CModel_X_Manager::Load()
{
	// モデルリストを展開
	std::ifstream ModelList{ "Data\\TXT\\model_path.txt" };

	if (!ModelList)
	{ // 展開に失敗
		assert(false);
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev{ CRenderer::GetInstance()->GetDeviece() };

	for (int nCntModel = 0; nCntModel < static_cast<int>(TYPE::MAX); nCntModel++)
	{
		// モデル名格納先
		std::string ModelName;

		// モデル名を取得する
		std::getline(ModelList, ModelName);

		// パスを作成する
		ModelName.insert(0, "Data\\MODEL\\");

		// モデルファイルの取得
		HRESULT hr = D3DXLoadMeshFromX(ModelName.c_str(),
			D3DXMESH_SYSTEMMEM,
			pDev,
			nullptr,
			&m_apModelTemp[nCntModel].pBuffMat,
			nullptr,
			&m_apModelTemp[nCntModel].dwNumMat,
			&m_apModelTemp[nCntModel].pMesh);

		if (FAILED(hr))
		{ // 取得失敗
#ifdef _DEBUG
			ModelName = ModelName.substr(ModelName.find_last_of("\\") + 1, ModelName.back());
			CRenderer::GetInstance()->SetTimeString("【警告】モデル[" + ModelName + "]は読み込みに失敗しました", 300);
#endif	// _DEBUG

			continue;
		}

		// モデルのサイズを取得する
		m_apModelTemp[nCntModel].Size = ImportSize(ModelName);

		// マテリアルデータへのポインタを取得
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_apModelTemp[nCntModel].pBuffMat->GetBufferPointer();

		// マテリアルの数分のテクスチャポインタを確保
		m_apModelTemp[nCntModel].apTex = DBG_NEW LPDIRECT3DTEXTURE9 [static_cast<int>(m_apModelTemp[nCntModel].dwNumMat)];

		// マテリアル分テクスチャの有無を確認
		for (int nCntMat = 0; nCntMat < static_cast<int>(m_apModelTemp[nCntModel].dwNumMat); nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != nullptr)
			{
				// テクスチャを読み取れたら生成
				hr = D3DXCreateTextureFromFileA(pDev,
					pMat[nCntMat].pTextureFilename,
					&m_apModelTemp[nCntModel].apTex[nCntMat]);

				if (FAILED(hr))
				{ // 生成失敗

					// テクスチャ名をコピー
					std::string TextureName{ pMat[nCntMat].pTextureFilename };
#ifdef _DEBUG
					ModelName = ModelName.substr(ModelName.find_last_of("\\") + 1, ModelName.back());
					CRenderer::GetInstance()->SetTimeString("【警告】モデル[" + ModelName + "]におけるテクスチャパスの[" + TextureName + "]は生成に失敗しました", 600);
#endif	// _DEBUG

					m_apModelTemp[nCntModel].apTex[nCntMat] = nullptr;
				}
			}
			else
			{
				// 読み取れなければ初期化
				m_apModelTemp[nCntModel].apTex[nCntMat] = nullptr;
			}
		}
	}

	// ファイルを閉じる
	ModelList.close();

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CModel_X_Manager::Release()
{
	if (m_pInstnce != nullptr)
	{
		// Xモデルの破棄
		m_pInstnce->Unload();

		// メモリの解放
		delete m_pInstnce;

		// ポインタの初期化
		m_pInstnce = nullptr;
	}
}

//============================================================================
// モデルを取得
//============================================================================
CModel_X_Manager::MODEL* CModel_X_Manager::GetModel(TYPE Type)
{
	if (m_apModelTemp[static_cast<int>(Type)].pMesh == nullptr)
	{ // モデル取得不能
#ifdef _DEBUG
		CRenderer::GetInstance()->SetTimeString("【警告】モデル取得エラー", 600);
#endif	// _DEBUG
	}

	return &m_apModelTemp[static_cast<int>(Type)];
}

//============================================================================
// Xモデルマネージャーを取得
//============================================================================
CModel_X_Manager* CModel_X_Manager::GetInstance()
{
	if (m_pInstnce == nullptr)
	{
		// 生成
		m_pInstnce->Create();
	}

	return m_pInstnce;
}

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CModel_X_Manager::CModel_X_Manager()
{
	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		// モデル情報の初期化
		m_apModelTemp[i].Size = { 0.0f, 0.0f, 0.0f };	// サイズ
		m_apModelTemp[i].pMesh = nullptr;				// メッシュのポインタ
		m_apModelTemp[i].pBuffMat = nullptr;			// マテリアルバッファのポインタ
		m_apModelTemp[i].dwNumMat = 0;					// マテリアル数
		m_apModelTemp[i].apTex = nullptr;				// テクスチャのポインタ
	}
}

//============================================================================
// デストラクタ
//============================================================================
CModel_X_Manager::~CModel_X_Manager()
{

}

//============================================================================
// 生成
//============================================================================
void CModel_X_Manager::Create()
{
	if (m_pInstnce != nullptr)
	{ // 二重生成禁止
		assert(false);
	}

	// インスタンスを生成
	m_pInstnce = DBG_NEW CModel_X_Manager{};
}

//============================================================================
// サイズ読み込み
//============================================================================
D3DXVECTOR3 CModel_X_Manager::ImportSize(std::string filename)
{
	// 比較処理用に数値を入れておく
	D3DXVECTOR3 sizeMin{ FLT_MAX, FLT_MAX, FLT_MAX };
	D3DXVECTOR3 sizeMax{ FLT_MIN, FLT_MIN, FLT_MIN };

	// モデルファイルそのものを展開
	std::ifstream file{ filename };

	if (!file)
	{ // 展開に失敗
#ifdef _DEBUG
		CRenderer::GetInstance()->SetTimeString("【警告】モデル[" + filename + "]はサイズ読み込みに失敗しました", 600);
#endif	// _DEBUG

		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f, };
	}

	// 文字列格納用
	std::string str{};

	// 頂点情報の箇所まで行を読み込む
	while (std::getline(file, str))
	{
		if (str == "Mesh {")
		{
			// 不要な行を一度読み込む
			std::getline(file, str);

			break;
		}
	}

	// 配列の要素数
	const int nNumArray = 3;

	// 数値を比較していく
	while (std::getline(file, str))
	{
		// 終了条件
		if (str == " ")
		{
			break;
		}

		// 数値用
		float fNum[nNumArray]{};

		// 読みとった数値を一旦格納する
		for (int i = 0; i < nNumArray; ++i)
		{
			fNum[i] = std::stof(str);
			str = str.substr(str.find(";") + 1, str.back());
		}

		// 各軸の最大・最小地点を更新
		if (sizeMin.x > fNum[0])
		{
			sizeMin.x = fNum[0];
		}

		if (sizeMin.y > fNum[1])
		{
			sizeMin.y = fNum[1];
		}

		if (sizeMin.z > fNum[2])
		{
			sizeMin.z = fNum[2];
		}

		if (sizeMax.x < fNum[0])
		{
			sizeMax.x = fNum[0];
		}

		if (sizeMax.y < fNum[1])
		{
			sizeMax.y = fNum[1];
		}

		if (sizeMax.z < fNum[2])
		{
			sizeMax.z = fNum[2];
		}
	}

	// ファイルを閉じる
	file.close();

	return sizeMax;
}

//============================================================================
// モデル破棄
//============================================================================
void CModel_X_Manager::Unload()
{
	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		// テクスチャポインタの破棄
		if (m_apModelTemp[i].apTex != nullptr)
		{
			// テクスチャの破棄
			for (int nCntMat = 0; nCntMat < static_cast<int>(m_apModelTemp[i].dwNumMat); nCntMat++)
			{
				if (m_apModelTemp[i].apTex[nCntMat] != nullptr)
				{
					m_apModelTemp[i].apTex[nCntMat]->Release();
					m_apModelTemp[i].apTex[nCntMat] = nullptr;
				}
			}

			delete[] m_apModelTemp[i].apTex;
			m_apModelTemp[i].apTex = nullptr;
		}

		// メッシュの破棄
		if (m_apModelTemp[i].pMesh != nullptr)
		{
			m_apModelTemp[i].pMesh->Release();
			m_apModelTemp[i].pMesh = nullptr;
		}

		// マテリアルの破棄
		if (m_apModelTemp[i].pBuffMat != nullptr)
		{
			m_apModelTemp[i].pBuffMat->Release();
			m_apModelTemp[i].pBuffMat = nullptr;
		}
	}
}