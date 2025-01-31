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
#include "field_builder.h"
#include "field_type.h"
#include "object_HUD.h"
#include "object_PopUp.h"
#include "motion_set.h"
#include "player.h"

// システム部分
#include "manager.h"
#include "renderer.h"
//#include "scene.h"
#include "sound.h"
#include "game.h"

// 生成オブジェクト
#include "boss.h"
#include "block.h"
#include "life.h"
#include "bright.h"
#include "sparks.h"

/* デバッグ用 */
namespace
{
	// 初期配置ブロックの調整蝶ポインタ
	CBlock* pAdjusrInitBlock = nullptr;
}

/* 修正用 */
#define SAFE 1

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CField_Manager* CField_Manager::m_pFieldManager = nullptr;	// フィールドマネージャーの本体
bool			CField_Manager::m_bRetry = false;			// 周回判定

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
// ゲーム向け追加初期設定
//============================================================================
void CField_Manager::InitForGame()
{
	if (!m_pFieldBuilder)
	{
		// フィールドビルダーの生成
		m_pFieldBuilder = CField_Builder::Create();

		// フィールドビルダーにプレイヤーをセット
		m_pFieldBuilder->SetSyncPlayer(m_pSyncPlayer);
	}

	// 初期ブロックのセット
	InitBlockSet();
}

//============================================================================
// 更新処理
//============================================================================
void CField_Manager::Update()
{
	// この更新処理がゲームシーン以外で呼ばれても処理を行わない
	if (typeid(*CScene_Manager::GetInstance()->GetScene()) != typeid(CGame))
		return;

	// フェーズの更新
	UpdatePhase();

	// 環境装飾の更新
	UpdateEnvironment();

	// 5フェーズ以降に進行していたら
	if (m_nPhase >= 5)
	{
		// フィールドビルダーの更新
		m_pFieldBuilder->Update();

		// ボス登場イベント
		AppearBossEvent();
	}

	// シーン遷移を通知する
	NotifySceneTransition();

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
	// フィールドビルダーの描画処理
	m_pFieldBuilder->Draw();
}

//============================================================================
// 座標をフィールドサイズに調整
//============================================================================
bool CField_Manager::AdjustPosToFieldSize(CObject* pObj)
{
	// モーションセットクラスにダウンキャスト
	CMotion_Set* pMotion = utility::DownCast<CMotion_Set, CObject>(pObj);

	// 現在の座標から
	Vec3 NewPos = pMotion->GetPos();

	// 角度を割り出し
	float fDirection = atan2f(-NewPos.z, NewPos.x);

	// 同じ方角でフィールドサイズ分の座標へ整える
	NewPos = {
		cosf(fDirection) * CField_Manager::FIELD_RADIUS,
		NewPos.y,
		sinf(fDirection) * CField_Manager::FIELD_RADIUS
	};

	// 座標をセット
	pMotion->SetPos(NewPos);

	return true;
}

//============================================================================
// におふぇいおふぃおあ
//============================================================================
void CField_Manager::AAABBB()
{
	// 銅像を振動モーションに変更
	m_pStatue->SetNowMotion(0);

	// 消滅設定
	if (m_pPopUp)
	{
		m_pPopUp->SetDisappear();
		m_pPopUp = nullptr;
	}
}

