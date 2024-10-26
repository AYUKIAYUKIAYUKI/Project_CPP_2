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
#include "object_X.h"

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

	/// <summary> ���[�u�J�E���g�̍ő吔 </summary>
	static constexpr int MAX_MOVE_COUNT = 60;

	//****************************************************
	// 2���x�W�F�Ȑ��\���̂̒�`
	//****************************************************
	struct QuadraticBezier
	{
		LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// ���_�o�b�t�@�̃|�C���^

		CObject_X*	pObject;		// ����
		D3DXVECTOR3 PinPos[2];		// �s���ƂȂ���W	
		D3DXVECTOR3 MagPos;			// ���΂ƂȂ���W	
		float		fCoef;			// �s���Ԃ̊���
		CObject_X*	pCoefObj[3];	// �s���Ԃ̊����\��
		//int		nMoveLine;		// �ړ����Ă����
	};

	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	HRESULT CreateIdxBuff();	// �C���f�b�N�X�o�b�t�@�̐���
	void	CalcBezier();		// �x�W�F�Ȑ����v�Z
	void	SetMtxWorld();		// ���[���h�s��ݒ�

	JSON					m_Json;				// �W�F�C�\���I�u�W�F�N�g
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;			// ���_�o�b�t�@�̃|�C���^
	int						m_nNumVtx;			// ���_��
	int						m_nNumPrim;			// �v���~�e�B�u��
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;			// �C���f�b�N�X�o�b�t�@�̃|�C���^
	int						m_nNumIdx;			// �C���f�b�N�X��
	D3DXVECTOR3				m_Pos;				// ���W
	D3DXMATRIX				m_MtxWorld;			// ���[���h�s��
	QuadraticBezier			m_QuadraticBezier;	// �񎟃x�W�F�Ȑ��\����
};

#endif	// _SPLINE_TEST_