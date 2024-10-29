//============================================================================
// 
// HUD�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_HUD.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_HUD_H_
#define _OBJECT_HUD_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_2D.h"

//****************************************************
// HUD�I�u�W�F�N�g�N���X
//****************************************************
class CObject_HUD : public CObject_2D
{
public:

	CObject_HUD(int nPriority = static_cast<int>(LAYER::UI));	// �`��D��x�w��R���X�g���N�^
	~CObject_HUD() override;									// �f�X�g���N�^

	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	static CObject_HUD* Create();	// ����

private:

	D3DXVECTOR3	m_PosTarget;	// �ڕW���W
	D3DXVECTOR3	m_RotTarget;	// �ڕW����
	D3DXVECTOR3 m_SizeTarget;	// �ڕW�T�C�Y
	D3DXCOLOR	m_ColTarget;	// �ڕW�F
};

#endif // _OBJECT_HUD_H_