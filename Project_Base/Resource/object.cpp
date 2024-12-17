//============================================================================
// 
// オブジェクト管理 [object.cpp]
// Author : 福田歩希
// 
//============================================================================

/* これはステンシルバッファの描画テスト用 */
namespace
{
	int
	StencilRefValue = 0,			// ステンシル参照値
	StencilCmp = D3DCMP_ALWAYS,		// ステンシル値の比較方法
	Pass = D3DSTENCILOP_KEEP,		// Zテスト・ステンシルテストに成功
	ZFail = D3DSTENCILOP_KEEP,		// Zテストのみ失敗
	Fail = D3DSTENCILOP_KEEP;		// Zテスト・ステンシルテストに失敗
}

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"
#include "renderer.h"

// ステンシルテスト用
#include "manager.h"
#include "mask_rectangle.h"

//****************************************************
// 静的メンバの初期化
//****************************************************
int		 CObject::m_nNumAll = 0;								// オブジェクト総数
CObject* CObject::m_pTop[static_cast<WORD>(LAYER::MAX)] = {};	// 先頭オブジェクトのポインタ
CObject* CObject::m_pCur[static_cast<WORD>(LAYER::MAX)] = {};	// 終端オブジェクトのポインタ

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CObject::CObject() :
	m_wPriority{ static_cast<WORD>(LAYER::BG) },
	m_pPrev{ nullptr },
	m_pNext{ nullptr },
	m_Type{ TYPE::NONE },
	m_bDeath{ false }
{
	// このオブジェクトをリストに登録
	if (m_pCur[m_wPriority] == nullptr)
	{ // 終端オブジェクトが無い (オブジェクトが1つも存在しない)

		// このオブジェクトを先頭と終端に登録
		m_pTop[m_wPriority] = this;
		m_pCur[m_wPriority] = this;
	}
	else
	{ // 終端オブジェクトがある

		// 現在の終端をこのオブジェクトの前として登録
		m_pPrev = m_pCur[m_wPriority];

		// 新たな終端としてこのオブジェクトを登録
		m_pCur[m_wPriority] = this;

		// 前のオブジェクトの次にこのオブジェクトを登録
		m_pPrev->m_pNext = this;
	}

	// 総数をカウントアップ
	m_nNumAll++;
}

//============================================================================
// 描画優先度指定コンストラクタ
//============================================================================
CObject::CObject(LAYER Priority) :
	m_wPriority{ static_cast<WORD>(Priority) },
	m_pPrev{ nullptr },
	m_pNext{ nullptr },
	m_Type{ TYPE::NONE },
	m_bDeath{ false }
{
	// このオブジェクトをリストに登録
	if (m_pCur[m_wPriority] == nullptr)
	{ // 終端オブジェクトが無い (オブジェクトが1つも存在しない)
		
		// このオブジェクトを先頭と終端に登録
		m_pTop[m_wPriority] = this;
		m_pCur[m_wPriority] = this;
	}
	else
	{ // 終端オブジェクトがある

		// 現在の終端をこのオブジェクトの前として登録
		m_pPrev = m_pCur[m_wPriority];

		// 新たな終端としてこのオブジェクトを登録
		m_pCur[m_wPriority] = this;

		// 前のオブジェクトの次にこのオブジェクトを登録
		m_pPrev->m_pNext = this;
	}

	// 総数をカウントアップ
	m_nNumAll++;
}

//============================================================================
// デストラクタ
//============================================================================
CObject::~CObject()
{
	
}

//============================================================================
// 解放設定
//============================================================================
void CObject::SetRelease()
{
	if (!m_bDeath)
	{
		m_bDeath = true;
	}
}

