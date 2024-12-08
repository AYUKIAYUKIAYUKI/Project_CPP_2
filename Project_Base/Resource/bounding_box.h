//============================================================================
//
// ���o�E���f�B���O�A�w�b�_�t�@�C�� [bouding_box.h]
// Author : ���c����
//
//============================================================================

#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bounding_volume.h"
#include "render_box.h"

//****************************************************
// �o�E���f�B���O�{�b�N�X�N���X
//****************************************************
class CBounding_Box : public CBounding_Volume
{
public:

	// <special function>
	CBounding_Box();			// �R���X�g���N�^
	~CBounding_Box() override;	// �f�X�g���N�^

	// <setter>
	void SetCenterPos(D3DXVECTOR3 Pos) override;	// ���S�_��ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// �T�C�Y���擾
	void SetSize(D3DXVECTOR3 Size);		// �T�C�Y��ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// �������擾
	void SetRot(D3DXVECTOR3 Rot);		// ������ݒ�

private:

	// <data>
	D3DXVECTOR3		m_Size;			// �T�C�Y
	D3DXVECTOR3		m_Rot;			// ����
#ifdef _DEBUG
	CRender_Box*	m_pRenderBox;	// ���\��
#endif
};

#endif	// _BOUNDING_BOX_H_