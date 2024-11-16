//============================================================================
// 
// 火の粉 [sparks.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "sparks.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
WORD CSparks::m_nCntGenerateSpan = 0;	// 生成スパンのカウント

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CSparks::CSparks() :
	CObject_Effect{},
	m_InitPos{ VEC3_INIT },
	m_fWaveCoef{ 0.0f },
	m_fAdderY{ 0.0f }
{

}

//============================================================================
// デストラクタ
//============================================================================
CSparks::~CSparks()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CSparks::Init()
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
void CSparks::Uninit()
{
	// エフェクトオブジェクトの終了処理
	CObject_Effect::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CSparks::Update()
{
	// 寿命の半分に到達したら消滅の準備に入る
	if (GetDuration() == GetMaxDuration() * 0.5f)
	{
		// 目標サイズを縮小
		SetSizeTarget(VEC3_INIT);

		// カラーを薄黒く
		SetColTarget(XCol(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// 現在地を基準に、動作用の目標数値を設定していく
	Vec3 NewPosTarget = GetPos();
	NewPosTarget += { cosf(NewPosTarget.y) * m_fWaveCoef, m_fAdderY, cosf(NewPosTarget.y) * m_fWaveCoef};
	SetPosTarget(NewPosTarget);

	// エフェクトオブジェクトの更新処理
	CObject_Effect::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CSparks::Draw()
{
	// エフェクトオブジェクトの描画処理
	CObject_Effect::Draw();
}

//============================================================================
// 自動生成
//============================================================================
void CSparks::AutoGenerate()
{
	// 生成スパンをカウントアップ
	m_nCntGenerateSpan++;

	// 設定された生成スパンに到達で
	if (m_nCntGenerateSpan > NUM_GENERATE_SPAN)
	{
		// 生成スパンのカウントをリセット
		m_nCntGenerateSpan = 0;

		// 生成
		Create();
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
void CSparks::Create()
{
	CSparks* pNewInstance = DBG_NEW CSparks();

	// 生成失敗
	if (!pNewInstance)
	{
		assert(false && "火の粉の生成に失敗");
	}

	// 火の粉の初期設定
	pNewInstance->Init();

	// 基礎パラメータを取得
	JSON Json = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\sparks.json");

	// パラメータをコピー
	auto const& TextureType = Json["TextureType"];
	auto const& CorrectionCoef = Json["CorrectionCoef"];
	auto const& SizeTarget = Json["SizeTarget"];
	auto const& ColTarget = Json["ColTarget"];
	auto const& MaxDuration = Json["MaxDuration"];
	auto const& WaveCoef = Json["WaveCoef"];
	auto const& AdderY = Json["AdderY"];

	// ランダムな座標を作成
	pNewInstance->m_InitPos = {
		utility::GetRandomValue<float>() * 1.5f,
		fabsf(utility::GetRandomValue<float>()),
		utility::GetRandomValue<float>() * 1.5f
	};

	// 各種パラメータ設定
	pNewInstance->BindTex(static_cast<CTexture_Manager::TYPE>(TextureType));					// テクスチャ
	pNewInstance->SetCorrectionCoef(static_cast<float>(CorrectionCoef));						// 補正係数
	pNewInstance->SetSizeTarget(Vec3(SizeTarget[0], SizeTarget[1], SizeTarget[2]));				// 目標サイズ
	pNewInstance->SetPos(pNewInstance->m_InitPos);												// 座標
	pNewInstance->SetPosTarget(pNewInstance->m_InitPos);										// 目標座標
	pNewInstance->SetColTarget(XCol(ColTarget[0], ColTarget[1], ColTarget[2], ColTarget[3]));	// 目標色
	pNewInstance->SetMaxDuration(static_cast<int>(MaxDuration));								// 最大期間
	pNewInstance->m_fWaveCoef = static_cast<float>(WaveCoef);									// 波打ち強度
	pNewInstance->m_fAdderY = static_cast<float>(AdderY);										// 上昇量
}