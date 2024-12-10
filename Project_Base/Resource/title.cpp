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
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "motion_set.h"
#include "object_parts.h"
#include "object_X.h"
#include "field_manager.h"
#include "HUD_manager.h"
#include "constellation.h"
#include "sparks.h"

#include "object_3D.h"

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
	m_pKESE{ nullptr },
	m_bTransition{ nullptr },
	m_pButterfly{ nullptr },
	m_vPath{},
	m_nNowFrame{ 0 },
	m_nMaxFrame{ 0 },
	m_pFakePlayer{ nullptr },
	m_pTree{ nullptr },
	m_pHole{ nullptr }
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
		m_pButterfly = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\BUTTERFLY\\butterfly_motion.json"));
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
		m_pTree = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\TREE\\tree_motion.json"));
		auto TreeParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\TREE\\tree_param.json");
		m_pTree->SetRot(utility::JsonConvertToVec3(TreeParam["Rot"]));
		m_pTree->SetPos(utility::JsonConvertToVec3(TreeParam["Pos"]));

		/* 最低すぎるタイトル仮表示 */
		CObject_3D* p = CObject_3D::Create();
		JSON Json = utility::OpenJsonFile("Data\\JSON\\debug_param.json");
		p->BindTex(CTexture_Manager::TYPE::TITLE);
		p->SetPos(utility::JsonConvertToVec3(Json["Pos"]));
		p->SetSize(utility::JsonConvertToVec3(Json["Size"]));

		/*最低すぎるスタート表示*/
		m_pKESE = CObject_3D::Create();
		m_pKESE->BindTex(CTexture_Manager::TYPE::START);
		m_pKESE->SetPos(utility::JsonConvertToVec3(Json["Pos"]));
		m_pKESE->SetPos({ m_pKESE->GetPos().x, m_pKESE->GetPos().y + -100.0f, m_pKESE->GetPos().z });
		m_pKESE->SetSize(utility::JsonConvertToVec3(Json["Size"]));
		m_pKESE->SetAlpha(0.0f);
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
	}

	// フェイクプレイヤーを破棄
	if (m_pFakePlayer != nullptr)
	{
		m_pFakePlayer->SetRelease();
	}

	// 樹を破棄
	if (m_pTree != nullptr)
	{
		m_pTree->SetRelease();
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

			/*最低すぎるスタート表示*/
			{
				float fAlpha = m_pKESE->GetAlpha();
				if (fAlpha < 1.0f)
					fAlpha += 0.025f;
				m_pKESE->SetAlpha(fAlpha);
			}

			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
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

		// 星座エフェクトを拡散発生
		CConstellation::GenerateSpread(m_pButterfly->GetPos());

		// 蝶の向き・座標をコピー
		D3DXVECTOR3 NewRot = m_pButterfly->GetRot(), NewPos = m_pButterfly->GetPos();

		// 蝶が上昇を開始し、一定の高度に達すると画面奥へ飛び立つ
		if (NewPos.y < m_vPath[1].y - 5.0f)
		{
			/*m_Path[1]のzはこの時のみRot.xの設定値として扱う、節約です*/
			NewRot.y += (m_vPath[1].z - NewRot.y) * 0.025f;
			NewPos.x += (m_vPath[1].x - NewPos.x) * 0.025f;
			NewPos.y += (m_vPath[1].y - NewPos.y) * 0.025f;
		}
		else
		{ // 画面奥へ飛行中

			// 銅像の座標をコピー
			D3DXVECTOR3 PosTarget = { 0.0f, 15.0f, 0.0f };

			// 銅像への進行度で挙動を変更
			if (PosTarget.z - NewPos.z > CField_Manager::FIELD_RADIUS * 1.1f)
			{ // 蝶がフィールドの半径より遠い場合

				// 多少蝶が銅像の方に寄って行ったら
				if (PosTarget.z - NewPos.z < CField_Manager::FIELD_RADIUS * 5.0f)
				{
					// 蝶のモーションを変身モーションに変更
					if (m_pButterfly->GetNowMotion() != 2)
						m_pButterfly->SetNowMotion(2);

					// フェイクプレイヤーの挙動
					if (m_pButterfly->GetStopState() && !m_pFakePlayer)
					{ // 蝶が消えて、フェイクプレイヤーが未生成なら

						// フェイクプレイヤーの生成
						m_pFakePlayer = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\player_motion.json"));
						m_pFakePlayer->SetNowMotion(2);
					}
					else if (m_pFakePlayer)
					{ // フェイクプレイヤー生成後

						// フェイクプレイヤーが登場モーションを終了したら飛行モーションに変更
						if (m_pFakePlayer->GetStopState())
							m_pFakePlayer->SetNowMotion(3);

						// フォグの範囲を変更
						CRenderer::CorrectFogEnd(550.0f);
					}
				}
				else
				{
					// フォグの範囲を変更
					CRenderer::CorrectFogEnd(1500.0f);
				}

				// 蝶の向き・座標を銅像の方向へ接近
				NewRot.y += (D3DX_PI - NewRot.y) * 0.1f;
				NewPos.x += (PosTarget.x - NewPos.x) * 0.005f;
				NewPos.y += (PosTarget.y - NewPos.y) * 0.005f;
				NewPos.z += (PosTarget.z - NewPos.z) * 0.005f;
			}
			else
			{ // 蝶がフィールドの範囲ほどに近づいたら

				// ゲームシーンへ
				To_Next();
				return;
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

		// カメラを追従させる
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		pCamera->SetPosTarget(NewPos);

		// カメラ距離をズームイン
		float fDistance = pCamera->GetDistance();
		fDistance += (50.0f - fDistance) * 0.05f;
		pCamera->SetDistance(fDistance);

		// カメラを振動させる
		pCamera->SetVibration(0.001f);

		/*最低すぎるスタート表示*/
		{
			static int nNum = 0;
			++nNum;
			if (nNum > 6)
			{
				nNum = 0;
				float fAlpha = m_pKESE->GetAlpha();
				if (fAlpha >= 1.0f)
					fAlpha = 0.0f;
				else if (fAlpha <= 0.0f)
					fAlpha = 1.0f;
				else
					fAlpha = 1.0f;
				m_pKESE->SetAlpha(fAlpha);
			}
		}

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