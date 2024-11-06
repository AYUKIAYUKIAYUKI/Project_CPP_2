//============================================================================
// 
// エネミー [enemy.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "enemy.h"

#include "player.h"
#include "player_state_damage.h"

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
CEnemy::CEnemy() :
	CCharacter{}
{

}

//============================================================================
// デストラクタ
//============================================================================
CEnemy::~CEnemy()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CEnemy::Init()
{
	// キャラクターの初期設定
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "エネミークラスの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CEnemy::Uninit()
{
	// キャラクタークラスの終了処理
	CCharacter::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CEnemy::Update()
{
	// 方角の取得
	float fNewDirection = GetDirection();
	CUtility::AdjustDirection(fNewDirection);
	fNewDirection += -DEFAULT_MOVE_SPEED * 3.0f;
	SetDirection(fNewDirection);

	// プレイヤーへの攻撃
	AttackOnPlayer();

	// キャラクタークラスの更新処理
	CCharacter::Update();

#if 0
#ifdef _DEBUG

	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("エネミー体力　　 : " + to_string(GetLife()));
	CRenderer::SetDebugString("エネミー方角　　 : " + to_string(GetDirection() * (180 / D3DX_PI)));
	CRenderer::SetDebugString("エネミー座標　　 : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::SetDebugString("エネミー速度　　 : " + to_string(GetMoveSpeed()));
	CRenderer::SetDebugString("エネミー向き　　 : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("エネミー目標向き : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");

#endif // _DEBUG
#endif
}

//============================================================================
// 描画処理
//============================================================================
void CEnemy::Draw()
{
	// キャラクターの描画処理
	CCharacter::Draw();
}

//============================================================================
// 生成
//============================================================================
CEnemy* CEnemy::Create()
{
	// インスタンスを生成
	CEnemy* pNewInstance = DBG_NEW CEnemy();

	// タイプを設定
	pNewInstance->SetType(TYPE::ENEMY);

	// 初期設定
	pNewInstance->Init();

	// モデルを設定
	pNewInstance->BindModel(CModel_X_Manager::TYPE::ENEMY);

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// プレイヤーへ攻撃
//============================================================================
void CEnemy::AttackOnPlayer()
{		
	// プレイヤーのポインタを作成
	CPlayer* pPlayer = nullptr;

	// プレイヤーを検索
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		// プレイヤーオブジェクトを保持
		pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// 既にダメージ状態の場合は処理を終了
		if (typeid(*pPlayer->GetNowState()) == typeid(CPlayer_State_Damage))
		{
			return;
		}

		// プレイヤーの持つ円柱範囲内に侵入していたら
		if (CUtility::CylinderAndPoint(pPlayer->GetPos(), 10.0f, 100.0f, GetPos()))
		{
			// ダメージ状態へ
			pPlayer->To_Damage(-1);
		}
	}
	else
	{
		assert(false && "プレイヤーの検索結果がありませんでした");
	}
}