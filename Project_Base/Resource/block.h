//============================================================================
//
// �u���b�N�A�w�b�_�t�@�C�� [block.h]
// Author : ���c����
//
//============================================================================

#ifndef _BLOCK_H_
#define _BLOCK_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// �u���b�N�N���X
//****************************************************
class CBlock : public CObject_X
{
public:

	CBlock();	// �f�t�H���g�R���X�g���N�^
	~CBlock();	// �f�X�g���N�^

	HRESULT Init() override;	// �����ݒ�
	void Uninit() override;		// �I���ݒ�
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	// ����
	static CBlock* Create();									
	static CBlock* Create(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Rot);

	CObject* m_pTest;	// ��
};

#endif // _BLOCK_H_