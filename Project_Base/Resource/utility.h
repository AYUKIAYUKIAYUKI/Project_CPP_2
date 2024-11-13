//============================================================================
// 
// �֗��֐��A�w�b�_�t�@�C�� [utility.h]
// Author : ���c����
// 
//============================================================================

#ifndef _UTILITY_H_
#define _UTILITY_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �֗��֐��N���X
//****************************************************
class CUtility final
{
public:

	// �ی��ɉ����āA�P�ʗ����̂̒��_�̍��W���쐬
	static D3DXVECTOR3 SetCubeVtxFromQuadrant(const WORD& wIdx);

	// �_��Y���ŉ�]������
	static D3DXVECTOR3 RotatePointAroundY(const float& fDirection, const D3DXVECTOR3& Pos);

	// ����Y���ŉ�]������
	static D3DXVECTOR3 RotateBoxAroundY(const WORD& wIdx, const float& fDirection, const D3DXVECTOR3& Size);

	// ����Y���̉�]��ł�����
	static D3DXVECTOR3 InverseRotateBoxAroundY(const float& fDirection, const D3DXVECTOR3& VtxPos);

	// �p�x�̍��̕␳(���W�A��)
	static void AdjustAngle(float& fAngle, const float& fDest);

	// �p�x�̍��̕␳(���W�A��)
	static void AdjustAngle(float& fAngle, const float& fDest, const float& fRadius);

	// �����͈̔͂̕␳(���W�A��)
	static void AdjustDirection(float& fAngle);

	// �����͈̔͂̕␳(���W�A��)
	static void AdjustDirection(float& fAngle1, float& fAngle2);

	// ���ǂ����̏Փ˔���
	static bool OnlySphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget);

	// ����AABB�̓����蔻��
	static bool SphereAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTaret);

	// �~���Ɠ_�̏Փ˔���
	static bool CylinderAndPoint(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget);

	// �~���Ƌ��̏Փ˔���
	static bool CylinderAndSphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget);

	// �~����AABB�̏Փ˔���
	static bool CylinderAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTarget);

	// Json�t�@�C���̓W�J
	static nlohmann::json OpenJsonFile(std::string FilePath);

	// ��������
	template <typename T> static T GetRandomValue();

	// �_�E���L���X�g
	template <typename T1, typename T2> static T1* DownCast(T1* pDest, T2* pBase);
};

//============================================================================
// ��������
//============================================================================
template <typename T> T CUtility::GetRandomValue()
{
	// ���l�̎��͈͂�ݒ�
	const int nRange = 180;

	// �{�͈̔͂��烉���_���Ȑ����𐶐�
	int nRandom{ rand() % (nRange * 2) };

	// ���Ƃ͈̔͂𒴂��Ă����ꍇ�����𔽓]
	if (nRandom > nRange)
	{
		nRandom %= nRange;
		nRandom *= -1;
	}

	// �������w�肳�ꂽ�^�ɃL���X�g
	return static_cast<T>(nRandom);
}

//============================================================================
// �_�E���L���X�g
//============================================================================
template <typename T1, typename T2> T1* CUtility::DownCast(T1* pDest, T2* pBase)
{
	pDest;	/* �x����� */

	// �����̃I�u�W�F�N�g���w�肳�ꂽ�N���X�Ƀ_�E���L���X�g
	T1* pObj{ dynamic_cast<T1*>(pBase) };

	// �_�E���L���X�g���s
	if (!pObj)
	{
		assert(false);
	}

	return pObj;
}

#endif // _UTILITY_H_