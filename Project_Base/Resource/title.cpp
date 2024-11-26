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
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "motion_set.h"
#include "object_parts.h"
#include "object_X.h"
#include "field_manager.h"

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
		CScene_Manager::ChangeScene(Create());
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
	m_pStatue{ nullptr },
	m_pButterfly{ nullptr },
	m_vPath{},
	m_nNowFrame{ 0 },
	m_nMaxFrame{ 0 },
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
	// 環境装飾を生成
	{
		// 銅像
		m_pStatue = CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\statue.json"));

		// 周辺領域
		CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\around.json"));

		// 蝶
		m_pButterfly = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\butterfly_motion.json"));
		auto ButterflyParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\butterfly.json");
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

		// 樹
		m_pTree = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\tree_motion.json"));
		auto TreeParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\tree.json");
		m_pTree->SetRot(utility::JsonConvertToVec3(TreeParam["Rot"]));
		m_pTree->SetPos(utility::JsonConvertToVec3(TreeParam["Pos"]));

		// 穴
		//m_pHole = CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\hole.json"));
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTitle::Uninit()
{
	// 蝶を破棄
	m_pButterfly->SetRelease();

	// 樹を破棄
	m_pTree->SetRelease();
}

//============================================================================
// 環境装飾の更新
//============================================================================
void CTitle::UpdateEnvironment()
{
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
		pCamera->SetPosTarget({ 100.0f, -50.0f, -4000.0f });
		float fDistance = pCamera->GetDistance();

		// カメラの距離に応じて挙動を変更
		if (fDistance < 300.0f)
		{ // 距離が近いうちはズームアウトで引いていく

			// 距離反映
			pCamera->SetDistance(fDistance += 5.0f);
		}
		else	
		{ // カメラの引きが完了したら

			// 次のシーンへ
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
				m_bTransition = 1;
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

		// 羽ばたきモーションに変更
		if (m_pButterfly->GetNowMotion() != 1)
			m_pButterfly->SetNowMotion(1);

		// 石が動きまくるモーションに変更
		if (m_pTree->GetNowMotion() != 1)
			m_pTree->SetNowMotion(1);

		// 蝶の向き・座標をコピー
		D3DXVECTOR3 NewRot = m_pButterfly->GetRot(), NewPos = m_pButterfly->GetPos();

		// 蝶が上昇を開始し、一定の高度に達すると画面奥へ飛び立つ
		if (NewPos.y < m_vPath[1].y - 5.0f)
		{
			/*m_Path[1]のzはこの時のみRot.xの設定値として扱う、節約です*/
			NewRot.x += (m_vPath[1].z - NewRot.x) * 0.025f;
			NewPos.x += (m_vPath[1].x - NewPos.x) * 0.025f;
			NewPos.y += (m_vPath[1].y - NewPos.y) * 0.025f;
		}
		else
		{ // 画面奥へ飛行中

			// 銅像の座標をコピー
			D3DXVECTOR3 PosTarget = m_pStatue->GetPos();

			// 銅像への進行度で挙動を変更
			if (PosTarget.z - NewPos.z > CField_Manager::FIELD_RADIUS * 1.5f)
			{ // 蝶がフィールドの半径より遠い場合

				// 向き・座標を銅像の方向へ接近
				NewRot.x += (D3DX_PI - NewRot.x) * 0.1f;
				NewPos.x += (PosTarget.x - NewPos.x) * 0.005f;
				NewPos.y += (PosTarget.y - NewPos.y) * 0.005f;
				NewPos.z += (PosTarget.z - NewPos.z) * 0.005f;

				// カメラ追従
				CCamera* pCamera = CManager::GetManager()->GetCamera();
				pCamera->SetPosTarget(NewPos);

				// カメラ距離をコピー
				float fDistance = pCamera->GetDistance();

				// 距離が遠いはズームインで迫っていく
				if (fDistance > 75.0f)
					pCamera->SetDistance(fDistance += -5.0f);
			}
			else
			{ // 蝶がフィールドの範囲ほどに近づいたら

				// ゲームシーンへ
				To_Next();
				return;
			}
		}

		// 蝶の向き・座標設定
		m_pButterfly->SetRot(NewRot);
		m_pButterfly->SetPos(NewPos);
	}
	else if (m_nNowFrame < m_nMaxFrame)
	{ // 初回の羽ばたき期間中

		// 蝶が目標地点へ飛行
		D3DXVECTOR3 NewPos = m_pButterfly->GetPos();
		int nFrameCoef = m_nMaxFrame - m_nNowFrame;
		NewPos += (m_vPath[0] - NewPos) / nFrameCoef;

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