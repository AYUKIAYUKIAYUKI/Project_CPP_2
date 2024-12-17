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
	static void AutoGenerate();					// 自動発生
	static void AreaGenerate(D3DXVECTOR3 Pos);	// 局所発生
	static void FuryGenerate();					// 猛発生

private:

	// <static fuction>
	static void AutoCreate();					// 自動生成
	static void AreaCreate(D3DXVECTOR3 Pos);	// 局所生成
	static void FuryCreate();					// 猛生成

	// <data>
	D3DXVECTOR3	m_InitPos;		// 初期座標
	float		m_fWaveCoef;	// ウェーブ強度
	float		m_fAdderY;		// 上昇量

	// <static data>
	static JSON m_InitParam;				// 基礎パラメータ
	static int  m_nCntGenerate;				// 生成カウント
	static const int m_nAutoGenerateSpan;	// 自動生成スパン
	static const int m_nAreaGenerateSpan;	// 局所発生スパン
	static const int m_nFuryGenerateSpan;	// 猛発生スパン
};

#endif	// _SPARKS_H_