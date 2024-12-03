//============================================================================
// 
// フィールドマネージャー [field_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "field_manager.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"
#include "motion_set.h"
#include "object_HUD.h"
#include "player.h"
#include "boss.h"
#include "block.h"
#include "sparks.h"

// デバッグ表示用
#include "fan.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define CHANGE_FIELRDCREATE_STYLE 0	// ブロックの生成方法切り替え

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CField_Manager* CField_Manager::m_pFieldManager = nullptr;	// フィールドマネージャーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// タイトル向け追加初期設定
//============================================================================
void CField_Manager::InitForTitle()
{
	// 環境装飾の初期設定
	InitEnvironment();
}

//============================================================================
// 更新処理
//============================================================================
void CField_Manager::Update()
{
	// 環境装飾の更新
	UpdateEnvironment();

	if (m_pSyncPlayer != nullptr)
	{
		// プレイヤーの現在の方角を扇形の方角にする
		m_pRenderFan->SetDirection(m_pSyncPlayer->GetDirection());

		// 扇形表示の更新処理
		m_pRenderFan->Update();
	}

	if (typeid(*CScene_Manager::GetInstance()->GetScene()) == typeid(CGame))
	{
		// ボス登場イベント
		AppearBossEvent();

		// 体力が無くなるとゲームシーンにゲーム終了を通知する
		if (m_pSyncPlayer->GetLife() <= 0)
		{
			// ゲームシーン取得
			CGame* const pScene = dynamic_cast<CGame*>(CScene_Manager::GetInstance()->GetScene());

			// シーン遷移開始
			pScene->SetTransition();
		}
	}

	// フィールドジェネレータ
	if(m_nCntDestroyBlock < MAX_DESTROY_BLOCK)
		FieldGenerator();

	// デバッグ表示
	PrintDebug();
}

//============================================================================
// 描画処理
//============================================================================
void CField_Manager::Draw()
{
	// 扇形の描画処理
	m_pRenderFan->Draw();
}

//============================================================================
// ジャンプした回数のインクリメント
//============================================================================
void CField_Manager::IncrementCntJump()
{
	++m_ActionData.nCntJump;
}

//============================================================================
// ダッシュした回数のインクリメント
//============================================================================
void CField_Manager::IncrementCntDash()
{
	++m_ActionData.nCntDash;
}

//============================================================================
// ボス出現
//============================================================================
void CField_Manager::AppearBoss()
{
	// 銅像が振動を始める
	m_pStatue->SetNowMotion(0);
}

//============================================================================
// ブロックの破壊数を取得
//============================================================================
int CField_Manager::GetCntDestroyBlock()
{
	return m_nCntDestroyBlock;
}

//============================================================================
// プレイヤーをセット
//============================================================================
void CField_Manager::SetSyncPlayer(const CPlayer* const pPlayer)
{
	m_pSyncPlayer = pPlayer;
}

