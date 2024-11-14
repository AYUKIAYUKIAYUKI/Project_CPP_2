//============================================================================
// 
// コリジョン系関数 [collision.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "collision.h"

//============================================================================
// 球どうしの衝突判定
//============================================================================
bool collision::OnlySphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget)
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
// 球とAABBの衝突判定
//============================================================================
bool collision::SphereAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTarget)
{
	// 全ての軸方向からお互いにめり込んでいるとき衝突
	if (PosSelf.x + fRadiusSelf > PosTarget.x - SizeTarget.x &&
		PosSelf.x - fRadiusSelf < PosTarget.x + SizeTarget.x &&
		PosSelf.y + fRadiusSelf > PosTarget.y - SizeTarget.y &&
		PosSelf.y - fRadiusSelf < PosTarget.y + SizeTarget.y &&
		PosSelf.z + fRadiusSelf > PosTarget.z - SizeTarget.z &&
		PosSelf.z - fRadiusSelf < PosTarget.z + SizeTarget.z)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱と点の衝突判定
//============================================================================
bool collision::CylinderAndPoint(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget)
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
bool collision::CylinderAndSphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget)
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
bool collision::CylinderAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTarget)
{
	// 全ての軸方向からお互いにめり込んでいるとき衝突
	if (PosSelf.x + fRadiusSelf > PosTarget.x - SizeTarget.x &&
		PosSelf.x - fRadiusSelf < PosTarget.x + SizeTarget.x &&
		PosSelf.y + fHeight > PosTarget.y - SizeTarget.y &&
		PosSelf.y < PosTarget.y + SizeTarget.y &&
		PosSelf.z + fRadiusSelf > PosTarget.z - SizeTarget.z &&
		PosSelf.z - fRadiusSelf < PosTarget.z + SizeTarget.z)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱とAABBの衝突面を取得
//============================================================================
int collision::GetCylinderAndAABB(const D3DXVECTOR3& SelfOldPos, const D3DXVECTOR3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const D3DXVECTOR3& OtherPos, const D3DXVECTOR3& OtherSize)
{
	// 衝突が無ければ
	if (!CylinderAndAABB(SelfNowPos, fSelfRadius, fSelfHeight, OtherPos, OtherSize))
	{
		return 0;
	}

	// もともとY軸の射影線分に重なっていたか (計算設計上前後に衝突はしない)
	if (SelfOldPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
		SelfOldPos.x - fSelfRadius < OtherPos.x + OtherSize.x)
	{ // 重なっていた -> 上下の判定

		// 上にいた
		if (SelfOldPos.y > OtherPos.y)
		{
			return 1;
		}

		// 下にいた
		if (SelfOldPos.y < OtherPos.y)
		{
			return 2;
		}
	}
	else
	{ // 重なっていなかった -> 左右の判定

		// 左にいた
		if (SelfOldPos.x < OtherPos.x)
		{
			return 3;
		}

		// 右にいた
		if (SelfOldPos.x > OtherPos.x)
		{
			return 4;
		}
	}

	return 0;
}