//============================================================================
//
// �o�E���f�B���O�{�b�N�X�A�w�b�_�t�@�C�� [bouding_box.h]
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
	CBounding_Box();				// �f�t�H���g�R���X�g���N�^
	CBounding_Box(CObject_X* pObj);	// �{�b�N�X�\���R���X�g���N�^
	~CBounding_Box() override;		// �f�X�g���N�^

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// �T�C�Y���擾
	void SetSize(D3DXVECTOR3 Size);		// �T�C�Y��ݒ�

private:

	// <data>
	D3DXVECTOR3		m_Size;			// �T�C�Y
	CRender_Box*	m_pRenderBox;	// �{�b�N�X�\��
};

#endif	// _BOUNDING_BOX_H_