//============================================================================
// 解放処理
//============================================================================
void CObject::Release()
{
	int nPriority = m_wPriority;	// プライオリティをコピーしておく

	if (m_pPrev == nullptr)
	{ // 前のオブジェクトが無い (このオブジェクトが先頭)

		if (m_pNext == nullptr)
		{ // 次のオブジェクトが無い (オブジェクトが全て無くなる)
			
			m_pTop[nPriority] = nullptr;	// 先頭オブジェクトのポインタを初期化
			m_pCur[nPriority] = nullptr;	// 終端オブジェクトのポインタを初期化
		}
		else
		{ // 次のオブジェクトはある

			// 新たな先頭として次のオブジェクトを登録
			m_pTop[nPriority] = m_pNext;

			// 次のオブジェクトの前のポインタを初期化
			m_pNext->m_pPrev = nullptr;
		}
	}
	else
	{ // 前のオブジェクトがある

		if (m_pNext == nullptr)
		{ // 次のオブジェクトが無い (このオブジェクトが終端)

			// 新たな終端として前のオブジェクト登録
			m_pCur[nPriority] = m_pPrev;

			// 前のオブジェクトの次のポインタを初期化
			/* そもそも「このオブジェクトの次のポインタ(m_pNext)を渡す」という行為なら、中身の有無にかかわらずつながるのでは */
			m_pPrev->m_pNext = nullptr;
		}
		else
		{ // 次のオブジェクトもある

			// 前のオブジェクトの次にこのオブジェクトの次を登録
			m_pPrev->m_pNext = m_pNext;

			// 次のオブジェクトの前に
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	Uninit();		// 終了処理
	delete this;	// メモリを解放
	m_nNumAll--;	// 総数をカウントダウン
}

//============================================================================
// タイプを取得
//============================================================================
const CObject::TYPE& CObject::GetType() const
{
	return m_Type;
}

//============================================================================
// タイプを設定
//============================================================================
void CObject::SetType(TYPE Type)
{
	m_Type = Type;
}

//============================================================================
// 前のポインタを取得
//============================================================================
CObject* CObject::GetPrev()
{
	return m_pPrev;
}

//============================================================================
// 次のポインタを取得
//============================================================================
CObject* CObject::GetNext()
{
	return m_pNext;
}

//============================================================================
// 全解放処理
//============================================================================
void CObject::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// 先頭オブジェクトのポインタをコピー
		CObject* pObj = m_pTop[nCntPriority];

		// 次のオブジェクトが無くなるまで
		while (pObj != nullptr)
		{
			// 次のオブジェクトのポインタをコピー
			CObject* pNext = pObj->m_pNext;

			// 解放処理
			pObj->Release();

			// このオブジェクトを次のポインタで上書き
			pObj = pNext;
		}
	}
}

//============================================================================
// 全更新処理
//============================================================================
void CObject::UpdateAll()
{
#ifdef _DEBUG	// オブジェクト数を表示

	CRenderer::SetDebugString("現在のオブジェクト数:" + std::to_string(m_nNumAll));

#endif	// _DEBUG

	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// 先頭オブジェクトのポインタをコピー
		CObject* pObj = m_pTop[nCntPriority];

		// 次のオブジェクトが無くなるまで
		while (pObj != nullptr)
		{
			// 次のオブジェクトのポインタをコピー
			CObject* pNext = pObj->m_pNext;

			// 更新処理
			pObj->Update();

			// 次のオブジェクトのポインタをコピー
			pObj = pNext;
		}
	}
}

//============================================================================
// 全後更新処理
//============================================================================
void CObject::LateUpdateAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// 先頭オブジェクトのポインタをコピー
		CObject* pObj = m_pTop[nCntPriority];

		// 次のオブジェクトが無くなるまで
		while (pObj != nullptr)
		{
			// 次のオブジェクトのポインタをコピー
			CObject* pNext = pObj->m_pNext;

			// 死亡フラグが出ていたら
			if (pObj->m_bDeath)
			{
				// 解放処理
				pObj->Release();
			}

			// 次のオブジェクトのポインタをコピー
			pObj = pNext;
		}
	}

#ifdef _DEBUG
#if 0	/* ステンシルテストの設定変更 */
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Stencil - All Obect"))
	{
		D3DCMP_GREATEREQUAL;
		ImGui::InputInt("StencilRefValue", &StencilRefValue);
		ImGui::InputInt("StencilCmp", &StencilCmp);
		ImGui::InputInt("Pass", &Pass);
		ImGui::InputInt("ZFail", &ZFail);
		ImGui::InputInt("Fail", &Fail);
		ImGui::End();
	}
#endif
#endif // _DEBUG
}

