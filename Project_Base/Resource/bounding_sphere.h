//============================================================================
//
// ���o�E���f�B���O�A�w�b�_�t�@�C�� [bouding_sphere.h]
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
// ���o�E���f�B���O�N���X
//****************************************************
class CBounding_Sphere : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Sphere();				// �R���X�g���N�^
	~CBounding_Sphere() override;	// �f�X�g���N�^

	// <setter>
	void SetCenterPos(D3DXVECTOR3 Pos) override;	// ���S�_��ݒ�

	// <getter/setter>
	const float& GetRadius() const;	// ���a���擾
	void SetRadius(float fRad);		// ���a��ݒ�

private:

	// <data>
	float			m_fRadius;			// ���a
#ifdef _DEBUG
	CRender_Sphere* m_pRenderSphere;	// ���\��
#endif
};

#endif	// _BOUNDING_SPHERE_H_