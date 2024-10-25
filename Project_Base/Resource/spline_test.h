//============================================================================
//
// スプライン曲線？のテスト、ヘッダファイル [spline_test.h]
// Author : 福田歩希
//
//============================================================================

#pragma once	// 二重インクルード防止

#include <nlohmann/json.hpp>

//************************************
// usingディレクティブ
//************************************
using JSON = nlohmann::json;

// てｓｔくｔｐ
class CSpline{
public:
	CSpline() :m_pVtxBuff{ nullptr }, m_pIdxBuff{ nullptr }, m_Pos{ 0.0f, 0.0f, 0.0f } {// ワールド行列の初期化
		D3DXMatrixIdentity(&m_MtxWorld); };
	~CSpline() { Uninit(); };
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	HRESULT CreateVtxBuff();	// 頂点バッファの生成
	HRESULT CreateIdxBuff();	// インデックスバッファの生
	void SetMtxWorld();			// ワールド行列設定
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファのポインタ
	D3DXVECTOR3 m_Pos;	
	D3DXMATRIX m_MtxWorld;				// わーるどあ
	JSON m_json;						// じぇいｎ
	int m_nNumVtx;
	int m_nNumIdx;
	int m_nNumPrim;
};