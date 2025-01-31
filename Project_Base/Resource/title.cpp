//============================================================================
// 
// タイトル [title.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "title.h"
#include "sound.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "motion_set.h"
#include "object_parts.h"
#include "object_X.h"
#include "object_TextMesh.h"
#include "field_manager.h"
#include "HUD_manager.h"
#include "constellation.h"
#include "sparks.h"

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CTitle::Update()
{
	// 基底クラスの更新処理
	CScene::Update();

	// 移動演出用の期間をインクリメント
	m_nNowFrame++;

	// BGMを遷移
	CSound::GetInstance()->Transition(CSound::LABEL::BOSSBGM, CSound::LABEL::FILEDBGM);

	// 環境装飾の更新
	UpdateEnvironment();

#ifdef _DEBUG	// パラメータ確認用
	if (CManager::GetKeyboard()->GetTrigger(DIK_F7))
	{
		CObject::ReleaseAll();
		CScene_Manager::ChangeScene(Create());
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_SPACE))
	{
		CHUD_Manager::Create();
		To_Next();
	}
#endif // _DEBUG
}

//============================================================================
// 描画処理
//============================================================================
void CTitle::Draw()
{
	// 基底クラスの描画処理
	CScene::Draw();
}

//============================================================================
// 次のシーンへ
//============================================================================
void CTitle::To_Next()
{
	// ゲームシーンへ変更
	CScene_Manager::ChangeScene(CGame::Create());
}

//============================================================================
// タイトルを生成
//============================================================================
CTitle* CTitle::Create()
{
	// タイトルを生成
	CTitle* pNew = DBG_NEW CTitle();

	if (pNew == nullptr)
	{
		assert(false && "タイトルの生成に失敗");
	}

	// タイトルの初期設定
	if (FAILED(pNew->Init()))
	{
		assert(false && "タイトルの初期設定に失敗");
	}

	return pNew;
}

//============================================================================
// 
// protectedメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CTitle::CTitle() :
	CScene{},
	m_bTransition{ nullptr },
	m_pButterfly{ nullptr },
	m_vPath{},
	m_nNowFrame{ 0 },
	m_nMaxFrame{ 0 },
	m_nCntStart{ 0 },
	m_pFakePlayer{ nullptr },
	m_pTree{ nullptr }
{
	m_vPath.clear();
}

