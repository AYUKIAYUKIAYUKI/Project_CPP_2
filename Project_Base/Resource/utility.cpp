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
// 角度の差の補正(ラジアン)
//============================================================================
void CUtility::AdjustAngle(float& fAngle, const float& fDest)
{
	// 目標角度と現在角度との差を計算
	const float& fDifference = fAngle - fDest;

	if (fDifference > D3DX_PI)
	{
		fAngle += -D3DX_PI * 2.0f;

#ifdef _DEBUG

		// どれだけの差があるか表示
		CRenderer::GetInstance()->SetTimeString("[りれき1]現在角度と目標角度の差 : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
	else if (fDifference < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	
#ifdef _DEBUG

		// どれだけの差があるか表示
		CRenderer::GetInstance()->SetTimeString("[りれき2]現在角度と目標角度の差 : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
}

//============================================================================
// 角度の差の補正(ラジアン)
//============================================================================
void CUtility::AdjustAngle(float& fAngle, const float& fDest, const float& fRadius)
{
	// 目標角度と現在角度との差を計算
	const float& fDifference = fAngle - fDest;

	if (fDifference > fRadius)
	{
		fAngle += -fRadius * 2.0f;

#ifdef _DEBUG

		// どれだけの差があるか表示
		CRenderer::GetInstance()->SetTimeString("[りれき1]現在角度と目標角度の差 : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
	else if (fDifference < -fRadius)
	{
		fAngle += fRadius * 2.0f;

#ifdef _DEBUG

		// どれだけの差があるか表示
		CRenderer::GetInstance()->SetTimeString("[りれき2]現在角度と目標角度の差 : " + std::to_string(fDifference), 120);

#endif	// _DEBUG
	}
}

//============================================================================
// 向きの範囲の補正(ラジアン)
//============================================================================
void CUtility::AdjustDirection(float& fAngle)
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
void CUtility::AdjustDirection(float& fAngle1, float& fAngle2)
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
// 球形どうしの衝突判定
//============================================================================
bool CUtility::OnlySphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget)
{
	// 対象位置へのベクトルを算出
	const D3DXVECTOR3& Vec = PosTarget - PosSelf;

	// お互いの距離が、お互いの半径の和以下であれば衝突
	if (sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z) <= fRadiusSelf - fRadiusTarget)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱と点の衝突判定
//============================================================================
bool CUtility::CylinderAndPoint(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget)
{
	// 対象位置へのベクトルを算出
	const D3DXVECTOR3& Vec = PosTarget - PosSelf;

	// お互いの距離が、半径以下で
	if (sqrtf(Vec.x * Vec.x + Vec.z * Vec.z) <= fRadiusSelf)
	{
		// 円柱の高さの範囲内に点が存在すれば衝突
		if (PosSelf.y + fHeight > PosTarget.y &&
			PosSelf.y - fHeight < PosTarget.y)
		{
			return 1;
		}
	}

	return 0;
}

//============================================================================
// 円柱と球の衝突判定
//============================================================================
bool CUtility::CylinderAndSphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget)
{
	// 対象位置へのベクトルを算出
	const D3DXVECTOR3& Vec = PosTarget - PosSelf;

	// お互いの距離が、半径以下で
	if (sqrtf(Vec.x * Vec.x + Vec.z * Vec.z) <= fRadiusSelf - fRadiusTarget)
	{
		// 円柱の高さの範囲内に球が存在すれば衝突
		if (PosSelf.y + fHeight > PosTarget.y &&
			PosSelf.y - fHeight < PosTarget.y)
		{
			return 1;
		}
	}

	return 0;
}

//============================================================================
// 円柱とAABBの衝突判定
//============================================================================
bool CUtility::CylinderAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& sizeTarget)
{
	// 全ての軸方向からお互いにめり込んでいるとき衝突
	if (PosSelf.x + fRadiusSelf > PosTarget.x - sizeTarget.x &&
		PosSelf.x - fRadiusSelf < PosTarget.x + sizeTarget.x &&
		PosSelf.y + fHeight > PosTarget.y - sizeTarget.y &&
		PosSelf.y < PosTarget.y + sizeTarget.y &&
		PosSelf.z + fRadiusSelf > PosTarget.z - sizeTarget.z &&
		PosSelf.z - fRadiusSelf < PosTarget.z + sizeTarget.z)
	{
		return 1;
	}

	return 0;
}