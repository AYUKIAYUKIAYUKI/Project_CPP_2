//============================================================================
// 
// �R���W�����n�֐� [collision.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "collision.h"
#include "renderer.h"

//============================================================================
// �����m
//============================================================================
bool collision::HitSphereToSphere(const CBounding_Sphere* const Self, const CBounding_Sphere* const Other)
{
	// ���݂��̋������v�Z
	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();

	// �������A���݂��̔��a�̘a�ȉ��Ȃ�ΏՓ�
	if (sqrtf(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z) <= Self->GetRadius() + Other->GetRadius())
	{
		return 1;
	}

	return 0;
}

//============================================================================
// ����AABB
//============================================================================
bool collision::HitSphereToAABB(const CBounding_Sphere* const Self, const CBounding_Box* const Other)
{
	// �p�����[�^���R�s�[
	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
	const float& fSelfRadius = Self->GetRadius();

	// �S�Ă̎������ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
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
// �~�����m
//============================================================================
bool collision::HitCylinderToCylinder(const CBounding_Cylinder* const Self, const CBounding_Cylinder* const Other)
{
	// ���݂��̋������v�Z
	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();

	// �������A���݂��̔��a�̘a���傫����ΏՓ˂��Ă��Ȃ�
 	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
	{
		return 0;
	}

	// Y���ւ̎ˉe���m���d�Ȃ��Ă�����Փ�
	if (Self->GetCenterPos().y + Self->GetHeight() > Other->GetCenterPos().y - Other->GetHeight() &&
		Self->GetCenterPos().y - Self->GetHeight() < Other->GetCenterPos().y + Other->GetHeight())
	{
		return 1;
	}

	return 0;
}

//============================================================================
// �~���Ɠ_
//============================================================================
bool collision::HitCylinderToPoint(const CBounding_Cylinder* const Self, const Vec3& Other)
{
	// ���݂��̋������v�Z
	const Vec3& Distance = Other - Self->GetCenterPos();

	// �������A�~���̔��a���傫����ΏՓ˂��Ă��Ȃ�
	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius())
	{
		return 0;
	}

	// Y���ւ̎ˉe�ɓ_�����݂���ΏՓ�
	if (Self->GetCenterPos().y + Self->GetHeight() > Other.y &&
		Self->GetCenterPos().y - Self->GetHeight() < Other.y)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// �~���Ƌ�
//============================================================================
bool collision::HitCylinderToSphere(const CBounding_Cylinder* const Self, const CBounding_Sphere* const Other)
{
	// ���݂��̋������v�Z
	const Vec3& Distance = Other->GetCenterPos() - Self->GetCenterPos();

	// �������A���݂��̔��a�̘a���傫����ΏՓ˂��Ă��Ȃ�
	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > Self->GetRadius() + Other->GetRadius())
	{
		return 0;
	}

	// Y���ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
	if (Self->GetCenterPos().y + Self->GetHeight() > Other->GetCenterPos().y - Other->GetRadius() &&
		Self->GetCenterPos().y - Self->GetHeight() < Other->GetCenterPos().y + Other->GetRadius())
	{
		return 1;
	}

	return 0;
}

//============================================================================
// �~���Ƌ�
//============================================================================
bool collision::HitCylinderToSphere(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const float& fOtherRadius)
{
	// ���݂��̋������v�Z
	const Vec3& Distance = OtherPos - SelfPos;

	// �������A���݂��̔��a�̘a���傫����ΏՓ˂��Ă��Ȃ�
	if (sqrtf(Distance.x * Distance.x + Distance.z * Distance.z) > fSelfRadius + fOtherRadius)
	{
		return 0;
	}

	// Y���ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
	if (SelfPos.y + fSelfHeight > OtherPos.y - fSelfRadius &&
		SelfPos.y - fSelfHeight < OtherPos.y + fSelfRadius)
	{
		return 1;
	}

	return 0;
}

//============================================================================
// �~����AABB
//============================================================================
bool collision::HitCylinderToAABB(const CBounding_Cylinder* const Self, const CBounding_Box* const Other)
{
	// �p�����[�^���R�s�[
	const Vec3& SelfPos = Self->GetCenterPos(), OtherPos = Other->GetCenterPos(), OtherSize = Other->GetSize();
	const float& fSelfRadius = Self->GetRadius(), fSelfHeight = Self->GetHeight();

	// �S�Ă̎������ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
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
// �~����AABB
//============================================================================
bool collision::HitCylinderToAABB(const Vec3& SelfPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
{
	// �S�Ă̎������ւ̎ˉe���m���d�Ȃ��Ă���ΏՓ�
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
// �~����AABB�̏Փ˖ʂ��擾
//============================================================================
int collision::GetCylinderToAABB(const Vec3& SelfOldPos, const Vec3& SelfNowPos, const float& fSelfRadius, const float& fSelfHeight, const Vec3& OtherPos, const Vec3& OtherSize)
{
	// �E�B���h�E��\��
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("[GetCylinderToAABB] UpdateInfo"))
	{
		ImGui::Text("OldPos:X %.2f:Y %.2f:Z %.2f", SelfOldPos.x, SelfOldPos.y, SelfOldPos.z);
		ImGui::Text("NowPos:X %.2f:Y %.2f:Z %.2f", SelfNowPos.x, SelfNowPos.y, SelfNowPos.z);
		ImGui::Text("SWidth:L %.2f:R %.2f", SelfNowPos.x + fSelfRadius, SelfNowPos.x - fSelfRadius);
	}
	ImGui::End();

	// �Փˎ��̂��������
	if (!HitCylinderToAABB(SelfNowPos, fSelfRadius, fSelfHeight, OtherPos, OtherSize))
	{
		return 0;
	}

	// ���Ƃ���X���ւ̎ˉe���m���d�Ȃ��Ă����� (�v�Z��Z�����ɏՓ˂͂��Ȃ�)
	if (SelfOldPos.x + fSelfRadius > OtherPos.x - OtherSize.x &&
		SelfOldPos.x - fSelfRadius < OtherPos.x + OtherSize.x)
	{ // �d�Ȃ��Ă��� -> �㉺�̔���

		// ��ɂ���
		if (SelfOldPos.y > OtherPos.y)
		{
			return 1;
		}

		// ���ɂ���
		if (SelfOldPos.y < OtherPos.y)
		{
			return 2;
		}
	}
	else
	{ // �d�Ȃ��Ă��Ȃ����� -> ���E�̔���

		// ���ɂ���
		if (SelfOldPos.x < OtherPos.x)
		{
#ifdef _DEBUG
			CRenderer::SetTimeString("[��ꂫ]�Փ˖� : ��", 60);
#endif	// _DEBUG
			return 3;
		}

		// �E�ɂ���
		if (SelfOldPos.x > OtherPos.x)
		{
#ifdef _DEBUG
			CRenderer::SetTimeString("[��ꂫ]�Փ˖� : �E", 60);
#endif	// _DEBUG
			return 4;
		}
	}

	return 0;
}