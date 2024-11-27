//============================================================================
// 
// 星座 [constellation.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "constellation.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
WORD CConstellation::m_nCntGenerateSpan = 0;	// 生成スパンのカウント

// 基礎パラメータの展開
JSON CConstellation::n_InitParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\constellation.json");

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CConstellation::CConstellation() :
	CObject_Effect{},
	m_InitPos{ VEC3_INIT }
{

}

//============================================================================
// デストラクタ
//============================================================================
CConstellation::~CConstellation()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CConstellation::Init()
{
	// エフェクトオブジェクトの初期設定
	if (FAILED(CObject_Effect::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CConstellation::Uninit()
{
	// エフェクトオブジェクトの終了処理
	CObject_Effect::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CConstellation::Update()
{
	// 寿命の半分に到達したら消滅の準備に入る
	if (GetDuration() == GetMaxDuration() * 0.5f)
	{
		// 目標サイズを縮小
		SetSizeTarget(VEC3_INIT);

		// カラーを薄黒く
		SetColTarget(XCol(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// エフェクトオブジェクトの更新処理
	CObject_Effect::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CConstellation::Draw()
{
	// エフェクトオブジェクトの描画処理
	CObject_Effect::Draw();
}

//============================================================================
// 拡散発生
//============================================================================
void CConstellation::GenerateSpread(D3DXVECTOR3 Pos)
{
	// 生成スパンをカウントアップ
	m_nCntGenerateSpan++;

	// 設定された生成スパンに到達で
	if (m_nCntGenerateSpan > SPREAD_SPAN)
	{
		// 生成スパンのカウントをリセット
		m_nCntGenerateSpan = 0;

		// オフセットをコピー
		auto Offset = n_InitParam["Offset"];

		// いくつか生成
		for (WORD wCnt = 0; wCnt < 3; ++wCnt)
		{
			// 渡された座標をランダムにずらす
			Pos += {
				utility::GetRandomValue<float>() * static_cast<float>(Offset),
				utility::GetRandomValue<float>() * static_cast<float>(Offset),
				utility::GetRandomValue<float>() * static_cast<float>(Offset)
			};

			// 星座の生成
			Create(Pos);
		}
	}
}

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 生成
//============================================================================
void CConstellation::Create(D3DXVECTOR3 Pos)
{
	CConstellation* pNew = DBG_NEW CConstellation();

	// 生成失敗
	if (!pNew)
	{
		assert(false && "星座の生成に失敗");
	}

	// 星座の初期設定
	pNew->Init();

	// 基礎パラメータをコピー
	auto const& CorrectionCoef = n_InitParam["CorrectionCoef"];
	auto const& SizeTarget = n_InitParam["SizeTarget"];
	auto const& ColTarget = n_InitParam["ColTarget"];
	auto const& MaxDuration = n_InitParam["MaxDuration"];

	// 各種パラメータ設定
	CTexture_Manager::TYPE Type = CTexture_Manager::TYPE::CONSTELLATION0;
	pNew->BindTex(Type + rand() % 4);													// テクスチャ
	pNew->SetCorrectionCoef(static_cast<float>(CorrectionCoef));						// 補正係数
	pNew->SetSizeTarget(Vec3(SizeTarget[0], SizeTarget[1], SizeTarget[2]));				// 目標サイズ
	pNew->SetPos(Pos);																	// 座標
	pNew->SetPosTarget(Pos);															// 目標座標
	pNew->SetColTarget(XCol(ColTarget[0], ColTarget[1], ColTarget[2], ColTarget[3]));	// 目標色
	pNew->SetMaxDuration(static_cast<int>(MaxDuration));								// 最大期間
}