//============================================================================
// 生成
//============================================================================
HRESULT CField_Manager::Create()
{
	// 既に生成されていたら
	if (m_pFieldManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("フィールドマネージャーは既に生成されています", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// インスタンスを生成
	m_pFieldManager = DBG_NEW CField_Manager();

	// 生成失敗
	if (m_pFieldManager == nullptr)
	{
		return E_FAIL;
	}

	// フィールドマネージャーの初期設定
	if (FAILED(m_pFieldManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CField_Manager::Release()
{
	if (m_pFieldManager != nullptr)
	{
		// 終了処理
		m_pFieldManager->Uninit();

		// メモリを解放
		delete m_pFieldManager;

		// ポインタを初期化
		m_pFieldManager = nullptr;
	}
}

//============================================================================
// フィールドマネージャーを取得
//============================================================================
CField_Manager* CField_Manager::GetInstance()
{
	// 本体が存在しなければ
	if (m_pFieldManager == nullptr)
	{
		// 生成
		if (FAILED(Create()))
		{
			assert(false && "フィールドマネージャーの取得に失敗");
		}
	}

	return m_pFieldManager;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CField_Manager::CField_Manager() :
	m_FiledType{ FIELD_TYPE::NORMAL },
	m_nCntDestroyBlock{ 0 },
	m_pSyncPlayer{ nullptr },
	m_pStatue{ nullptr },
	m_nCntStatueVibration{ 0 },
	m_pRenderFan{ nullptr }
{
	// アクションデータの初期化
	m_ActionData.nCntDash = 0;
	m_ActionData.nCntJump = 0;
}

//============================================================================
// デストラクタ
//============================================================================
CField_Manager::~CField_Manager()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CField_Manager::Init()
{
	// 扇形表示を生成
	m_pRenderFan = CFan::Create();

#if CHANGE_FIELRDCREATE_STYLE
	// 円の生成
	DEBUG_CIRCLE();
#endif	// CHANGE_FIELRDCREATE_STYLE

	return S_OK;
}

//============================================================================
// 環境装飾の初期設定
//============================================================================
void CField_Manager::InitEnvironment()
{
	{ // 銅像の生成

		// パラメータを反映
		m_pStatue = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\statue_motion.json"));
		auto StatueParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\statue.json");
		m_pStatue->SetRot(utility::JsonConvertToVec3(StatueParam["Rot"]));
		m_pStatue->SetPos(utility::JsonConvertToVec3(StatueParam["Pos"]));
		
		// 初期モーションを設定
		m_pStatue->SetNowMotion(2);
	}
}

//============================================================================
// 終了処理
//============================================================================
void CField_Manager::Uninit()
{
	// 扇形表示を破棄
	if (m_pRenderFan != nullptr)
	{
		m_pRenderFan->Release();	// 解放
		m_pRenderFan = nullptr;		// ポインタを初期化
	}
}

//============================================================================
// 環境装飾の更新
//============================================================================
void CField_Manager::UpdateEnvironment()
{
	// 火の粉を生成
	CSparks::AutoGenerate();
}

//============================================================================
// ボス登場イベント
//============================================================================
void CField_Manager::AppearBossEvent()
{
	// 銅像のモーションが終了していたらこのメソッドを無視
	if (m_pStatue->GetStopState())
		return;

	// 再生中のモーションに応じて処理を分岐
	if (m_pStatue->GetNowMotion() == 0)
	{ // 銅像が振動モーション再生中

		// 銅像振動カウントをインクリメント
		++m_nCntStatueVibration;

		// カメラを振動させる
		CManager::GetManager()->GetCamera()->SetVibration(0.01f);

		// カメラをボス登場用にセット
		CManager::GetManager()->GetCamera()->SetAppearBoss();

		// カウントが最大値に達したら
		if (m_nCntStatueVibration >= MAX_CNT_STATUEVIBERATION)
		{
			// カウントをリセット
			m_nCntStatueVibration = 0;

			// 銅像が吹き飛ばされるモーションをセット
			m_pStatue->SetNowMotion(1);

			// ボスを生成する
			CBoss::Create();
		}
	}
	else if (m_pStatue->GetNowMotion() == 1)
	{ // 銅像が吹きとばされるモーションを再生中

		// 銅像振動カウントをインクリメント
		++m_nCntStatueVibration;

		// モーション内で銅像が崩壊するあたりまでカウントされたら
		if (m_nCntStatueVibration >= 40)
		{
			// カメラを振動させる
			CManager::GetManager()->GetCamera()->SetVibration(0.05f);
		}
	}
}

//============================================================================
// フィールドジェネレータ
//============================================================================
void CField_Manager::FieldGenerator()
{
	// フィールドタイプの分岐
	BranchFieldType();

#if !CHANGE_FIELRDCREATE_STYLE
	// プレイヤーの目標座標へのベクトルを作成
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	// プレイヤーの移動時のノルムに応じてブロック生成
	if (Norm.x * Norm.x + Norm.z * Norm.z > 0.1f)
	{
		// ブロックの自動生成
		//AutoCreateBlock(1);
		AutoCreateBlockDash();
	}

	// ブロックの自動削除
	AutoDestroyBlock();
#endif	// CHANGE_FIELRDCREATE_STYLE

#ifdef _DEBUG	// ブロックを全削除
	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
		DestroyAllBlock();
#endif // _DEBUG
}

//============================================================================
// フィールドタイプの分岐
//============================================================================
void CField_Manager::BranchFieldType()
{
	//if (m_ActionData.nCntJump > m_ActionData.nCntDash)
	if (0)
	{
		m_FiledType = FIELD_TYPE::JUMP;
	}
	else
	{
		m_FiledType = FIELD_TYPE::DASH;
	}
}

//============================================================================
// ブロックの自動生成
//============================================================================
void CField_Manager::AutoCreateBlock(int nAmount)
{
	// 生成座標計算用 ((方角 + 扇形幅の角度)の場所が生成ポイント)
	float fDirection = m_pSyncPlayer->GetDirection();	// プレイヤーの現在の方角をコピー
	float fRange = m_pRenderFan->GetRange();			// 扇形範囲の幅をコピー
	Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;		// ブロック用の座標・向きを作成

	// 現在座標と目標座標に対し原点からの方向ベクトルを作成
	Vec3 OldVec = m_pSyncPlayer->GetPos() - VEC3_INIT, NewVec = m_pSyncPlayer->GetPosTarget() - VEC3_INIT;
	D3DXVec3Normalize(&OldVec, &OldVec);
	D3DXVec3Normalize(&NewVec, &NewVec);

	// 2本の方向ベクトルの外積を作成
	float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

	// 左に移動している場合角度を反転させる
	if (fCross < 0.0f)
		fRange = -fRange;

	// ブロック数が上限に満たなければ
	for (int nCntBlock = 0; nCntBlock < nAmount; nCntBlock++)
	{
		// 破棄範囲にはみ出さず生成されるように調整
		/* 初期座標が原点の場合、生成範囲の半径がフィールドの半径を下回ると無限ループ */
		do
		{
			// 生成用の座標を決定
			NewPos.x = cosf(fDirection + fRange) * FIELD_RADIUS;
			NewPos.y = fabsf(utility::GetRandomValue<float>());
			NewPos.z = sinf(fDirection + fRange) * FIELD_RADIUS;

			// ブロック同士の幅を検出
			if (DetectNearBlock(NewPos))
			{
				return;

				//NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
			}

		} while (!m_pRenderFan->DetectInFanRange(NewPos));

		// 向きを決定
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// ブロックを生成
		CBlock::Create(NewPos, NewRot);
	}
}

//============================================================================
// ダッシュタイプの自動生成
//============================================================================
void CField_Manager::AutoCreateBlockDash()
{
	// 生成座標計算用 ((方角 + 扇形幅の角度)の場所が生成ポイント)
	float fDirection = m_pSyncPlayer->GetDirection();	// プレイヤーの現在の方角をコピー
	float fRange = m_pRenderFan->GetRange();			// 扇形範囲の幅をコピー
	Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;		// ブロック用の座標・向きを作成

	// 現在座標と目標座標に対し原点からの方向ベクトルを作成
	Vec3 OldVec = m_pSyncPlayer->GetPos() - VEC3_INIT, NewVec = m_pSyncPlayer->GetPosTarget() - VEC3_INIT;
	D3DXVec3Normalize(&OldVec, &OldVec);
	D3DXVec3Normalize(&NewVec, &NewVec);

	// 2本の方向ベクトルの外積を作成
	float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

	// 左に移動している場合角度を反転させる
	if (fCross < 0.0f)
		fRange = -fRange;

	// 破棄範囲にはみ出さず生成されるように調整
	/* 初期座標が原点の場合、生成範囲の半径がフィールドの半径を下回ると無限ループ */
	do
	{
		// 生成用の座標を決定
		NewPos.x = cosf(fDirection + fRange) * FIELD_RADIUS;
		NewPos.y = fabsf(utility::GetRandomValue<float>());
		NewPos.z = sinf(fDirection + fRange) * FIELD_RADIUS;

		// ブロック同士の幅を検出
		if (DetectNearBlock(NewPos))
		{
			return;

			//NewPos = { FLT_MAX, FLT_MAX, FLT_MAX };
		}

	} while (!m_pRenderFan->DetectInFanRange(NewPos));

	// 向きを決定
	NewRot.y = atan2f(-NewPos.x, -NewPos.z);

	// ブロックを生成
	CBlock::Create(NewPos, NewRot);
}

//============================================================================
// 隣接し合うブロックを検出
//============================================================================
bool CField_Manager::DetectNearBlock(D3DXVECTOR3 Pos)
{
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロックタグにダウンキャスト
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			// お互いの距離を求める
			const Vec3& Vec = pBlock->GetPos() - Pos;

			// ブロックのサイズをコピー
			const Vec3& Size = pBlock->GetSize();

			// ブロックのサイズぐらいに近づいていたら
			if ((Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= (Size.x * Size.x + Size.y * Size.y + Size.z * Size.z) * 10.0f)
			{
				// 座標の生成をやり直す
				return 1;
			}
		}

		pObj = pObj->GetNext();
	}

	return 0;
}

//============================================================================
// ブロックの自動削除
//============================================================================
void CField_Manager::AutoDestroyBlock()
{
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::MIDDLE);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロックタグにダウンキャスト
			CBlock* pBlock = nullptr;
			pBlock = utility::DownCast(pBlock, pObj);

			if (!m_pRenderFan->DetectInFanRange(pBlock->GetPos()))
			{ // 扇形の範囲内にブロックが無ければ

				// ブロックを破棄
				pBlock->SetRelease();
				
				// ブロックの破壊カウントを増加
				++m_nCntDestroyBlock;

				// ブロックの破壊数カウントが最大で
				if (m_nCntDestroyBlock >= MAX_DESTROY_BLOCK)
				{
					// ボス出現
					AppearBoss();

					// 全ブロックを破壊
					DestroyAllBlock();

					// 処理を強制終了
					return;
				}
			}
		}

		pObj = pObj->GetNext();
	}
}

//============================================================================
// 全ブロックの削除
//============================================================================
void CField_Manager::DestroyAllBlock()
{
	// ミドルオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::MIDDLE);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// 破棄予約
			pObj->SetRelease();
		}

		pObj = pObj->GetNext();
	}
}

