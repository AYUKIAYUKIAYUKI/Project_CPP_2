//============================================================================
//
// 魔法陣、ヘッダファイル [summoning.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _SUMMONING_H_
#define _SUMMONING_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "motion_set.h"

//****************************************************
// 魔法陣クラス
//****************************************************
class CSummoning : public CMotion_Set
{
public:

	// <special function>
	CSummoning(LAYER Layer);		// コンストラクタ
	virtual ~CSummoning() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了設定
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <stati function>
	static CSummoning* Create();

private:

	// <function>
	bool Disappear();	// 消滅

	// <static data>
	static const JSON m_MotionData;	// モーションデータ
};

#endif // _SUMMONING_H_