//============================================================================
// デストラクタ
//============================================================================
CTitle::~CTitle()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CTitle::Init()
{
	//　フィールドマネージャーのタイトル向け初期設定
	CField_Manager::GetInstance()->InitForTitle();

	{ // 環境装飾を生成

		// 周辺領域
		CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\around.json"));

		// 蝶
		m_pButterfly = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\BUTTERFLY\\butterfly_motion.json"));
		auto ButterflyParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\BUTTERFLY\\butterfly_param.json");
		m_pButterfly->SetRot(utility::JsonConvertToVec3(ButterflyParam["Rot"]));
		m_pButterfly->SetPos(utility::JsonConvertToVec3(ButterflyParam["Pos"]));
		m_pButterfly->SetNowMotion(1);
		for (WORD wCntPath = 0; wCntPath < ButterflyParam["Path"].size(); ++wCntPath)
			m_vPath.push_back(utility::JsonConvertToVec3(ButterflyParam["Path"][wCntPath]));
		m_nMaxFrame = ButterflyParam["MaxFrame"];

		// カメラ情報を変更
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetDistance(100.0f);
		pCamera->SetUpAdjust(0.0f);
		pCamera->SetPos(utility::JsonConvertToVec3(ButterflyParam["Pos"]));
		pCamera->SetPosTarget(utility::JsonConvertToVec3(ButterflyParam["Pos"]));
		pCamera->ChangeTrackPlayer(false);

		// 樹
		m_pTree = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\TREE\\tree_motion.json"));
		auto TreeParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\TREE\\tree_param.json");
		m_pTree->SetRot(utility::JsonConvertToVec3(TreeParam["Rot"]));
		m_pTree->SetPos(utility::JsonConvertToVec3(TreeParam["Pos"]));

		// タイトル表示
		CObject_TextMesh::Create(utility::OpenJsonFile("Data\\JSON\\TEXTMESH\\title.json"));

		// 操作表示
		CObject_TextMesh::Create(utility::OpenJsonFile("Data\\JSON\\TEXTMESH\\enter.json"));
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTitle::Uninit()
{
	// 蝶を破棄
	if (m_pButterfly != nullptr)
	{
		m_pButterfly->SetRelease();
		m_pButterfly = nullptr;
	}

	// フェイクプレイヤーを破棄
	if (m_pFakePlayer != nullptr)
	{
		m_pFakePlayer->SetRelease();
		m_pFakePlayer = nullptr;
	}

	// 樹を破棄
	if (m_pTree != nullptr)
	{
		m_pTree->SetRelease();
		m_pTree = nullptr;
	}
}

//============================================================================
// 環境装飾の更新
//============================================================================
void CTitle::UpdateEnvironment()
{
	// 蝶の周辺に火の粉を発生
	CSparks::AreaGenerate(m_pButterfly->GetPos());

	// 樹のモーションが停止していたら高速モーションに変更
	if (m_pTree->GetStopState())
	{
		m_pTree->SetNowMotion(1);
	}

	// 蝶の更新
	UpdateButterfly();

	// ゲームシーンへ遷移を開始したらカメラ固定は無視
	if (m_bTransition)
		return;

	// 移動演出が終わっていたらカメラを固定
	if (m_nNowFrame >= m_nMaxFrame)
	{
		// カメラの座標を固定、距離をコピー
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetPosTarget({ -400.0f, -50.0f, -4000.0f });
		float fDistance = pCamera->GetDistance();

		// カメラの距離に応じて挙動を変更
		if (fDistance < DISTANCE_SHOW_TITLE - 10.0f)
		{ // 距離が近いうちはズームアウトで引いていく

			fDistance += (DISTANCE_SHOW_TITLE - fDistance) * 0.05f;
			pCamera->SetDistance(fDistance);
		}
		else	
		{ // カメラの引きが完了したら

			// コントローラーを取得
			CInputPad* pPad = CManager::GetPad();

			// 次のシーンへ
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) ||
				pPad->GetTrigger(CInputPad::JOYKEY::A) ||
				pPad->GetTrigger(CInputPad::JOYKEY::START))
			{
				// HUDマネージャーの生成
				CHUD_Manager::Create();

				// 次のシーンへ移行開始
				m_bTransition = 1;

				// 羽ばたきモーションに変更
				if (m_pButterfly->GetNowMotion() != 1)
					m_pButterfly->SetNowMotion(1);

				// 樹のモーションを変更
				if(m_pTree->GetNowMotion() != 2)
					m_pTree->SetNowMotion(2);
			}
		}
	}
}

