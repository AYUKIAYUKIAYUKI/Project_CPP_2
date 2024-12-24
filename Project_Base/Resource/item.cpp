//============================================================================
// 
// アイテム [item.cpp]
// Author : 福田歩希
// 
//============================================================================

/* これはステンシルバッファの描画テスト用 */
namespace
{
	int
	ZFuncCmp = D3DCMP_LESSEQUAL,	// デプスステンシル値の比較方法
	StencilRefValue = 0,			// ステンシル参照値
	StencilCmp = D3DCMP_EQUAL,		// ステンシル値の比較方法
	Pass = D3DSTENCILOP_KEEP,		// Zテスト・ステンシルテストに成功
	ZFail = D3DSTENCILOP_INCRSAT,	// Zテストのみ失敗
	Fail = D3DSTENCILOP_KEEP;		// Zテスト・ステンシルテストに失敗
}

//****************************************************
// インクルードファイル
//****************************************************
#include "item.h"
#include "summoning.h"
#include "bounding_sphere.h"
#include "sound.h"
#include "field_manager.h"
#include "player.h"
#include "collision.h"

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
CItem::CItem() :
	CObject_X{ LAYER::STENCIL },
	m_pBndSphere{ DBG_NEW CBounding_Sphere() },
	m_fCorrectCoef{ 0.0f },
	m_fDirection{ 0.0f },
	m_RotTarget{ VEC3_INIT },
	m_PosTarget{ VEC3_INIT },
	m_pSummoning{ CSummoning::Create() }
{

}

