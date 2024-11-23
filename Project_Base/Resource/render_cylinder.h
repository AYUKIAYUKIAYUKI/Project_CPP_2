//============================================================================
// 
// �V�����_�[�\���A�w�b�_�t�@�C�� [render_cylinder.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RENDER_CYLINDER_H_
#define _RENDER_CYLINDER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "render_collision.h"
#include "X_manager.h"

//****************************************************
// �V�����_�[����\���N���X
//****************************************************
class CRender_Cylinder : public CRender_Collision
{
public:

	// <special function>
	CRender_Cylinder(LAYER Priority = LAYER::FRONT);	// �`��D��x�w��R���X�g���N�^
	~CRender_Cylinder() override;						// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <setter>
	void ChangeModel(CX_Manager::TYPE Type);	// ���f����ύX

	// <static function>
	static CRender_Cylinder* Create(CObject_X* pRef);	// ����

private:

	// <data>
	CObject_X* m_pCylinder;	// �V�����_�[
};

#endif	// _RENDER_CYLINDER_H_