//============================================================================
// デバッグ表示
//============================================================================
void CField_Manager::PrintDebug()
{
	// 外積確認用
	Vec3 OldVec = m_pSyncPlayer->GetPos() - VEC3_INIT, NewVec = m_pSyncPlayer->GetPosTarget() - VEC3_INIT;
	D3DXVec3Normalize(&OldVec, &OldVec);
	D3DXVec3Normalize(&NewVec, &NewVec);
	float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

	// ノルム確認用
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Action Data")) {
		ImGui::Text("CountJump:%d", m_ActionData.nCntJump);
		ImGui::Text("CountDash:%d", m_ActionData.nCntDash);
		ImGui::Text("FieldType:%d", m_FiledType);
		ImGui::Text("DestroyBlock:%d", m_nCntDestroyBlock);
		ImGui::Text("Cross:%f", fCross);
		ImGui::Text("Norm:%f", Norm.x * Norm.x + Norm.z * Norm.z);
		ImGui::End();
	}
}

//============================================================================
// デバッグサークル
//============================================================================
void CField_Manager::DEBUG_CIRCLE()
{
	float fDirection = 0.0f;						// 方角
	float fAdder = (D3DX_PI * 2.0f) / MAX_BLOCK;	// 増加量

	for (WORD i = 0; i < MAX_BLOCK; ++i)
	{
		// 生成用の座標を決定
		Vec3 NewPos = VEC3_INIT;
		NewPos.x = cosf(fDirection) * FIELD_RADIUS;
		NewPos.y = 40.0f - utility::GetRandomValue<float>() * 0.15f;
		NewPos.z = -sinf(fDirection) * FIELD_RADIUS;

		// 生成用の向きを決定
		Vec3 NewRot = VEC3_INIT;
		//NewRot.y = utility::GetRandomValue<float>();
		NewRot.y = fDirection + D3DX_PI * 0.5f;

		// ブロックを生成
		CBlock::Create(NewPos, NewRot);

		// 方角を増加
		fDirection += fAdder;
	}
}