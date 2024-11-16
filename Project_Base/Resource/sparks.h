//============================================================================
// 
// 火の粉、ヘッダファイル [sparks.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _SPARKS_H_
#define _SPARKS_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_Effect.h"

//****************************************************
// 火の粉オブジェクトクラス
//****************************************************
class CSparks : public CObject_Effect
{
public:

	// <special function>
	CSparks();				// コンストラクタ
	~CSparks() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <static function>
	static void AutoGenerate();	// 自動生成

private:

	/// <summary> 生成スパン </summary>
	static constexpr WORD NUM_GENERATE_SPAN = 30;

	// <static fuction>
	static void Create();	// 生成

	// <data>
	D3DXVECTOR3	m_InitPos;		// 初期座標
	float		m_fWaveCoef;	// ウェーブ強度
	float		m_fAdderY;		// 上昇量

	// <static data>
	static WORD m_nCntGenerateSpan;	// 生成スパンのカウント
};

#endif	// _SPARKS_H_