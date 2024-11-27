//============================================================================
//
// �o�E���f�B���O�{�����[���A�w�b�_�t�@�C�� [bouding_volume.h]
// Author : ���c����
//
//============================================================================

#ifndef _BOUNDING_VOLUME_H_
#define _BOUNDING_VOLUME_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �o�E���f�B���O�{�����[���N���X
//****************************************************
class CBounding_Volume
{
public:

	// <special function>
	CBounding_Volume();					// �R���X�g���N�^
	virtual ~CBounding_Volume() = 0;	// �f�X�g���N�^

	// <getter>
	const D3DXVECTOR3& GetCenterPos() const;	// ���S�_���擾

	// <virtual setter>
	virtual void SetCenterPos(D3DXVECTOR3 Pos);	// ���S�_��ݒ�

private:

	// <data>
	D3DXVECTOR3 m_CenterPos;	// ���S�_
};

#endif	// _BOUNDING_VOLUME_H_