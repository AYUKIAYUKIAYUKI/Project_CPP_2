//============================================================================
// 
// ���ꂫ�G�t�F�N�g�A�w�b�_�t�@�C�� [rubble.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RUBBLE_H_
#define _RUBBLE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// ���ꂫ�G�t�F�N�g�N���X
//****************************************************
class CRubble : public CObject_X
{
public:

	CRubble();	// �R���X�g���N�^
	~CRubble();	// �f�X�g���N�^

	HRESULT Init() override;	// �����ݒ�
	void Uninit() override;		// �I������
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	D3DXVECTOR3& GetVelocity();				// �����x���擾
	void SetVelocity(D3DXVECTOR3 Velocity);	// �����x��ݒ�

	static CRubble* Create(int nType, D3DXVECTOR3 Pos, D3DXVECTOR3 Velocity);	// ����

private:

	void CheckDisappear();	// ���ł��邩�`�F�b�N

	D3DXVECTOR3 m_Velocity;	// �����x
};

#endif	// _RUBBLE_H_