//============================================================================
// フィールドビルダーの取得
//============================================================================
CField_Builder* const CField_Manager::GetFieldBuilder() const
{
	return m_pFieldBuilder;
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
	m_pFieldBuilder{ nullptr },
	m_nPhase{ 0 },
	m_pPopUp{ nullptr },
	m_pStatue{ nullptr },
	m_nCntStatueVibration{ 0 },
	m_pSyncPlayer{ nullptr }
{
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
	/* 現在は無し */

	return S_OK;
}

//============================================================================
// 環境装飾の初期設定
//============================================================================
void CField_Manager::InitEnvironment()
{
	{ // 銅像の生成

		// パラメータを反映
		m_pStatue = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_motion.json"));
		auto StatueParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_param.json");
		m_pStatue->SetRot(utility::JsonConvertToVec3(StatueParam["Rot"]));
		m_pStatue->SetPos(utility::JsonConvertToVec3(StatueParam["Pos"]));

		// 初期モーションを設定
		m_pStatue->SetNowMotion(2);
	}

	{ // 真実の銅像を生成

		// パラメータを反映
		CMotion_Set* pTrueStatue = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::LATEDRAW, utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_motion.json"));
		auto StatueParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\STATUE\\statue_param.json");
		pTrueStatue->SetRot(utility::JsonConvertToVec3(StatueParam["Rot"]));
		pTrueStatue->SetPos(utility::JsonConvertToVec3(StatueParam["Pos"]));

		// 破壊モーションを設定
		pTrueStatue->SetNowMotion(2);
	}
}

//============================================================================
// 初期ブロックのセット
//============================================================================
void CField_Manager::InitBlockSet()
{
	// 方角格納用
	float fDirection = 0.0f;

	{ // 左側の道をふさぐ

		// 初期のプレイヤー位置から見て左側を取る方角
		fDirection = D3DX_PI * -0.6f;

		// 高いブロックを縦に並べる
		for (WORD wCntBlock = 0; wCntBlock < 2; wCntBlock++)
		{
			// 新規向き・新規座標格納
			Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

			// 生成用の座標を決定
			NewPos = utility::DirectionConvertVec3(fDirection, 20.0f + (40.0f * wCntBlock), FIELD_RADIUS);

			// 向きを決定
			NewRot.y = atan2f(-NewPos.x, -NewPos.z);

			// ブロックを生成
			CBlock* pBlock = CBlock::Create(NewPos, NewRot);

			// ブロックタイプを固定
			if (pBlock->GetModelType() != CX_Manager::TYPE::BLOTALL)
			{
				// 高いブロックモデルを割り当てる
				pBlock->BindModel(CX_Manager::TYPE::BLOTALL);

				// バウンディングサイズを割り当て直す
				pBlock->SetSize(pBlock->GetModel()->Size);
			}
		}
	}

	{ // 右側に行けるようにブロック配置

		// 右側のブロックの方角を用意
		fDirection = D3DX_PI * -0.4f;

		// 新規向き・新規座標格納
		Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

		// 生成用の座標を決定
		NewPos = utility::DirectionConvertVec3(fDirection, 20.0f, FIELD_RADIUS);

		// 向きを決定
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// ブロックを生成
		CBlock* pBlock = CBlock::Create(NewPos, NewRot);

		// ブロックタイプを固定
		if (pBlock->GetModelType() != CX_Manager::TYPE::BLOTALL)
		{
			// 高いブロックモデルを割り当てる
			pBlock->BindModel(CX_Manager::TYPE::BLOTALL);

			// バウンディングサイズを割り当て直す
			pBlock->SetSize(pBlock->GetModel()->Size);
		}
	}

	{ // ダッシュさせるため離れた場所にブロック配置

		// ブロックの方角を用意
		fDirection = D3DX_PI * -0.25f;

		// 新規向き・新規座標格納
		Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

		// 生成用の座標を決定
		NewPos = utility::DirectionConvertVec3(fDirection, 50.0f, FIELD_RADIUS);

		// 向きを決定
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// ブロックを生成
		CBlock* pBlock = CBlock::Create(NewPos, NewRot);

		// ブロックタイプを固定
		if (pBlock->GetModelType() != CX_Manager::TYPE::BLOSIDE)
		{
			// 平たいブロックモデルを割り当てる
			pBlock->BindModel(CX_Manager::TYPE::BLOSIDE);

			// バウンディングサイズを割り当て直す
			pBlock->SetSize(pBlock->GetModel()->Size);
		}
	}

	{ // 平たいブロックのの下に妨害壁

		// 右側のブロックの方角を用意
		fDirection = -0.736f;

		// 新規向き・新規座標格納
		Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

		// 生成用の座標を決定
		NewPos = utility::DirectionConvertVec3(fDirection, 20.0f, FIELD_RADIUS);

		// 向きを決定
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// ブロックを生成
		CBlock* pBlock = CBlock::Create(NewPos, NewRot);

		// ブロックタイプを固定
		if (pBlock->GetModelType() != CX_Manager::TYPE::BLOTALL)
		{
			// 高いブロックモデルを割り当てる
			pBlock->BindModel(CX_Manager::TYPE::BLOTALL);

			// バウンディングサイズを割り当て直す
			pBlock->SetSize(pBlock->GetModel()->Size);
		}
	}

	{ // 敵から逃げるためのブロック

		// 初期のプレイヤー位置から見て左側を取る方角
		fDirection = D3DX_PI * -0.12f;

		// 新規向き・新規座標格納
		Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

		// 生成用の座標を決定
		NewPos = utility::DirectionConvertVec3(fDirection, 30.0f, FIELD_RADIUS);

		// 向きを決定
		NewRot.y = atan2f(-NewPos.x, -NewPos.z);

		// ブロックを生成
		CBlock* pBlock = CBlock::Create(NewPos, NewRot);

		// ブロックタイプを固定
		if (pBlock->GetModelType() != CX_Manager::TYPE::BLOSIDE)
		{
			// 平たいブロックモデルを割り当てる
			pBlock->BindModel(CX_Manager::TYPE::BLOSIDE);

			// バウンディングサイズを割り当て直す
			pBlock->SetSize(pBlock->GetModel()->Size);
		}
	}

	{ // 右側の道をふさぐ

		// 右端の方角を設定
		fDirection = D3DX_PI * -0.01f;

		// 高いブロックを縦に並べる
		for (WORD wCntBlock = 0; wCntBlock < 3; wCntBlock++)
		{
			// 新規向き・新規座標格納
			Vec3 NewRot = VEC3_INIT, NewPos = VEC3_INIT;

			// 生成用の座標を決定
			NewPos = utility::DirectionConvertVec3(fDirection, 20.0f + (40.0f * wCntBlock), FIELD_RADIUS);

			// 向きを決定
			NewRot.y = atan2f(-NewPos.x, -NewPos.z);

			// ブロックを生成
			CBlock* pBlock = CBlock::Create(NewPos, NewRot);

			// ブロックタイプを固定
			if (pBlock->GetModelType() != CX_Manager::TYPE::BLOTALL)
			{
				// 高いブロックモデルを割り当てる
				pBlock->BindModel(CX_Manager::TYPE::BLOTALL);

				// バウンディングサイズを割り当て直す
				pBlock->SetSize(pBlock->GetModel()->Size);
			}
		}
	}
}

//============================================================================
// 終了処理
//============================================================================
void CField_Manager::Uninit()
{
	// フィールドビルダーの破棄
	if (m_pFieldBuilder)
	{
		// 解放処理
		m_pFieldBuilder->Release();
	
		// ポインタを初期化
		m_pFieldBuilder = nullptr;
	}
}

//============================================================================
// フェーズの更新
//============================================================================
void CField_Manager::UpdatePhase()
{
#if 0
	// 2週目なら
	if (m_bRetry && m_nPhase == 0)
	{
		// 初回のフェーズを変更
		m_nPhase = 11;
	}
#endif

	/* ブロックの破壊量を取得 */
	const int nCntDestroyBlock = m_pFieldBuilder->GetCntDestroyBlock();

	/* ブロックの破壊量が最大で強制的に最終フェーズへ */
	if (nCntDestroyBlock >= MAX_DESTROY_BLOCK)
	{
		m_nPhase = 10;
	}

	switch (m_nPhase)
	{
	case 0:

		// ポップアップを生成
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_0.json"));
		}

		// 最初の右のブロックを越したら
		if (m_pSyncPlayer->GetDirection() > D3DX_PI * -0.4f)
		{
			// 消滅設定
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
				m_pPopUp = nullptr;
			}

			// 次のフェーズへ
			++m_nPhase;
		}

		break;

	case 1:

		// ポップアップを生成
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_1.json"));
		}
		else
		{
			// プレイヤーへの同期
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// 目標向き・目標座標を格納

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y軸向きへプレイヤーの方角をコピー
			RotTarget.y += D3DX_PI * -0.5f;					// カメラの正面を向くように調整
			m_pPopUp->SetRotTarget(RotTarget);				// 目標向きをセット

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// プレイヤーの奥へ配置
			PosTarget.y += 30.0f;							// 見やすいよう少し高さを付ける
			m_pPopUp->SetPosTarget(PosTarget);				// 目標座標をセット
		}

		// 空中ダッシュ必須の高台を超えたら
		if (m_pSyncPlayer->GetDirection() > D3DX_PI * -0.235f)
		{
			// 消滅設定
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
				m_pPopUp = nullptr;
			}

			// 次のフェーズへ
			++m_nPhase;
		}

		break;

	case 2:

		// ポップアップを生成
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_2.json"));

			// この際にエネミーも生成
			CBright::Generate(utility::DirectionConvertVec3(D3DX_PI * -0.12f, 95.0f, CField_Manager::FIELD_RADIUS), CBright::CREATETYPE::FLYER);
		}
		else
		{
			// プレイヤーへの同期
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// 目標向き・目標座標を格納

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y軸向きへプレイヤーの方角をコピー
			RotTarget.y += D3DX_PI * -0.5f;					// カメラの正面を向くように調整
			m_pPopUp->SetRotTarget(RotTarget);				// 目標向きをセット

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// プレイヤーの奥へ配置
			PosTarget.y += 30.0f;							// 見やすいよう少し高さを付ける
			m_pPopUp->SetPosTarget(PosTarget);				// 目標座標をセット
		}

		{ // 閃光エフェクトの残存中は以降の処理をしない

			// 通常優先度のオブジェクトを取得
			CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

			while (pObj != nullptr)
			{
				if (typeid(*pObj) == typeid(CBright))
				{
					return;
				}

				pObj = pObj->GetNext();
			}
		}

		// エネミーが全滅したら
		if (!CObject::CountSpecificObject(CObject::TYPE::ENEMY))
		{
			// 消滅設定
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
				m_pPopUp = nullptr;
			}

			// 次のフェーズへ
			++m_nPhase;
		}

		break;

	case 3:

		// ポップアップを生成
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_3.json"));

			/* エネミーを生成していく */
			CBright::Generate(utility::DirectionConvertVec3(D3DX_PI * -0.5f, 50.0f, CField_Manager::FIELD_RADIUS), CBright::CREATETYPE::GHOST);
			CBright::Generate(utility::DirectionConvertVec3(D3DX_PI * -0.35f, 50.0f, CField_Manager::FIELD_RADIUS), CBright::CREATETYPE::MONSTER);
		}
		else
		{
			// プレイヤーへの同期
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// 目標向き・目標座標を格納

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y軸向きへプレイヤーの方角をコピー
			RotTarget.y += D3DX_PI * -0.5f;					// カメラの正面を向くように調整
			m_pPopUp->SetRotTarget(RotTarget);				// 目標向きをセット

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// プレイヤーの奥へ配置
			PosTarget.y += 30.0f;							// 見やすいよう少し高さを付ける
			m_pPopUp->SetPosTarget(PosTarget);				// 目標座標をセット
		}

		// 最初の右のブロックより戻ったら
		if (m_pSyncPlayer->GetDirection() < D3DX_PI * -0.4f)
		{
			// 消滅設定
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
				m_pPopUp = nullptr;
			}

			// 次のフェーズへ
			++m_nPhase;
		}

		break;

	case 4:

		{ // アイテムを生成
			CItem* pItem = CLife::Create();

			// 方角を設定
			pItem->SetDirection(D3DX_PI * 0.5f);

			// 高さに設定
			pItem->SetPosY(100.0f);

			// 描画前に一度更新
			pItem->Update();
		}

		// 次のフェーズへ
		++m_nPhase;

		break;

	case 5:

		// 移動による地形の再生成が、多少行われていた痕跡があれば
		if (nCntDestroyBlock >= 10)
		{
			// 次のフェーズへ
			++m_nPhase;
		}

		break;

	case 6:

		// ポップアップを生成
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_4.json"));
		}
		else
		{
			// プレイヤーへの同期
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// 目標向き・目標座標を格納

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y軸向きへプレイヤーの方角をコピー
			RotTarget.y += D3DX_PI * -0.5f;					// カメラの正面を向くように調整
			m_pPopUp->SetRotTarget(RotTarget);				// 目標向きをセット

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// プレイヤーの奥へ配置
			PosTarget.y += 30.0f;							// 見やすいよう少し高さを付ける
			m_pPopUp->SetPosTarget(PosTarget);				// 目標座標をセット
		}

		{ // アイテムオブジェクトを取得
			CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

			// アイテムオブジェクトが消えていたら
			if (!pObj)
			{
				// 消滅設定
				if (m_pPopUp)
				{
					m_pPopUp->SetDisappear();
					m_pPopUp = nullptr;
				}

				// 次のフェーズへ
				++m_nPhase;
			}
		}

		break;

	case 7:

		// ポップアップを生成
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_5.json"));
		}
		else
		{
			// プレイヤーへの同期
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// 目標向き・目標座標を格納

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y軸向きへプレイヤーの方角をコピー
			RotTarget.y += D3DX_PI * -0.5f;					// カメラの正面を向くように調整
			m_pPopUp->SetRotTarget(RotTarget);				// 目標向きをセット

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// プレイヤーの奥へ配置
			PosTarget.y += 30.0f;							// 見やすいよう少し高さを付ける
			m_pPopUp->SetPosTarget(PosTarget);				// 目標座標をセット
		}

		{ // アイテムオブジェクトを取得
			CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

			// アイテムオブジェクトが発見出来たら
			if (pObj)
			{
#if SAFE
				// アイテムクラスにダウンキャスト
				CItem* pItem = utility::DownCast<CItem, CObject>(pObj);

				Vec3 Distance = m_pSyncPlayer->GetPos() - pItem->GetPos();
				float fDistanceNorm = Distance.x * Distance.x + Distance.z * Distance.z;

				// アイテムに接近したら
				if (fDistanceNorm <= CField_Type::GetAreaNorm())
				{
					// 消滅設定
					if (m_pPopUp)
					{
						m_pPopUp->SetDisappear();
						m_pPopUp = nullptr;
					}

					// 次のフェーズへ
					++m_nPhase;
				}
#endif // SAFE
			}
		}

		break;

	case 8:

		{ // アイテムオブジェクトを取得
			CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

			// アイテムオブジェクトが無くなっていたら
			if (!pObj)
			{
				// 次のフェーズへ
				++m_nPhase;
			}
		}

		break;

	case 9:

		// ポップアップを生成
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_6.json"));
		}
		else
		{
			// プレイヤーへの同期
			Vec3 RotTarget = VEC3_INIT, PosTarget = VEC3_INIT;	// 目標向き・目標座標を格納

			RotTarget.y = -m_pSyncPlayer->GetDirection();	// Y軸向きへプレイヤーの方角をコピー
			RotTarget.y += D3DX_PI * -0.5f;					// カメラの正面を向くように調整
			m_pPopUp->SetRotTarget(RotTarget);				// 目標向きをセット

			PosTarget = m_pSyncPlayer->GetPos() * 0.95f;	// プレイヤーの奥へ配置
			PosTarget.y += 30.0f;							// 見やすいよう少し高さを付ける
			m_pPopUp->SetPosTarget(PosTarget);				// 目標座標をセット
		}

		{ // アイテムオブジェクトを取得
			CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

			// アイテムオブジェクトが無くなっていたら
			if (!pObj)
			{
				// 消滅設定
				if (m_pPopUp)
				{
					m_pPopUp->SetDisappear();
					m_pPopUp = nullptr;
				}

				// フェーズ進行
				++m_nPhase;
			}
		}

		break;

		// 終点となるフェーズ
	case 10:

		// 周回判定を出す
		m_bRetry = true;

		//// ポップアップに消滅設定
		//if (m_pPopUp)
		//{
		//	m_pPopUp->SetDisappear();
		//	m_pPopUp = nullptr;
		//}

		break;

		// 2週目以降はこのフェーズ
	case 11:

		// ポップアップを生成
		if (!m_pPopUp)
		{
			m_pPopUp = CObject_PopUp::Create(utility::OpenJsonFile("Data\\JSON\\POPUP\\popup_ex.json"));
		}

		// ある程度移動していたら
		if (nCntDestroyBlock > 25)
		{
			// 消滅設定
			if (m_pPopUp)
			{
				m_pPopUp->SetDisappear();
				m_pPopUp = nullptr;
			}

			// 次のフェーズへ
			++m_nPhase;
		}

		break;
	}
}

