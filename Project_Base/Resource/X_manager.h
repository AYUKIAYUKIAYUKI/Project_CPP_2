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
		RENDER_SPHERE = 0,	// render sphere
		RENDER_CYLINDER,	// render cylinder
		RENDER_BOX,			// render box
		BUTBODY,			// 蝶胴
		BUTLWING,			// 蝶左羽
		BUTRWING,			// 蝶右羽
		PLAWAIST,			// 人腰
		PLABODY,			// 人胴
		PLAHEAD,			// 人頭
		PLALEGL,			// 人左脚
		PLAFOOTL,			// 人左足
		PLALEGR,			// 人右脚
		PLAFOOTR,			// 人右足
		PLAARML,			// 人左腕
		PLAHANDL,			// 人左手
		PLAARMR,			// 人右腕
		PLAHANDR,			// 人右手
		ONLYTREE,			// 樹のみ
		ONLYSTONE,			// 樹の石
		STAAROUND,			// 銅像周辺
		STADOWN0,			// 銅像下0
		STADOWN1,			// 銅像下1
		STADOWN2,			// 銅像下2
		STADOWN3,			// 銅像下3
		STAUP0,				// 銅像上0
		STAUP1,				// 銅像上1
		STAUP2,				// 銅像上2
		STAUP3,				// 銅像上3
		STAUP4,				// 銅像上4
		AROUND,				// 周辺領域
		PUMPKIN,			// パンプキン
		GHOST,				// ゴースト
		MONUP,				// モンスター上
		MONDOWN,			// モンスター下
		FLYBODY,			// フライヤー体
		FLYLEFT,			// フライヤー左羽
		FLYRIGHT,			// フライヤー右羽
		BLONORMAL,			// 通常ブロック
		BLOSIDE,			// 横長ブロック
		BLOTALL,			// 縦長ブロック
		LIFE,				// ライフ
		SUMCIRCLE,			// 魔法陣
		SUM0,				// ルーン0
		SUM1,				// ルーン1
		SUM2,				// ルーン2
		SUM3,				// ルーン3
		SUM4,				// ルーン4
		SUM5,				// ルーン5
		SUM6,				// ルーン6
		SUM7,				// ルーン7
		SUM8,				// ルーン8
		SUM9,				// ルーン9
		SUM10,				// ルーン10
		SUM11,				// ルーン11
		MAX
	};

	// +演算子のオーバーロード
	friend TYPE operator+(TYPE Type, WORD wNum)
	{
		return static_cast<TYPE>(static_cast<WORD>(Type) + wNum);
	}

	//****************************************************
	// モデル情報の構造体の定義
	//****************************************************
	struct MODEL
	{
		LPD3DXMESH			pMesh;		// メッシュ情報
		D3DXVECTOR3			Size;		// サイズ
		LPD3DXBUFFER		pBuffMat;	// マテリアル情報
		DWORD				dwNumMat;	// マテリアル数
		D3DXCOLOR*			apColMat;	// マテリアル色	
		LPDIRECT3DTEXTURE9*	apTex;		// テクスチャ
		TYPE				Type;		// タイプ
	};

	// <special function>
	CX_Manager(const CX_Manager&) = delete;				// コピーコンストラクタ
	CX_Manager& operator=(const CX_Manager&) = delete;	// コピー代入演算子
	CX_Manager(CX_Manager&&) = delete;					// ムーブコンストラクタ
	CX_Manager& operator=(CX_Manager&&) = delete;		// ムーブ代入演算子

	// <getter>
	MODEL* GetModel(TYPE Type);	// モデルを取得

	// <static function>
	static HRESULT	Create();	// 生成
	static void		Release();	// 解放

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