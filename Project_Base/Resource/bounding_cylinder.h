//============================================================================
//
// �o�E���f�B���O�V�����_�[�A�w�b�_�t�@�C�� [bouding_cylinder.h]
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
#include "X_manager.h"

//****************************************************
// �o�E���f�B���O�V�����_�[�N���X
//****************************************************
class CBounding_Cylinder : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Cylinder();					// �R���X�g���N�^
	CBounding_Cylinder(CObject_X* pObj);	// �V�����_�[�\���R���X�g���N�^
	~CBounding_Cylinder() override;			// �f�X�g���N�^

	// <setter>
	void ChangeModel(CX_Manager::TYPE Type);	// ���f����ύX

	// <getter/setter>
	const float& GetRadius() const;	// ���a���擾
	void SetRadius(float fRad);		// ���a��ݒ�

	// <getter/setter>
	const float& GetHeight() const;	// �������擾
	void SetHeight(float fHeight);	// ������ݒ�

private:

	// <data>
	float m_fRadius;						// ���a
	float m_fHeight;						// ����
	CRender_Cylinder* m_pRenderCylinder;	// �V�����_�[�\��
};

#endif	// _BOUNDING_CYLINDER_H_