//============================================================================
//
// �񎟃x�W�F�Ȑ��A�w�b�_�t�@�C�� [quadratic_bezier.h]
// Author : ���c����
//
//============================================================================

#ifndef _QUADRATIC_BEZIER_
#define _QUADRATIC_BEZIER_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// 2���x�W�F�Ȑ��N���X�̒�`
//****************************************************
class CQuadratic_Bezier
{
public:

	/// <summary> ����_�̐� </summary>
	static constexpr int NUM_CONTROLPOINT = 3;

	CQuadratic_Bezier(const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Pos3);	// ���W�n���R���X�g���N�^
	CQuadratic_Bezier(const std::array<D3DXVECTOR3, NUM_CONTROLPOINT>& ControlPoint);				// ���W�n���R���X�g���N�^
	~CQuadratic_Bezier();																			// �f�X�g���N�^

	HRESULT Init();		// �����ݒ�
	void	Uninit();	// �I������
	void	Update();	// �X�V����
	void	Draw();		// �`�揈��

private:

	/// <summary> �O�Ղ̐� </summary>
	static constexpr WORD NUM_TRAJECTORY = 3;

	/// <summary> �ړ����x </summary>
	static constexpr float MOVE_SPEED = 0.005f;

	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�𐶐�

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							// ���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3				m_ControlPoint[NUM_CONTROLPOINT];	// ����_
	float					m_fParameter;						// �i�s�x
	CObject_X*				m_pTrajectory[NUM_TRAJECTORY];		// �O��
};

#endif	// _QUADRATIC_BEZIER_