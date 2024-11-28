//============================================================================
// 
// ボス [boss.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "boss.h"
#include "bounding_cylinder.h"
#include "object_parts.h"

#include "field_manager.h"
#include "manager.h"
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
CBoss::CBoss() :
	CCharacter{},
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() }
{

}

//============================================================================
// デストラクタ
//============================================================================
CBoss::~CBoss()
{
	// バウンディングシリンダーの破棄
	if (m_pBndCylinder != nullptr)
	{
		// メモリを解放
		delete m_pBndCylinder;

		// ポインタを初期化
		m_pBndCylinder = nullptr;
	}
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CBoss::Init()
{
	// 補正強度を設定
	SetCorrectCoef(CORRECT_COEF);

	// 初期方角を設定
	SetDirection(D3DX_PI * -0.5f);
	SetDirectionTarget(D3DX_PI * -0.5f);

	// 初期体力を設定
	SetLife(MAX_LIFE);

	// キャラクタークラスの初期設定
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "ボスクラスの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CBoss::Uninit()
{
	// キャラクタークラスの終了処理
	CCharacter::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CBoss::Update()
{
	// 円柱バウンディングの中心点をモデルの中心に
	m_pBndCylinder->SetCenterPos(GetParentParts()->GetPos());

	// キャラクタークラスの更新処理
	CCharacter::Update();

#if 1
#ifdef _DEBUG
	// ウィンドウを表示
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Boss Param"))
	{
		ImGui::Text("Direction:%.3f", GetDirection() * (180 / D3DX_PI));
		ImGui::Text("DirectionTarget:%.3f", GetDirectionTarget() * (180 / D3DX_PI));
		ImGui::Text("Rot:X %.1f:Y %.1f:Z %.1f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("RotTarget:X %.1f:Y %.1f:Z %.1f", GetRotTarget().x * (180 / D3DX_PI), GetRotTarget().y * (180 / D3DX_PI), GetRotTarget().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.1f:Y %.1f:Z %.1f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::Text("PosTarget:X %.1f:Y %.1f:Z %.1f", GetPosTarget().x, GetPosTarget().y, GetPosTarget().z);
		ImGui::Text("VelY:%.1f", GetVelY());
		ImGui::Text("Life:%df", GetLife());
	}
	ImGui::End();
#endif	// _DEBUG
#endif
}

//============================================================================
// 描画処理
//============================================================================
void CBoss::Draw()
{
	// キャラクタークラスの描画処理
	CCharacter::Draw();
}

//============================================================================
// ダメージを受ける
//============================================================================
void CBoss::SetDamage(int nDamage)
{
	// ダメージ量分、体力を変動
	int nNewLife = GetLife();
	nNewLife += nDamage;
	SetLife(nNewLife);
}

//============================================================================
// 半径を取得
//============================================================================
float CBoss::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// 高さを取得
//============================================================================
float CBoss::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// バウンディングシリンダーを取得
//============================================================================
const CBounding_Cylinder* const CBoss::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// 生成
//============================================================================
CBoss* CBoss::Create()
{
	// インスタンスを生成
	CBoss* pNewInstance = DBG_NEW CBoss();

	// タイプを設定
	pNewInstance->SetType(TYPE::BOSS);

	// 初期設定
	pNewInstance->Init();

	// モーションをセット
	pNewInstance->SetMotion(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\pumpkin_motion.json"));

	// 半径を設定
	pNewInstance->m_pBndCylinder->SetRadius(50.0f);

	// 高さを設定
	pNewInstance->m_pBndCylinder->SetHeight(50.0f);

	return pNewInstance;
}