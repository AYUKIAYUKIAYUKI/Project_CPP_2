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
#include "field_type.h"
#include "field_type_normal.h"
#include "field_type_dash.h"
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
#include "wings.h"
#include "boots.h"
#include "powerstone.h"

/* 修正用 */
#define SAFE 1

//****************************************************
// 無名名前空間を定義
//****************************************************
namespace
{
	// ビルダーの更新を一時停止
	bool bStopBuilder = false;

	// ブロック編集用ウィンドウの表示
	bool bShowBlockEditWindow = false;

	// 編集用ブロック
	CBlock* pEditBlock = nullptr;
}

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

/* デバッグ用 -> ブロックの密集度合 */
float fBlockDensity = 2.5f;

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

	if (!bStopBuilder)
	{
		// ビルダーの更新
		UpdateBuilder();
	}

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
// 扇形範囲内に存在しているか判定
//============================================================================
bool CField_Builder::DetectInFanRange(D3DXVECTOR3 Pos)
{
	return m_pFan->DetectInFanRange(Pos);
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
		assert(false && "フィールドビルダーの生成に失敗");
	}

	// フィールドビルダーの初期設定
	if (FAILED(pNewInstance->Init()))
	{
		assert(false && "フィールドビルダーの初期設定エラー");
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
	m_upFieldType{ nullptr },
	m_nCntDestroyBlock{ 0 },
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
	// 最初のフィールドタイプを生成
	m_upFieldType = std::unique_ptr<CField_Type>(std::make_unique<CField_Type_Normal>().release());

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
// ビルダーの更新
//============================================================================
void CField_Builder::UpdateBuilder()
{
	// ブロックの破壊カウントが上限に達していたら処理を行わない
	if (m_nCntDestroyBlock >= CField_Manager::MAX_DESTROY_BLOCK)
		return;

	// フィールドタイプの分岐
	/* 一時的に手動で行うものとなります */
#ifdef _DEBUG
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Branch")) {
		if (ImGui::Button("to Dash")) {
			BranchFieldType();
		}
		ImGui::End();
	}
#endif // _DEBUG

	// アイテムの自動生成
	GenerateItem();

	// プレイヤーの目標座標へのベクトルを作成
	Vec3 Norm = m_pSyncPlayer->GetPosTarget() - m_pSyncPlayer->GetPos();

	// プレイヤーがある程度移動していることを検知した時のみ
	if (Norm.x * Norm.x + Norm.z * Norm.z > 0.1f)
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

		// ブロックの発生
		m_upFieldType->GenerateBlock(fRange + fDirection);
	}

	// 破壊判定
	DestroyCheck();

#ifdef _DEBUG	
	// ブロックを全削除
	if (CManager::GetKeyboard()->GetTrigger(DIK_DELETE))
		DestroyAllBlock();
#endif // _DEBUG
}

//============================================================================
// フィールドタイプの分岐
//============================================================================
void CField_Builder::BranchFieldType()
{
	// フィールドタイプクラスのユニークポインタ
	std::unique_ptr<CField_Type> upSuper = nullptr;

	/* なんらかの条件で新たなタイプを作成し */
	upSuper = std::make_unique<CField_Type_Dash>();

	// 状態を変更
	m_upFieldType = std::move(upSuper);
}

//============================================================================
// アイテムの自動生成
//============================================================================
void CField_Builder::GenerateItem()
{
	// 既にアイテムが1つ以上存在していれば処理をしない
	if (CObject::FindSpecificObject(CObject::TYPE::ITEM))
		return;

	// アイテム用ポインタ
	CItem* pItem = nullptr;

	// アイテム出現テーブル
	if (!m_pSyncPlayer->IsEnabledWings())
	{
		// 浮遊が有効化されていなければ羽を生成
		pItem = CWings::Create();
	}
	else if (!m_pSyncPlayer->IsEnabledBoots())
	{
		// 失踪が有効化されていなければブーツを生成
		pItem = CBoots::Create();
	}
	else if (!m_pSyncPlayer->IsEnabledPowerStone())
	{
		// 弾岩が有効化されていなければパワーストーンを生成
		pItem = CPowerStone::Create();
	}
	
	// 効果付与アイテムが何も生成されていないなら
	if (!pItem)
	{
		// ライフを生成
		pItem = CLife::Create();
	}

	do { // この方角における座標が、扇形範囲内であれば方角を再抽選する

		// 方角をランダムに設定
		pItem->SetDirection(fabsf(utility::GetRandomValue<float>()));

	} while (m_pFan->DetectInFanRange(pItem->GetPos()));

	// Y座標をランダムに設定
	float fHeight = 50.0f + fabsf(utility::GetRandomValue<float>());
	fHeight = utility::RoundToAnyMultiple<float>(fHeight, 20, 9);
	pItem->SetPosY(fHeight);

	// 描画前に一度更新
	pItem->Update();

	// アイテム出現音を鳴らす
	CSound::GetInstance()->Play(CSound::LABEL::IAPPEAR);
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
// 破壊判定
//============================================================================
void CField_Builder::DestroyCheck()
{
	// 通常レイヤーのオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// タイプをコピー
		const CObject::TYPE& Type = pObj->GetType();

		// タイプに応じて破棄の詳細を分岐
		if (Type == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロックタグにダウンキャスト
			CBlock* pBlock = utility::DownCast<CBlock, CObject>(pObj);

			// 扇形の範囲内にブロックが無ければ
			if (!m_pFan->DetectInFanRange(pBlock->GetPos()))
			{
				// このブロックは破棄
				pBlock->SetRelease();

				// ブロックの破壊量をインクリメント
				++m_nCntDestroyBlock;

				// ブロックの破壊量が最大カウントに達したら
				if (m_nCntDestroyBlock >= CField_Manager::MAX_DESTROY_BLOCK)
				{
#if SAFE
					// 銅像を振動モーションに変更
					CField_Manager::GetInstance()->AAABBB();
#endif // SAFE

					// 全ブロックを破壊
					DestroyAllBlock();

					// 処理を強制終了
					return;
				}
			}
		}
		else if (Type == CObject::TYPE::ENEMY)
		{
			// オブジェクトをエネミークラスにダウンキャスト
			CEnemy* pEnemy = utility::DownCast<CEnemy, CObject>(pObj);

			// 扇形の範囲外に出ていたら
			if (!m_pFan->DetectInFanRange(pEnemy->GetPos()))
			{
				// 敵を破棄
				pEnemy->SetRelease();
			}
		}

		// 次のオブジェクトを取得
		pObj = pObj->GetNext();
	}
}

