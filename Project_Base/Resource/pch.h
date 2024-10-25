//============================================================================
// 
// プリコンパイルヘッダー [pch.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _PCH_H_
#define _PCH_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include <assert.h>
#include <stdio.h>
#include <windows.h>	// Windowsアプリケーション動作用

// DirectX9
#include <d3dx9.h>					// DirectX9動作用
#define DIRECTINPUT_VERSION	0x0800	// DirectInputのバージョン
#include <dinput.h>					// DirectInput動作用
#include <xinput.h>					// コントローラー動作用
#include <xaudio2.h>				// サウンド動作用

// JSON
#include <nlohmann/json.hpp>

// 標準ライブラリ
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// 便利関数
#include "utility.h"

//****************************************************
// ライブラリのリンク
//****************************************************
#pragma comment(lib, "winmm.lib")
#pragma	comment(lib, "d3d9.lib")	// 描画処理用
#pragma	comment(lib, "d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma	comment(lib, "dxguid.lib")	// DirectXのコーポネント使用のため
#pragma	comment(lib, "dinput8.lib")	// DirectInput動作用
#pragma comment(lib, "xinput.lib")	// コントローラ動作用

//****************************************************
// マクロ定義
//****************************************************

// メモリリーク検出用
#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// 頂点フォーマット
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//****************************************************
// 定数を宣言
//****************************************************

/// <summary> スクリーン横幅 </summary>
static constexpr WORD SCREEN_WIDTH = 1980;

/// <summary> スクリーン縦幅 </summary>
static constexpr WORD SCREEN_HEIGHT = 1080;

//****************************************************
// 名前空間を定義
//****************************************************
namespace abbr
{
	//****************************************************
	// using宣言
	//****************************************************
	using std::to_string;

	//****************************************************
	// 型エイリアスを定義
	//****************************************************
	using Vec2 = D3DXVECTOR2;
	using Vec3 = D3DXVECTOR3;
	using Mtx = D3DXMATRIX;

	/// <summary> D3DXVECTOR2初期化用 </summary>
	static const Vec2 VEC2_INIT = Vec2(0.0f, 0.0f);

	/// <summary> D3DXVECTOR3初期化用 </summary>
	static const Vec3 VEC3_INIT = Vec3(0.0f, 0.0f, 0.0f);
}

//****************************************************
// usingディレクティブ
//****************************************************
using JSON = nlohmann::json;

//****************************************************
// 2Dポリゴン頂点フォーマットの定義
//****************************************************
struct VERTEX_2D
{
	abbr::Vec3	pos;	// 頂点座標
	float		rhw;	// 除算数
	D3DCOLOR	col;	// 頂点カラー
	abbr::Vec2	tex;	// テクスチャ座標
};

//****************************************************
// 3Dポリゴン頂点フォーマットの定義
//****************************************************
struct VERTEX_3D
{
	abbr::Vec3	pos;	// 頂点座標
	abbr::Vec3	nor;	// 法線ベクトル
	D3DCOLOR	col;	// 頂点カラー
	abbr::Vec2	tex;	// テクスチャ座標
};

#endif	_PCH_H_