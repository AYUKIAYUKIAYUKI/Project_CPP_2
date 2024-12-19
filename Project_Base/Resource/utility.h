//============================================================================
// 
// �֗��֐��A�w�b�_�t�@�C�� [utility.h]
// Author : ���c����
// 
//============================================================================

#ifndef _UTILITY_H_
#define _UTILITY_H_	// ��d�C���N���[�h�h�~

//****************************************************
// ���O���utility���`
//****************************************************
namespace utility
{
	// �ی��ɉ����āA�P�ʗ����̂̒��_�̍��W���쐬
	D3DXVECTOR3 SetCubeVtxFromQuadrant(const WORD& wIdx);

	// �_��Y���ŉ�]������
	D3DXVECTOR3 RotatePointAroundY(const float& fDirection, const D3DXVECTOR3& Pos);

	// ����Y���ŉ�]������
	D3DXVECTOR3 RotateBoxAroundY(const WORD& wIdx, const float& fDirection, const D3DXVECTOR3& Size);

	// ����Y���̉�]��ł�����
	D3DXVECTOR3 InverseRotateBoxAroundY(const float& fDirection, const D3DXVECTOR3& VtxPos);

	// �p�x�̍��̕␳(���W�A��)
	void AdjustAngle(float& fAngle, const float& fDest);

	// �p�x�̍��̕␳(���W�A��)
	void AdjustAngle(float& fAngle, const float& fDest, const float& fRadius);

	// �����͈̔͂̕␳(���W�A��)
	void AdjustDirection(float& fAngle);

	// �����͈̔͂̕␳(���W�A��)
	void AdjustDirection(float& fAngle1, float& fAngle2);

	// Json�t�@�C���̓W�J
	nlohmann::json OpenJsonFile(std::string FilePath);

	// Json�f�[�^��D3DXVECTOR2�ɕϊ�
	D3DXVECTOR2 JsonConvertToVec2(nlohmann::json Json);

	// Json�f�[�^��D3DXVECTOR3�ɕϊ�
	D3DXVECTOR3 JsonConvertToVec3(nlohmann::json Json);

	// Json�f�[�^��D3DXCOLR�ɕϊ�
	D3DXCOLOR JsonConvertToXCol(nlohmann::json Json);

	// UTF8��Json�f�[�^��SJIS�ɕϊ�
	std::string JsonConvertToSJIS(nlohmann::json Json);

	// ��������
	template <typename T> static T GetRandomValue();

	// �_�E���L���X�g
	template <typename T1, typename T2> static T1* DownCast(T2* pBase);
	template <typename T1, typename T2> static T1* DownCast(T1* pDest, T2* pBase);
};

//============================================================================
// ��������
//============================================================================
template <typename T> T utility::GetRandomValue()
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
template <typename T1, typename T2> T1* utility::DownCast(T2* pBase)
{
	// �����̃I�u�W�F�N�g���w�肳�ꂽ�N���X�Ƀ_�E���L���X�g
	T1* pObj = dynamic_cast<T1*>(pBase);

	// �_�E���L���X�g���s
	if (!pObj) throw std::bad_cast();

	return pObj;
}

//============================================================================
// �_�E���L���X�g
//============================================================================
template <typename T1, typename T2> T1* utility::DownCast(T1* pDest, T2* pBase)
{
	/* �x����� */
	pDest;

	// �����̃I�u�W�F�N�g���w�肳�ꂽ�N���X�Ƀ_�E���L���X�g
	T1* pObj = dynamic_cast<T1*>(pBase);

	// �_�E���L���X�g���s
	if (!pObj) throw std::bad_cast();

	return pObj;
}

#endif // _UTILITY_H_