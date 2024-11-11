//============================================================================
//
// バウンディングボックス、ヘッダファイル [bouding_box.h]
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
	CBounding_Box();				// デフォルトコンストラクタ
	CBounding_Box(CObject_X* pObj);	// ボックス表示コンストラクタ
	~CBounding_Box() override;		// デストラクタ

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// サイズを取得
	void SetSize(D3DXVECTOR3 Size);		// サイズを設定

private:

	// <data>
	D3DXVECTOR3		m_Size;			// サイズ
	CRender_Box*	m_pRenderBox;	// ボックス表示
};

#endif	// _BOUNDING_BOX_H_