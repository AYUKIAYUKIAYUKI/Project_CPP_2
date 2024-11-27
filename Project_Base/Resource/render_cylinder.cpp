//============================================================================
// 
// シリンダー表示 [render_cylinder.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "render_cylinder.h"
#include "object_X.h"

// デバイス取得用
#include "renderer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// 描画優先度指定コンストラクタ
//============================================================================
CRender_Cylinder::CRender_Cylinder(LAYER Priority) :
	CRender_Collision{ Priority },
	m_fSyncRadius{ 0.0f },
	m_fSyncHeight{ 0.0f },
	m_pCylinderModel{ CObject_X::Create(Priority, CX_Manager::TYPE::RENDER_CYLINDER) }
{

}

//============================================================================
// デストラクタ
//============================================================================
CRender_Cylinder::~CRender_Cylinder()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CRender_Cylinder::Init()
{
	// 判定表示の初期設定
	if (FAILED(CRender_Collision::Init()))
	{
		return E_FAIL;
	}

	// 表示の透明度を設定
	m_pCylinderModel->SetCol({ 1.0f, 1.0f, 1.0f, 0.5f });
	m_pCylinderModel->SetUseCol(true);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CRender_Cylinder::Uninit()
{
	// 判定表示の終了処理
	CRender_Collision::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CRender_Cylinder::Update()
{
	if (m_pRef != nullptr)
	{ // 対象オブジェクトが設定されていたら

		// 判定のサイズに合わせてスケールを拡大
		const float& fRad = m_pRef->GetRadius(), fHeight = m_pRef->GetHeight();
		const Vec3& Scale = { fRad, fHeight, fRad };
		m_pCylinderModel->SetScale(Scale);

		// オブジェクトの座標に判定を表示
		m_pCylinderModel->SetPos(m_pRef->GetPos());
	}
	else
	{ // 対象オブジェクトが設定されていない

		// 判定のサイズに合わせてスケールを拡大
		const Vec3& Scale = { m_fSyncRadius, m_fSyncHeight, m_fSyncRadius };
		m_pCylinderModel->SetScale(Scale);

		// オブジェクトの座標に判定を表示
		m_pCylinderModel->SetPos(m_CenterSyncPos);
	}
}

//============================================================================
// 描画処理
//============================================================================
void CRender_Cylinder::Draw()
{

}

//============================================================================
// シンクロ半径を設定
//============================================================================
void CRender_Cylinder::SetSyncRadius(float fRadius)
{
	m_fSyncRadius = fRadius;
}

//============================================================================
// シンクロ高さを設定
//============================================================================
void CRender_Cylinder::SetSyncHeight(float fHeight)
{
	m_fSyncHeight = fHeight;
}

//============================================================================
// 色を設定
//============================================================================
void CRender_Cylinder::SetCol(D3DXCOLOR Col)
{
	m_pCylinderModel->SetCol(Col);
}

//============================================================================
// 色反映を設定
//============================================================================
void CRender_Cylinder::SetUseCol(bool bUse)
{
	m_pCylinderModel->SetUseCol(bUse);
}

//============================================================================
// モデルを変更
//============================================================================
void CRender_Cylinder::ChangeModel(CX_Manager::TYPE Type)
{
	m_pCylinderModel->BindModel(Type);
}

//============================================================================
// 生成
//============================================================================
CRender_Cylinder* CRender_Cylinder::Create()
{
	// 判定表示を生成
	CRender_Cylinder* pRender_Collision = DBG_NEW CRender_Cylinder(LAYER::FRONT);

	// 生成失敗
	if (pRender_Collision == nullptr)
	{
		assert(false && "シリンダー表示の生成に失敗しました");
	}

	// 判定表示の初期設定
	pRender_Collision->Init();


	return pRender_Collision;
}

//============================================================================
// 生成
//============================================================================
CRender_Cylinder* CRender_Cylinder::Create(CObject_X* pRef)
{
	// 判定表示を生成
	CRender_Cylinder* pRender_Collision = DBG_NEW CRender_Cylinder(LAYER::FRONT);

	// 生成失敗
	if (pRender_Collision == nullptr)
	{
		assert(false && "シリンダー表示の生成に失敗しました");
	}

	// 判定表示の初期設定
	pRender_Collision->Init();

	// 対象オブジェクトの設定
	pRender_Collision->SetRefObj(pRef);

	return pRender_Collision;
}