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
#include "object_X.h"
#include "manager.h"
#include "renderer.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define SORT_ON_EXPORT 1	// エクスポート時に整理

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
	// 初回インポート
	if (!m_bInitImport)
	{
		if (Import())
			m_bInitImport = true;

		return;
	}

	// インポート
	if (Import())
		return;	// インポート時更新しない

	// ウィンドウを表示
	ImVec2 Rect = { 600, -1 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ SCREEN_WIDTH - (Rect.x + 100), 10 });
	ImGui::Begin("Edit MotionList");

	// 編集
	Edit();

	if (m_bPlay)
	{
		// 動作
		m_MotionSet->Animation();
	}

	if (m_bShowKeyEnd)
	{
		// キーの最終的な動きを見せる
		ShowKeyEnd();
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
	m_bInitImport{ false },
	m_NowSetFileName{},
	m_Json{},
	m_MotionSet{ nullptr },
	m_wSelectParts{ 0 },
	m_CopyKey{ 0, nullptr },
	m_wSelectKey{ 0 },
	m_bPartsAppeal{ false },
	m_bPlay{ false },
	m_bShowKeyEnd{ false }
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
#if 0
	// ジェイソンデータを取得
	m_Json = utility::OpenJsonFile("Data\\JSON\\motion_export.json");

	// モーションセットを生成
	m_MotionSet = CMotion_Set::Create(m_Json);
#endif

	// 選択パーツをアピール
	m_bPartsAppeal = true;

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
#if 0
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
#else
	// ウィンドウを表示
	ImVec2 Rect = { 500, 400 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ 50, 610 });
	ImGui::Begin("MotionManager Infomation");
	ImGui::Text("MaxParts:%d", m_MotionSet->m_wMaxParts);
	ImGui::SameLine();
	ImGui::Text("MaxMotion:%d", m_MotionSet->m_wMaxMotion);
	ImGui::Separator();
	ImGui::Text("PlayingMotion:%d :(%d)", m_MotionSet->m_wNowMotion, m_MotionSet->m_wMaxMotion);
	ImGui::Text("PlayingKey:%d :(%d)", m_MotionSet->m_wNowKey, m_MotionSet->GetNowMotionPtr()->wMaxKey);
	ImGui::Text("PlayingFrame:%d :(%d)", m_MotionSet->m_wNowFrame, m_MotionSet->GetNowKey()->nMaxFrame);
	for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
	{
		const CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[wCntModelParts];
		const CObject_Parts* const pParts = m_MotionSet->m_vpModelParts[wCntModelParts];
		ImGui::Separator();
		ImGui::BulletText("Parts[%d] KeyDest Infomation", wCntModelParts);
		ImGui::Text("Scale[ X:(%.2f / %.2f) X:(%.2f / %.2f) X:(%.2f / %.2f) ]", pParts->GetScale().x, pDest->ScaleTarget.x, pParts->GetScale().y, pDest->ScaleTarget.y, pParts->GetScale().z, pDest->ScaleTarget.z);
		ImGui::Text("Rot[ X:(%.2f / %.2f) X:(%.2f / %.2f) X:(%.2f / %.2f) ]", pParts->GetRot().x, pDest->RotTarget.x, pParts->GetRot().y, pDest->RotTarget.y, pParts->GetRot().z, pDest->RotTarget.z);
		ImGui::Text("Pos[ X:(%.2f / %.2f) X:(%.2f / %.2f) X:(%.2f / %.2f) ]", pParts->GetPos().x, pDest->PosTarget.x, pParts->GetPos().y, pDest->PosTarget.y, pParts->GetPos().z, pDest->PosTarget.z);
	}
	ImGui::End();
#endif
}

