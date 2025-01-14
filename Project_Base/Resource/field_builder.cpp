//============================================================================
// 
// フィールドビルダー [field_builder.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "field_builder.h"
#include "field_manager.h"
#include "player.h"
#include "fan.h"

// システム部分
#include "manager.h"
#include "renderer.h"
#include "sound.h"

// 生成オブジェクト
#include "block.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"
#include "life.h"

/* 修正用 */
#define SAFE 0

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
//CField_Builder* CField_Builder::m_pFieldBuilder = nullptr;	// フィールドビルダーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CField_Builder::Update()
{
	// 扇形範囲の更新
	UpdateFan();

	// フィールドの更新
	UpdateField();

#ifdef _DEBUG
	// デバッグ表示
	PrintDebug();
#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CField_Builder::Draw()
{
#ifdef _DEBUG
	// 扇形範囲の描画
	m_pFan->Draw();
#endif // _DEBUG
}

//============================================================================
// ジャンプした回数のインクリメント
//============================================================================
void CField_Builder::IncrementCntJump()
{
	++m_ActionData.nCntJump;
}

//============================================================================
// ダッシュした回数のインクリメント
//============================================================================
void CField_Builder::IncrementCntDash()
{
	++m_ActionData.nCntDash;
}

//============================================================================
// 攻撃した回数のインクリメント
//============================================================================
void CField_Builder::IncrementCntSlash()
{
	++m_ActionData.nCntSlash;
}

//============================================================================
// ブロックの破壊数を取得
//============================================================================
int CField_Builder::GetCntDestroyBlock()
{
	return m_nCntDestroyBlock;
}

//============================================================================
// プレイヤーをセット
//============================================================================
void CField_Builder::SetSyncPlayer(const CPlayer* const pPlayer)
{
	m_pSyncPlayer = pPlayer;
}

//============================================================================
// 生成
//============================================================================
CField_Builder* CField_Builder::Create()
{
	// インスタンスを生成
	CField_Builder* pNewInstance = DBG_NEW CField_Builder();

	// 生成失敗
	if (pNewInstance == nullptr)
	{
		//return E_FAIL;
	}

	// フィールドビルダーの初期設定
	if (FAILED(pNewInstance->Init()))
	{
		//return E_FAIL;
	}

	return pNewInstance;
}

//============================================================================
// 解放
//============================================================================
void CField_Builder::Release()
{
	if (this != nullptr)
	{
		// 終了処理
		Uninit();

		// メモリを解放
		delete this;
	}
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CField_Builder::CField_Builder() :
	m_nCntDestroyBlock{ 0 },
	m_FiledType{ FIELD_TYPE::NORMAL },
	m_pSyncPlayer{ nullptr },
	m_pFan{ nullptr }
{
	// アクションデータの初期化
	m_ActionData.nCntDash = 0;
	m_ActionData.nCntJump = 0;
	m_ActionData.nCntSlash = 0;
}

//============================================================================
// デストラクタ
//============================================================================
CField_Builder::~CField_Builder()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CField_Builder::Init()
{
	// 扇形範囲を生成
	m_pFan = CFan::Create();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CField_Builder::Uninit()
{
	// 扇形範囲を破棄
	if (m_pFan != nullptr)
	{
		m_pFan->Release();	// 解放
		m_pFan = nullptr;	// ポインタを初期化
	}
}

//============================================================================
// 扇形の更新
//============================================================================
void CField_Builder::UpdateFan()
{
	// プレイヤー情報がセットされていなければ処理を行わない
	if (m_pSyncPlayer == nullptr)
		return;

	// プレイヤーの現在の方角を扇形の方角にする
	m_pFan->SetDirection(m_pSyncPlayer->GetDirection());

	// 扇形範囲の更新処理
	m_pFan->Update();
}

//============================================================================
// フィールド更新
//============================================================================
void CField_Builder::UpdateField()
{
	// ブロックの破壊カウントが上限に達していたら処理を行わない
	if (m_nCntDestroyBlock >= CField_Manager::MAX_DESTROY_BLOCK)
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
void CField_Builder::BranchFieldType()
{
	/* ここは何らかの分岐を設定予定です */
	m_FiledType = FIELD_TYPE::JUMP;
}

//============================================================================
// アイテムの自動生成
//============================================================================
void CField_Builder::AutoCreateItem()
{
	// 既にアイテムが1つ以上存在していれば処理をしない
	if (CObject::FindSpecificObject(CObject::TYPE::ITEM))
		return;

	// アイテム用ポインタ
	CItem* pItem = nullptr;

#if SAFE
	// アイテムを生成
	if (m_bFirstItem)
	{
		pItem = CLife::Create();

		do { // この方角における座標が、扇形範囲内であれば方角を再抽選する

			// 方角をランダムに設定
			pItem->SetDirection(fabsf(utility::GetRandomValue<float>()));

		} while (m_pFan->DetectInFanRange(pItem->GetPos()));

		/* 応急処置 */
		float fHeight = fabsf(utility::GetRandomValue<float>());
		if (fHeight >= 125.0f)
		{
			fHeight = 125.0f;
		}

		// Y座標をランダムに設定
		pItem->SetPosY(fHeight);

		// 描画前に一度更新
		pItem->Update();
	}
	else
#endif // SAFE
	{ // 初回のみ定位置に生成する

		pItem = CLife::Create();

		// 方角を設定
		pItem->SetDirection(D3DX_PI * 0.5f);

		// 高さに設定
		pItem->SetPosY(100.0f);

		// 描画前に一度更新
		pItem->Update();

#if SAFE
		// 最初のアイテムの生成完了
		m_bFirstItem = true;
#endif // SAFE
	}

	// アイテム出現音を鳴らす
	CSound::GetInstance()->Play(CSound::LABEL::IAPPEAR);
}

//============================================================================
// ダッシュタイプの自動生成
//============================================================================
void CField_Builder::AutoCreateBlockDash()
{
	// 生成座標計算用 ((方角 + 扇形幅の角度)の場所が生成ポイント)
	float fDirection = m_pSyncPlayer->GetDirection();	// プレイヤーの現在の方角をコピー
	float fRange = m_pFan->GetRange();					// 扇形範囲の幅をコピー
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
		NewPos.x = cosf(fDirection + fRange) * CField_Manager::FIELD_RADIUS;
		NewPos.y = fabsf(utility::GetRandomValue<float>());
		NewPos.z = sinf(fDirection + fRange) * CField_Manager::FIELD_RADIUS;

		// ブロック同士の幅を検出
		if (DetectNearBlock(NewPos))
		{
			return;
		}

	} while (!m_pFan->DetectInFanRange(NewPos));

	// 向きを決定
	NewRot.y = atan2f(-NewPos.x, -NewPos.z);

	// ブロックを生成
	CBlock::Create(NewPos, NewRot);
}

//============================================================================
// 隣接し合うブロックを検出
//============================================================================
bool CField_Builder::DetectNearBlock(D3DXVECTOR3 Pos)
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
void CField_Builder::AutoDestroyBlock()
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

			if (!m_pFan->DetectInFanRange(pBlock->GetPos()))
			{ // 扇形の範囲内にブロックが無ければ

				// ブロックを破棄
				pBlock->SetRelease();

				// ブロックの破壊カウントを増加
				++m_nCntDestroyBlock;

				// ブロックの破壊数カウントが最大で
				if (m_nCntDestroyBlock >= CField_Manager::MAX_DESTROY_BLOCK)
				{
#if SAFE
					// 銅像を振動モーションに変更
					m_pStatue->SetNowMotion(0);
#endif // SAFE

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
void CField_Builder::DestroyAllBlock()
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
// デバッグ表示
//============================================================================
void CField_Builder::PrintDebug()
{
	// 外積確認用
	Vec3 OldVec = m_pSyncPlayer->GetPos() - VEC3_INIT, NewVec = m_pSyncPlayer->GetPosTarget() - VEC3_INIT;
	D3DXVec3Normalize(&OldVec, &OldVec);
	D3DXVec3Normalize(&NewVec, &NewVec);
	float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

	// ノルム確認用
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Field Builder Data")) {
		ImGui::Text("DestroyBlock:%d", m_nCntDestroyBlock);
		ImGui::Text("CountJump:%d", m_ActionData.nCntJump);
		ImGui::Text("CountDash:%d", m_ActionData.nCntDash);
		ImGui::Text("FieldType:%d", m_FiledType);
		ImGui::Text("Cross:%f", fCross);
		ImGui::Text("Norm:%f", Norm.x * Norm.x + Norm.z * Norm.z);
		ImGui::End();
	}
}