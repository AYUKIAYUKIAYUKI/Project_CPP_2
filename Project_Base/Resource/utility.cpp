//============================================================================
// 
// 便利関数 [utility.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "renderer.h"

//============================================================================
// 象限に応じて、単位立方体用の頂点の座標を作成
//============================================================================
D3DXVECTOR3 utility::SetCubeVtxFromQuadrant(const WORD& wIdx)
{
	D3DXVECTOR3 VtxPos = { 0.0f, 0.0f, 0.0f };

	// 渡された頂点の象限に応じて、符号を返す
	switch (wIdx)
	{
	case 0: VtxPos = { -1.0f, +1.0f, -1.0f }; break;
	case 1: VtxPos = { +1.0f, +1.0f, -1.0f }; break;
	case 2: VtxPos = { -1.0f, -1.0f, -1.0f }; break;
	case 3: VtxPos = { +1.0f, -1.0f, -1.0f }; break;
	case 4: VtxPos = { -1.0f, +1.0f, +1.0f }; break;
	case 5: VtxPos = { +1.0f, +1.0f, +1.0f }; break;
	case 6: VtxPos = { -1.0f, -1.0f, +1.0f }; break;
	case 7: VtxPos = { +1.0f, -1.0f, +1.0f }; break;
	default: CRenderer::SetTimeString("象限の判定に対応していない番号が渡されました：" + std::to_string(wIdx), 120); break;
	}

	return VtxPos;
}

//============================================================================
// 点をY軸で回転させる
//============================================================================
D3DXVECTOR3 utility::RotatePointAroundY(const float& fDirection, const D3DXVECTOR3& Pos)
{
	// Y軸の回転量からsinとcosの値を計算
	const float& fSinY = sinf(fDirection), fCosY = cosf(fDirection);

	// 回転行列を用いて、頂点座標をXZ平面で回転させる
	const D3DXVECTOR3& ResultPos = {
		Pos.x * fCosY + Pos.z * fSinY,
		Pos.y,
		-Pos.x * fSinY + Pos.z * fCosY
	};

	return ResultPos;
}

//============================================================================
// 直方体をY軸で回転させる
//============================================================================
D3DXVECTOR3 utility::RotateBoxAroundY(const WORD& wIdx, const float& fDirection, const D3DXVECTOR3& Size)
{
	// 象限に応じて、頂点の座標を作成
	const D3DXVECTOR3& VtxPos = SetCubeVtxFromQuadrant(wIdx);

	// Y軸の回転量からsinとcosの値を計算
	const float& fSinY = sinf(fDirection), fCosY = cosf(fDirection);

	// 回転行列を用いて、頂点座標をXZ平面で回転させる
	const D3DXVECTOR3& ResultPos = {
		VtxPos.x * fCosY * Size.x + VtxPos.z * fSinY * Size.z,
		VtxPos.y * Size.y,
		-VtxPos.x * fSinY * Size.x + VtxPos.z * fCosY * Size.z
	};

	return ResultPos;
}

//============================================================================
// 直方体のY軸の回転を打ち消す
//============================================================================
D3DXVECTOR3 utility::InverseRotateBoxAroundY(const float& fDirection, const D3DXVECTOR3& VtxPos)
{
	// Y軸の逆の回転量からsinとcosの値を計算
	const float& fSinY = sinf(-fDirection), fCosY = cosf(-fDirection);

	// 回転行列の逆を用いて、XZ平面で回転した頂点の座標を元に戻す
	const D3DXVECTOR3& ResultPos = {
		VtxPos.x * fCosY + VtxPos.z * fSinY,
		VtxPos.y,
		-VtxPos.x * fSinY + VtxPos.z * fCosY
	};

	return ResultPos;
}

//============================================================================
// 角度の差の補正(ラジアン)
//============================================================================
void utility::AdjustAngle(float& fAngle, const float& fDest)
{
	// 目標角度と現在角度との差を計算
	const float& fDifference = fAngle - fDest;

	if (fDifference > D3DX_PI)
	{
		fAngle += -D3DX_PI * 2.0f;

#if 0	// どれだけの差があるか表示
#ifdef _DEBUG
		CRenderer::SetTimeString("[りれき1]現在角度と目標角度の差 : " + std::to_string(fDifference), 120);
#endif	// _DEBUG
#endif
	}
	else if (fDifference < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	
#if 0	// どれだけの差があるか表示
#ifdef _DEBUG
		CRenderer::SetTimeString("[りれき2]現在角度と目標角度の差 : " + std::to_string(fDifference), 120);
#endif	// _DEBUG
#endif
	}
}

