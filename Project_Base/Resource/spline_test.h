//============================================================================
//
// �X�v���C���Ȑ��e�X�g�A�w�b�_�t�@�C�� [spline_test.h]
// Author : ���c����
//
//============================================================================

#ifndef _SPLINE_TEST_
#define _SPLINE_TEST_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "quadratic_bezier.h"

//****************************************************
// �X�v���C���e�X�g�N���X
//****************************************************
class CSpline_Test
{
public:

	CSpline_Test();		// �f�t�H���g�R���X�g���N�^
	~CSpline_Test();	// �f�X�g���N�^

	HRESULT Init();		// �����ݒ�
	void	Uninit();	// �I������
	void	Update();	// �X�V����
	void	Draw();		// �`�揈��

private:

	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	void	SetMtxWorld();		// ���[���h�s��ݒ�

	JSON					m_Json;					// �W�F�C�\���I�u�W�F�N�g
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;				// ���_�o�b�t�@�̃|�C���^
	int						m_nNumVtx;				// ���_��
	int						m_nNumPrim;				// �v���~�e�B�u��
	D3DXVECTOR3				m_Pos;					// ���W
	D3DXMATRIX				m_MtxWorld;				// ���[���h�s��
	CQuadratic_Bezier*		m_pQuadratic_Bezier;	// �񎟃x�W�F�Ȑ�
};

#endif	// _SPLINE_TEST_