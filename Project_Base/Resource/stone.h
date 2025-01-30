//============================================================================
//
// �X�g�[���A�w�b�_�t�@�C�� [stone.h]
// Author : ���c����
//
//============================================================================

#ifndef _STONE_H_
#define _STONE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// �O���錾
//****************************************************
class CBounding_Sphere;

//****************************************************
// �X�g�[���N���X
//****************************************************
class CStone : public CObject_X
{
public:

	// <special function>
	CStone();			// �R���X�g���N�^
	~CStone() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <getter/setter>
	float GetRadius() const override;	// ���a���擾
	void SetRadius(float fRadius);		// ���a��ݒ�

	static CStone* Create(D3DXVECTOR3 InitPos, D3DXVECTOR3 Accel);	// ����

private:

	// �؋󎞊�
	static const int WAIT_DURATION = 35;

	// <function>
	void HitCheckEnemy();	// �G�Ƃ̐ڐG�����o

	// <data>
	CBounding_Sphere* m_pBndSphere;	// ���o�E���f�B���O
	D3DXVECTOR3		  m_Accel;		// ���i��
	int				  m_nDuration;	// ����
};

#endif // _STONE_H_