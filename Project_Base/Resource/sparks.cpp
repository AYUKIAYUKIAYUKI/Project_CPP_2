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

namespace {
	float n_fCoef = 0.0f;
	int n_nDuration = 0;
	float n_fWaveCoef = 0.0f;
	float n_fAdderY = 0.0f;
}

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************

// 基礎パラメータの展開
JSON CSparks::m_InitParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\SPARKS\\sparks_param.json");

// 生成カウント
int CSparks::m_nCntGenerate = 0;

// 自動発生スパン
const int CSparks::m_nAutoGenerateSpan = static_cast<int>(CSparks::m_InitParam["AutoGenerateSpan"]);

// 局所発生スパン
const int CSparks::m_nAreaGenerateSpan = static_cast<int>(CSparks::m_InitParam["AreaGenerateSpan"]);

// 猛発生スパン
const int CSparks::m_nFuryGenerateSpan = static_cast<int>(CSparks::m_InitParam["FuryGenerateSpan"]);

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
// 自動発生
//============================================================================
void CSparks::AutoGenerate()
{
	// 生成カウントをインクリメント
	++m_nCntGenerate;

	// 規定の生成スパンに到達で
	if (m_nCntGenerate > m_nAutoGenerateSpan)
	{
		// 生成カウントをリセット
		m_nCntGenerate = 0;

		// 自動生成
		AutoCreate();
	}
}

//============================================================================
// 局所発生
//============================================================================
void CSparks::AreaGenerate(D3DXVECTOR3 Pos)
{
	// 生成カウントをインクリメント
	++m_nCntGenerate;

	// 規定の生成スパンに到達で
	if (m_nCntGenerate > m_nAreaGenerateSpan)
	{
		// 生成カウントをリセット
		m_nCntGenerate = 0;

		// 局所生成
		AreaCreate(Pos);
	}
}

//============================================================================
// 集中発生
//============================================================================
void CSparks::PointGenerate(D3DXVECTOR3 Pos)
{
	PointCreate(Pos);
}

//============================================================================
// 画面発生
//============================================================================
void CSparks::ScreenGenerate(D3DXVECTOR3 Pos)
{
	ScreenCreate(Pos);
}

//============================================================================
// 猛発生
//============================================================================
void CSparks::FuryGenerate()
{
	// 生成カウントをインクリメント
	++m_nCntGenerate;

	// 規定の生成スパンに到達で
	if (m_nCntGenerate > m_nFuryGenerateSpan)
	{
		// 生成カウントをリセット
		m_nCntGenerate = 0;

		// 猛生成
		FuryCreate();
	}
}

