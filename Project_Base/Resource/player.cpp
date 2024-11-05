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
#include "player_state_default.h"

// インプット取得
#include "manager.h"

// フィールドマネージャー
#include "field_manager.h"

// デバッグ表示用
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
	m_pStateManager{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CPlayer::~CPlayer()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CPlayer::Init()
{
	// ステートマネージャーの生成
	if (!m_pStateManager)
	{
		m_pStateManager = CCharacter_State_Manager::Create(DBG_NEW CPlayer_State_Default());
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
	
#ifdef _DEBUG

	// 新しい体力情報を作成
	int nNewLife = GetLife();

	// 体力を変動
	if (CManager::GetKeyboard()->GetTrigger(DIK_1))
	{
		nNewLife > 0 ? nNewLife += -1 : nNewLife = 0;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
	{
		nNewLife < MAX_LIFE ? nNewLife += 1 : nNewLife = MAX_LIFE;
	}

	// 変動した体力情報を反映
	SetLife(nNewLife);

#endif // _DEBUG

	// キャラクタークラスの更新処理
	CCharacter::Update();

#ifdef _DEBUG

	CRenderer::GetInstance()->SetDebugString("プレイヤー体力　　 : " + to_string(GetLife()));
	CRenderer::GetInstance()->SetDebugString("プレイヤー方角　　 : " + to_string(GetDirection() * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("プレイヤー座標　　 : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::GetInstance()->SetDebugString("プレイヤー速度　　 : " + to_string(GetMoveSpeed()));
	CRenderer::GetInstance()->SetDebugString("プレイヤー向き　　 : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("プレイヤー目標向き : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));

#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CPlayer::Draw()
{
	// キャラクタークラスの描画処理
	CCharacter::Draw();

#if 1	// ステンシルバッファにプレイヤーのシルエットを描画する

	// ステンシルバッファを有効化
	//CRenderer::GetInstance()->GetDeviece()->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// デプスバッファの比較方法を変更し無効化
	CRenderer::GetInstance()->GetDeviece()->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

	// キャラクタークラスの描画処理 (ステンシルバッファ)
	CCharacter::Draw();

	// ステンシルバッファを無効化
	//CRenderer::GetInstance()->GetDeviece()->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// デプスバッファの比較方法を通常に戻し有効化
	CRenderer::GetInstance()->GetDeviece()->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

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

	return pNewInstance;
}