//============================================================================
// 環境装飾の更新
//============================================================================
void CField_Manager::UpdateEnvironment()
{
	// 火の粉を生成
	CSparks::AutoGenerate();

#ifdef _DEBUG
	CSparks::EditInitParam();
#endif // _DEBUG
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

	// フォグ範囲を拡大
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

	// BGMを遷移
	CSound::GetInstance()->Transition(CSound::LABEL::FILEDBGM, CSound::LABEL::BOSSBGM);

	/* 一時的にボス登場イベント時にアイテムを強制削除 */
#if SAFE

	{ // アイテムオブジェクトを取得
		CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

		// アイテムオブジェクトが発見出来たら
		if (pObj)
		{
			// このオブジェクトに破棄予約
			pObj->SetRelease();
		}
	}

#endif
}

//============================================================================
// シーン遷移を通知する
//============================================================================
void CField_Manager::NotifySceneTransition()
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
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Field Manager Data")) {
		ImGui::Text("Phase:%d", m_nPhase);
		ImGui::End();
	}

#if 1	// フェーズスキップ
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Skip Phase")) {
		if (ImGui::Button("Skip to 5")) {
			m_nPhase = 5;
		}
		ImGui::End();
	}
#endif

#if 0	// 初期配置ブロックの調整
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("InitBlock Edit")) {
		if (!pAdjusrInitBlock)
		{

		}
		ImGui::End();
	}
#endif

#if 1	// ボスを呼び出す
	if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT) &&
		CManager::GetKeyboard()->GetTrigger(DIK_SPACE))
	{
		m_nPhase = 11;
		m_pStatue->SetNowMotion(0);
	}
#endif
}