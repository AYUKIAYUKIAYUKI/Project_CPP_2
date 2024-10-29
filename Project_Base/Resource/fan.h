//============================================================================
//
// ��`�A�w�b�_�t�@�C�� [fan.h]
// Author : ���c����
//
//============================================================================

#ifndef _FAN_H_
#define _FAN_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

//****************************************************
// ��`�N���X
//****************************************************
class CFan
{
public:
	
	void Release();							// �j��
	void Update();							// �X�V����
	void Draw();							// �`�揈��
	bool DetectInFanRange(D3DXVECTOR3 Pos);	// ��`�͈͓��ɂ��邩���o

	const D3DXVECTOR3& GetPos() const;	// ���W���擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W��ݒ�

	const float& GetDirection() const;		// ���p���擾
	void SetDirection(float fDirection);	// ���p��ݒ�

	const float& GetLength() const;	// �������擾
	void SetLength(float fLength);	// ������ݒ�

	const float& GetRange() const;	// �͈͂��擾
	void SetRange(float fRange);	// �͈͂�ݒ�

	static CFan* Create();																	// ����
	static CFan* Create(D3DXVECTOR3 Pos, float fDirection, float fLength, float fRange);	// ����

private:

	static constexpr WORD NUM_VTX = 2;	// ���_��

	CFan();		// �f�t�H���g�R���X�g���N�^
	~CFan();	// �f�X�g���N�^

	HRESULT Init();				// �����ݒ�
	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	void Uninit();				// �I������
	void SetVtx();				// ���_���W�̐ݒ�
	void SetMtxWorld();			// ���[���h�s��ݒ�

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
	D3DXMATRIX				m_MtxWorld;		// ���[���h�s��
	D3DXVECTOR3				m_Pos;			// ���W
	float					m_fDirection;	// ���p
	float					m_fLength;		// ����
	float					m_fRange;		// �͈�
};

#endif	// _FAN_H_