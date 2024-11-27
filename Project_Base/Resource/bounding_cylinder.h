//============================================================================
//
// �~���o�E���f�B���O�A�w�b�_�t�@�C�� [bouding_cylinder.h]
// Author : ���c����
//
//============================================================================

#ifndef _BOUNDING_CYLINDER_H_
#define _BOUNDING_CYLINDER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bounding_volume.h"
#include "render_cylinder.h"

//****************************************************
// �~���o�E���f�B���O�N���X
//****************************************************
class CBounding_Cylinder : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Cylinder();			// �R���X�g���N�^
	~CBounding_Cylinder() override;	// �f�X�g���N�^

	// <setter>
	void SetCenterPos(D3DXVECTOR3 Pos) override;	// ���S�_��ݒ�

	// <getter/setter>
	const float& GetRadius() const;	// ���a���擾
	void SetRadius(float fRad);		// ���a��ݒ�

	// <getter/setter>
	const float& GetHeight() const;	// �������擾
	void SetHeight(float fHeight);	// ������ݒ�

private:

	// <data>
	float			  m_fRadius;			// ���a
	float			  m_fHeight;			// ����
#ifdef _DEBUG
	CRender_Cylinder* m_pRenderCylinder;	// �V�����_�[�\��
#endif // _DEBUG
};

#endif	// _BOUNDING_CYLINDER_H_