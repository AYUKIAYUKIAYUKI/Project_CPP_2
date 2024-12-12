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
#include "monster.h"
#include "ghost.h"
//#include "flyer.h"
#include "boss.h"
#include "block.h"
#include "life.h"
#include "sparks.h"

// デバッグ表示用
#include "fan.h"

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
	// この更新処理がゲームシーン以外で呼ばれても処理を行わない
	if (typeid(*CScene_Manager::GetInstance()->GetScene()) != typeid(CGame))
		return;

	// 環境装飾の更新
	UpdateEnvironment();

	// 扇形の更新
	UpdateFan();

	// フィールド更新
	UpdateField();

	// ボス登場イベント
	AppearBossEvent();

	// 遷移を通知する
	NotifyTransition();

#ifdef _DEBUG
	// デバッグ表示
	PrintDebug();
#endif // _DEBUG
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

	// エネミーを生成
#if 1
	CMonster::Create();
#else
	CEnemy* pEnemy = CGhost::Create();
	pEnemy->SetDirection(-D3DX_PI);
	Vec3 PosTarget = {
		cosf(-D3DX_PI) * 300.0f,
		0.0f,
		sinf(D3DX_PI) * 300.0f,
	};
	pEnemy->SetPos(PosTarget);
	pEnemy->SetPosTarget(PosTarget);
#endif

	return S_OK;
}

//============================================================================
// 環境装飾の初期設定
//============================================================================
void CField_Manager::InitEnvironment()
{
	{ // 銅像の生成

		// パラメータを反映
		m_pStatue = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_motion.json"));
		auto StatueParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_param.json");
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
// 扇形の更新
//============================================================================
void CField_Manager::UpdateFan()
{
	// プレイヤー情報がセットされていなければ処理を行わない
	if (m_pSyncPlayer == nullptr)
		return;

	// プレイヤーの現在の方角を扇形の方角にする
	m_pRenderFan->SetDirection(m_pSyncPlayer->GetDirection());

	// 扇形表示の更新処理
	m_pRenderFan->Update();
}

//============================================================================
// フィールド更新
//============================================================================
void CField_Manager::UpdateField()
{
	// ブロックの破壊カウントが上限に達していたら処理を行わない
	if (m_nCntDestroyBlock >= MAX_DESTROY_BLOCK)
		return;

	// フィールドタイプの分岐
	BranchFieldType();

	// アイテムの自動生成
	AutoCreateItem();

	// プレイヤーの目標座標へのベクトルを作成
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	// プレイヤーの移動時のノルムに応じてブロック生成
	if (Norm.x * Norm.x + Norm.z * Norm.z > 0.1f)
	{
		// ブロックの自動生成
		AutoCreateBlockDash();
	}

	// ブロックの自動削除
	AutoDestroyBlock();

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
	/* ここは何らかの分岐を設定予定です */
	m_FiledType = FIELD_TYPE::JUMP;
}

//============================================================================
// アイテムの自動生成
//============================================================================
void CField_Manager::AutoCreateItem()
{
	// 既にアイテムが1つ以上存在していれば処理をしない
	if (CObject::FindSpecificObject(CObject::TYPE::ITEM))
		return;

	// アイテム用ポインタ
	CItem* pItem = nullptr;

	// アイテムを生成
	/* ここは何らかの分岐を設定予定です*/
	pItem = CLife::Create();

	do { // この方角における座標が、扇形範囲内であれば方角を再抽選する

		// 方角をランダムに設定
		pItem->SetDirection(fabsf(utility::GetRandomValue<float>()));

	} while (m_pRenderFan->DetectInFanRange(pItem->GetPos()));

	// Y座標をランダムに設定
	pItem->SetPosY(fabsf(utility::GetRandomValue<float>()));
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
	// 通常優先度のオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

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
	// 通常優先度のオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

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
					// 銅像を振動モーションに変更
					m_pStatue->SetNowMotion(0);

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
	// 通常優先度のオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

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
// ボス登場イベント
//============================================================================
void CField_Manager::AppearBossEvent()
{
	// 銅像のモーション再生が終了していたらこのメソッドを無視
	if (m_pStatue->GetStopState())
		return;

	// 銅像が待機モーションならこのメソッドを無視
	if (m_pStatue->GetNowMotion() == 2)
		return;

	CRenderer::CorrectFogEnd(1350.0f);

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
// 遷移を通知する
//============================================================================
void CField_Manager::NotifyTransition()
{
	// プレイヤー情報がセットされていなければ処理を行わない
	if (m_pSyncPlayer == nullptr)
		return;

	// 体力が無くなるとゲームシーンにゲーム終了を通知する
	if (m_pSyncPlayer->GetLife() <= 0)
	{
		// ゲームシーン取得
		CGame* const pScene = dynamic_cast<CGame*>(CScene_Manager::GetInstance()->GetScene());

		// シーン遷移開始
		pScene->SetTransition();
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

	// ボスを呼び出すデバッグコマンド
	if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT) &&
		CManager::GetKeyboard()->GetTrigger(DIK_SPACE))
	{
		m_nCntDestroyBlock = MAX_DESTROY_BLOCK;
		m_pStatue->SetNowMotion(0);
		DestroyAllBlock();
	}
}