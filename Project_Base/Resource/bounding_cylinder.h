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

#ifdef _DEBUG
	// <setter>
	void ChangeModel(CX_Manager::TYPE Type);	// ���f����ύX
	void SetCol(D3DXCOLOR Col);					// �F��ݒ�
	void SetUseCol(bool bUse);					// �F���f��ݒ�
#endif // _DEBUG

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
	float m_fRadius;						// ���a
	float m_fHeight;						// ����
#ifdef _DEBUG
	CRender_Cylinder* m_pRenderCylinder;	// �V�����_�[�\��
#endif // _DEBUG
};

#endif	// _BOUNDING_CYLINDER_H_