//============================================================================
// デストラクタ
//============================================================================
CItem::~CItem()
{
	// 球バウンディングの破棄
	if (m_pBndSphere != nullptr)
	{
		// メモリを解放
		delete m_pBndSphere;

		// ポインタを初期化
		m_pBndSphere = nullptr;
	}
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CItem::Init()
{
	// Xオブジェクトクラスの初期設定
	if (FAILED(CObject_X::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CItem::Uninit()
{
	// Xオブジェクトクラスの終了処理
	CObject_X::Uninit();

	// 魔法陣を消滅モーションに変更
	m_pSummoning->SetNowMotion(1);
}

//============================================================================
// 更新処理
//============================================================================
void CItem::Update()
{
	// くるくる回る
	AppealRotate();

	// 目標値への補間
	CorrectToTarget();

	// 球バウンディングに中心点セット
	m_pBndSphere->SetCenterPos(GetPos());

	// プレイヤーとの接触を検出
	HitChecklPlayer();

	// 魔法陣をセット
	SetSummoning();

	// Xオブジェクトクラスの更新処理
	CObject_X::Update();

#if 1
#ifdef _DEBUG
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Item Param"))
	{
		ImGui::Text("Direction:%.1f", GetDirection());
		ImGui::Text("Size:X %.2f:Y %.2f:Z %.2f", GetSize().x, GetSize().y, GetSize().z);
		ImGui::Text("Rot:X %.2f:Y %.2f:Z %.2f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.2f:Y %.2f:Z %.2f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::End();
	}
#endif // _DEBUG
#endif

#ifdef _DEBUG
#if 0	/* ステンシルテストの設定変更 */
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Sstencil - Only Item"))
	{
		ImGui::InputInt("ZFuncCmp", &ZFuncCmp);
		ImGui::InputInt("StencilRefValue", &StencilRefValue);
		ImGui::InputInt("StencilCmp", &StencilCmp);
		ImGui::InputInt("Pass", &Pass);
		ImGui::InputInt("ZFail", &ZFail);
		ImGui::InputInt("Fail", &Fail);
		ImGui::End();
	}
#endif
#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CItem::Draw()
{
	// デバイスを取得
	auto pDev = CRenderer::GetDeviece();

	// 深度バッファへの書き込みを有効化
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ステンシル参照値を設定
	pDev->SetRenderState(D3DRS_STENCILREF, StencilRefValue);

	// ステンシルバッファの比較方法を変更
	pDev->SetRenderState(D3DRS_STENCILFUNC, StencilCmp);

	// ステンシルテストの結果に対してのふるまいを設定する
	pDev->SetRenderState(D3DRS_STENCILPASS, Pass);		// Zテスト・ステンシルテストに成功
	pDev->SetRenderState(D3DRS_STENCILZFAIL, ZFail);	// Zテストのみ失敗
	pDev->SetRenderState(D3DRS_STENCILFAIL, Fail);		// Zテスト・ステンシルテストに失敗

	// Xオブジェクトクラスの描画処理
	CObject_X::Draw();

	// 深度バッファへの書き込みを有効化
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//============================================================================
// 半径を取得
//============================================================================
float CItem::GetRadius() const
{
	return m_pBndSphere->GetRadius();
}

//============================================================================
// 半径を設定
//============================================================================
void CItem::SetRadius(float fRadius)
{
	m_pBndSphere->SetRadius(fRadius);
}

//============================================================================
// 補間強度を設定
//============================================================================
void CItem::SetCorrectCoef(float fCoef)
{
	m_fCorrectCoef = fCoef;
}

//============================================================================
// 方角を取得
//============================================================================
const float& CItem::GetDirection() const
{
	return m_fDirection;
}

//============================================================================
// 方角を設定
//============================================================================
void CItem::SetDirection(float fDirection)
{
	m_fDirection = fDirection;

	// 方角から座標を設定し
	Vec3 NewPos = GetPos();
	NewPos.x = cosf(fDirection) * CField_Manager::FIELD_RADIUS;
	NewPos.z = sinf(fDirection) * CField_Manager::FIELD_RADIUS;
	SetPos(NewPos);

	// 目標座標も同時に設定
	m_PosTarget = NewPos;
}

//============================================================================
// 目標向きを取得
//============================================================================
const D3DXVECTOR3& CItem::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// 目標向きを設定
//============================================================================
void CItem::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// 目標座標を取得
//============================================================================
const D3DXVECTOR3& CItem::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// 目標座標を設定
//============================================================================
void CItem::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// Y座標を設定
//============================================================================
void CItem::SetPosY(float fPosY)
{
	// 座標を設定し
	Vec3 NewPos = GetPos();
	NewPos.y = fPosY;
	SetPos(NewPos);

	// 目標座標も同時に設定
	m_PosTarget.y = fPosY;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 魔法陣をセット
//============================================================================
void CItem::SetSummoning()
{
	// 座標をコピー
	Vec3 Pos = GetPos();

	// 座標に合わせて向きをセット
	Vec3 Rot = { 0.0f, atan2f(Pos.z, -Pos.x) + D3DX_PI * 0.5f, 0.0f };
	m_pSummoning->SetRot(Rot);

#if 0
	// 座標をアイテムの背後に
	Pos.x *= 0.95f;
	Pos.z *= 0.95f;
#endif

	// 座標をセット
	m_pSummoning->SetPos(Pos);
}

//============================================================================
// プレイヤーとの接触を検出
//============================================================================
void CItem::HitChecklPlayer()
{
	// プレイヤータイプのオブジェクトを探す
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// 見つからなければ処理を行わない
	if (!pObj)
		return;

	// オブジェクトをプレイヤークラスにキャスト
	CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// プレイヤーの円柱バウンディングとの当たり判定
	if (collision::HitCylinderToSphere(pPlayer->GetBndCylinder(), m_pBndSphere))
	{ // 接触していたら

		// 取得時の効果を発動
		GainEffect();

		// 自身を破棄予約
		SetRelease();

		// アイテム取得音を鳴らす
		CSound::GetInstance()->Play(CSound::LABEL::HEAL);
	}
}

//============================================================================
// くるくる回る
//============================================================================
void CItem::AppealRotate()
{
	// 向きをY軸回転
	m_RotTarget.y += 0.05f;

	// 簡易的な向き制限
	if (m_RotTarget.y > D3DX_PI * 2.0f)
		m_RotTarget.y += D3DX_PI * -2.0f;

	// 若干上下に揺れて浮遊感を出す
	Vec3 NewPos = GetPos();
	NewPos.y += cosf(m_RotTarget.y) * 0.5f;
	SetPos(NewPos);
}

//============================================================================
// 目標値への補間
//============================================================================
void CItem::CorrectToTarget()
{
	// 目標向きへ向ける
	Vec3 NewRot = GetRot();
	utility::AdjustAngle(NewRot.y, m_RotTarget.y);	// 角度の差を補正
	NewRot += (m_RotTarget - NewRot) * 0.1f;
	SetRot(NewRot);

	// 目標座標へ移動
	Vec3 NewPos = GetPos();
	NewPos += (m_PosTarget - NewPos) * m_fCorrectCoef;
	SetPos(NewPos);
}