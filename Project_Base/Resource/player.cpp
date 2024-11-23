//============================================================================
// 
// プレイヤー [player.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player.h"
#include "bounding_cylinder.h"
#include "player_state.h"
#include "player_state_default.h"
#include "player_state_dash.h"

#include "block.h"
#include "collision.h"
#include "field_manager.h"
#include "manager.h"
#include "mask_rectangle.h"
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
CPlayer::CPlayer() :
	CCharacter{},
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder(this) },
	m_pState{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CPlayer::~CPlayer()
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
HRESULT CPlayer::Init()
{
	// 初期ステートの生成
	if (!m_pState)
	{
		m_pState = DBG_NEW CPlayer_State_Default();
	}
	
	// 補正強度を設定
	SetCorrectCoef(CORRECT_COEF);

	// 初期方角を設定
	SetDirection(D3DX_PI * -0.5f);

	// 初期移動速度を設定
	SetMoveSpeed(DEFAULT_MOVE_SPEED);

	// 初期体力を設定
	SetLife(MAX_LIFE);

	// キャラクタークラスの初期設定
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "プレイヤークラスの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CPlayer::Uninit()
{
	// ステート破棄
	if (m_pState)
	{
		// ステートの破棄
		delete m_pState;
		
		// ポインタを初期化
		m_pState = nullptr;
	}

	// キャラクタークラスの終了処理
	CCharacter::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer::Update()
{
	// ステートの変更確認
	m_pState = m_pState->CheckChangeState<CPlayer_State>();

	// ステートの更新
	m_pState->Update();

	// 高さの補正
	AdjustHeight();

	// 自動で目標向きを移動方向に向ける
	AutoSetRotTarget();

	// 自動で目標座標を変動した方角に合わせる
	AutoSetPosTarget();

	// 当たり判定
	HitCheck();

	// キャラクタークラスの更新処理
	CCharacter::Update();

#if 1
#ifdef _DEBUG
	// ウィンドウを表示
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Player Param"))
	{
		ImGui::Text("State:%s", typeid(*m_pState).name());
		ImGui::Text("Direction:%.3f", GetDirection() * (180 / D3DX_PI));
		ImGui::Text("Speed:%.1f", GetMoveSpeed());
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
void CPlayer::Draw()
{
	// キャラクタークラスの描画処理
	//CCharacter::Draw();

#if 1	// ステンシルバッファにプレイヤーのシルエットを描画する

	// デバイスを取得
	auto pDev = CRenderer::GetDeviece();

	// ステンシルマスクを設定
	pDev->SetRenderState(D3DRS_STENCILMASK, 0x000000ff);

	// ステンシル参照値を設定
	pDev->SetRenderState(D3DRS_STENCILREF, 0x01);

	// ステンシルバッファの比較方法を変更
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	// ステンシルテストの結果に対してのふるまいを設定する
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_REPLACE);	// Zテスト・ステンシルテストに成功
	pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILCAPS_KEEP);		// Zテストのみ失敗
	pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILCAPS_KEEP);		// Zテスト・ステンシルテストに失敗

	// キャラクタークラスの描画処理 (ステンシルバッファ)
	CCharacter::Draw();

	// ステンシルマスクを設定
	pDev->SetRenderState(D3DRS_STENCILMASK, 0x000000ff);

	// ステンシル参照値を設定
	pDev->SetRenderState(D3DRS_STENCILREF, 0x02);

	// ステンシルバッファの比較方法を変更
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);

	// 四角形マスクの描画
	CManager::GetManager()->GetMask_Rectangle()->Draw();

#endif

#if 0
	for (DWORD i = 0; i < 10; ++i)
	{
		// ステートマネージャーの更新
		CCharacter::Update();

		// キャラクタークラスのクラスの描画処理
		CCharacter::Draw();
	}
#endif
}

//============================================================================
// ダメージを受ける
//============================================================================
void CPlayer::SetDamage(int nDamage)
{
	// ダメージ量分、体力を変動
	 int nNewLife = GetLife();
	 nNewLife += nDamage;
	 SetLife(nNewLife);

	// ダメージ状態へ変更するよう命令する
	m_pState->To_Damage();
}

//============================================================================
// 半径を取得
//============================================================================
float CPlayer::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// 高さを取得
//============================================================================
float CPlayer::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// バウンディングシリンダーを取得
//============================================================================
const CBounding_Cylinder* const CPlayer::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// 現在のステートを取得
//============================================================================
const CPlayer_State* const CPlayer::GetNowState() const
{
	// ステートのポインタを返す
	return m_pState;
}

//============================================================================
// 生成
//============================================================================
CPlayer* CPlayer::Create()
{
	// インスタンスを生成
	CPlayer* pNewInstance = DBG_NEW CPlayer();

	// タイプを設定
	pNewInstance->SetType(TYPE::PLAYER);
	
	// 初期設定
	pNewInstance->Init();

	// モデルを設定
	pNewInstance->BindModel(CX_Manager::TYPE::SAMUS);

	// 半径を設定
	float fRad = 0.0f;
	pNewInstance->GetModel()->Size.x > pNewInstance->GetModel()->Size.z ? fRad = pNewInstance->GetModel()->Size.x : fRad = pNewInstance->GetModel()->Size.z;
	pNewInstance->m_pBndCylinder->SetRadius(fRad);

	// 高さを設定
	pNewInstance->m_pBndCylinder->SetHeight(pNewInstance->GetModel()->Size.y);

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 高さを補正
//============================================================================
void CPlayer::AdjustHeight()
{
	// 目標座標をY軸の加速度分変動する
	Vec3 NewPosTarget = GetPosTarget();
	NewPosTarget.y += GetVelY();
	SetPosTarget(NewPosTarget);

	// 高さの目標に下限を設定
	if (GetPosTarget().y < m_pBndCylinder->GetHeight())
	{
		// 目標座標を下限に固定
		NewPosTarget.y = m_pBndCylinder->GetHeight();
		SetPosTarget(NewPosTarget);

		// Y軸方向の加速度を初期化
		SetVelY(0.0f);
	}
}

//============================================================================
// 当たり判定
//============================================================================
void CPlayer::HitCheck()
{
	static CObject_X* TESTTEST = CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\testtest.json"));
	static CObject_X* ててて = CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ててて.json"));

	// 衝突の有無を検出
	bool bDetect = false;

	// 当たり判定の中心点を設定
	m_pBndCylinder->SetCenterPos(GetPos());

	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	while (pObj != nullptr)
	{
		// ブロッククラスにダウンキャスト
		CBlock* pBlock = CObject::DownCast<CBlock>(pObj);

		// キャスト失敗で
		if (!pBlock)
		{
			continue;
		}

		// バウンディングシリンダーのパラメータをコピー
		const Vec3& CylinderPosTarget = GetPosTarget();
		const Vec3& CylinderOldPos = GetPos();
		const float& CylinderRadius = GetRadius();
		const float& CylinderHeight = GetHeight();

		// バウンディングボックスのパラメータをコピー
		const Vec3& BoxSize = pBlock->GetSize();
		const Vec3& BoxPos = pBlock->GetPos();
		const float& fBoxDirection = pBlock->GetRot().y;
		{
			TESTTEST->SetScale(BoxSize);
		}

		// ①ボックスの中心点からシリンダーの座標への距離ベクトルを計算
		const Vec3& RelativePos = CylinderPosTarget - BoxPos;
		const Vec3& RelativeOldPos = CylinderOldPos - BoxPos;

		// ②座標系を正規化するため、距離ベクトルにボックスの回転量を打ち消すように回転行列を適用
		const Vec3& ResultPos = utility::RotatePointAroundY(-fBoxDirection, RelativePos);
		const Vec3& ResultOldPos = utility::RotatePointAroundY(-fBoxDirection, RelativeOldPos);
		{
			ててて->SetScale({ GetBndCylinder()->GetRadius(), GetBndCylinder()->GetHeight(), GetBndCylinder()->GetRadius() });
			ててて->SetPos(ResultPos);
		}

		// 座標系を正規化した仮定し、ボックスからシリンダーへの距離ベクトルを用いて衝突判定
		// (ボックスの座標は原点で、回転は考慮せず、AABBと仮定する)
		if (collision::HitCylinderToAABB(ResultPos, CylinderRadius, CylinderHeight, VEC3_INIT, BoxSize))
		{
			// 判定表示を赤色に
			//m_pBndCylinder->ChangeModel(CX_Manager::TYPE::RENDER_CYLINDER_HIT);
				
			// 衝突があったことを検出
			bDetect = 1;
			
			// 衝突面を判定
			int nIdx = collision::GetCylinderToAABB(ResultOldPos, ResultPos, CylinderRadius, CylinderHeight, VEC3_INIT, BoxSize);

			// 上下の判定は2DのY軸判定と変わらないので普通にブロックの上下幅で押し出し
			// 左右の判定は正規化した座標で押し出しを行った後に回転させて本来のあるべき座標を設定
			switch (nIdx)
			{
			case 0:	// 分類不能
				break;

			case 1: // 上
			{
				// Y軸方向の加速度を初期化
				SetVelY(0.0f);

				// 新しい目標座標を作成
				const Vec3& NewPosTarget = { CylinderPosTarget.x,
					BoxPos.y + BoxSize.y + CylinderHeight,
					CylinderPosTarget.z };
			 
				// 目標座標を反映
				SetPosTarget(NewPosTarget);
			
				break;
			}
			case 2: // 下
			{
				// Y軸方向の加速度を重力方向へ固定
				SetVelY(0.0f);

				// 新しい目標座標を作成
				const Vec3& NewPosTarget = {
					CylinderPosTarget.x,
					BoxPos.y - BoxSize.y - CylinderHeight,
					CylinderPosTarget.z };

				// 目標座標を反映
				SetPosTarget(NewPosTarget);

				break;
			}
			case 3:	// 左
			{
#if 0	// 押し出し方法の切り替え (調整中)
				// 正規化された座標系内で新しい目標座標を作成
				Vec3 NewPosTarget = {
					/* BoxPos.x <- 0.0f */ + BoxSize.x + CylinderRadius,
					CylinderPosTarget.y,
					//(ResultOldPos.z/* - BoxPos.z <- 0.0f */) * (ResultPos.x / ResultOldPos.x)};
					//ResultOldPos.z};
					CylinderOldPos.z * (ResultPos.x / ResultOldPos.x) };
				/* ↑Z座標について、円弧で移動するため、膨大なスイープ判定を行うことになってしまう…
				ボックスの中心から元のZ座標への方向成分をXのサイズ分と均等な割合になるように調整する */

				// 打ち消した回転を再度かけ合わせて本来の向きに戻す
				NewPosTarget = utility::RotatePointAroundY(fBoxDirection + D3DX_PI * 0.5f, NewPosTarget);

				CRenderer::SetTimeString("正規化座標系での座標:x " + to_string(NewPosTarget.x) + ":y " + to_string(NewPosTarget.y) + ":z " + to_string(NewPosTarget.z), 600);
				CRenderer::SetTimeString("Z割合" + to_string((ResultPos.x / ResultOldPos.x)), 600);

				// 直した向きからいるべき方角を補正
				SetDirection(atan2f(NewPosTarget.x, NewPosTarget.z));

				// 補正方角から目標座標を設定
				AutoSetPosTarget();
				
				CRenderer::SetTimeString("通常座標系での座標:x " + to_string(GetPosTarget().x) + ":y " + to_string(GetPosTarget().y) + ":z " + to_string(GetPosTarget().z), 600);
#else
				// 過去の方角へ戻す
				SetDirection(GetOldDirection());

				// 目標座標を再設定
				AutoSetPosTarget();
#endif
				break;
			}
			case 4:	// 右
			{
				// 過去の方角へ戻す
				SetDirection(GetOldDirection());

				// 目標座標を再設定
				AutoSetPosTarget();

				break;
			}
			default:
				break;
			}

			break;
		}

		pObj = pObj->GetNext();
	}

	// 何にも衝突していなければ
	if (!bDetect)
	{
		// 判定表示を通常色に戻す
		m_pBndCylinder->ChangeModel(CX_Manager::TYPE::RENDER_CYLINDER);
	}
}