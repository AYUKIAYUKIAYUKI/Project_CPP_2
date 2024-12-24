//============================================================================
// 
// カメラ [camera.cpp]
// Author : 福田歩希
// 
//============================================================================

// カメラ調整用
namespace
{
	float
		FDistance = 135.0f,	// 間距離
		FAdderY = 25.0f;	// 俯瞰度合い
}

//****************************************************
// インクルードファイル
//****************************************************
#include "camera.h"

// インプット取得用
#include "manager.h"

// デバイス取得用
#include "renderer.h"

// 追従対象取得用
#include "object.h"
#include "player.h"

/* 応急処置 */
#include "scene.h"
#include "result.h"

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
CCamera::CCamera() :
	m_Rot{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_PosTarget{ VEC3_INIT },
	m_PosV{ VEC3_INIT },
	m_PosTargetV{ VEC3_INIT },
	m_PosR{ VEC3_INIT },
	m_PosTargetR{ VEC3_INIT },
	m_fDistance{ 0.0f },
	m_VecU{ 0.0f, 1.0f, 0.0f },
	m_fAdjust{ 0.0f },
	m_bTrackPlayer{ false },
	m_bVertical{ false },
	m_bHorizon{ false }
{
	// 行列を初期化
	D3DXMatrixIdentity(&m_MtxProjection);	// プロジェクション行列
	D3DXMatrixIdentity(&m_MtxView);			// ビュー行列
}

//============================================================================
// デストラクタ
//============================================================================
CCamera::~CCamera()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CCamera::Init()
{
	/* 現在は無し */

	return S_OK;
}

//============================================================================
// 更新処理
//============================================================================
void CCamera::Update()
{
	// ビューモード分岐
	BranchViewMode();

	// 目標値への補間
	CorrectToTarget();

#ifdef _DEBUG
	// デバッグ表示
	PrintDebug();
#endif // _DEBUG
}

//============================================================================
// カメラ振動
//============================================================================
void CCamera::SetVibration(float fCoef)
{
	// 指定された強度でカメラ座標をランダムにずらす
	m_Pos += {
		utility::GetRandomValue<float>() * fCoef,
		utility::GetRandomValue<float>() * fCoef,
		utility::GetRandomValue<float>() * fCoef,
	};
}

//============================================================================
// カメラをセット
//============================================================================
void CCamera::SetCamera(D3DXVECTOR3 Pos)
{
#if 0	// バッファのクリアはレンダラーに委ねる
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 画面バッファクリア
	pDev->Clear(0,
		nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);
#endif

	// プロジェクション行列を計算
	CalcMtxProjection();

	// ビュー行列を計算
	CalcMtxView(Pos);
}

//============================================================================
// ボス登場用のカメラセット
//============================================================================
void CCamera::SetAppearBoss()
{
#ifdef _DEBUG
	static float a = 0.0f, b = 0.0f;
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Boss View"))
	{
		ImGui::DragFloat("a", &a);
		ImGui::DragFloat("b", &b);
		ImGui::End();
	}
#endif // _DEBUG

	// プレイヤーを取得
	CPlayer* pPlayer = nullptr;
	pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

	// カメラのパラメータを自動で設定
	m_PosTarget = pPlayer->GetPosTarget();				// カメラ目標座標はプレイヤーの座標に
	const Vec3& NegVec = VEC3_INIT - pPlayer->GetPos();	// プレイヤーから原点への逆位置ベクトルを作成
	m_RotTarget = VEC3_INIT;							// カメラの目標向きをリセット
	m_RotTarget.y = atan2f(NegVec.x, NegVec.z);			// カメラの目標向きを作成した逆位置ベクトル方向に
	utility::AdjustAngle(m_Rot.y, m_RotTarget.y);		// カメラ回転の角度の差を補正

	// 距離を目標値まで補間
	m_fDistance += (550.0f - m_fDistance) * COEF_ADJUST;

	// 俯瞰度合いを目標値まで補間
	m_fAdjust += (300.0f - m_fAdjust) * COEF_ADJUST;
}

//============================================================================
// 向きを取得
//============================================================================
const D3DXVECTOR3& CCamera::GetRot() const
{
	return m_Rot;
}

//============================================================================
// 向きを設定
//============================================================================
void CCamera::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// 目標向きを取得
//============================================================================
const D3DXVECTOR3& CCamera::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// 目標向きを設定
//============================================================================
void CCamera::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// 座標を取得
//============================================================================
const D3DXVECTOR3& CCamera::GetPos() const
{
	return m_Pos;
}

//============================================================================
// 座標を設定
//============================================================================
void CCamera::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// 目標座標を取得
//============================================================================
const D3DXVECTOR3& CCamera::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// 目標座標を設定
//============================================================================
void CCamera::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// 間距離を取得
//============================================================================
const float& CCamera::GetDistance() const
{
	return m_fDistance;
}

//============================================================================
// 間距離を設定
//============================================================================
void CCamera::SetDistance(float fDistance)
{
	m_fDistance = fDistance;
}

//============================================================================
// 俯瞰度合いを取得
//============================================================================
const float& CCamera::GetUpAdjust() const
{
	return m_fAdjust;
}

//============================================================================
// 俯瞰度合いを設定
//============================================================================
void CCamera::SetUpAdjust(float fAdjust)
{
	m_fAdjust = fAdjust;
}

//============================================================================
// プレイヤー追従切り替え
//============================================================================
void CCamera::ChangeTrackPlayer(bool bTrack)
{
	m_bTrackPlayer = bTrack;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// モード分岐
//============================================================================
void CCamera::BranchViewMode()
{
#ifdef _DEBUG 
	// ビューモード選択用ウィンドウを表示
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("VeiwMode"))
	{
		if (ImGui::Checkbox("Track", &m_bTrackPlayer)) {}
		ImGui::SameLine();
		if (ImGui::Checkbox("Vertical", &m_bVertical))
			m_bHorizon = 0;
		ImGui::SameLine();
		if (ImGui::Checkbox("Horizon", &m_bHorizon))
			m_bVertical = 0;
		ImGui::End();
	}
#endif // _DEBUG 

	// プレイヤー追従フラグに応じて処理を変更
	if (m_bTrackPlayer)
	{ // 追従する

		// プレイヤーオブジェクトを検索
		CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

		// オブジェクトが存在していたら
		if (pObj)
		{
			// プレイヤークラスにダウンキャスト
			CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

			// カメラのパラメータを自動で設定
			m_PosTarget = pPlayer->GetPosTarget();					// カメラ目標座標はプレイヤーの座標に
			const Vec3& NegVec = VEC3_INIT - pPlayer->GetPos();		// プレイヤーから原点への逆位置ベクトルを作成
			m_RotTarget = VEC3_INIT;								// カメラの目標向きをリセット
			m_RotTarget.y = atan2f(NegVec.x, NegVec.z);				// カメラの目標向きを作成した逆位置ベクトル方向に
			utility::AdjustAngle(m_Rot.y, m_RotTarget.y);			// カメラ回転の角度の差を補正
			m_fDistance += (FDistance - m_fDistance) * COEF_ADJUST;	// 間距離を固定
			m_fAdjust += (FAdderY - m_fAdjust) * COEF_ADJUST;		// 俯瞰度合いを固定
		}
	}

#ifdef _DEBUG
	// カメラ操作
	Control();
#endif // _DEBUG
}

//============================================================================
// カメラ操作
//============================================================================
void CCamera::Control()
{
#ifdef _DEBUG
	if (ImGui::Begin("Camera Param"))
	{
		ImGui::DragFloat("fDistance", &FDistance);
		ImGui::DragFloat("AdderY", &FAdderY);
		ImGui::End();
	}
#endif // _DEBUG
}

//============================================================================
// 目標値への補間
//============================================================================
void CCamera::CorrectToTarget()
{
	// 自動で目標向きへ補間
	AutoSetRot();

	// 自動で目標座標へ補間
	AutoSetPos();

	// 視点座標を計算
	CalcPosV();

	// 注視点座標を計算
	CalcPosR();
}

//============================================================================
// 自動で目標向きへ補間
//============================================================================
void CCamera::AutoSetRot()
{
	// ヨー角の範囲を制限
	utility::AdjustDirection(m_RotTarget.y, m_Rot.y);

	// ピッチ角の範囲を制限
	AutoRestrictPitch();

	// 目標向きへ補間する
	m_Rot += (m_RotTarget - m_Rot) * COEF_ADJUST;
}

//============================================================================
// ピッチ角の範囲を制限
//============================================================================
void CCamera::AutoRestrictPitch()
{
	// 0.0f ～ 0.5f
	float fCoeff = 0.48f;

	if (m_RotTarget.x > D3DX_PI * fCoeff)
	{
		m_RotTarget.x = D3DX_PI * fCoeff;
	}
	else if (m_RotTarget.x < -D3DX_PI * fCoeff)
	{
		m_RotTarget.x = -D3DX_PI * fCoeff;
	}
}

//============================================================================
// 自動で目標座標へ補間
//============================================================================
void CCamera::AutoSetPos()
{
	// 目標座標へ補間する
	m_Pos += (m_PosTarget - m_Pos) * COEF_ADJUST;
}

//============================================================================
// 視点座標を計算
//============================================================================
void CCamera::CalcPosV()
{
	// 視点は対象物の後方に
	m_PosV =
	{
		m_Pos.x - (sinf(m_Rot.y) * cosf(m_Rot.x) * (m_fDistance)),
		m_Pos.y - (sinf(m_Rot.x) * (m_fDistance)),
		m_Pos.z - (cosf(m_Rot.y) * cosf(m_Rot.x) * (m_fDistance))
	};
}

//============================================================================
// 注視点座標を計算
//============================================================================
void CCamera::CalcPosR()
{
	// 注視点を対象物の前方に
	m_PosR =
	{
		m_Pos.x + (sinf(m_Rot.y) * cosf(m_Rot.x) * m_fDistance),
		m_Pos.y + (sinf(m_Rot.x) * m_fDistance),
		m_Pos.z + (cosf(m_Rot.y) * cosf(m_Rot.x) * m_fDistance)
	};
}

//============================================================================
// プロジェクション行列計算
//============================================================================
void CCamera::CalcMtxProjection()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// プロジェクション行列の初期化
	D3DXMatrixIdentity(&m_MtxProjection);

	// プロジェクション行列を生成
#if 1
	// 透視投影
	D3DXMatrixPerspectiveFovLH(&m_MtxProjection,
		D3DXToRadian(45.0f),
		FSCREEN_WIDTH / FSCREEN_HEIGHT,
		10.0f,
		5000.0f);
#else
	// 平行投影
	D3DXMatrixOrthoLH(&m_MtxProjection,
		(float)SCREEN_WIDTH * 0.25f,
		(float)SCREEN_HEIGHT * 0.25f,
		10.0f,
		5000.0f);
#endif

	// プロジェクション行列の設定
	pDev->SetTransform(D3DTS_PROJECTION,
		&m_MtxProjection);
}

