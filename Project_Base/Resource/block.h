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
// 前方宣言
//****************************************************
class CBounding_Box;

//****************************************************
// ブロッククラス
//****************************************************
class CBlock : public CObject_X
{
public:

	// <special function>
	CBlock();	// デフォルトコンストラクタ
	~CBlock();	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void Uninit() override;		// 終了設定
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	// <getter>
	D3DXVECTOR3 GetSize() const override;	// サイズを取得

	// <static function>
	static CBlock* Create(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Rot);	// 生成

private:

	// <data>
	CBounding_Box* m_pBndBox;	// バウンディングボックス
};

#endif // _BLOCK_H_