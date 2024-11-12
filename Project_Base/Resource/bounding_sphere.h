//============================================================================
//
// �o�E���f�B���O�X�t�B�A�A�w�b�_�t�@�C�� [bouding_sphere.h]
// Author : ���c����
//
//============================================================================

#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bounding_volume.h"
#include "render_sphere.h"

//****************************************************
// �o�E���f�B���O�X�t�B�A�N���X
//****************************************************
class CBounding_Sphere : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Sphere();					// �f�t�H���g�R���X�g���N�^
	CBounding_Sphere(CObject_X* pObj);	// �X�t�B�A�\���R���X�g���N�^
	~CBounding_Sphere() override;		// �f�X�g���N�^

	// <getter/setter>
	const float& GetRadius() const;	// ���a���擾
	void SetRadius(float fRad);		// ���a��ݒ�

private:

	// <data>
	float m_fRadius;					// ���a
	CRender_Sphere* m_pRenderSphere;	// �X�t�B�A�\��
};

#endif	// _BOUNDING_SPHERE_H_