//============================================================================
// インポート
//============================================================================
bool CMotion_Manager::Import()
{
	// ウィンドウを表示
	ImVec2 Rect = { 600, 80 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ 50, 50 });
	if (ImGui::Begin("Import Motion Set"))
	{		
		static char FilePath[256] = "";

		if (ImGui::Button("Done"))
		{
			// 現状リセット
			Reset();

			// 現在のモーションセットを破棄
			Uninit();

			// ジェイソンデータを取得
			std::string Copy = FilePath;
			m_Json = utility::OpenJsonFile("Data\\JSON\\EXPORT\\" + Copy + ".json");

			// ファイル名をコピー
			m_NowSetFileName = Copy;

			// モーションセットを生成
			m_MotionSet = CMotion_Set::Create(m_Json);

			ImGui::End();
			return true;
		}
		
		ImGui::SameLine();
		ImGui::InputText("##", FilePath, 256);

		ImGui::End();
	}

	return false;
}

//============================================================================
// 編集
//============================================================================
void CMotion_Manager::Edit()
{
	// 再生切り替え
	if( ImGui::Checkbox("Playing motion", &m_bPlay))
	{
		// 再生を押したときにモーションストップのフラグが立っていたら
		if (m_bPlay && m_MotionSet->m_bStop)
			m_MotionSet->m_bStop = false;
	}
	ImGui::SameLine();
	ImGui::Checkbox("Show this key end", &m_bShowKeyEnd);

	// キーの最後の動きを見せる時はアニメーションしない
	if (m_bShowKeyEnd)
	{
		m_bPlay = false;
	}

	// パーツ情報の編集
	EditParts();

	// モーション情報の編集
	EditMotion();

	// キー情報の編集
	EditKey();

	// フレーム情報の編集
	EditFrame();

	// 目標値情報の編集
	EditDest();

	// エクスポート
	ImGui::Separator();
	static char FilePath[256] = "";
	std::string Copy = FilePath;
	ImGui::InputText("##", FilePath, 256);
	ImGui::SameLine();
	if (ImGui::Button("Export"))
	{
		Export(Copy);
	}
}

//============================================================================
// パーツ情報の編集
//============================================================================
void CMotion_Manager::EditParts()
{
	// 選択パーツのアピール切り替え
	ImGui::Separator();
	ImGui::BulletText("Select Parts");
	ImGui::Checkbox("Appeal select parts", &m_bPartsAppeal);

	// 選択パーツ切り替え
	if (ImGui::Button("FirstParts"))
	{
		m_wSelectParts = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("PrevParts"))
	{
		m_wSelectParts > 0 ? m_wSelectParts-- : m_wSelectParts = static_cast<WORD>(m_MotionSet->m_vpModelParts.size()) - 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("NextParts"))
	{
		m_wSelectParts < m_MotionSet->m_vpModelParts.size() - 1 ? m_wSelectParts++ : m_wSelectParts = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("LastParts"))
	{
		m_wSelectParts = m_MotionSet->m_vpModelParts.size() - 1;
	}
	ImGui::SameLine();
	ImGui::Text("Select:%d", m_wSelectParts);

	// 明滅
	static float fMin = 0.45f, fMax = 0.6f, fCoef = 0.03f;
	static XCol Col = { 1.0f, 1.0f, 1.0f, fMin };
	static bool bAdder = false;
	bAdder ?
		Col.a += (fMax - Col.a) * fCoef :
		Col.a += (fMin - Col.a) * fCoef;
	if (Col.a > fMax - fCoef)
		bAdder = false;
	else if (Col.a < fMin + fCoef)
		bAdder = true;

	// 選択パーツを透過
	for (WORD wCntParts = 0; wCntParts < m_MotionSet->m_vpModelParts.size(); ++wCntParts)
	{
		if (m_wSelectParts == wCntParts && m_bPartsAppeal)
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetCol(Col);
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(true);
		}
		else
		{
			m_MotionSet->m_vpModelParts[wCntParts]->SetUseCol(false);
		}
	}

	// オフセット情報の編集
	EditOffset();
}

