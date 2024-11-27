//============================================================================
// 
// �~���\���A�w�b�_�t�@�C�� [render_cylinder.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RENDER_CYLINDER_H_
#define _RENDER_CYLINDER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "render_collision.h"

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
	void SetSyncRadius(float fRadius);	// �V���N�����a��ݒ�
	void SetSyncHeight(float fHeight);	// �V���N��������ݒ�

	// <static function>
	static CRender_Cylinder* Create();	// ����

private:

	// <data>
	float	   m_fSyncRadius;		// �V���N�����a
	float	   m_fSyncHeight;		// �V���N������
	CObject_X* m_pCylinderModel;	// �~�����f��
};

#endif	// _RENDER_CYLINDER_H_