//============================================================================
// ビュー行列計算
//============================================================================
void CCamera::CalcMtxView(D3DXVECTOR3 Pos)
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ビュー行列の初期化
	D3DXMatrixIdentity(&m_MtxView);

	// 視点座標の調整用
	Vec3 posV = m_PosV, posR = m_PosR;

	// 俯瞰度合いを反映
	posV.y += FAdderY;

	/* 応急処置 */
	auto aa = CScene_Manager::GetInstance()->GetScene();

	if (aa)
	{
		if (typeid(*aa) == typeid(CResult))
		{
			posV.y = 0.0f;
		}
	}

	// 初期座標以外を渡された場合その視点をコピー
	if (Pos != VEC3_INIT)
	{
		posV = Pos;
		posV.y += -30.0f;
		posR = VEC3_INIT;
	}

	// ビュー行列の生成
	D3DXMatrixLookAtLH(&m_MtxView,
		&posV,
		&posR,
		&m_VecU);

	// ビュー行列の設定
	pDev->SetTransform(D3DTS_VIEW,
		&m_MtxView);
}

//============================================================================
// デバッグ表示
//============================================================================
void CCamera::PrintDebug()
{
#if 0
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("カメラ座標　　 : " + to_string(m_Pos.x) + " :  " + to_string(m_Pos.y) + " : " + to_string(m_Pos.z));
	CRenderer::SetDebugString("目標カメラ座標 : " + to_string(m_PosTarget.x) + " :  " + to_string(m_PosTarget.y) + " : " + to_string(m_PosTarget.z));
	CRenderer::SetDebugString("カメラ向き　　 : " + to_string(m_Rot.x * (180 / D3DX_PI)) + " :  " + to_string(m_Rot.y * (180 / D3DX_PI)) + " : " + to_string(m_Rot.z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("目標カメラ向き : " + to_string(m_RotTarget.x * (180 / D3DX_PI)) + " :  " + to_string(m_RotTarget.y * (180 / D3DX_PI)) + " : " + to_string(m_RotTarget.z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("カメラ間距離 　: " + to_string(m_fDistance));
	CRenderer::SetDebugString("カメラモード　 : " + to_string(m_bTrackPlayer));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
#endif
}