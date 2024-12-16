//============================================================================
// 
// �|�b�v�A�b�v�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_PopUp.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_POPUP_H_
#define _OBJECT_POPUP_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_3D.h"

//****************************************************
// �|�b�v�A�b�v�I�u�W�F�N�g�N���X
//****************************************************
class CObject_PopUp : public CObject_3D
{
public:

	// <special function>
	CObject_PopUp(LAYER Priority = LAYER::UI);	// �R���X�g���N�^
	~CObject_PopUp() override;					// �f�X�g���N�^

	// <function>
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <Setter>
	void SetCorrectionCoef(float fCorrectionCoef);	// �␳�W���ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// �ڕW���W�擾
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// �ڕW���W�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// �ڕW�����擾
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// �ڕW�����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetSizeTarget() const;	// �ڕW�T�C�Y�擾
	void SetSizeTarget(D3DXVECTOR3 SizeTarget);	// �ڕW�T�C�Y�ݒ�

	// <getter/setter>
	const D3DXCOLOR& GetColTarget() const;	// �ڕW�F�擾
	void SetColTarget(D3DXCOLOR ColTarget);	// �ڕW�F�ݒ�

	// <static function>
	static CObject_PopUp* Create(JSON Json);	// ����

private:

	// <function>
	void CorrectToTarget();	// �ڕW�l�֕␳

	// <data>
	float		m_fCorrectionCoef;	// �␳�W��
	D3DXVECTOR3	m_PosTarget;		// �ڕW���W
	D3DXVECTOR3	m_RotTarget;		// �ڕW����
	D3DXVECTOR3 m_SizeTarget;		// �ڕW�T�C�Y
	D3DXCOLOR	m_ColTarget;		// �ڕW�F
};

#endif // _OBJECT_POPUP_H_