//============================================================================
// オフセット情報の編集
//============================================================================
void CMotion_Manager::EditOffset()
{
	// 選択中のパーツのポインタ
	CObject_Parts* const pParts = m_MotionSet->m_vpModelParts[m_wSelectParts];

	// オフセット情報をコピー
	Vec3 ScaleOffset = m_MotionSet->m_vpModelParts[0]->GetScaleOffset(), RotOffset = pParts->GetRotOffset(), PosOffset = pParts->GetPosOffset();

	// ウィンドウを生成
	ImVec2 Rect = { 600, 450 };
	ImGui::SetNextWindowSize(Rect);
	ImGui::SetNextWindowPos({ 50, 145 });
	if (ImGui::Begin("Parts Offset"))
	{
		// 縮尺オフセットを変動
		ImGui::Separator();
		ImGui::BulletText("ScaleOffset");
		{
			// 縮尺：X
			if (ImGui::Button("Reset:ScaleX"))
			{
				ScaleOffset.x = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Scale:X", &ScaleOffset.x, 0.01f, 0.0f);
			// 縮尺：Y
			if (ImGui::Button("Reset:ScaleY"))
			{
				ScaleOffset.y = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Scale:Y", &ScaleOffset.y, 0.01f, 0.0f);
			// 縮尺：Z
			if (ImGui::Button("Reset:ScaleZ"))
			{
				ScaleOffset.z = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Scale:Z", &ScaleOffset.z, 0.01f, 0.0f);
			m_MotionSet->m_vpModelParts[0]->SetScaleOffset(ScaleOffset);
		}

		// 向きオフセットを変動
		ImGui::Separator();
		ImGui::BulletText("RotOffset");
		{
			// 向き：X
			if (ImGui::Button("Reset:RotX"))
			{
				RotOffset.x = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Rot:X", &RotOffset.x, D3DX_PI * 0.01f, -D3DX_PI, D3DX_PI);
			// 向き：X
			if (ImGui::Button("Reset:RotY"))
			{
				RotOffset.y = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Rot:Y", &RotOffset.y, D3DX_PI * 0.01f, -D3DX_PI, D3DX_PI);
			// 向き：Z
			if (ImGui::Button("Reset:RotZ"))
			{
				RotOffset.z = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Rot:Z", &RotOffset.z, D3DX_PI * 0.01f, -D3DX_PI, D3DX_PI);
			pParts->SetRotOffset(RotOffset);
		}

		// 座標オフセットを変動
		ImGui::Separator();
		ImGui::BulletText("PosOffset");
		{
			// 座標：X
			if (ImGui::Button("Reset:PosX"))
			{
				PosOffset.x = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Pos:X", &PosOffset.x, 0.01f);
			// 座標：Y
			if (ImGui::Button("Reset:PosY"))
			{
				PosOffset.y = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Pos:Y", &PosOffset.y, 0.01f);
			// 座標：Z
			if (ImGui::Button("Reset:PosZ"))
			{
				PosOffset.z = 0.0f;
			}
			ImGui::SameLine();
			ImGui::DragFloat("Pos:Z", &PosOffset.z, 0.01f);
			pParts->SetPosOffset(PosOffset);
		}

		ImGui::End();
	}
}

//============================================================================
// モーション情報の編集
//============================================================================
void CMotion_Manager::EditMotion()
{
	// 選択モーション切り替え
	bool bChange = false;
	ImGui::Separator();
	ImGui::BulletText("Select Motion");
	if (ImGui::Button("FirstMotion"))
	{
		m_MotionSet->m_wNowMotion = 0;
		bChange = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("PrevMotion"))
	{
		m_MotionSet->m_wNowMotion > 0 ? m_MotionSet->m_wNowMotion-- : m_MotionSet->m_wNowMotion = m_MotionSet->m_wMaxMotion - 1;
		bChange = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("NextMotion"))
	{
		m_MotionSet->m_wNowMotion < m_MotionSet->m_wMaxMotion - 1 ? m_MotionSet->m_wNowMotion++ : m_MotionSet->m_wNowMotion = 0;
		bChange = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("LastMotion"))
	{
		m_MotionSet->m_wNowMotion = m_MotionSet->m_wMaxMotion - 1;
		bChange = true;
	}
	ImGui::SameLine();
	ImGui::Text("Select:%d", m_MotionSet->m_wNowMotion);

	// モーションを切り替えていたら
	if (bChange)
	{
		m_MotionSet->m_bStop = false;
		m_MotionSet->m_wNowKey = 0;
		m_MotionSet->m_wNowFrame = 0;
	}

	// 現在の再生キーが総キー数を超えないよう制限
	if (m_MotionSet->m_wNowKey >= GetSelectMotion()->wMaxKey)
	{
		m_MotionSet->m_wNowKey = GetSelectMotion()->wMaxKey - 1;
	}

	// 選択キー番号を調整
	if (m_wSelectKey >= GetSelectMotion()->wMaxKey)
	{
		m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
	}
	
	// ループフラグ切り替え
	ImGui::BulletText("Edit Motion");
	ImGui::Checkbox("Motion Loop", &m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].bLoop);

	// 選択中のモーションのループフラグを立てたときに、現在の再生モーションがストップしていたら
	if (m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].bLoop && m_MotionSet->m_bStop)
		m_MotionSet->m_bStop = false;

	// 総モーション数の切り替え
	if (ImGui::Button("--Motion") && m_MotionSet->m_wMaxMotion > 1)
	{
		// 総モーション数をデクリメントし
		m_MotionSet->m_wMaxMotion--;
#if !SORT_ON_EXPORT
		m_Json["MaxMotion"] = m_MotionSet->m_wMaxMotion;
#endif	// _SORT_ON_EXPORT_

		// 現在の再生モーションが総モーション数を超えないよう制限
		if (m_MotionSet->m_wNowMotion >= m_MotionSet->m_wMaxMotion)
		{
			m_MotionSet->m_wNowMotion = m_MotionSet->m_wMaxMotion - 1;
		}

		// モーション情報のポインタを作成
		CMotion_Set::Motion* pMotion = &m_MotionSet->m_vpMotion[m_MotionSet->m_wMaxMotion];

		// キー数分の目標値情報のポインタ配列を破棄
		for (WORD wCntKey = 0; wCntKey < pMotion->wMaxKey; ++wCntKey)
		{
			if (pMotion->vpKey[wCntKey].apDest != nullptr)
			{
				delete[] pMotion->vpKey[wCntKey].apDest;
				pMotion->vpKey[wCntKey].apDest = nullptr;
			}
		}

		// キー情報も削除
		pMotion->vpKey.clear();

		// 末尾のモーション情報を削除
		m_MotionSet->m_vpMotion.pop_back();
	}
	ImGui::SameLine();
	if (ImGui::Button("++Motion"))
	{
		// モーション情報オブジェクトを新規作成
		CMotion_Set::Motion Motion;

		// 新たなモーションのループフラグを設定
		Motion.bLoop = false;

		// 新たなモーションの総キー数を設定
		Motion.wMaxKey = 1;
#if !SORT_ON_EXPORT
		m_Json["MaxKey"][m_MotionSet->m_wMaxMotion] = 1;
#endif	// _SORT_ON_EXPORT_

		// キー情報オブジェクトを新規作成
		CMotion_Set::Key Key;

		// 新たなキーの総フレーム数を設定
		Key.nMaxFrame = 1;
#if !SORT_ON_EXPORT
		m_Json["MaxFrame"][Motion.wMaxKey] = 1;
#endif	// _SORT_ON_EXPORT_

		// パーツ数分の目標値情報を追加生成
		Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet->m_wMaxParts];

		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// 目標値情報のポインタを作成
			CMotion_Set::KeyDest* const pDest = &Key.apDest[wCntModelParts];

			// 新たなモーションのパーツの目標値を設定
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };	// 目標縮尺
			pDest->RotTarget = { VEC3_INIT };			// 目標向き
			pDest->PosTarget = { VEC3_INIT };			// 目標座標
		}

		// キー情報を追加
		Motion.vpKey.push_back(Key);

		// モーション情報を追加
		m_MotionSet->m_vpMotion.push_back(Motion);

		// 総モーション数をインクリメント
		m_MotionSet->m_wMaxMotion++;
#if !SORT_ON_EXPORT
		m_Json["MaxMotion"] = m_MotionSet->m_wMaxMotion;
#endif	// _SORT_ON_EXPORT_
	}
	ImGui::SameLine();
	ImGui::Text("MaxMotion:%d", m_MotionSet->m_wMaxMotion);
}

//============================================================================
// キー情報の編集
//============================================================================
void CMotion_Manager::EditKey()
{
	// 選択キー切り替え
	ImGui::Separator();
	ImGui::BulletText("Select Key");
	if (ImGui::Button("FirstKey"))
	{
		m_wSelectKey = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("PrevKey") || CManager::GetMouse()->GetTrigger(3))
	{
		m_wSelectKey > 0 ? m_wSelectKey-- : m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("NextKey") || CManager::GetMouse()->GetTrigger(4))
	{
		m_wSelectKey < GetSelectMotion()->wMaxKey - 1 ? m_wSelectKey++ : m_wSelectKey = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("LastKey"))
	{
		m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
	}
	ImGui::SameLine();
	ImGui::Text("Select:%d", m_wSelectKey);

#if 1
	ImGui::BulletText("Copy Key");
	{ // キーごとコピーペースト

		// ウィンドウとウィジェットの幅をコピー
		float fWidth = ImGui::GetContentRegionAvail().x, fSeparate = ImGui::GetStyle().ItemSpacing.x;

		// ボタンのサイズ
		const float fCopyPasteButtonWidth = (fWidth + -fSeparate * 1.0f) / 2.0f;

		// コピー用
		static std::vector<CMotion_Set::KeyDest> CopyAllDest;

		if (ImGui::Button("Copy##All", { fCopyPasteButtonWidth, 30.0f }))
		{
			// 先のコピーをリセット
			CopyAllDest.clear();

			// 現在のキーの全パーツの全目標をコピー
			for (WORD wCntParts = 0; wCntParts < m_MotionSet->m_wMaxParts; ++wCntParts)
				CopyAllDest.push_back(GetSelectKey()->apDest[wCntParts]);
		}
		ImGui::SameLine();
		if (ImGui::Button("Paste##All", { fCopyPasteButtonWidth, 30.0f }))
		{
			// 現在のキーの全パーツの全目標をペースト
			for (WORD wCntParts = 0; wCntParts < m_MotionSet->m_wMaxParts; ++wCntParts)
				GetSelectKey()->apDest[wCntParts] = CopyAllDest[wCntParts];
		}
	}
#endif

	// 総キー数の切り替え
	ImGui::BulletText("Edit Key");
	if (ImGui::Button("--Key") && GetSelectMotion()->wMaxKey > 1)
	{
		// 総キー数をデクリメントし
		GetSelectMotion()->wMaxKey--;
#if !SORT_ON_EXPORT
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;
#endif	// _SORT_ON_EXPORT_

		// 現在の再生キーが総キー数を超えないよう制限
		if (m_MotionSet->m_wNowKey >= GetSelectMotion()->wMaxKey)
		{
			m_MotionSet->m_wNowKey = GetSelectMotion()->wMaxKey - 1;
		}

		// 選択キー番号を調整
		if (m_wSelectKey >= GetSelectMotion()->wMaxKey)
		{
			m_wSelectKey = GetSelectMotion()->wMaxKey - 1;
		}

#if !SORT_ON_EXPORT
		// 消去するキー番号のフレーム・目標値情報をジェイソンデータから消去
		m_Json["MaxFrame"].erase(GetSelectMotion()->wMaxKey);
		m_Json["ScaleTarget"].erase(GetSelectMotion()->wMaxKey);
		m_Json["RotTarget"].erase(GetSelectMotion()->wMaxKey);
		m_Json["PosTarget"].erase(GetSelectMotion()->wMaxKey);
#endif	// _SORT_ON_EXPORT_

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
	}
	ImGui::SameLine();
	if (ImGui::Button("++Key"))
	{
		// キー情報オブジェクトを新規作成
		CMotion_Set::Key Key;

		// 新たなキーの総フレーム数を設定
		Key.nMaxFrame = 1;
#if !SORT_ON_EXPORT
		m_Json["MaxFrame"][GetSelectMotion()->wMaxKey] = Key.nMaxFrame;
#endif	// _SORT_ON_EXPORT_

		// パーツ数分の目標値情報を追加生成
		Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_MotionSet->m_wMaxParts];

		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// 目標値情報のポインタを作成
			CMotion_Set::KeyDest* const pDest = &Key.apDest[wCntModelParts];

			// 新たなキーのパーツの目標値を設定し、ジェイソンデータに保存
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };	// 目標縮尺
			pDest->RotTarget = { VEC3_INIT };			// 目標向き
			pDest->PosTarget = { VEC3_INIT };			// 目標座標
#if !SORT_ON_EXPORT
			m_Json["ScaleTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
			m_Json["RotTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
			m_Json["PosTarget"][GetSelectMotion()->wMaxKey][wCntModelParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
#endif	// _if !SORT_ON_EXPORT_
		}

		GetSelectMotion()->vpKey.push_back(Key);

		// 総キー数をインクリメント
		GetSelectMotion()->wMaxKey++;
#if !SORT_ON_EXPORT
		m_Json["MaxKey"] = GetSelectMotion()->wMaxKey;
#endif	// _SORT_ON_EXPORT_
	}
	ImGui::SameLine();
	ImGui::Text("MaxKey:%d", m_MotionSet->GetNowMotionPtr()->wMaxKey);
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

	// 現在の再生フレームが総フレーム数を超えないよう制限
	if (m_MotionSet->m_wNowFrame >= m_MotionSet->GetNowKey()->nMaxFrame)
	{
		m_MotionSet->m_wNowFrame = m_MotionSet->GetNowKey()->nMaxFrame - 1;
	}

#if !SORT_ON_EXPORT
	// 増減があればジェイソンデータを変更
	if (nOldMaxFrame != GetSelectKey()->nMaxFrame)
	{
		m_Json["MaxFrame"][m_wSelectKey] = GetSelectKey()->nMaxFrame;
	}
#endif	// _SORT_ON_EXPORT_
}

//============================================================================
// 目標値情報の編集
//============================================================================
void CMotion_Manager::EditDest()
{
	// 選択中のパーツの目標値情報のポインタを作成
	CMotion_Set::KeyDest* const pDest = &GetSelectKey()->apDest[m_wSelectParts];

	// ウィンドウとウィジェットの幅をコピー
	float fWidth = ImGui::GetContentRegionAvail().x, fSeparate = ImGui::GetStyle().ItemSpacing.x;
	
	// ボタンのサイズ
	const float 
		fResetButtonWidth = (fWidth + -fSeparate * 3.0f) / 4.0f,
		fCopyPasteButtonWidth = (fWidth + -fSeparate * 1.0f) / 2.0f;

	// 目標縮尺を変動
	ImGui::SeparatorText("ScaleDest");
	{
		// 縮尺リセット
		if (ImGui::Button("Reset:X##Scale", { fResetButtonWidth, 30.0f }))
			pDest->ScaleTarget.x = 1.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Y##Scale", { fResetButtonWidth, 30.0f }))
			pDest->ScaleTarget.y = 1.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Z##Scale", { fResetButtonWidth, 30.0f }))
			pDest->ScaleTarget.z = 1.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:All##Scale", { fResetButtonWidth, 30.0f }))
			pDest->ScaleTarget = { 1.0f, 1.0f, 1.0f };
		ImGui::Separator();

		// 縮尺変更
		float* ScalePtr[3] = { &pDest->ScaleTarget.x, &pDest->ScaleTarget.y, &pDest->ScaleTarget.z };
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::DragFloat3("##Scale", *ScalePtr, 0.01f, 0.0f);
		ImGui::Separator();

		// コピーペースト
		static Vec3 Copy = { 1.0f, 1.0f, 1.0f };
		if (ImGui::Button("Copy##Scale", { fCopyPasteButtonWidth, 30.0f }))
			Copy = pDest->ScaleTarget;
		ImGui::SameLine();
		if (ImGui::Button("Paste##Scale", { fCopyPasteButtonWidth, 30.0f }))
			pDest->ScaleTarget = Copy;
	}

	// 目標向きを変動
	ImGui::SeparatorText("RotDest");
	{
		// 向きリセット
		if (ImGui::Button("Reset:X##Rot", { fResetButtonWidth, 30.0f }))
			pDest->RotTarget.x = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Y##Rot", { fResetButtonWidth, 30.0f }))
			pDest->RotTarget.y = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Z##Rot", { fResetButtonWidth, 30.0f }))
			pDest->RotTarget.z = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:All##Rot", { fResetButtonWidth, 30.0f }))
			pDest->RotTarget = VEC3_INIT;
		ImGui::Separator();

		// 向き変更
		float* RotPtr[3] = { &pDest->RotTarget.x, &pDest->RotTarget.y, &pDest->RotTarget.z };
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::DragFloat3("##Rot", *RotPtr, 0.01f, 0.0f);
		ImGui::Separator();

		// コピーペースト
		static Vec3 Copy = { 0.0f, 0.0f, 0.0f };
		if (ImGui::Button("Copy##Rot", { fCopyPasteButtonWidth, 30.0f }))
			Copy = pDest->RotTarget;
		ImGui::SameLine();
		if (ImGui::Button("Paste##Rot", { fCopyPasteButtonWidth, 30.0f }))
			pDest->RotTarget = Copy;
	}

	// 目標座標
	ImGui::SeparatorText("PosDest");
	{
		// 座標リセット
		if (ImGui::Button("Reset:X##Pos", { fResetButtonWidth, 30.0f }))
			pDest->PosTarget.x = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Y##Pos", { fResetButtonWidth, 30.0f }))
			pDest->PosTarget.y = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:Z##Pos", { fResetButtonWidth, 30.0f }))
			pDest->PosTarget.z = 0.0f;
		ImGui::SameLine();
		if (ImGui::Button("Reset:All##Pos", { fResetButtonWidth, 30.0f }))
			pDest->PosTarget = VEC3_INIT;
		ImGui::Separator();

		// 座標変更
		float* PosPtr[3] = { &pDest->PosTarget.x, &pDest->PosTarget.y, &pDest->PosTarget.z };
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::DragFloat3("##Pos", *PosPtr, 0.01f, 0.0f);
		ImGui::Separator();

		// コピーペースト
		static Vec3 Copy = { 0.0f, 0.0f, 0.0f };
		if (ImGui::Button("Copy##Pos", { fCopyPasteButtonWidth, 30.0f }))
			Copy = pDest->PosTarget;
		ImGui::SameLine();
		if (ImGui::Button("Paste##Pos", { fCopyPasteButtonWidth, 30.0f }))
			pDest->PosTarget = Copy;
	}

#if !SORT_ON_EXPORT
	// 目標値を反映
	m_Json["ScaleTarget"][m_wSelectKey][m_wSelectParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
	m_Json["RotTarget"][m_wSelectKey][m_wSelectParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
	m_Json["PosTarget"][m_wSelectKey][m_wSelectParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
#endif	// _SORT_ON_EXPORT_
}

//============================================================================
// キーの最終的な状態を見せる
//============================================================================
void CMotion_Manager::ShowKeyEnd()
{
	// 全てのパーツが、このキーの目標値に固定される
	for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
	{
		// 目標縮尺
		m_MotionSet->m_vpModelParts[wCntModelParts]->SetScale(m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].vpKey[m_wSelectKey].apDest[wCntModelParts].ScaleTarget);

		// 目標向き
		m_MotionSet->m_vpModelParts[wCntModelParts]->SetRot(m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].vpKey[m_wSelectKey].apDest[wCntModelParts].RotTarget);

		// 目標座標
		m_MotionSet->m_vpModelParts[wCntModelParts]->SetPos(m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion].vpKey[m_wSelectKey].apDest[wCntModelParts].PosTarget);
	}
}

//============================================================================
// エクスポート
//============================================================================
void CMotion_Manager::Export(std::string Copy)
{
	// ファイル名を作成
	std::string FileName = "Data\\JSON\\EXPORT\\" + Copy + ".json";

	// ファイルを書き出し展開
	std::ofstream Ofs(FileName, std::ios::out);

	if (Ofs.good())
	{
#if SORT_ON_EXPORT

		// 書き出し用ジェイソンオブジェクトを作成
		JSON Export;

		// 総モーション数
		Export["MaxMotion"] = m_MotionSet->m_wMaxMotion;

		// 総パーツ数
		Export["MaxParts"] = m_MotionSet->m_wMaxParts;

		// モデルタイプ
		Export["ModelType"] = m_Json["ModelType"];

		// 親パーツのインデックス
		Export["ParentIdx"] = m_Json["ParentIdx"];

		// 縮尺オフセット
		Export["ScaleOffset"] = { m_MotionSet->m_vpModelParts[0]->GetScaleOffset().x, m_MotionSet->m_vpModelParts[0]->GetScaleOffset().y, m_MotionSet->m_vpModelParts[0]->GetScaleOffset().z };
			
		for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
		{
			// 選択中のパーツのポインタ
			CObject_Parts* const pParts = m_MotionSet->m_vpModelParts[wCntModelParts];

			// 向きオフセット
			Export["RotOffset"][wCntModelParts] = { pParts->GetRotOffset().x, pParts->GetRotOffset().y, pParts->GetRotOffset().z };
			
			// 座標オフセット
			Export["PosOffset"][wCntModelParts] = { pParts->GetPosOffset().x, pParts->GetPosOffset().y, pParts->GetPosOffset().z };
		}

		for (WORD wCntMotion = 0; wCntMotion < m_MotionSet->m_wMaxMotion; ++wCntMotion)
		{
			// ループフラグ
			Export["Loop"][wCntMotion] = m_MotionSet->m_vpMotion[wCntMotion].bLoop;

			// 総キー数
			Export["MaxKey"][wCntMotion] = m_MotionSet->m_vpMotion[wCntMotion].wMaxKey;

			for (WORD wCntKey = 0; wCntKey < m_MotionSet->m_vpMotion[wCntMotion].wMaxKey; ++wCntKey)
			{
				const CMotion_Set::Key* const pKey = &m_MotionSet->m_vpMotion[wCntMotion].vpKey[wCntKey];

				// 総フレーム数
				Export["MaxFrame"][wCntMotion][wCntKey] = pKey->nMaxFrame;

				for (WORD wCntModelParts = 0; wCntModelParts < m_MotionSet->m_wMaxParts; ++wCntModelParts)
				{
					const CMotion_Set::KeyDest* const pDest = &pKey->apDest[wCntModelParts];

					Export["ScaleTarget"][wCntMotion][wCntKey][wCntModelParts] = { pDest->ScaleTarget.x, pDest->ScaleTarget.y, pDest->ScaleTarget.z };
					Export["RotTarget"][wCntMotion][wCntKey][wCntModelParts] = { pDest->RotTarget.x, pDest->RotTarget.y, pDest->RotTarget.z };
					Export["PosTarget"][wCntMotion][wCntKey][wCntModelParts] = { pDest->PosTarget.x, pDest->PosTarget.y, pDest->PosTarget.z };
				}
			}
		}
#endif	// _SORT_ON_EXPORT_

		// ジェイソンデータをシリアライズ
		Ofs << Export.dump(1, '	');	// 第一引数 -> インデント数, 第二引数 -> インデント形式

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

	if (ImGui::Button("Reset"))
	{
		// 選択番号情報を初期化
		m_wSelectParts = 0;
		m_wSelectKey = 0;

		// 一旦解放
		m_MotionSet->Release();

		// 即再生成
		m_MotionSet = CMotion_Set::Create(utility::OpenJsonFile("Data\\JSON\\EXPORT\\" + m_NowSetFileName + ".json"));
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
	return &m_MotionSet->m_vpMotion[m_MotionSet->m_wNowMotion];
}

//============================================================================
// 選択中のキーのポインタを取得
//============================================================================
CMotion_Set::Key* const CMotion_Manager::GetSelectKey() const
{
	return &GetSelectMotion()->vpKey[m_wSelectKey];
}