#ifdef _DEBUG
//============================================================================
#include "flyer.h"
//============================================================================
void CSparks::EditInitParam()
{
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("aaabbbccc")) {
		ImGui::InputFloat("Coef", &n_fCoef);
		ImGui::InputInt("Duration", &n_nDuration);
		ImGui::InputFloat("Wave", &n_fWaveCoef);
		ImGui::InputFloat("Adder", &n_fAdderY);
		if (ImGui::Button("Add Flyer")) { auto p = CFlyer::Create(); p->SetDirection(-D3DX_PI * 0.5f); p->AdjustInitDirection(50.0f); }
		ImGui::End();
	}
}
#endif // _DEBUG

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 自動生成
//============================================================================
void CSparks::AutoCreate()
{
	CSparks* pNewInstance = DBG_NEW CSparks();

	// 生成失敗
	if (!pNewInstance)
	{
		assert(false && "火の粉の生成に失敗");
	}

	// 火の粉の初期設定
	pNewInstance->Init();

	// パラメータをコピー
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = m_InitParam["CorrectionCoef"];
	auto const& SizeTarget = m_InitParam["SizeTarget"];
	auto const& ColTarget = m_InitParam["ColTarget"];
	auto const& MaxDuration = m_InitParam["MaxDuration"];
	auto const& WaveCoef = m_InitParam["WaveCoef"];
	auto const& AdderY = m_InitParam["AdderY"];

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

//============================================================================
// 局所生成
//============================================================================
void CSparks::AreaCreate(D3DXVECTOR3 Pos)
{
	// 火の粉を生成
	CSparks* pNewInstance = DBG_NEW CSparks();

	// 生成失敗
	if (!pNewInstance)
	{
		assert(false && "火の粉の生成に失敗");
	}

	// 火の粉の初期設定
	pNewInstance->Init();

	// パラメータをコピー
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = m_InitParam["CorrectionCoef"];
	auto const& SizeTarget = m_InitParam["SizeTarget"];
	auto const& ColTarget = m_InitParam["ColTarget"];
	auto const& MaxDuration = m_InitParam["MaxDuration"];
	auto const& WaveCoef = m_InitParam["WaveCoef"];
	auto const& AdderY = m_InitParam["AdderY"];

	// 指定された場所を基準にランダムな座標を作成
	pNewInstance->m_InitPos = {
		Pos.x + utility::GetRandomValue<float>(),
		Pos.y + fabsf(utility::GetRandomValue<float>()),
		Pos.z + utility::GetRandomValue<float>()
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

//============================================================================
// 集中生成
//============================================================================
void CSparks::PointCreate(D3DXVECTOR3 Pos)
{
	// 火の粉を生成
	CSparks* pNewInstance = DBG_NEW CSparks();

	// 生成失敗
	if (!pNewInstance)
	{
		assert(false && "火の粉の生成に失敗");
	}

	// 火の粉の初期設定
	pNewInstance->Init();

	// パラメータをコピー
#if 1
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = m_InitParam["PointCorrectionCoef"];
	auto const& SizeTarget = m_InitParam["PointSizeTarget"];
	auto const& ColTarget = m_InitParam["PointColTarget"];
	auto const& MaxDuration = m_InitParam["PointMaxDuration"];
	auto const& WaveCoef = m_InitParam["PointWaveCoef"];
	auto const& AdderY = m_InitParam["PointAdderY"];
#else
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = n_fCoef;
	auto const& SizeTarget = m_InitParam["PointSizeTarget"];
	auto const& ColTarget = m_InitParam["PointColTarget"];
	auto const& MaxDuration = n_nDuration;
	auto const& WaveCoef = n_fWaveCoef;
	auto const& AdderY = n_fAdderY;
#endif

	// 先行してキャスト
	float fCoef = static_cast<float>(CorrectionCoef);

	// 指定された場所を基準にランダムな座標を作成
	pNewInstance->m_InitPos = {
		Pos.x + utility::GetRandomValue<float>() * fCoef,
		Pos.y + fabsf(utility::GetRandomValue<float>()) * fCoef,
		Pos.z + utility::GetRandomValue<float>() * fCoef
	};

	// 各種パラメータ設定
	pNewInstance->BindTex(static_cast<CTexture_Manager::TYPE>(TextureType));					// テクスチャ
	pNewInstance->SetCorrectionCoef(static_cast<float>(fCoef));									// 補正係数
	pNewInstance->SetSizeTarget(Vec3(SizeTarget[0], SizeTarget[1], SizeTarget[2]));				// 目標サイズ
	pNewInstance->SetPos(pNewInstance->m_InitPos);												// 座標
	pNewInstance->SetPosTarget(pNewInstance->m_InitPos);										// 目標座標
	pNewInstance->SetColTarget(XCol(ColTarget[0], ColTarget[1], ColTarget[2], ColTarget[3]));	// 目標色
	pNewInstance->SetMaxDuration(static_cast<int>(MaxDuration));								// 最大期間
	pNewInstance->m_fWaveCoef = static_cast<float>(WaveCoef);									// 波打ち強度
	pNewInstance->m_fAdderY = static_cast<float>(AdderY);										// 上昇量
}

//============================================================================
// 画面生成
//============================================================================
void CSparks::ScreenCreate(D3DXVECTOR3 Pos)
{

}

//============================================================================
// 猛生成
//============================================================================
void CSparks::FuryCreate()
{
	CSparks* pNewInstance = DBG_NEW CSparks();

	// 生成失敗
	if (!pNewInstance)
	{
		assert(false && "火の粉の生成に失敗");
	}

	// 火の粉の初期設定
	pNewInstance->Init();

	// パラメータをコピー
	auto const& TextureType = m_InitParam["TextureType"];
	auto const& CorrectionCoef = m_InitParam["CorrectionCoef"];
	auto const& SizeTarget = m_InitParam["FurySizeTarget"];
	auto const& ColTarget = m_InitParam["FuryColTarget"];
	auto const& MaxDuration = m_InitParam["FuryMaxDuration"];
	auto const& WaveCoef = m_InitParam["FuryWaveCoef"];
	auto const& AdderY = m_InitParam["FuryAdderY"];

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