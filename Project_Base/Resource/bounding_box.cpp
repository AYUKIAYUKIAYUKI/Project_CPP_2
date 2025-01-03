//============================================================================
//
// 箱バウンディング [bouding_box.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "bounding_box.h"
#include "object_X.h"

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
// コンストラクタ
//============================================================================
CBounding_Box::CBounding_Box() :
	CBounding_Volume{},
#ifdef _DEBUG
	m_Size{ VEC3_INIT },
	m_pRenderBox{ nullptr }
#else
	m_Size{ VEC3_INIT }
#endif // _DEBUG
{
#ifdef _DEBUG
	// 箱表示を生成
	m_pRenderBox = CRender_Box::Create();
#endif // _DEBUG
}

//============================================================================
// デストラクタ
//============================================================================
CBounding_Box::~CBounding_Box()
{
#ifdef _DEBUG
	// 箱表示の破棄予約
	if (m_pRenderBox != nullptr)
	{
		m_pRenderBox->SetRelease();
		m_pRenderBox = nullptr;
	}
#endif // _DEBUG
}

//============================================================================
// 中心点を設定
//============================================================================
void CBounding_Box::SetCenterPos(D3DXVECTOR3 Pos)
{
	// バウンディング基底クラスの持つ中心点にセット
	CBounding_Volume::SetCenterPos(Pos);

#ifdef _DEBUG
	// 箱表示の持つシンクロ座標にセット
	m_pRenderBox->SetCenterSyncPos(Pos);
#endif // _DEBUG
}

//============================================================================
// サイズを取得
//============================================================================
const D3DXVECTOR3& CBounding_Box::GetSize() const
{
	return m_Size;
}

//============================================================================
// サイズを設定
//============================================================================
void CBounding_Box::SetSize(D3DXVECTOR3 Size)
{
	// 自身のサイズにセット
	m_Size = Size;

#ifdef _DEBUG
#if RENDER_COLLISION
	// 箱表示の持つシンクロサイズにセット
	m_pRenderBox->SetSyncSize(Size);
#endif
#endif // _DEBUG
}

//============================================================================
// 向きを取得
//============================================================================
const D3DXVECTOR3& CBounding_Box::GetRot() const
{
	return m_Rot;
}

//============================================================================
// 向きを設定
//============================================================================
void CBounding_Box::SetRot(D3DXVECTOR3 Rot)
{
	// 自身のサイズにセット
	m_Rot = Rot;

#ifdef _DEBUG
	// 箱表示の持つシンクろ向きにセット
	m_pRenderBox->SetSyncRot(Rot);
#endif // _DEBUG
}