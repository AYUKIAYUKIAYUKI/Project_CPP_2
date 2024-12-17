//============================================================================
// 
// 星座、ヘッダファイル [constellation.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _CONSTELLATION_H_
#define _CONSTELLATION_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_Effect.h"

//****************************************************
// 星座オブジェクトクラス
//****************************************************
class CConstellation : public CObject_Effect
{
public:

	// <special function>
	CConstellation();			// コンストラクタ
	~CConstellation() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <static function>
	static void GenerateSpread(D3DXVECTOR3 Pos);	// 拡散発生

private:

	// <static fuction>
	static void Create(D3DXVECTOR3 Pos);	// 生成

	// <data>
	D3DXVECTOR3	m_InitPos;	// 初期座標

	// <static data>
	static JSON		 m_InitParam;		// 基礎パラメータ
	static int		 m_nCntGenetrate;	// 生成カウント
	static const int m_nGenerateSpan;	// 生成スパン
};

#endif	// _CONSTELLATION_H_