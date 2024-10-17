//============================================================================
// 
// 便利関数 [utility.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "utility.h"

//============================================================================
// 球形どうしの衝突判定
//============================================================================
bool CUtility::OnlySphere(const D3DXVECTOR3& posSelf, const float& fRadiusSelf, const D3DXVECTOR3& posTarget, const float& fRadiusTarget)
{
	// 目標地点へのベクトルを算出
	D3DXVECTOR3 vec = posTarget - posSelf;

	// お互いの距離が、お互いの半径の和以下であれば衝突
	if (sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z) <= (fRadiusSelf + fRadiusTarget))
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱と点の衝突判定
//============================================================================
bool CUtility::CylinderAndPoint(const D3DXVECTOR3& posSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& posTarget)
{
	// 目標地点へのベクトルを算出
	D3DXVECTOR3 vec = posTarget - posSelf;

	// お互いの距離が、半径以下で
	//if (sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z) <= fRadiusSelf)
	if (sqrtf(vec.x * vec.x + vec.z * vec.z) <= fRadiusSelf)
	{
		// 円柱の高さの範囲内に点が存在すれば衝突
		if (posSelf.y + fHeight > posTarget.y &&
			posSelf.y - fHeight < posTarget.y)
		{
			return 1;
		}
	}

	return 0;
}