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

// 基礎パラメータの展開
JSON CConstellation::m_InitParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\CONSTELLATION\\constellation_param.json");

// 生成カウント
int CConstellation::m_nCntGenetrate = 0;

// 生成スパン
const int CConstellation::m_nGenerateSpan = static_cast<int>(CConstellation::m_InitParam["GenerateSpan"]);

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
	// 生成カウントをインクリメント
	++m_nCntGenetrate;

	// 規定の生成スパンに到達で
	if (m_nCntGenetrate > m_nGenerateSpan)
	{
		// 生成カウントをリセット
		m_nCntGenetrate = 0;

		// 拡散オフセットをコピー
		float fSpreadOffset = static_cast<float>(m_InitParam["Spreadoffset"]);

		// いくつか生成
		for (WORD wCntCreate = 0; wCntCreate < 3; ++wCntCreate)
		{
			// 渡された座標をランダムにずらす
			Pos += {
				utility::GetRandomValue<float>() * fSpreadOffset,
				utility::GetRandomValue<float>() * fSpreadOffset,
				utility::GetRandomValue<float>() * fSpreadOffset
			};

			// 星座の生成
			Create(Pos);
		}
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 生成
//============================================================================
void CConstellation::Create(D3DXVECTOR3 Pos)
{
	CConstellation* pNewInstance = DBG_NEW CConstellation();

	// 生成失敗
	if (!pNewInstance)
	{
		assert(false && "星座の生成に失敗");
	}

	// 星座の初期設定
	pNewInstance->Init();

	{ // パラメータ設定

		// データをキャスト
		float
			CorrectCoef = m_InitParam["CorrectCoef"];
		Vec3
			SizeTarget = utility::JsonConvertToVec3(m_InitParam["SizeTarget"]);
		XCol
			ColTarget = utility::JsonConvertToXCol(m_InitParam["ColTarget"]);
		int
			nMaxDuration = static_cast<int>(m_InitParam["MaxDuration"]);

		// データをセット
		pNewInstance->CObject_Effect::SetCorrectionCoef(CorrectCoef);	// 補間強度
		pNewInstance->CObject_Effect::SetSizeTarget(SizeTarget);		// 目標サイズ
		pNewInstance->CObject_Effect::SetPos(Pos);						// 座標
		pNewInstance->CObject_Effect::SetPosTarget(Pos);				// 目標座標
		pNewInstance->CObject_Effect::SetColTarget(ColTarget);			// 目標色
		pNewInstance->CObject_Effect::SetMaxDuration(nMaxDuration);		// 最大期間

		// テクスチャを設定
		CTexture_Manager::TYPE Type = CTexture_Manager::TYPE::CONSTELLATION0;

		// ランダムな星座テクスチャに分岐
		pNewInstance->BindTex(Type + rand() % 4);
	}
}