//============================================================================
// 角度の差の補正(ラジアン)
//============================================================================
void utility::AdjustAngle(float& fAngle, const float& fDest, const float& fRadius)
{
	// 目標角度と現在角度との差を計算
	const float& fDifference = fAngle - fDest;

	if (fDifference > fRadius)
	{
		fAngle += -fRadius * 2.0f;

#if 0	// どれだけの差があるか表示
#ifdef _DEBUG
		CRenderer::SetTimeString("[りれき1]現在角度と目標角度の差 : " + std::to_string(fDifference), 120);
#endif	// _DEBUG
#endif
	}
	else if (fDifference < -fRadius)
	{
		fAngle += fRadius * 2.0f;

#if 0	// どれだけの差があるか表示
#ifdef _DEBUG
		CRenderer::SetTimeString("[りれき2]現在角度と目標角度の差 : " + std::to_string(fDifference), 120);
#endif	// _DEBUG
#endif
	}
}

//============================================================================
// 向きの範囲の補正(ラジアン)
//============================================================================
void utility::AdjustDirection(float& fAngle)
{
	if (fAngle > D3DX_PI)
	{
		fAngle += -D3DX_PI * 2.0f;
	}
	else if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
}

//============================================================================
// 向きの範囲の補正(ラジアン)
//============================================================================
void utility::AdjustDirection(float& fAngle1, float& fAngle2)
{
	if (fAngle1 > D3DX_PI)
	{
		fAngle1 += -D3DX_PI * 2.0f;
		fAngle2 += -D3DX_PI * 2.0f;
	}
	else if (fAngle1 < -D3DX_PI)
	{
		fAngle1 += D3DX_PI * 2.0f;
		fAngle2 += D3DX_PI * 2.0f;
	}
}

//============================================================================
// JSONファイルの展開
//============================================================================
nlohmann::json utility::OpenJsonFile(std::string FilePath)
{
	// JSONファイルを読み取り展開
	std::ifstream ifs(FilePath);

	// ファイルが展開出来ていたら
	if (ifs.good())
	{
		// JSONデータをパース
		nlohmann::json Json;
		ifs >> Json;

		// JSONオブジェクトを引き渡す
		return Json;
	}
	else
	{
#if 0
		CRenderer::SetTimeString("JSONファイル【" + FilePath + "】の展開に失敗しました", 90);
#else
		assert(false && "JSONファイルの展開に失敗");
#endif
	}

	return nullptr;
}

//============================================================================
// JsonデータをD3DXVECTOR2に変換
//============================================================================
D3DXVECTOR2 utility::JsonConvertToVec2(nlohmann::json Json)
{
	return D3DXVECTOR2(static_cast<float>(Json[0]), static_cast<float>(Json[1]));
}

//============================================================================
// JsonデータをD3DXVECTOR3に変換
//============================================================================
D3DXVECTOR3 utility::JsonConvertToVec3(nlohmann::json Json)
{
	return D3DXVECTOR3(static_cast<float>(Json[0]), static_cast<float>(Json[1]), static_cast<float>(Json[2]));
}

//============================================================================
// UTF8のJsonデータをSJISに変換
//============================================================================
std::string utility::JsonConvertToSJIS(nlohmann::json Json)
{
	// 文字列の長さを格納
	int nLength = 0;

	// UTF8の文字列をstring型にコピー
	const std::string& Str = Json;

	// UTF8の文字列の長さを取得
	nLength = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);

	// エラー発生
	if (nLength == 0) 
	{
		// エラーコードを取得
		DWORD error = GetLastError();

		// 例外をスロー
		throw std::runtime_error("Failed");
	}

	// UTF16変換後の文字列を格納
	std::wstring WideStr(nLength, L'\0');

	// UTF8をUTF16に変換
	MultiByteToWideChar(
		CP_UTF8,		// 変換コードページ
		0,				// 変換の種類
		Str.c_str(),	// 変換したい文字列の
		-1,				// 文字列の長さ (-1で全体指定)
		&WideStr[0],	// 変換後の文字列を受け取るポインタ
		nLength);		// 文字列のバッファサイズ

	// UTF16の文字列の長さを取得
	nLength = WideCharToMultiByte(CP_ACP, 0, WideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	// エラー発生
	if (nLength == 0)
	{
		// エラーコードを取得
		DWORD error = GetLastError();

		// 例外をスロー
		throw std::runtime_error("Failed");
	}

	// SJIS変換後の文字列を格納
	std::string SjisStr(nLength, '\0');

	// UTF16をSJISに変換
	WideCharToMultiByte(
		CP_ACP,				// 変換コードページ
		0,					// 変換の種類
		WideStr.c_str(),	// 変換したいワイド文字列
		-1,					// 文字列の長さ (-1で全体指定)
		&SjisStr[0],		// 変換後の文字列を受け取るポインタ
		nLength,			// 文字列のバッファサイズ
		nullptr,			// 変換できない文字の代わりに出す文字
		nullptr);			// 代わりの文字の使用判定を受け取るポインタ

	return SjisStr;
}