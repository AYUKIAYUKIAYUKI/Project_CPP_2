//============================================================================
// 
// Xモデルマネージャー、ヘッダファイル [X_manager.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _X_MANAGER_H_
#define _X_MANAGER_H_	// 二重インクルード防止

//****************************************************
// Xモデルマネージャークラス
//****************************************************
class CX_Manager final
{
public:

	//****************************************************
	// モデル識別
	//****************************************************
	enum class TYPE : WORD
	{
		TEST = 0,	// TEST
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

	// <special function>
	CX_Manager(const CX_Manager&) = delete;				// コピーコンストラクタ
	CX_Manager& operator=(const CX_Manager&) = delete;	// コピー代入演算子
	CX_Manager(CX_Manager&&) = delete;					// ムーブコンストラクタ
	CX_Manager& operator=(CX_Manager&&) = delete;		// ムーブ代入演算子

	// <getter>
	MODEL* GetModel(TYPE Type);	// モデルを取得

	// <static function>
	static HRESULT Create();	// 生成
	static void	Release();		// 解放

	// <static getter>
	static CX_Manager* GetInstance();	// Xモデルマネージャーを取得

private:

	// <special function>
	CX_Manager();	// コンストラクタ
	~CX_Manager();	// デストラクタ

	// <function>
	HRESULT		Init();							// 初期設定
	D3DXVECTOR3 LoadSize(std::string FileName);	// サイズ読み込み
	void		Uninit();						// 終了処理

	// <data>
	MODEL m_apModelTemp[static_cast<int>(TYPE::MAX)];	// Xモデル情報

	// <static data>
	static CX_Manager* m_pXModelManager;	// Xモデルマネージャーの本体
};

#endif // _X_MANAGER_H_