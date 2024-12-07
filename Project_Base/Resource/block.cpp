//============================================================================
// 
// ブロック [block.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "block.h"
#include "bounding_box.h"
#include "rubble.h"

#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CBlock::CBlock() :
	CObject_X{ LAYER::DEFAULT },
	m_pBndBox{ DBG_NEW CBounding_Box() }
{

}

//============================================================================
// デストラクタ
//============================================================================
CBlock::~CBlock()
{
	// バウンディングボックスの破棄
	if (m_pBndBox != nullptr)
	{
		// メモリを解放
		delete m_pBndBox;

		// ポインタを初期化
		m_pBndBox = nullptr;
	}

#if 0
	for (int i = 0; i < 2; i++)
	{
		// ランダムな加速度を設定 ( ややY↑に偏った)
		D3DXVECTOR3 RandomVelocity{ utility::GetRandomValue<float>() * 0.01f, fabsf(utility::GetRandomValue<float>()) * 0.03f, -0.5f };

		// がれきを生成
		CRubble::Create(0,						// 通常
			GetPos() + RandomVelocity * 2.0f,	// 座標
			RandomVelocity * 0.5f);				// 加速度
	}
#endif
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CBlock::Init()
{
	// 基底クラスの初期設定
	HRESULT hr = CObject_X::Init();

	if (FAILED(hr))
	{
		assert(false && "プレイヤークラスの初期設定に失敗");
	}

	return hr;
}

//============================================================================
// 終了処理
//============================================================================
void CBlock::Uninit()
{
	// 基底クラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CBlock::Update()
{
#if 0
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Block Param"))
	{
		static float fRotY = 0.0f;
		if (ImGui::Button("--"))
			fRotY += D3DX_PI * -0.1f;
		ImGui::SameLine();
		if (ImGui::Button("++"))
			fRotY += D3DX_PI * 0.1f;
		ImGui::SameLine();
		ImGui::SliderFloat("Add RotY", &fRotY, -D3DX_PI, D3DX_PI);
		Vec3 NewRot = GetRot();
		NewRot.y = fRotY;
		SetRot(NewRot);
		ImGui::End();
	}
#endif

	// 箱バウンディングに中心点・サイズ・向きをセット
	m_pBndBox->SetCenterPos(GetPos());
	m_pBndBox->SetSize(GetSize());
	m_pBndBox->SetRot(GetRot());

	// 基底クラスの更新処理
	CObject_X::Update();

#if 0
#ifdef _DEBUG
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Block Param"))
	{
		ImGui::Text("Size::X %.2f:Y %.2f:Z %.2f", GetSize().x, GetSize().y, GetSize().z);
		ImGui::Text("Rot:X %.2f:Y %.2f:Z %.2f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.2f:Y %.2f:Z %.2f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::End();
	}
#endif // _DEBUG
#endif
}

//============================================================================
// 描画処理
//============================================================================
void CBlock::Draw()
{
	// 基底クラスの描画処理
	CObject_X::Draw();
}

//============================================================================
// サイズを取得
//============================================================================
D3DXVECTOR3 CBlock::GetSize() const
{
	return m_pBndBox->GetSize();
}

//============================================================================
// 生成
//============================================================================
CBlock* CBlock::Create(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Rot)
{
	// インスタンスを生成
	CBlock* pNewInstance = DBG_NEW CBlock();

	// タイプを設定
	pNewInstance->SetType(TYPE::BLOCK);

	// 初期設定
	pNewInstance->Init();

	// 座標を設定
	pNewInstance->SetPos(Pos);

	// 向きを設定
	pNewInstance->SetRot(Rot);

	// モデルを設定
	pNewInstance->BindModel(CX_Manager::TYPE::BLONORMAL + rand() % 3);

	// サイズを設定
	pNewInstance->m_pBndBox->SetSize(pNewInstance->GetModel()->Size);

	// 描画前に1度更新
	pNewInstance->Update();

	return pNewInstance;
}