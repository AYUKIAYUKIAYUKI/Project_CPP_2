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
// �O���錾
//****************************************************
class CMotion_Set;

//****************************************************
// ��`�N���X
//****************************************************
class CFan
{
public:
	
	// <fuction>
	void Release();							// �j��
	void Update();							// �X�V����
	void Draw();							// �`�揈��
	bool DetectInFanRange(D3DXVECTOR3 Pos);	// ��`�͈͓��ɂ��邩���o

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// ���W���擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W��ݒ�

	// <getter/setter>
	const float& GetDirection() const;		// ���p���擾
	void SetDirection(float fDirection);	// ���p��ݒ�

	// <getter/setter>
	const float& GetRange() const;	// �͈͂��擾
	void SetRange(float fRange);	// �͈͂�ݒ�

	// <staic function>
	static CFan* Create();	// ����

private:

	/// <summary> �`��p���_�� </summary>
	static constexpr WORD NUM_VTX = 3;

	/// <summary> �����x�N�g���̐� </summary>
	static constexpr WORD NUM_VEC = 2;

	// <special fuction>
	CFan();		// �R���X�g���N�^
	~CFan();	// �f�X�g���N�^

	// <fuction>
	HRESULT	Init();				// �����ݒ�
	HRESULT	CreateVtxBuff();	// ���_�o�b�t�@�̐���
	void	Uninit();			// �I������
	void	SetVtx();			// ���_���W�̐ݒ�
	void	SetMtxWorld();		// ���[���h�s��ݒ�

	// <data>
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;				// ���_�o�b�t�@�̃|�C���^
	D3DXMATRIX				m_MtxWorld;				// ���[���h�s��
	D3DXVECTOR3				m_Pos;					// ���W
	D3DXVECTOR3				m_DirVec[NUM_VEC];		// �����x�N�g��
	float					m_fDirection;			// ���p
	float					m_fRange;				// �͈�
	CMotion_Set*			m_pLineDisp[NUM_VEC];	// ���C���\��

	// <static data>
	static const JSON m_MotionData[NUM_VEC];	// ���[�V�����f�[�^
	static const JSON m_InitParam;				// ��b�p�����[�^
};

#endif	// _FAN_H_