//============================================================================
// 蝶の更新
//============================================================================
void CTitle::UpdateButterfly()
{
	// タイトルの進行に合わせて蝶の挙動を変化
	if (m_bTransition)
	{ // ゲームシーンへ遷移を開始したら

		// 蝶の向き・座標をコピー
		D3DXVECTOR3 NewRot = m_pButterfly->GetRot(), NewPos = m_pButterfly->GetPos();

		// カメラを取得
		CCamera* pCamera = CManager::GetManager()->GetCamera();

		// カメラ座標調整用
		D3DXVECTOR3 AdjustCameraPos = { 0.0f, 0.0f, 0.0f };

		// 目標カメラ距離を格納
		float fTargetDistance = 0.0f;

		// カメラを振動させる
		pCamera->SetVibration(0.001f);

		// 星座エフェクトを拡散発生
		CConstellation::GenerateSpread(m_pButterfly->GetPos());

		// 蝶が上昇を開始し、一定の高度に達すると画面奥へ飛び立つ
		if (NewPos.y < m_vPath[1].y - 5.0f)
		{
			/*m_Path[1]のzはこの時のみRot.xの設定値として扱う、節約です*/
			NewRot.y += (m_vPath[1].z - NewRot.y) * 0.025f;
			NewPos.x += (m_vPath[1].x - NewPos.x) * 0.025f;
			NewPos.y += (m_vPath[1].y - NewPos.y) * 0.025f;

			// カメラ距離をセット
			fTargetDistance = 100.0f;
		}
		else
		{ // 画面奥へ飛行中

			// 目標地点の座標を作成
			D3DXVECTOR3 TargetPoint = { 0.0f, 60.0f, -CField_Manager::FIELD_RADIUS };

			// 目標値への補間強度を設定
			float fCoef = 0.0f;

			// 銅像への進行度で挙動を変更
			if (TargetPoint.z != NewPos.z)
			{ // 蝶がフィールドの半径より遠い場合

				if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 1.1f)
				{ // 接近中の終盤
					
					// スタートカウントをインクリメント
					++m_nCntStart;

					// 一定カウント経過で
					if (m_nCntStart > 210)
					{	
						// ゲームシーンへ
						To_Next();

						return;
					}

					// プレイヤーにダッシュモーションをセット
					if (m_pFakePlayer->GetNowMotion() != 6 &&
						m_pFakePlayer->GetNowMotion() != 8)
					{
						m_pFakePlayer->SetNowMotion(6);
					}

					// ダッシュモーションの再生が終了していたら飛行モーションをセット
					if (m_pFakePlayer->GetStopState())
					{
						m_pFakePlayer->SetNowMotion(8);
					}

					// カメラ距離
					fTargetDistance = 50.0f;

					// カメラ座標調整
					if (m_nCntStart > 180)
					{
						AdjustCameraPos.y = (m_nCntStart - 180) * -1.0f;
						AdjustCameraPos.z = (m_nCntStart - 180) * -1.0f;
					}

					// モーションに応じてパラメータをセット
					if (m_pFakePlayer->GetNowMotion() == 6 &&
						m_nCntStart < 10)
					{
						// 補間強度をセット
						fCoef = 0.001f;
					}
					else
					{
#if 0
						// カメラ座標調整
						AdjustCameraPos.y = -40.0f + (TargetPoint.z - NewPos.z) * 0.1f;
#endif
						// 補間強度をセット
						fCoef = 0.015f;
					}
				}
				else if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 1.25f)
				{
					// カメラをとても振動
					pCamera->SetVibration(0.003f);

					// カメラ距離
					fTargetDistance = 30.0f;

					// カメラ座標調整
					AdjustCameraPos.y = -10.0f;

					// 補間強度をセット
					fCoef = 0.001f;
				}
				else if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 1.5f)
				{
					// カメラをわりと振動
					pCamera->SetVibration(0.002f);

					// カメラ距離
					fTargetDistance = 35.0;

					// カメラ座標調整
					AdjustCameraPos.y = -5.0f;

					// 補間強度をセット
					fCoef = 0.0025f;
				}
				else if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 2.0f)
				{
					// カメラすこし振動
					pCamera->SetVibration(0.001f);

					// カメラ距離
					fTargetDistance = 40.0f;

					// カメラ座標調整
					AdjustCameraPos.y = -2.5f;

					// 補間強度をセット
					fCoef = 0.004f;
				}
				else if (TargetPoint.z - NewPos.z < CField_Manager::FIELD_RADIUS * 5.0f)
				{ // 接近中の序盤

					// 蝶のモーションを変身モーションに変更
					if (m_pButterfly->GetNowMotion() != 2)
					{
						m_pButterfly->SetNowMotion(2);
					}

					// フェイクプレイヤーの挙動
					if (m_pButterfly->GetStopState() && !m_pFakePlayer)
					{ // 蝶が消えて、フェイクプレイヤーが未生成なら

						// フェイクプレイヤーの生成
						m_pFakePlayer = CMotion_Set::Create(CObject::LAYER::DEFAULT, CObject::TYPE::NONE, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\player_motion.json"));
						m_pFakePlayer->SetNowMotion(2);
					}
					else if (m_pFakePlayer)
					{ // フェイクプレイヤー生成後

						// カメラをとても振動
						pCamera->SetVibration(0.0025f);

						// フェイクプレイヤーが登場モーションを終了したら飛行モーションに変更
						if (m_pFakePlayer->GetStopState())
							m_pFakePlayer->SetNowMotion(3);

						// フォグの範囲を変更
						CRenderer::CorrectFogEnd(550.0f);
					}

					// カメラ距離をセット
					fTargetDistance = 50.0f;

					// 補間強度をセット
					fCoef = 0.005f;
				}
				else
				{ // 飛び立ってすぐ

					// フォグの範囲を変更
					CRenderer::CorrectFogEnd(1500.0f);

					// カメラ距離をセット
					fTargetDistance = 50.0f;

					// 補間強度をセット
					fCoef = 0.005f;
				}

				// 蝶の向き・座標を銅像の方向へ接近
				NewRot.y += (D3DX_PI - NewRot.y) * 0.1f;
				NewPos.x += (TargetPoint.x - NewPos.x) * fCoef;
				NewPos.y += (TargetPoint.y - NewPos.y) * fCoef;
				NewPos.z += (TargetPoint.z - NewPos.z) * fCoef;
			}

			// フェイクプレイヤーが生成されていたら
			if (m_pFakePlayer)
			{
				// 向きを合わせる
				m_pFakePlayer->SetRot(m_pButterfly->GetRot());
				
				// 座標を合わせる
				D3DXVECTOR3 Pos = m_pButterfly->GetPos();
				Pos.z += 3.0f;
				m_pFakePlayer->SetPos(Pos);
			}
		}

		// 蝶の向き・座標設定
		m_pButterfly->SetRot(NewRot);
		m_pButterfly->SetPos(NewPos);

		// カメラ座標を調整しつつ、プレイヤーに追従させる
		pCamera->SetPosTarget(NewPos + AdjustCameraPos);

		// カメラ距離を補間
		float fDistance = pCamera->GetDistance();
		fDistance += (fTargetDistance - fDistance) * 0.05f;
		pCamera->SetDistance(fDistance);

		// マップシンボルの特殊挙動
		/* 蝶がフィールドからどれくらい離れてるかを渡す */
		CHUD_Manager::GetInstance()->SpecialMapSymbol((NewPos.z + CField_Manager::FIELD_RADIUS * 1.1f) / 30.0f);
	}
	else if (m_nNowFrame < m_nMaxFrame)
	{ // 初回の羽ばたき期間中

		// 星座エフェクトを拡散発生
		CConstellation::GenerateSpread(m_pButterfly->GetPos());

		// 蝶が目標地点へ飛行
		D3DXVECTOR3 NewPos = m_pButterfly->GetPos();
		float fFrameCoef = static_cast<float>(m_nMaxFrame - m_nNowFrame);
		NewPos += (m_vPath[0] - NewPos) / fFrameCoef;

		// さらに、飛びながら上下に揺れる
		NewPos.y += utility::GetRandomValue<float>() * 0.01f;
		m_pButterfly->SetPos(NewPos);

		// カメラはある程度追従 (完全同期するとブレブレに)
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		D3DXVECTOR3 CameraPos = pCamera->GetPos();
		pCamera->SetPos({ NewPos.x, CameraPos.y, NewPos.z });
		pCamera->SetPosTarget(NewPos);
	}
	else if (m_nNowFrame == m_nMaxFrame)
	{ // 移動が完了

		// 蝶をとどまりモーションに変更
		m_pButterfly->SetNowMotion(0);
	}
}