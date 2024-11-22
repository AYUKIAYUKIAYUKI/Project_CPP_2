//============================================================================
// 
// ÉRÉäÉWÉáÉìånä÷êî [collision.cpp]
// Author : ïüìcï‡äÛ
// 
//============================================================================

//****************************************************
// ÉCÉìÉNÉãÅ[ÉhÉtÉ@ÉCÉã
//****************************************************
#include "collision.h"
#include "renderer.h"

//============================================================================
// ãÖìØém
//============================================================================
bool collision::HitSphereToSphere(const CBounding_Sphere* const Self, const CBounding_Sphere* const Other)
{
	// Ç®å›Ç¢ÇÃãóó£ÇåvéZ
	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();

	// ãóó£Ç™ÅAÇ®å›Ç¢ÇÃîºåaÇÃòaà»â∫Ç»ÇÁÇŒè’ìÀ
	if (sqrtf(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z) <= Self->GetRadius() + Other->GetRadius())
	{
		return 1;
	}

	return 0;
}

//============================================================================
// ãÖÇ∆AABB
//============================================================================
bool collision::HitSphereToAABB(const CBounding_Sphere* const Self, const CBounding_Box* const Other)
{
	// ÉpÉâÉÅÅ[É^ÇÉRÉsÅ[
	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
	const float& fSelfRadius = Self->GetRadius();

	// ëSÇƒÇÃé≤ï˚å¸Ç÷ÇÃéÀâeìØémÇ™èdÇ»Ç¡ÇƒÇ¢ÇÍÇŒè’ìÀ
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
// â~íåìØém
//============================================================================
bool collision::HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other)
{
	// Ç®å›Ç¢ÇÃãóó£ÇåvéZ
	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();

	// ãóó£Ç™ÅAÇ®å›Ç¢ÇÃîºåaÇÃòaÇÊÇËëÂÇ´ÇØÇÍÇŒè’ìÀÇµÇƒÇ¢Ç»Ç¢
 	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
	{
		return 0;
	}

	// Yé≤Ç÷ÇÃéÀâeìØémÇ™èdÇ»Ç¡ÇƒÇ¢ÇΩÇÁè’ìÀ
	if (Self->GetCenterPos().y + Self->GetHeight() > Other->GetCenterPos().y - Other->GetHeight() &&
		Self->GetCenterPos().y - Self->GetHeight() < Other->GetCenterPos().y + Other->GetHeight())
	{
		return 1;
	}

	return 0;
}

//============================================================================
// â~íåÇ∆ì_
//============================================================================
bool collision::HitCylinderToPoint(const CBounding_Cylinder* const Self, const Vec3& Other)
{
	// Ç®å›Ç¢ÇÃãóó£ÇåvéZ
	const Vec3& Distance = Other - Self->GetCenterPos();

	// ãóó£Ç™ÅAâ~íåÇÃîºåaÇÊÇËëÂÇ´ÇØÇÍÇŒè’ìÀÇµÇƒÇ¢Ç»Ç¢
	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius())
	{
		return 0;
	}

	// Yé≤Ç÷ÇÃéÀâeÇ…ì_Ç™ë∂ç›Ç∑ÇÍÇŒè’ìÀ
	if (Self->GetCenterPos().y + Self->GetHeight() > Other.y &&
		Self->GetCenterPos().y - Self->GetHeight() < Other.y)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// â~íåÇ∆ãÖ
//============================================================================
bool collision::HitCylinderToSphere(const CBounding_Cylinder* const Self, const CBounding_Sphere* const Other)
{
	// Ç®å›Ç¢ÇÃãóó£ÇåvéZ
	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();

	// ãóó£Ç™ÅAÇ®å›Ç¢ÇÃîºåaÇÃòaÇÊÇËëÂÇ´ÇØÇÍÇŒè’ìÀÇµÇƒÇ¢Ç»Ç¢
	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
	{
		return 0;
	}

	// Yé≤Ç÷ÇÃéÀâeìØémÇ™èdÇ»Ç¡ÇƒÇ¢ÇÍÇŒè’ìÀ
	if (Self->GetCenterPos().y + Self->GetHeight() > Other->GetCenterPos().y - Other->GetRadius() &&
		Self->GetCenterPos().y - Self->GetHeight() < Other->GetCenterPos().y + Other->GetRadius())
	{
		return 1;
	}

	return 0;
}

