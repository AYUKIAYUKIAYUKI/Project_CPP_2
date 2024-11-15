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
#include "renderer.h"

//============================================================================
// 球同士
//============================================================================
bool collision::HitSphereToSphere(const CBounding_Sphere* const Self, const CBounding_Sphere* const Other)
{
	// お互いの距離を計算
	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();

	// 距離が、お互いの半径の和以下ならば衝突
	if (sqrtf(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z) <= Self->GetRadius() + Other->GetRadius())
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 球とAABB
//============================================================================
bool collision::HitSphereToAABB(const CBounding_Sphere* const Self, const CBounding_Box* const Other)
{
	// パラメータをコピー
	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
	const float& fSelfRadius = Self->GetRadius();

	// 全ての軸方向への射影同士が重なっていれば衝突
	if (SelfPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
		SelfPos.x - fSelfRadius < OtherPos.x + OtherSize.x &&
		SelfPos.y + fSelfRadius > OtherPos.y - OtherSize.y &&
		SelfPos.y - fSelfRadius < OtherPos.y + OtherSize.y &&
		SelfPos.z + fSelfRadius > OtherPos.z - OtherSize.z &&
		SelfPos.z - fSelfRadius < OtherPos.z + OtherSize.z)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱同士
//============================================================================
bool collision::HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other)
{
	// お互いの距離を計算
	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();

	// 距離が、お互いの半径の和より大きければ衝突していない
 	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
	{
		return 0;
	}

	// Y軸への射影同士が重なっていたら衝突
	if (Self->GetCenterPos().y + Self->GetHeight() > Other->GetCenterPos().y - Other->GetHeight() &&
		Self->GetCenterPos().y - Self->GetHeight() < Other->GetCenterPos().y + Other->GetHeight())
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱と点
//============================================================================
bool collision::HitCylinderToPoint(const CBounding_Cylinder* const Self, const Vec3& Other)
{
	// お互いの距離を計算
	const Vec3& Distance = Other - Self->GetCenterPos();

	// 距離が、円柱の半径より大きければ衝突していない
	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius())
	{
		return 0;
	}

	// Y軸への射影に点が存在すれば衝突
	if (Self->GetCenterPos().y + Self->GetHeight() > Other.y &&
		Self->GetCenterPos().y - Self->GetHeight() < Other.y)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱と球
//============================================================================
bool collision::HitCylinderToSphere(const CBounding_Cylinder* const Self, const CBounding_Sphere* const Other)
{
	// お互いの距離を計算
	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();

	// 距離が、お互いの半径の和より大きければ衝突していない
	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
	{
		return 0;
	}

	// Y軸への射影同士が重なっていれば衝突
	if (Self->GetCenterPos().y + Self->GetHeight() > Other->GetCenterPos().y - Other->GetRadius() &&
		Self->GetCenterPos().y - Self->GetHeight() < Other->GetCenterPos().y + Other->GetRadius())
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱と球
//============================================================================
bool collision::HitCylinderToSphere(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const float& fOtherRadius)
{
	// お互いの距離を計算
	const Vec3& Distance = OtherPos - SelfPos;

	// 距離が、お互いの半径の和より大きければ衝突していない
	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > fSelfRadius + fOtherRadius)
	{
		return 0;
	}

	// Y軸への射影同士が重なっていれば衝突
	if (SelfPos.y + fSelfHeight > OtherPos.y - fSelfRadius &&
		SelfPos.y - fSelfHeight < OtherPos.y + fSelfRadius)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱とAABB
//============================================================================
bool collision::HitCylinderToAABB(const CBounding_Cylinder* const Self, const CBounding_Box* const Other)
{
	// パラメータをコピー
	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
	const float& fSelfRadius = Self->GetRadius(), fSelfHeight = Self->GetHeight();

	// 全ての軸方向への射影同士が重なっていれば衝突
	if (SelfPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
		SelfPos.x - fSelfRadius < OtherPos.x + OtherSize.x &&
		SelfPos.y + fSelfHeight > OtherPos.y - OtherSize.y &&
		SelfPos.y < OtherPos.y + OtherSize.y &&
		SelfPos.z + fSelfRadius > OtherPos.z - OtherSize.z &&
		SelfPos.z - fSelfRadius < OtherPos.z + OtherSize.z)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱とAABB
//============================================================================
bool collision::HitCylinderToAABB(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
{
	// 全ての軸方向への射影同士が重なっていれば衝突
	if (SelfPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
		SelfPos.x - fSelfRadius < OtherPos.x + OtherSize.x &&
		SelfPos.y + fSelfHeight > OtherPos.y - OtherSize.y &&
		SelfPos.y - fSelfHeight < OtherPos.y + OtherSize.y &&
		SelfPos.z + fSelfRadius > OtherPos.z - OtherSize.z &&
		SelfPos.z - fSelfRadius < OtherPos.z + OtherSize.z)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// 円柱とAABBの衝突面を取得
//============================================================================
int collision::GetCylinderToAABB(const Vec3& SelfOldPos, const Vec3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
{
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");
	CRenderer::SetDebugString("プレイヤー過去回転 : " + to_string(SelfOldPos.x) + " :  " + to_string(SelfOldPos.y) + " : " + to_string(SelfOldPos.z));
	CRenderer::SetDebugString("プレイヤー現在回転 : " + to_string(SelfNowPos.x) + " :  " + to_string(SelfNowPos.y) + " : " + to_string(SelfNowPos.z));
	CRenderer::SetDebugString("プレイヤー・右・左 : " + to_string(SelfNowPos.x + fSelfRadius) + " :  " + to_string(SelfOldPos.x - fSelfRadius));
	CRenderer::SetDebugString("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝");

	// 衝突自体が無ければ
	if (!HitCylinderToAABB(SelfNowPos, fSelfRadius, fSelfHeight, OtherPos, OtherSize))
	{
		return 0;
	}

	// もともとX軸への射影同士が重なっていたか (計算上Z方向に衝突はしない)
	if (SelfOldPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
		SelfOldPos.x - fSelfRadius < OtherPos.x + OtherSize.x)
	{ // 重なっていた -> 上下の判定

		// 上にいた
		if (SelfOldPos.y > OtherPos.y)
		{
#ifdef _DEBUG
			CRenderer::SetTimeString("[りれき]衝突面 : 上", 60);
#endif	// _DEBUG
			return 1;
		}

		// 下にいた
		if (SelfOldPos.y < OtherPos.y)
		{
#ifdef _DEBUG
			CRenderer::SetTimeString("[りれき]衝突面 : 下", 60);
#endif	// _DEBUG
			return 2;
		}
	}
	else
	{ // 重なっていなかった -> 左右の判定

		// 左にいた
		if (SelfOldPos.x < OtherPos.x)
		{
#ifdef _DEBUG
			CRenderer::SetTimeString("[りれき]衝突面 : 左", 60);
#endif	// _DEBUG
			return 3;
		}

		// 右にいた
		if (SelfOldPos.x > OtherPos.x)
		{
#ifdef _DEBUG
			CRenderer::SetTimeString("[りれき]衝突面 : 右", 60);
#endif	// _DEBUG
			return 4;
		}
	}

	return 0;
}