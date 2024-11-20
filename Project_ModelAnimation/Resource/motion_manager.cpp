//============================================================================
// 
// モーションマネージャー [motion_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "motion_manager.h"
#include "object_Parts.h"
#include "manager.h"
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバの初期化
//****************************************************
CMotion_Manager* CMotion_Manager::m_pMotionManager = nullptr;	// モーションマネージャーの本体

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 更新処理
//============================================================================
void CMotion_Manager::Update()
{
	// ウィンドウを表示
	ImVec2 Rect = { 600, 800 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ SCREEN_WIDTH - (Rect.x + 100), 100, });
	ImGui::Begin("Motion_Manager");

	// 編集
	Edit();

	if (m_bPlay)
	{
		// 動作
		m_MotionSet->Animation();
	}

	// リセット
	Reset();

	// ウィンドウ表示終了
	ImGui::End();

	// デバッグ表示
	PrintDebug();
}

//============================================================================
// 生成
//============================================================================
HRESULT CMotion_Manager::Create()
{
	// 既に生成されていたら
	if (m_pMotionManager != nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetTimeString("モーションマネージャーは既に生成されています", 120);
#endif	// _DEBUG

		return S_OK;
	}

	// インスタンスを生成
	m_pMotionManager = DBG_NEW CMotion_Manager();

	// 生成失敗
	if (m_pMotionManager == nullptr)
	{
		return E_FAIL;
	}

	// モーションマネージャーの初期設定
	if (FAILED(m_pMotionManager->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// 解放
//============================================================================
void CMotion_Manager::Release()
{
	if (m_pMotionManager != nullptr)
	{
		// モーションマネージャーの終了処理
		m_pMotionManager->Uninit();

		// メモリの解放
		delete m_pMotionManager;

		// ポインタの初期化
		m_pMotionManager = nullptr;
	}
}

//============================================================================
// モーションマネージャーを取得
//============================================================================
CMotion_Manager* CMotion_Manager::GetInstance()
{
	// 本体が存在しなければ
	if (m_pMotionManager == nullptr)
	{
		// 生成
		if (FAILED(Create()))
		{
			assert(false && "モーションマネージャーの取得に失敗");
		}
	}

	return m_pMotionManager;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CMotion_Manager::CMotion_Manager() :
	m_Json{},
	m_MotionSet{ nullptr },
	m_wSelectParts{ 0 },
	m_wSelectMotion{ 0 },
	m_wSelectKey{ 0 },
	m_bPlay{ false }
{
	
}

//============================================================================
// デストラクタ
//============================================================================
CMotion_Manager::~CMotion_Manager()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CMotion_Manager::Init()
{
	// ジェイソンデータを取得
	m_Json = utility::OpenJsonFile("Data\\JSON\\motion_export.json");

	// モーションセットを生成
	m_MotionSet = CMotion_Set::Create(m_Json);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMotion_Manager::Uninit()
{
	if (m_MotionSet != nullptr)
	{
		// モーションセットの解放処理
		m_MotionSet->Release();

		// ポインタを初期化
		m_MotionSet = nullptr;
	}
}

//============================================================================
// デバッグ表示
//============================================================================
void CMotion_Manager::PrintDebug()
{
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("総パーツ数：" + to_string(m_MotionSet->m_wMaxParts));
	CRenderer::SetDebugString("総モーション数：" + to_string(m_MotionSet->m_wMaxMotion));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	for (WORD wCntMotion = 0; wCntMotion < m_MotionSet->m_wMaxMotion; ++wCntMotion)
	{
		const CMotion_Set::Motion* const pMotion = &m_MotionSet->m_apMotion[wCntMotion];
		CRenderer::SetDebugString("<再生待機モーション情報>");
		CRenderer::SetDebugString("現在のフレーム数　：" + to_string(m_MotionSet->m_wNowFrame));
		CRenderer::SetDebugString("現在のキー数　　　：" + to_string(m_MotionSet->m_wNowKey));
		CRenderer::SetDebugString("ループフラグ　　　：" + to_string(pMotion->bLoop));
		CRenderer::SetDebugString("総キー数　　　　　：" + to_string(pMotion->wMaxKey));
		CRenderer::SetDebugString("ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー");
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			const CMotion_Set::Key* const pKey = &pMotion->vpKey[wCntMotionKey];
			CRenderer::SetDebugString("キー[" + to_string(wCntMotionKey) + "]の総フレーム数：" + to_string(pKey->nMaxFrame));
		}
	}
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("<編集情報>");
	CRenderer::SetDebugString("現在の選択パーツ　　：" + to_string(m_wSelectParts));
	CRenderer::SetDebugString("現在の選択モーション：" + to_string(m_wSelectMotion));
	CRenderer::SetDebugString("現在の選択キー　　　：" + to_string(m_wSelectKey));
	for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_vpModelParts.size(); ++wCntModelParts)
	{
		const CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[wCntModelParts];
		CRenderer::SetDebugString("Scale：" + utility::ToPrecision(pDest->ScaleTarget.x) + "：" + utility::ToPrecision(pDest->ScaleTarget.y) + "：" + utility::ToPrecision(pDest->ScaleTarget.z) + "："
			"Rot：" + utility::ToPrecision(pDest->RotTarget.x) + "：" + utility::ToPrecision(pDest->RotTarget.y) + "：" + utility::ToPrecision(pDest->RotTarget.z) + "："
			"Pos：" + utility::ToPrecision(pDest->PosTarget.x) + "：" + utility::ToPrecision(pDest->PosTarget.y) + "：" + utility::ToPrecision(pDest->PosTarget.z));
	}
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
}

//============================================================================
// 編集
//============================================================================
void CMotion_Manager::Edit()
{
	// 再生切り替え
	ImGui::Checkbox("Playing Motion", &m_bPlay);

	// パーツ情報の編集
	EditParts();

	// モーション情報の編集
	EditMotion();

	// キー情報の編集
	EditKey();

	// フレーム情報の編集
	EditFrame();

	// エクスポート
	ImGui::Separator();
	if (ImGui::Button("Export Edit Data"))
	{
		Export();
	}
}

//============================================================================
// パーツ情報の編集
//============================================================================
void CMotion_Manager::EditParts()
{
	// 選択パーツ切り替え
	ImGui::Separator();
	ImGui::BulletText("Select Parts");
	if (ImGui::Button("First"))
	{
		m_wSelectParts = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Prev"))
	{
		m_wSelectParts > 0 ? m_wSelectParts-- : m_wSelectParts = static_cast<WORD>(m_MotionSet->m_vpModelParts.size()) - 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("Next"))
	{
		m_wSelectParts < m_MotionSet->m_vpModelParts.size() - 1 ? m_wSelectParts++ : m_wSelectParts = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Last"))
	{
		m_wSelectParts = m_MotionSet->m_vpModelParts.size() - 1;
	}

	// 選択パーツを透過
	for (WORD wCntParts = 0; wCntParts < m_MotionSet->m_vpModelParts.size(); ++wCntParts)
	{
		if (m_wSelectParts == wCntParts)
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetCol({ 1.0f, 0.5f, 0.25f, 0.25f });
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(true);
		}
		else
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(false);
		}
	}

	// 目標値情報の編集
	EditDest();
}

//============================================================================
// 目標値情報の編集
//============================================================================
void CMotion_Manager::EditDest()
{
	// 選択中のパーツの目標値情報のポインタを作成
	CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[m_wSelectParts];

	// 目標縮尺を変動
	ImGui::Separator();
	ImGui::BulletText("ScaleDest");
	{
		// 縮尺：X
		if (ImGui::Button("Reset:ScaleX"))
		{
			pDest->ScaleTarget.x = 1.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Scale:X", &pDest->ScaleTarget.x, 1.0f, 1.0f);
		// 縮尺：Y
		if (ImGui::Button("Reset:ScaleY"))
		{
			pDest->ScaleTarget.y = 1.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Scale:Y", &pDest->ScaleTarget.y, 1.0f, 1.0f);
		// 縮尺：Z
		if (ImGui::Button("Reset:ScaleZ"))
		{
			pDest->ScaleTarget.z = 1.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Scale:Z", &pDest->ScaleTarget.z, 1.0f, 1.0f);
	}

	// 目標向きを変動
	ImGui::Separator();
	ImGui::BulletText("RotDest");
	{
		// 向き：X
		if (ImGui::Button("Reset:RotX"))
		{
			pDest->RotTarget.x = 0.0f;
		}
		ImGui::SameLine();
		ImGui::SliderFloat("Rot:X", &pDest->RotTarget.x, -D3DX_PI, D3DX_PI);
		// 向き：X
		if (ImGui::Button("Reset:RotY"))
		{
			pDest->RotTarget.y = 0.0f;
		}
		ImGui::SameLine();
		ImGui::SliderFloat("Rot:Y", &pDest->RotTarget.y, -D3DX_PI, D3DX_PI);
		// 向き：Z
		if (ImGui::Button("Reset:RotZ"))
		{
			pDest->RotTarget.z = 0.0f;
		}
		ImGui::SameLine();
		ImGui::SliderFloat("Rot:Z", &pDest->RotTarget.z, -D3DX_PI, D3DX_PI);
	}

	// 目標座標
	ImGui::Separator();
	ImGui::BulletText("ScaleDest");
	{
		// 座標：X
		if (ImGui::Button("Reset:PosX"))
		{
			pDest->PosTarget.x = 0.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Pos:X", &pDest->PosTarget.x, 1.0f, 1.0f);
		// 座標：Y
		if (ImGui::Button("Reset:PosY"))
		{
			pDest->PosTarget.y = 0.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Pos:Y", &pDest->PosTarget.y, 1.0f, 1.0f);
		// 座標：Z
		if (ImGui::Button("Reset:PosZ"))
		{
			pDest->PosTarget.z = 0.0f;
		}
		ImGui::SameLine();
		ImGui::InputFloat("Pos:Z", &pDest->PosTarget.z, 1.0f, 1.0f);
	}

	// 目標値を反映
	m_Json["ScaleTarget"][m_wSelectKey][m_wSelectParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
	m_Json["RotTarget"][m_wSelectKey][m_wSelectParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
	m_Json["PosTarget"][m_wSelectKey][m_wSelectParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
}

//============================================================================
// モーション情報の編集
//============================================================================
void CMotion_Manager::EditMotion()
{
	// 選択モーション切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_3))
	{
		m_wSelectMotion > 0 ? m_wSelectMotion-- : m_wSelectMotion = m_MotionSet->m_wMaxMotion - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_4))
	{
		m_wSelectMotion < m_MotionSet->m_wMaxMotion - 1 ? m_wSelectMotion++ : m_wSelectMotion = 0;
	}
}

//============================================================================
// キー情報の編集
//============================================================================
void CMotion_Manager::EditKey()
{
	// 選択キー切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_5))
	{
		m_wSelectKey > 0 ? m_wSelectKey-- : m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_6))
	{
		m_wSelectKey < GetSelectMotion()->wMaxKey - 1 ? m_wSelectKey++ : m_wSelectKey = 0;
	}

	// 総キー数の切り替え
	if (CManager::GetKeyboard()->GetTrigger(DIK_F2) && GetSelectMotion()->wMaxKey > 1)
	{
		// 総キー数をデクリメントし、ジェイソンデータに保存
		GetSelectMotion()->wMaxKey--;
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;

		// 消去するキー番号のフレーム・目標値情報をジェイソンデータから消去
		m_Json["MaxFrame"].erase(GetSelectMotion()->wMaxKey);
		m_Json["ScaleTarget"].erase(GetSelectMotion()->wMaxKey);
		m_Json["RotTarget"].erase(GetSelectMotion()->wMaxKey);
		m_Json["PosTarget"].erase(GetSelectMotion()->wMaxKey);

		// キー情報のポインタを作成
		CMotion_Set::Key* const pKey = &GetSelectMotion()->vpKey[GetSelectMotion()->wMaxKey];

		// 目標値情報のポインタ配列を破棄
		if (pKey->apDest != nullptr)
		{
			delete[] pKey->apDest;
			pKey->apDest = nullptr;
		}

		// 末尾のキー情報を削除
		GetSelectMotion()->vpKey.pop_back();

		// 選択キー番号を調整
		if (m_wSelectKey >= GetSelectMotion()->wMaxKey)
		{
			m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
		}
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F3))
	{
		// キー情報オブジェクトを新規作成
		CMotion_Set::Key Key;

		// 新たなキーの総フレーム数を設定し、ジェイソンデータに保存
		Key.nMaxFrame = 1;
		m_Json["MaxFrame"][GetSelectMotion()->wMaxKey] = Key.nMaxFrame;

		// パーツ数分の目標値情報を追加生成
		Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet->m_wMaxParts];

		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// 目標値情報のポインタを作成
			CMotion_Set::KeyDest* const pDest = &Key.apDest[wCntModelParts];

			// 新たなキーのパーツの目標値を設定し、ジェイソンデータに保存
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };	// 目標縮尺
			m_Json["ScaleTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
			pDest->RotTarget = { VEC3_INIT };	// 目標向き
			m_Json["RotTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
			pDest->PosTarget = { VEC3_INIT };	// 目標座標
			m_Json["PosTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
		}

		GetSelectMotion()->vpKey.push_back(Key);

		// 総キー数をインクリメント、ジェイソンデータに保存
		GetSelectMotion()->wMaxKey++;
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;
	}
}

//============================================================================
// キー情報の再確保
//============================================================================
void CMotion_Manager::ResizeKey()
{
	
}

//============================================================================
// フレーム情報の編集
//============================================================================
void CMotion_Manager::EditFrame()
{
	const int nOldMaxFrame = GetSelectKey()->nMaxFrame;

	// 総フレーム増減
	ImGui::Separator();
	ImGui::BulletText("Frame");
	ImGui::InputInt("MaxFrame", &GetSelectKey()->nMaxFrame, 1, 1);

	// フレーム数の下限を設定
	if (GetSelectKey()->nMaxFrame < 1)
	{
		GetSelectKey()->nMaxFrame = 1;
	}

	// 増減があればジェイソンデータを変更
	if (nOldMaxFrame != GetSelectKey()->nMaxFrame)
	{
		m_Json["MaxFrame"][m_wSelectKey] = GetSelectKey()->nMaxFrame;
	}
}

//============================================================================
// エクスポート
//============================================================================
void CMotion_Manager::Export()
{
	// ファイル名を作成
	std::string FileName = "Data\\JSON\\motion_export.json";

	// ファイルを書き出し展開
	std::ofstream Ofs(FileName, std::ios::out);

	if (Ofs.good())
	{
		// ジェイソンデータをシリアライズ
		Ofs << m_Json.dump(1, '	');	// 第一引数 -> インデント数, 第二引数 -> インデント形式

		CRenderer::SetTimeString("モーションデータ[" + FileName + "]を書き出しました", 120);
	}
	else
	{
		assert(false && "ファイルの書き込みに失敗");
	}
}

//============================================================================
// リセット
//============================================================================
void CMotion_Manager::Reset()
{
	// エクスポートボタンの横
	ImGui::SameLine();

	if (ImGui::Button("Reset Edit Data"))
	{
		// 選択番号情報を初期化
		m_wSelectParts = 0;
		m_wSelectMotion = 0;
		m_wSelectKey = 0;

		// 一旦解放
		m_MotionSet->Release();

		// 即再生成
		m_MotionSet = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\motion_export.json"));
	}
}

//============================================================================
// 選択中のパーツのポインタを取得
//============================================================================
CObject_Parts* const CMotion_Manager::GetSelectParts() const
{
	return m_MotionSet->m_vpModelParts[m_wSelectParts];
}

//============================================================================
// 選択中のモーションのポインタを取得
//============================================================================
CMotion_Set::Motion* const CMotion_Manager::GetSelectMotion() const
{
	return &m_MotionSet->m_apMotion[m_wSelectMotion];
}

//============================================================================
// 選択中のキーのポインタを取得
//============================================================================
CMotion_Set::Key* const CMotion_Manager::GetSelectKey() const
{
	return &GetSelectMotion()->vpKey[m_wSelectKey];
}