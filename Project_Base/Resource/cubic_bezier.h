//============================================================================
//
// �O���x�W�F�Ȑ��A�w�b�_�t�@�C�� [cubic_bezier.h]
// Author : ���c����
//
//============================================================================

#ifndef _CUBIC_BEZIER_
#define _CUBIC_BEZIER_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// 3���x�W�F�Ȑ��N���X�̒�`
//****************************************************
class CCubic_Bezier
{
public:

	/// <summary> ����_�̐� </summary>
	static constexpr int NUM_CONTROLPOINT = 3;

	CCubic_Bezier(CObject_X* p1, CObject_X* p2);	// �񎟋O�Փn���R���X�g���N�^
	~CCubic_Bezier();								// �f�X�g���N�^

	HRESULT Init();		// �����ݒ�
	void	Uninit();	// �I������
	void	Update();	// �X�V����
	void	Draw();		// �`�揈��

private:

	/// <summary> �ړ����x </summary>
	static constexpr float MOVE_SPEED = 0.005f;

	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�𐶐�

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;					// ���_�o�b�t�@�̃|�C���^
	float					m_fParameter;				// �i�s�x
	CObject_X* 				m_pQuadratic_Trajectory[2];	// �񎟋O��
	CObject_X*				m_pCubic_Trajectory;		// �O���O��
};

#endif	// _QUADRATIC_BEZIER_