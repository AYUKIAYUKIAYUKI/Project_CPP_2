//============================================================================
// 
// ���\���A�w�b�_�t�@�C�� [RENDER_SPHERE.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RENDER_SPHERE_H_
#define _RENDER_SPHERE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "render_collision.h"

//****************************************************
// ���\���N���X
//****************************************************
class CRender_Sphere : public CRender_Collision
{
public:

	// <special function>
	CRender_Sphere(LAYER Priority = LAYER::FRONT);	// �`��D��x�w��R���X�g���N�^
	~CRender_Sphere() override;						// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <setter>
	void SetSyncRadius(float fRadius);	// �V���N�����a��ݒ�

	// <static function>
	static CRender_Sphere* Create();	// ����

private:

	// <data>
	float	   m_fSyncRadius;	// �V���N�����a
	CObject_X* m_pSphereModel;	// �����f��
};

#endif	// _RENDER_SPHERE_H_