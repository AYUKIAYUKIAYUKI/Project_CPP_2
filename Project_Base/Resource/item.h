//============================================================================
//
// �A�C�e���A�w�b�_�t�@�C�� [item.h]
// Author : ���c����
//
//============================================================================

#ifndef _ITEM_H_
#define _ITEM_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// �O���錾
//****************************************************
class CBounding_Sphere;

//****************************************************
// �A�C�e���N���X
//****************************************************
class CItem : public CObject_X
{
public:

	// <special function>
	CItem();			// �R���X�g���N�^
	~CItem() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <getter/setter>
	float GetRadius() const override;	// ���a���擾
	void SetRadius(float fRadius);		// ���a��ݒ�

	// <setter>
	void SetCorrectCoef(float fCoef);	// �␳���x��ݒ�

	// <getter/setter>
	const float& GetDirection() const;		// ���p���擾
	void SetDirection(float fDirection);	// ���p��ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// �ڕW�������擾
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// �ڕW������ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// �ڕW���W���擾
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// �ڕW���W��ݒ�

	// <setter>
	void SetPosY(float fPosY);	// Y���W��ݒ�

private:

	// <function>
	void HitChecklPlayer();	// �v���C���[�Ƃ̐ڐG�����o
	void AppealRotate();	// ���邭����
	void CorrectToTarget();	// �ڕW�l�ւ̕��

	// <virtual function>
	virtual void GainEffect() = 0;	// �擾���̌���

	// <data>
	CBounding_Sphere* m_pBndSphere;		// ���o�E���f�B���O
	float			  m_fCorrectCoef;	// ��ԋ��x
	float			  m_fDirection;		// ���p
	D3DXVECTOR3		  m_RotTarget;		// �ڕW����
	D3DXVECTOR3		  m_PosTarget;		// �ڕW���W
	CObject_X*		  m_pSummoning;		// ���@�w
};

#endif // _ITEM_H_