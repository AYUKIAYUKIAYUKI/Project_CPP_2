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
// �O���錾
//****************************************************
class CBounding_Box;

//****************************************************
// �u���b�N�N���X
//****************************************************
class CBlock : public CObject_X
{
public:

	// <special function>
	CBlock();	// �f�t�H���g�R���X�g���N�^
	~CBlock();	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void Uninit() override;		// �I���ݒ�
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	// <getter>
	D3DXVECTOR3 GetSize() const override;	// �T�C�Y���擾

	// <static function>
	static CBlock* Create(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Rot);	// ����

private:

	// <data>
	CBounding_Box* m_pBndBox;	// �o�E���f�B���O�{�b�N�X
};

#endif // _BLOCK_H_