//============================================================================
// 
// Xモデルマネージャー、ヘッダファイル [model_X_manager.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _MODEL_X_MANAGER_H_
#define _MODEL_X_MANAGER_H_	// 二重インクルード防止

//****************************************************
// Xモデルマネージャークラス
//****************************************************
class CModel_X_Manager final
{
public:

	//****************************************************
	// モデル識別
	//****************************************************
	enum class TYPE
	{
		TEST = 0,	// テスト
		MAX,
	};

	//****************************************************
	// モデル情報の構造体の定義
	//****************************************************
	struct MODEL
	{
		D3DXVECTOR3 Size;			// サイズ
		LPD3DXMESH pMesh;			// メッシュ情報
		LPD3DXBUFFER pBuffMat;		// マテリアル情報
		DWORD dwNumMat;				// マテリアルの数
		LPDIRECT3DTEXTURE9* apTex;	// テクスチャ情報
	};

	HRESULT Load();	// モデル読込
	void Release();	// 解放

	MODEL* GetModel(TYPE Type);	// モデルを取得

	static CModel_X_Manager* GetInstance();	// Xモデルマネージャーを取得

private:

	CModel_X_Manager();		// デフォルトコンストラクタ
	~CModel_X_Manager();	// デストラクタ

	void Create();									// 生成
	D3DXVECTOR3 ImportSize(std::string Filename);	// サイズ読み込み
	void Unload();									// モデル破棄

	MODEL m_apModelTemp[static_cast<int>(TYPE::MAX)];	// Xモデル情報

	static CModel_X_Manager* m_pInstnce;	// Xモデルマネージャー
};

#endif // _MODEL_X_MANAGER_H_