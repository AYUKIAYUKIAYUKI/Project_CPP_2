//============================================================================
// 
// ����\���A�w�b�_�t�@�C�� [render_collision.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RENDER_COLLISION_H_
#define _RENDER_COLLISION_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

//****************************************************
// �O���錾
//****************************************************
class CObject_X;

//****************************************************
// ����\���N���X
//****************************************************
class CRender_Collision : public CObject
{
public:

	// <special function>
	CRender_Collision(LAYER Priority = LAYER::FRONT);	// �`��D��x�w��R���X�g���N�^
	~CRender_Collision() override;						// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	
	// <virtual function>
	virtual void Draw() override = 0;	// �`�揈��

	// <setter>
	void SetRefObj(CObject_X* pRef);	// �ΏۃI�u�W�F�N�g�̐ݒ�

protected:

	// <data>
	CObject_X*	m_pRef;		// �ΏۃI�u�W�F�N�g
	D3DXMATRIX	m_MtxWorld;	// ���[���h�s��

private:

	// <function>
	void SetMtxWorld();	// ���[���h�s��ݒ�
};

#endif	// _RENDER_COLLISION_H_