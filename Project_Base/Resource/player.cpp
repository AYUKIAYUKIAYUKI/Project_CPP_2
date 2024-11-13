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
#include "bounding_sphere.h"
#include "player_state_default.h"

#include "manager.h"
#include "mask_rectangle.h"

#include "block.h"
#include "field_manager.h"

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
	m_pBndSphere{ DBG_NEW CBounding_Sphere(this) },
	m_pStateManager{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CPlayer::~CPlayer()
{
	// バウンディングスフィアの破棄
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
HRESULT CPlayer::Init()
{
	// ステートマネージャーの生成
	if (!m_pStateManager)
	{
		m_pStateManager = CCharacter_State_Manager::Create(DBG_NEW CPlayer_State_Default());	// 初期ステートを設定しておく
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
	// ステートマネージャーの破棄
	if (m_pStateManager)
	{
		// 解放
		m_pStateManager->Release();
		
		// ポインタを初期化
		m_pStateManager = nullptr;
	}

	// キャラクタークラスの終了処理
	CCharacter::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer::Update()
{		
	// ステートマネージャーの更新
	m_pStateManager->Update();

	// 当たり判定
	HitCheck();

	// 高さの補正
	AdjustHeight();

	// キャラクタークラスの更新処理
	CCharacter::Update();

#ifdef _DEBUG

	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("プレイヤー方角　　 : " + to_string(GetDirection() * (180 / D3DX_PI)));
	CRenderer::SetDebugString("プレイヤー速度　　 : " + to_string(GetMoveSpeed()));
	CRenderer::SetDebugString("プレイヤー向き　　 : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("プレイヤー目標向き : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("プレイヤー座標　　 : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
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
// ステート - ダメージ状態へ
//============================================================================
void CPlayer::To_Damage(int nDamage)
{
	// ダメージ量分、体力を変動
	 int nNewLife = GetLife();
	 nNewLife += nDamage;
	 SetLife(nNewLife);

	// マネージャーにダメージ状態へ変更するよう命令する
	m_pStateManager->To_Damage();
}

//============================================================================
// 半径を取得
//============================================================================
float CPlayer::GetRadius() const
{
	return m_pBndSphere->GetRadius();
}

//============================================================================
// 現在のステートを取得
//============================================================================
CCharacter_State* CPlayer::GetNowState()
{
	// ステートマネージャーの持つステートオブジェクトのポインタを返す
	return m_pStateManager->GetNowState();
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

	// サイズを設定
	pNewInstance->m_pBndSphere->SetRadius(10.0f);

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
	// 座標をY軸の加速度文変動する
	Vec3 NewPos = GetPos();
	NewPos.y += GetAccelY();
	SetPos(NewPos);

	// 高さに下限を設定
	if (GetPos().y < 0.0f)
	{
		// 座標を下限に固定
		NewPos.y = 0.0f;
		SetPos(NewPos);

		// Y軸の加速度を無くす
		SetAccelY(0.0f);
	}
}

//============================================================================
// 当たり判定
//============================================================================
void CPlayer::HitCheck()
{
	// ミドルオブジェクトを取得
	//CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));
	CObject* pObj = CObject::GetTopObject();

	while (pObj != nullptr)
	{
		// ブロッククラスにダウンキャスト
		CBlock* pBlock = CObject::DownCast<CBlock>(pObj);

		// 成功していれば
		if (pBlock)
		{
			// バウンディングボックスのパラメータを取得
			const Vec3& BoxSize = pBlock->GetSize();
			const Vec3& BoxPos = pBlock->GetPos();
			const float& fBoxDirection = pBlock->GetRot().y;

			// プレイヤーのバウンディングスフィアの中心点に、ブロックの回転角度分の逆回転行列をかける
			const Vec3& SpherePos = CUtility::RotatePointAroundY(fBoxDirection, m_pBndSphere->GetCenterPos());

			// 中心点とAABBで衝突判定を行う
			if (CUtility::SphereAndAABB(SpherePos, m_pBndSphere->GetRadius(), BoxPos, BoxSize))
			{
				const D3DXCOLOR& Col = { 1.0f, 0.0f, 0.0f, 0.0f };
				m_pBndSphere->SetColRenderSphere(Col);
			}
			else
			{

			}
		}

		pObj = pObj->GetNext();
	}
}