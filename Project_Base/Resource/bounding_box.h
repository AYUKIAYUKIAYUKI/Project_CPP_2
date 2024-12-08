//============================================================================
//
// 箱バウンディング、ヘッダファイル [bouding_box.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "bounding_volume.h"
#include "render_box.h"

//****************************************************
// バウンディングボックスクラス
//****************************************************
class CBounding_Box : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Box();			// コンストラクタ
	~CBounding_Box() override;	// デストラクタ

	// <setter>
	void SetCenterPos(D3DXVECTOR3 Pos) override;	// 中心点を設定

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// サイズを取得
	void SetSize(D3DXVECTOR3 Size);		// サイズを設定

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// 向きを取得
	void SetRot(D3DXVECTOR3 Rot);		// 向きを設定

private:

	// <data>
	D3DXVECTOR3		m_Size;			// サイズ
	D3DXVECTOR3		m_Rot;			// 向き
#ifdef _DEBUG
	CRender_Box*	m_pRenderBox;	// 箱表示
#endif
};

#endif	// _BOUNDING_BOX_H_