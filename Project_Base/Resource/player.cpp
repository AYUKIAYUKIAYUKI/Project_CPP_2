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
// デフォルトコンストラクタ
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
	// ステートー破棄
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

	// 自動で目標向きを移動方向に向ける
	AutoSetRotTarget();

	// 自動で目標座標を変動した方角に合わせる
	AutoSetPosTarget();

	// 高さの補正
	AdjustHeight();

	// 当たり判定
	HitCheck();

	// キャラクタークラスの更新処理
	CCharacter::Update();

#ifdef _DEBUG

	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("プレイヤー方角　　 : " + to_string(GetDirection() * (180 / D3DX_PI)));
	CRenderer::SetDebugString("プレイヤー速度　　 : " + to_string(GetMoveSpeed()));
	CRenderer::SetDebugString("プレイヤー向き　　 : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("プレイヤー目標向き : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("プレイヤー現在座標 : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::SetDebugString("プレイヤー目標座標 : " + to_string(GetPosTarget().x) + " :  " + to_string(GetPosTarget().y) + " : " + to_string(GetPosTarget().z));
	CRenderer::SetDebugString("プレイヤー加速度　 : " + to_string(GetAccelY()));
	CRenderer::SetDebugString("プレイヤー体力　　 : " + to_string(GetLife()));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");

#endif // _DEBUG
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
	pNewInstance->BindModel(CModel_X_Manager::TYPE::SAMUS);

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
	NewPosTarget.y += GetAccelY();
	SetPosTarget(NewPosTarget);

	// 高さの目標に下限を設定
	if (GetPosTarget().y < m_pBndCylinder->GetHeight())
	{
		// 目標座標を下限に固定
		NewPosTarget.y = m_pBndCylinder->GetHeight();
		SetPosTarget(NewPosTarget);

		// Y軸方向の加速度を初期化
		SetAccelY(0.0f);
	}
}

//============================================================================
// 当たり判定
//============================================================================
void CPlayer::HitCheck()
{
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

		// ボックスの中心点からシリンダーの座標への相対座標を計算
#if 0
		const Vec3& RelativePos = CylinderPosTarget - BoxPos;
		const Vec3& RelativeOldPos = CylinderOldPos - BoxPos;
#else
		Vec3 RelativePos = BoxPos - CylinderPosTarget;
		RelativePos.y = -RelativePos.y;
		Vec3 RelativeOldPos = BoxPos - CylinderOldPos;
		RelativeOldPos.y = -RelativeOldPos.y;
#endif

		// 相対座標に、ボックスの回転角度分を打ち消す回転行列を適用
		const Vec3& ResultPos = utility::RotatePointAroundY(fBoxDirection, RelativePos);
		const Vec3& ResultOldPos = utility::RotatePointAroundY(fBoxDirection, RelativeOldPos);

		CRenderer::SetDebugString("プあああああああ : " + to_string(BoxPos.x) + " :  " + to_string(BoxPos.y) + " : " + to_string(BoxPos.z));
		CRenderer::SetDebugString("プレイヤーrelati : " + to_string(RelativePos.x) + " :  " + to_string(RelativePos.y) + " : " + to_string(RelativePos.z));
		CRenderer::SetDebugString("プレイヤーoldrel : " + to_string(RelativeOldPos.x) + " :  " + to_string(RelativeOldPos.y) + " : " + to_string(RelativeOldPos.z));
		CRenderer::SetDebugString("プレイヤーbububu : " + to_string(ResultPos.x) + " :  " + to_string(ResultPos.y) + " : " + to_string(ResultPos.z));
		CRenderer::SetDebugString("プレイヤーbibibi : " + to_string(ResultOldPos.x) + " :  " + to_string(ResultOldPos.y) + " : " + to_string(ResultOldPos.z));

		// ボックスの回転量を打ち消したと仮定し、シリンダーの相対座標を用いて衝突判定
		// (ボックスの座標に関わらず、仮定したAABBとシリンダーの相対距離で判定するだけなので、渡すボックス座標は原点にする)
		if (collision::HitCylinderToAABB(ResultPos, CylinderRadius, CylinderHeight, VEC3_INIT, BoxSize))
		{
			// 判定表示を赤色に
			m_pBndCylinder->ChangeModel(CModel_X_Manager::TYPE::RENDER_CYLINDER_HIT);
				
			// 衝突があったことを検出
			bDetect = 1;
			
			// 衝突面を判定
			int nIdx = collision::GetCylinderToAABB(ResultOldPos, ResultPos, CylinderRadius, CylinderHeight, VEC3_INIT, BoxSize);

			switch (nIdx)
			{
			case 0:
				break;

			case 1:
			{
				// Y軸方向の加速度を初期化
				SetAccelY(0.0f);

				// 新しい目標座標を作成
				const Vec3& NewPosTarget = { CylinderPosTarget.x, BoxPos.y + BoxSize.y + CylinderHeight, CylinderPosTarget.z };
			 
				// 目標座標を反映
				SetPosTarget(NewPosTarget);
			
				break;
			}
			case 2:
			{
				// Y軸方向の加速度を重力方向へ固定
				SetAccelY(0.0f);

				// 新しい目標座標を作成
				const Vec3& NewPosTarget = { CylinderPosTarget.x, BoxPos.y - BoxSize.y - CylinderHeight, CylinderPosTarget.z };

				// 目標座標を反映
				SetPosTarget(NewPosTarget);

				break;
			}
			case 3:
			{
				// 目標座標を反映
				SetPosTarget(CylinderOldPos);

				break;
			}
			case 4:
			{
				// 目標座標を反映
				SetPosTarget(CylinderOldPos);

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
		m_pBndCylinder->ChangeModel(CModel_X_Manager::TYPE::RENDER_CYLINDER);
	}
}