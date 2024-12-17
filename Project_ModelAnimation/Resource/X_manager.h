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
		STATUE = 0,	// grid
		WAIST,		// 腰
		BODY,		// 胴
		HEAD,		// 頭
		LEGL,		// 左脚
		FOOTL,		// 左足
		LEGR,		// 右脚
		FOOTR,		// 右足
		ARML,		// 左腕
		HANDL,		// 左手
		ARMR,		// 右腕
		HANDR,		// 右手
		TREE,
		STONE,
		BUTBODY,
		BUTLWING,
		BUTRWING,
		AROUND,		// 周辺
		DOWN0,		// 下0
		DOWN1,		// 下1
		DOWN2,		// 下2
		DOWN3,		// 下3
		UP0,		// 上0
		UP1,		// 上1
		UP2,		// 上2
		UP3,		// 上3
		UP5,		// 上4
		PUMPKIN,	// パンプキン
		SIDE0,
		SIDE1,
		FLOOR,
		GHOST,		// ゴースト
		MONUP,		// モンスター上
		MONDOWN,	// モンスター下
		GRID,
		FLYBODY,	// フライヤー体
		FLYLEFT,	// フライヤー左羽
		FLYRIGHT,	// フライヤー右羽
		SUMCIRCLE,	// 魔法陣
		SUM0,		// ルーン0
		SUM1,		// ルーン1
		SUM2,		// ルーン2
		SUM3,		// ルーン3
		SUM4,		// ルーン4
		SUM5,		// ルーン5
		SUM6,		// ルーン6
		SUM7,		// ルーン7
		SUM8,		// ルーン8
		SUM9,		// ルーン9
		SUM10,		// ルーン10
		SUM11,		// ルーン11
		BRIGHT_0,	// 閃光0
		BRIGHT_1,	// 閃光1
		MAX
	};

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