//============================================================================
// â~íåÇ∆ãÖ
//============================================================================
bool collision::HitCylinderToSphere(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const float& fOtherRadius)
{
	// Ç®å›Ç¢ÇÃãóó£ÇåvéZ
	const Vec3& Distance = OtherPos - SelfPos;

	// ãóó£Ç™ÅAÇ®å›Ç¢ÇÃîºåaÇÃòaÇÊÇËëÂÇ´ÇØÇÍÇŒè’ìÀÇµÇƒÇ¢Ç»Ç¢
	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > fSelfRadius + fOtherRadius)
	{
		return 0;
	}

	// Yé≤Ç÷ÇÃéÀâeìØémÇ™èdÇ»Ç¡ÇƒÇ¢ÇÍÇŒè’ìÀ
	if (SelfPos.y + fSelfHeight > OtherPos.y - fSelfRadius &&
		SelfPos.y - fSelfHeight < OtherPos.y + fSelfRadius)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// â~íåÇ∆AABB
//============================================================================
bool collision::HitCylinderToAABB(const CBounding_Cylinder* const Self, const CBounding_Box* const Other)
{
	// ÉpÉâÉÅÅ[É^ÇÉRÉsÅ[
	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
	const float& fSelfRadius = Self->GetRadius(), fSelfHeight = Self->GetHeight();

	// ëSÇƒÇÃé≤ï˚å¸Ç÷ÇÃéÀâeìØémÇ™èdÇ»Ç¡ÇƒÇ¢ÇÍÇŒè’ìÀ
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
// â~íåÇ∆AABB
//============================================================================
bool collision::HitCylinderToAABB(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
{
	// ëSÇƒÇÃé≤ï˚å¸Ç÷ÇÃéÀâeìØémÇ™èdÇ»Ç¡ÇƒÇ¢ÇÍÇŒè’ìÀ
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
// â~íåÇ∆AABBÇÃè’ìÀñ ÇéÊìæ
//============================================================================
int collision::GetCylinderToAABB(const Vec3& SelfOldPos, const Vec3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
{
	// ÉEÉBÉìÉhÉEÇï\é¶
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("[GetCylinderToAABB] UpdateInfo"))
	{
		ImGui::Text("OldPos:X %.2f:Y %.2f:Z %.2f", SelfOldPos.x, SelfOldPos.y, SelfOldPos.z);
		ImGui::Text("NowPos:X %.2f:Y %.2f:Z %.2f", SelfNowPos.x, SelfNowPos.y, SelfNowPos.z);
		ImGui::Text("SWidth:L %.2f:R %.2f", SelfNowPos.x + fSelfRadius, SelfNowPos.x - fSelfRadius);
	}
	ImGui::End();

	// è’ìÀé©ëÃÇ™ñ≥ÇØÇÍÇŒ
	if (!HitCylinderToAABB(SelfNowPos, fSelfRadius, fSelfHeight, OtherPos, OtherSize))
	{
		return 0;
	}

	// Ç‡Ç∆Ç‡Ç∆Xé≤Ç÷ÇÃéÀâeìØémÇ™èdÇ»Ç¡ÇƒÇ¢ÇΩÇ© (åvéZè„Zï˚å¸Ç…è’ìÀÇÕÇµÇ»Ç¢)
	if (SelfOldPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
		SelfOldPos.x - fSelfRadius < OtherPos.x + OtherSize.x)
	{ // èdÇ»Ç¡ÇƒÇ¢ÇΩ -> è„â∫ÇÃîªíË

		// è„Ç…Ç¢ÇΩ
		if (SelfOldPos.y > OtherPos.y)
		{
			return 1;
		}

		// â∫Ç…Ç¢ÇΩ
		if (SelfOldPos.y < OtherPos.y)
		{
			return 2;
		}
	}
	else
	{ // èdÇ»Ç¡ÇƒÇ¢Ç»Ç©Ç¡ÇΩ -> ç∂âEÇÃîªíË

		// ç∂Ç…Ç¢ÇΩ
		if (SelfOldPos.x < OtherPos.x)
		{
#ifdef _DEBUG
			CRenderer::SetTimeString("[ÇËÇÍÇ´]è’ìÀñ  : ç∂", 60);
#endif	// _DEBUG
			return 3;
		}

		// âEÇ…Ç¢ÇΩ
		if (SelfOldPos.x > OtherPos.x)
		{
#ifdef _DEBUG
			CRenderer::SetTimeString("[ÇËÇÍÇ´]è’ìÀñ  : âE", 60);
#endif	// _DEBUG
			return 4;
		}
	}

	return 0;
}