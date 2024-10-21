//============================================================================
//
// ブロック、ヘッダファイル [block.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _BLOCK_H_
#define _BLOCK_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// ブロッククラス
//****************************************************
class CBlock : public CObject_X
{
public:

	CBlock();	// デフォルトコンストラクタ
	~CBlock();	// デストラクタ

	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了設定
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	// 生成
	static CBlock* Create();									
	static CBlock* Create(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Rot);

	CObject* m_pTest;	// 仮
};

#endif // _BLOCK_H_