//============================================================================
// 全ブロックの削除
//============================================================================
void CField_Builder::DestroyAllBlock()
{
	// 通常レイヤーのオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// ブロックタイプのオブジェクトなら
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// そのまま破棄予約
			pObj->SetRelease();
		}

		// 次のオブジェクトへ
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

	// アクションデータの表示
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Field Builder Data")) {
		ImGui::Text("DestroyBlock:%d", m_nCntDestroyBlock);
		ImGui::Text("CountJump:%d", m_ActionData.nCntJump);
		ImGui::Text("CountDash:%d", m_ActionData.nCntDash);
		ImGui::Text("FieldType:%s", typeid(*m_upFieldType).name());
		ImGui::Text("Cross:%f", fCross);
		ImGui::Text("Norm:%f", Norm.x * Norm.x + Norm.z * Norm.z);
		ImGui::End();
	}

	// 操作
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Control")) {

		// ビルダーの更新を停止
		ImGui::Checkbox("StopBuilder", &bStopBuilder);

		// ブロック編集用のウィンドウの表示
		ImGui::Checkbox("ShowBlockEditWindow", &bShowBlockEditWindow);

		// アイテムを削除
		if (ImGui::Button("Delete All Item"))
		{
			auto p = CObject::FindSpecificObject(CObject::TYPE::ITEM);
			
			if (p)
			{
				p->SetRelease();
			}
		}

		// ブロックを全て削除
		if (ImGui::Button("Delete All Block"))
		{
			DestroyAllBlock();
		}

		// エネミーを全て削除
		if (ImGui::Button("Delete All Enemy"))
		{
			// 通常レイヤーのオブジェクトを取得
			CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

			while (pObj != nullptr)
			{
				//　エネミータイプのオブジェクトなら
				if (pObj->GetType() == CObject::TYPE::ENEMY)
				{
					// そのまま破棄予約
					pObj->SetRelease();
				}

				// 次のオブジェクトへ
				pObj = pObj->GetNext();
			}
		}

		// ブロックの密集度合を変更
		ImGui::InputFloat("Density", &fBlockDensity);
		ImGui::Text("Area:%f", CField_Type::GetAreaNorm() * fBlockDensity);

		ImGui::End();
	}

	// 操作
	if (bStopBuilder && bShowBlockEditWindow) {
		ImGui::SetNextWindowSize({ -1, -1 });
		ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Block Edit")) {

			// 新しいブロックを作成
			if (!pEditBlock)
			{
				if (ImGui::Button("AddBlock"))
				{
					pEditBlock = CBlock::Create(VEC3_INIT, VEC3_INIT);
				}
			}

			// 編集用ブロックが存在していれば
			if (pEditBlock) {

				{ // ブロックモデルの変更

					// ブロックタイプをコピー
					CX_Manager::TYPE Type = pEditBlock->GetModelType();

					// 表示
					ImGui::Text("Type: %d", static_cast<WORD>(Type));
					ImGui::SameLine();

					// タイプを変更
					if (ImGui::Button("++## Type") && Type != CX_Manager::TYPE::BLOTALL)
					{
						Type = static_cast<CX_Manager::TYPE>(Type + 1);
					}
					ImGui::SameLine();
					if (ImGui::Button("--## Type") && Type != CX_Manager::TYPE::BLONORMAL)
					{
						Type = static_cast<CX_Manager::TYPE>(Type - 1);
					}

					// タイプの変更を反映
					pEditBlock->BindModel(Type);

					// サイズも調整
					auto pModel = pEditBlock->GetModel();
					pEditBlock->SetSize(pModel->Size);
				}

				{ // 方角の編集

					// 座標を取得
					Vec3 Pos = pEditBlock->GetPos();

					// 座標を方角に変換
					float fDirection = utility::Vec3ConvertDirection(Pos);

					// 方角を変更
					ImGui::DragFloat("Direction", &fDirection, 0.001f, -D3DX_PI, D3DX_PI);

					// 新しい方角から適切な向きを作成
					float fRotY = fDirection + D3DX_PI * -0.5f;

					// 方角の変更を反映
					pEditBlock->SetPos(utility::DirectionConvertVec3(fDirection, Pos.y, CField_Manager::FIELD_RADIUS));
					pEditBlock->SetRot({ 0.0f, fRotY, 0.0f });
				}

				{ // 高さの編集

					// 座標を取得
					Vec3 Pos = pEditBlock->GetPos();

					// 表示
					ImGui::Text("Height: %.1f", Pos.y);
					ImGui::SameLine();

					// 高さを変更
					if (ImGui::Button("++"))
					{
						Pos.y += 20.0f;
					}
					ImGui::SameLine();
					if (ImGui::Button("--"))
					{
						Pos.y += -20.0f;
					}

					// 高さの変更を反映
					pEditBlock->SetPos(Pos);
				}
			}

			ImGui::End();
		}
	}
}