//============================================================================
// 全描画処理
//============================================================================
void CObject::DrawAll()
{
	// デバイスを取得
	auto pDev = CRenderer::GetDeviece();

	// ステンシルバッファへの書き込みを有効化
	pDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// 先頭オブジェクトのポインタをコピー
		CObject* pObj = m_pTop[nCntPriority];

		// 次のオブジェクトが無くなるまで
		while (pObj != nullptr)
		{
			// 次のオブジェクトのポインタをコピー
			CObject* pNext = pObj->m_pNext;

			// ステンシル参照値を設定
			pDev->SetRenderState(D3DRS_STENCILREF, StencilRefValue);

			// ステンシルバッファの比較方法を変更
			pDev->SetRenderState(D3DRS_STENCILFUNC, StencilCmp);

			// ステンシルテストの結果に対してのふるまいを設定する
			pDev->SetRenderState(D3DRS_STENCILPASS, Pass);		// Zテスト・ステンシルテストに成功
			pDev->SetRenderState(D3DRS_STENCILZFAIL, ZFail);	// Zテストのみ失敗
			pDev->SetRenderState(D3DRS_STENCILFAIL, Fail);		// Zテスト・ステンシルテストに失敗

			// 特定オブジェクト以外描画処理
			if (pObj->GetType() != TYPE::LATEDRAW)
			{
				// 描画処理
				pObj->Draw();
			}

			// 次のオブジェクトのポインタをコピー
			pObj = pNext;
		}
	}
	
#if 1	// 画面を覆うポリゴンの描画

	// Zバッファに書き込まない
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ステンシル参照値を設定
	pDev->SetRenderState(D3DRS_STENCILREF, 1);

	// ステンシルバッファの比較方法を変更
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// ステンシルテストの結果に対してのふるまいを設定する (Zテストはしない)
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);	// (Zテスト・)ステンシルテストに成功
	pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);	// (Zテスト・)ステンシルテストに失敗

	// マスクポリゴンの描画
	CManager::GetManager()->GetMask_Rectangle()->Draw();

	// Zバッファの書き込みを有効化
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ステンシルバッファへの書き込みを無効化
	pDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
#endif
}

//============================================================================
// 全後描画処理
//============================================================================
void CObject::LateDrawAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// 先頭オブジェクトのポインタをコピー
		CObject* pObj = m_pTop[nCntPriority];

		// 次のオブジェクトが無くなるまで
		while (pObj != nullptr)
		{
			// 次のオブジェクトのポインタをコピー
			CObject* pNext = pObj->m_pNext;

			// 特定オブジェクトのみ描画処理
			if (pObj->GetType() == TYPE::LATEDRAW)
			{
				pObj->Draw();
			}

			// 次のオブジェクトのポインタをコピー
			pObj = pNext;
		}
	}
}

//============================================================================
// 先頭オブジェクトのポインタ取得
//============================================================================
CObject* CObject::GetTopObject()
{
	return *m_pTop;
}

//============================================================================
// 先頭オブジェクトのポインタ取得
//============================================================================
CObject* CObject::GetTopObject(WORD wPriority)
{
	return m_pTop[wPriority];
}

//============================================================================
// 先頭オブジェクトのポインタ取得
//============================================================================
CObject* CObject::GetTopObject(LAYER Priority)
{
	return m_pTop[static_cast<WORD>(Priority)];
}

//============================================================================
// 特定タイプのオブジェクトを探す
//============================================================================
CObject* CObject::FindSpecificObject(TYPE Type)
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// 先頭オブジェクトのポインタをコピー
		CObject* pObj = m_pTop[nCntPriority];

		// 次のオブジェクトが無くなるまで
		while (pObj != nullptr)
		{
			if (pObj->GetType() == Type)
			{
				// タイプ一致なら返す
				return pObj;
			}

			// 次のオブジェクトのポインタをコピー
			pObj = pObj->m_pNext;
		}
	}

	return nullptr;
}

//============================================================================
// 特定タイプのオブジェクト数を取得
//============================================================================
int CObject::CountSpecificObject(TYPE Type)
{
	// 数カウント用
	int nCount = 0;

	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// 先頭オブジェクトのポインタをコピー
		CObject* pObj = m_pTop[nCntPriority];

		// 次のオブジェクトが無くなるまで
		while (pObj != nullptr)
		{
			if (pObj->GetType() == Type)
			{
				nCount++;
			}

			pObj = pObj->GetNext();
		}
	}

	return nCount;
}